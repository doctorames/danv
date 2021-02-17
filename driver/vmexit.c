#include <ntddk.h>
#include <intrin.h>
#include "vmm_intrin.h"
#include "vmx.h"
#include "interrupt_info.h"
#include "field_encoding.h"
#include "cpuid.h"






static vmexit_status_t vmexit_vmx_instruction_executed(struct __gcpu_context_t * gcpu) {

	struct __vmentry_event_information_t ud_exception;

	ud_exception.instruction_length = 0;
	ud_exception.error_code = 0;

	ud_exception.interrupt_info.bits.vector = exception_undefined_opcode;
	ud_exception.interrupt_info.bits.interruption_type = interrupt_type_hardware_exception;
	ud_exception.interrupt_info.bits.deliver_error_code = 0;
	ud_exception.interrupt_info.bits.valid = 1;

	__vmx_vmwrite(VmEntryInterruptionInfoField, ud_exception.interrupt_info.all);
	__vmx_vmwrite(VmEntryInstructionLength, ud_exception.instruction_length);

	// The vmexit was caused by the guest attempting to execute an instruction that causes a vmexit (either unconditionally, or via vmcs control configuration).
	// Therefore, the processor will set rf to 0. We have to set it to 1 so the guest will resume (if it hit a break).
	// See 3.27.3.3
	gcpu->ext_registers.rflags.bits.resume = 1;
	__vmx_vmwrite(GuestRflags, gcpu->ext_registers.rflags.all);

	return VMEXIT_HANDLED;
}


static vmexit_status_t vmexit_cpuid_handler(struct __gcpu_context_t *gcpu) {
	struct __cpuid_params_t cpuid_registers;

	cpuid_registers.rax = gcpu->guest_registers.rax;
	cpuid_registers.rbx = gcpu->guest_registers.rbx;
	cpuid_registers.rcx = gcpu->guest_registers.rcx;
	cpuid_registers.rdx = gcpu->guest_registers.rdx;

	// Go ahead an execute the cpuid intrinsic on the guest's behalf
	__cpuidex(&cpuid_registers, cpuid_registers.rax, cpuid_registers.rcx);

	switch (cpuid_registers.rax) {
	case 1:
		if (QUERY_CPUID_BIT(cpuid_registers.rcx, 31))
			cpuid_registers.rcx = CLEAR_CPUID_BIT(cpuid_registers.rcx, 31);
		break;
	default:
		break;
	}

	// adjust_rip();
}




int vmexit_handler(struct __vmexit_guest_registers_t *guest_registers) {
	guest_registers;
	DbgBreakPointWithStatus((ULONG)STATUS_BREAKPOINT);
	return 0;
}

