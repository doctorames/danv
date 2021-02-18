#include <ntddk.h>
#include <intrin.h>
#include "vmm_intrin.h"
#include "vmx.h"
#include "interrupt_info.h"
#include "field_encoding.h"
#include "cpuid.h"




void logit(ULONG level, const char *fmt, ...);

static vmexit_status_t vmexit_vmx_instruction_executed(struct __gcpu_context_t * gcpu) {

	struct __vmentry_event_information_t ud_exception;
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

	ud_exception.instruction_length = 0;
	ud_exception.error_code = 0;

	ud_exception.interrupt_info.bits.vector = exception_undefined_opcode;
	ud_exception.interrupt_info.bits.interruption_type = interrupt_type_hardware_exception;
	ud_exception.interrupt_info.bits.deliver_error_code = 0;
	ud_exception.interrupt_info.bits.valid = 1;

	__vmx_vmwrite(VmEntryInterruptionInfoField, ud_exception.interrupt_info.all);
	__vmx_vmwrite(VmEntryInstructionLength, ud_exception.instruction_length);

	// The vmexit was caused by the guest attempting to execute an instruction that causes a vmexit (either unconditionally, or via vmcs control configuration).
	// Therefore, the processor will set rf to 0. We have to set it to 1 so the guest will resume (if it hit a break, i guess).
	// See 3.27.3.3
	gcpu->ext_registers.rflags.bits.resume = 1;
	__vmx_vmwrite(GuestRflags, gcpu->ext_registers.rflags.all);

	return VMEXIT_HANDLED;
}


static vmexit_status_t vmexit_cpuid_handler(struct __gcpu_context_t *gcpu) {
	struct __cpuid_params_32_t cpuid_registers_32;
	unsigned __int32 instruction_length;
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);
	
	cpuid_registers_32.eax = (unsigned __int32)gcpu->guest_registers->rax;
	cpuid_registers_32.ebx = (unsigned __int32)gcpu->guest_registers->rbx;
	cpuid_registers_32.ecx = (unsigned __int32)gcpu->guest_registers->rcx;
	cpuid_registers_32.edx = (unsigned __int32)gcpu->guest_registers->rdx;


	// Go ahead an execute the cpuid intrinsic on the guest's behalf
	__cpuidex((int*)&cpuid_registers_32, cpuid_registers_32.eax, cpuid_registers_32.ecx);

	gcpu->guest_registers->rax = cpuid_registers_32.eax;
	gcpu->guest_registers->rbx = cpuid_registers_32.ebx;
	gcpu->guest_registers->rcx = cpuid_registers_32.ecx;
	gcpu->guest_registers->rdx = cpuid_registers_32.edx;

	switch (gcpu->guest_registers->rax) {
	case 1:
		if (QUERY_CPUID_BIT(gcpu->guest_registers->rcx, 31))
			// Nothing to see here. Move along, sir.
			gcpu->guest_registers->rcx = CLEAR_CPUID_BIT(gcpu->guest_registers->rcx, 31);
		break;
	default:
		break;
	}

	__vmx_vmread(VmExitInstructionLength, (size_t*)&instruction_length);
	gcpu->ext_registers.rip += instruction_length;
	__vmx_vmwrite(GuestRip, gcpu->ext_registers.rip);

	return VMEXIT_HANDLED;
}




int vmexit_handler(struct __vmm_stack_t *vmm_stack) {
	
	union __vmx_exit_reason_field_t vmexit_reason;
	struct __gcpu_context_t gcpu;
	vmexit_status_t status = VMEXIT_ERROR;
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

	__vmx_vmread(ExitReason, &vmexit_reason.all);

	__vmx_vmread(GuestRip, &gcpu.ext_registers.rip);
	__vmx_vmread(GuestRsp, &gcpu.ext_registers.rsp);
	__vmx_vmread(GuestRflags, (size_t*)&gcpu.ext_registers.rflags);
	gcpu.guest_registers = (struct __guest_registers_t*)((unsigned __int64)(&vmm_stack->limit) + VMM_STACK_SIZE - sizeof(struct __guest_registers_t));

	switch (vmexit_reason.bits.basic_exit_reason) {
	case vmexit_vmcall:
	case vmexit_vmclear:
	case vmexit_vmlaunch:
	case vmexit_vmresume:
	case vmexit_vmptrld:
	case vmexit_vmptrst:
	case vmexit_vmread:
	case vmexit_vmwrite:
	case vmexit_vmxon:
	case vmexit_vmxoff:
	case vmexit_vmfunc:
	case vmexit_invept:
	case vmexit_invvpid:
		status = vmexit_vmx_instruction_executed(&gcpu);
		break;
	case vmexit_cpuid:
		status = vmexit_cpuid_handler(&gcpu);
		break;
	default:
		status = VMEXIT_ERROR;
		break;
	}

	if (status != VMEXIT_HANDLED)
		DbgBreakPoint();

	return status;
}

