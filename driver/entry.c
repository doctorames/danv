#include <ntddk.h>
//#include <basetsd.h>

#include "defs.h"
#include "arch.h"
#include "vmx.h"
#include "FieldEncoding.h"

#include <stdarg.h>

unsigned int VmHasVmxSupport();
void logit(ULONG level, const char *fmt, ...);
void init_logical_processor(struct __vmm_context_t *vmm, void *guest_rip);
int enable_vmx_operation();
int vmm_init();

NTSTATUS
DriverEntry(
	PDRIVER_OBJECT DriverObject,
	PUNICODE_STRING RegistryPath
)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	DbgPrint("hey its me!!");
	DbgPrint("VMX is %s", VmHasVmxSupport() == 1u ? "SUPPORTED" : "NOT SUPPORTED");

	logit(DPFLTR_ERROR_LEVEL, "%s", "DPFLTR_ERROR_LEVEL");
	logit(DPFLTR_WARNING_LEVEL, "%s", "DPFLTR_WARNING_LEVEL");
	logit(DPFLTR_TRACE_LEVEL, "%s", "DPFLTR_TRACE_LEVEL");
	logit(DPFLTR_INFO_LEVEL, "%s", "DPFLTR_INFO_LEVEL");

	enable_vmx_operation();
	vmm_init();

	return STATUS_SUCCESS;
}

struct __vmm_context_t* allocate_vmm_context() {
	struct __vmm_context_t *vmm = NULL;
	logit(DPFLTR_INFO_LEVEL, "%s: entry", __FUNCTION__);
	vmm = ExAllocatePoolWithTag(NonPagedPool, sizeof(struct __vmm_context_t), VMM_TAG);

	if (vmm == NULL) {
		// this is bad
		logit(DPFLTR_ERROR_LEVEL, "%s: Failed to alloc memory for vmm context", __FUNCTION__);
		return NULL;
	}

	vmm->processor_count = KeQueryActiveProcessorCountEx(ALL_PROCESSOR_GROUPS);
	vmm->vcpu_table = ExAllocatePoolWithTag(NonPagedPool, sizeof(struct __vcpu_t*) * vmm->processor_count, VMM_TAG);
	vmm->stack = ExAllocatePoolWithTag(NonPagedPool, VMM_STACK_SIZE, VMM_TAG);

	memset(vmm->stack, 0xcc, VMM_STACK_SIZE);

	return vmm;
}

struct __vcpu_t* init_vcpu() {
	struct __vcpu_t *vcpu = NULL;

	logit(DPFLTR_INFO_LEVEL, "%s: entry", __FUNCTION__);

	vcpu = ExAllocatePoolWithTag(NonPagedPool, sizeof(struct __vcpu_t), VMM_TAG);
	if (vcpu == NULL) {
		// this is bad
		logit(DPFLTR_ERROR_LEVEL, "%s: Failed to alloc memory for vcpu", __FUNCTION__);
		return NULL;
	}

	RtlSecureZeroMemory(vcpu, sizeof(struct __vcpu_t));
	logit(DPFLTR_ERROR_LEVEL, "%s: vcpu:%p", __FUNCTION__, vcpu);
	return vcpu;
}

int vmm_init() {
	logit(DPFLTR_INFO_LEVEL, "%s: entry", __FUNCTION__);
	struct __vmm_context_t *vmm_context = allocate_vmm_context();
	logit(DPFLTR_INFO_LEVEL, "%s: vmm_context->processor_count: %d", __FUNCTION__, vmm_context->processor_count);


	for (unsigned i = 0; i < vmm_context->processor_count; i++) {
		vmm_context->vcpu_table[i] = init_vcpu();
		logit(DPFLTR_ERROR_LEVEL, "%s: vmm_context->vcpu_table[%d]:%p", __FUNCTION__, i, vmm_context->vcpu_table[i]);
		vmm_context->vcpu_table[i]->vmm_context = vmm_context;
	}

	// struct __vmm_context_t *vmm, void *guest_rip
	init_logical_processor(vmm_context, NULL);

	return 1;
}


int enable_vmx_operation() {
	
	union __cr4_t cr4 = { 0 };
	union __ia32_feature_control_msr feature_msr = { 0 };
	logit(DPFLTR_INFO_LEVEL, "%s: entry", __FUNCTION__);

	// wait, where do we enforce IA32_VMX_CRx_FIXEDx?
	// TODO: react to IA32_VMX_CRx_FIXEDx

	cr4.all = (unsigned int)__readcr4();
	cr4.bits.vme = 1;
	__writecr4(cr4.all);

	feature_msr.control = __readmsr(IA32_FEATURE_CONTROL_MSR);
	if (feature_msr.bits.lock == 0) {
		feature_msr.bits.vmxon_outside_smx = 1;
		feature_msr.bits.lock = 1;
		__writemsr(IA32_FEATURE_CONTROL_MSR, feature_msr.control);
		return 1;
	}

	return 0;
}


unsigned int VmHasVmxSupport() {
	union __cpuid_t cpuid = {0};
	logit(DPFLTR_INFO_LEVEL, "%s: entry", __FUNCTION__);

	__cpuid(cpuid.cpu_info, 1);

	return cpuid.regs.ecx.vmx;
}

void logit(ULONG level, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	vDbgPrintEx(DPFLTR_IHVDRIVER_ID, level, fmt, args);

	va_end(args);
}


// Grab some contiguous memory out in physical memory space, and map that into virtual memory
// as a page and make that a __vmcs_t for the passed-in vcpu.
int init_vmxon(struct __vcpu_t *vcpu) {
	
	union __ia32_vmx_basic_msr msr = { 0 };
	PHYSICAL_ADDRESS maxAddr;
	logit(DPFLTR_INFO_LEVEL, "%s: entry", __FUNCTION__);

	if (vcpu == NULL) return 0;

	msr.control = __readmsr(IA32_VMX_BASIC_MSR);
	maxAddr.QuadPart = ~0ULL;

	if (msr.bits.vmxon_region_size > PAGE_SIZE)
		vcpu->vmxon = MmAllocateContiguousMemory( PAGE_SIZE, maxAddr);
	else
		vcpu->vmxon = MmAllocateContiguousMemory(msr.bits.vmxon_region_size, maxAddr);

	vcpu->vmxon_physical = (MmGetPhysicalAddress(vcpu->vmxon)).QuadPart;
	RtlSecureZeroMemory(vcpu->vmxon, PAGE_SIZE);

	vcpu->vmxon->header.all = (unsigned int)msr.bits.vmcs_revision_identifier;

	return 1;
}

int init_vmcs(struct __vcpu_t *vcpu, void *guest_rsp, void (*guest_rip)(), int is_pt_allowed) {
	union __ia32_vmx_basic_msr msr = { 0 };
	PHYSICAL_ADDRESS maxAddr;
	is_pt_allowed;
	guest_rsp;
	guest_rip;
	logit(DPFLTR_INFO_LEVEL, "%s: entry", __FUNCTION__);

	msr.control = __readmsr(IA32_VMX_BASIC_MSR);
	maxAddr.QuadPart = ~0ULL;
	vcpu->vmcs = MmAllocateContiguousMemory(PAGE_SIZE, maxAddr);
	vcpu->vmcs_physical = (MmGetPhysicalAddress(vcpu->vmcs)).QuadPart;

	RtlSecureZeroMemory(vcpu->vmcs, PAGE_SIZE);

	vcpu->vmcs->header.all = (unsigned int)msr.bits.vmcs_revision_identifier;
	vcpu->vmcs->header.bits.shadow_vmcs_indicator = 0;

	return 1;
}

void adjust_control_registers() {
	logit(DPFLTR_INFO_LEVEL, "%s: entry", __FUNCTION__);
	union __cr0_t cr0 = { 0 };
	union __cr4_t cr4 = { 0 };
	union __ia32_vmx_cr_fixed_msr msr = { 0 };

	cr0.all = (unsigned int)__readcr0();
	msr.all = __readmsr(IA32_VMX_CR0_FIXED0);
	cr0.all |= msr.split.low;
	msr.all = __readmsr(IA32_VMX_CR0_FIXED1);
	cr0.all &= msr.split.low;
	__writecr0(cr0.all);

	cr4.all = (unsigned int)__readcr4();
	msr.all = __readmsr(IA32_VMX_CR4_FIXED0);
	cr4.all |= msr.split.low;
	msr.all = __readmsr(IA32_VMX_CR4_FIXED1);
	cr4.all &= msr.split.low;
	__writecr4(cr4.all);
}



void init_logical_processor(struct __vmm_context_t *vmm, void *guest_rip) {
	//union __ia32_vmx_misc_msr msr;
	guest_rip;
	logit(DPFLTR_INFO_LEVEL, "%s: entry", __FUNCTION__);

	unsigned long procNum = KeGetCurrentProcessorNumber();

	adjust_control_registers();

	if (!VmHasVmxSupport()) {
		// do something
		logit(DPFLTR_ERROR_LEVEL, "%s: Uh oh. No VMX support for vcpu %d", __FUNCTION__, procNum);
		return;
	}

	logit(DPFLTR_ERROR_LEVEL, "%s: vmm->vcpu_table[%d]:%p", __FUNCTION__, procNum, vmm->vcpu_table[procNum]);
	if (!init_vmxon(vmm->vcpu_table[procNum])) {
		// do something
		logit(DPFLTR_ERROR_LEVEL, "%s: Uh oh. init_vmxon() no worky for vcpu %d", __FUNCTION__, procNum);
		return;
	}

	// call the vmxon instrinsic
	if (__vmx_on(&vmm->vcpu_table[procNum]->vmxon_physical) != 0) {
		// do something
		logit(DPFLTR_ERROR_LEVEL, "%s: Uh oh. __vmx_on() instrinsic no worky for vcpu %d", __FUNCTION__, procNum);
		return;
	}

	logit(DPFLTR_INFO_LEVEL, "%s: SUCCESS! vcpu %d is in vmxon!", __FUNCTION__, procNum);

}