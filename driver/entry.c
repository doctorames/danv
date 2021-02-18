#include <ntddk.h>
//#include <basetsd.h>

#include "defs.h"
#include "arch.h"
#include "vmx.h"
#include "msr.h"
#include "field_encoding.h"
#include "vmm_intrin.h"
#include "asm_helpers.h"

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

	DbgPrint("hey its me again!!\n");
	DbgPrint("VMX is %s\n", VmHasVmxSupport() == 1u ? "SUPPORTED" : "NOT SUPPORTED");

	logit(DPFLTR_ERROR_LEVEL, "%s", "DPFLTR_ERROR_LEVEL\n");
	logit(DPFLTR_WARNING_LEVEL, "%s", "DPFLTR_WARNING_LEVEL\n");
	logit(DPFLTR_TRACE_LEVEL, "%s", "DPFLTR_TRACE_LEVEL\n");
	logit(DPFLTR_INFO_LEVEL, "%s", "DPFLTR_INFO_LEVEL\n");

	enable_vmx_operation();
	vmm_init();

	return STATUS_SUCCESS;
}

struct __vmm_context_t* allocate_vmm_context() {
	struct __vmm_context_t *vmm = NULL;
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);
	vmm = ExAllocatePoolWithTag(NonPagedPool, sizeof(struct __vmm_context_t), VMM_TAG);

	if (vmm == NULL) {
		// this is bad
		logit(DPFLTR_ERROR_LEVEL, "%s: Failed to alloc memory for vmm context\n", __FUNCTION__);
		return NULL;
	}

	vmm->processor_count = KeQueryActiveProcessorCountEx(ALL_PROCESSOR_GROUPS);
	vmm->vcpu_table = ExAllocatePoolWithTag(NonPagedPool, sizeof(struct __vcpu_t*) * vmm->processor_count, VMM_TAG);
	vmm->msr_bitmap = ExAllocatePoolWithTag(NonPagedPool, PAGE_SIZE, VMM_TAG);
	RtlSecureZeroMemory(vmm->msr_bitmap, PAGE_SIZE);
	//vmm->stack = ExAllocatePoolWithTag(NonPagedPool, VMM_STACK_SIZE, VMM_TAG);

	//memset(vmm->stack, 0xcc, VMM_STACK_SIZE);

	return vmm;
}

struct __vcpu_t* init_vcpu() {
	struct __vcpu_t *vcpu = NULL;

	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

	vcpu = ExAllocatePoolWithTag(NonPagedPool, sizeof(struct __vcpu_t), VMM_TAG);
	if (vcpu == NULL) {
		// this is bad
		logit(DPFLTR_ERROR_LEVEL, "%s: Failed to alloc memory for vcpu\n", __FUNCTION__);
		return NULL;
	}

	RtlSecureZeroMemory(vcpu, sizeof(struct __vcpu_t));
	logit(DPFLTR_ERROR_LEVEL, "%s: vcpu:%p\n", __FUNCTION__, vcpu);

	memset(vcpu->vmm_stack.limit, 0xcc, sizeof(vcpu->vmm_stack.limit));

	
	//vcpu->msr_bitmap_physical = MmGetPhysicalAddress(vcpu->msr_bitmap).QuadPart;


	return vcpu;
}

int vmm_init() {
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);
	struct __vmm_context_t *vmm_context = allocate_vmm_context();
	PROCESSOR_NUMBER procnum;
	GROUP_AFFINITY affinity, old_affinity;
	//KIRQL old_irql;

	//vmm_context = allocate_vmm_context();


	logit(DPFLTR_INFO_LEVEL, "%s: vmm_context->processor_count: %d\n", __FUNCTION__, vmm_context->processor_count);


	for (unsigned i = 0; i < vmm_context->processor_count; i++) {
		vmm_context->vcpu_table[i] = init_vcpu();
		logit(DPFLTR_ERROR_LEVEL, "%s: vmm_context->vcpu_table[%d]:%p\n", __FUNCTION__, i, vmm_context->vcpu_table[i]);
		//vmm_context->vcpu_table[i]->vmm_stack.vmm_context = vmm_context;
	}

	for (unsigned i = 0; i < vmm_context->processor_count; i++) {
		KeGetProcessorNumberFromIndex(i, &procnum);
		RtlSecureZeroMemory(&affinity, sizeof(affinity));
		affinity.Group = procnum.Group;
		affinity.Mask = (KAFFINITY) 1 << procnum.Number;
		KeSetSystemGroupAffinityThread(&affinity, &old_affinity);

		init_logical_processor(vmm_context, 0);

		KeRevertToUserGroupAffinityThread(&old_affinity);
	}

	return 1;
}


int enable_vmx_operation() {
	
	union __cr4_t cr4 = { 0 };
	union __ia32_feature_control_msr feature_msr = { 0 };
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

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
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

	__cpuid(cpuid.cpu_info, 1);

	return cpuid.regs.ecx.vmx;
}

void logit(ULONG level, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	vDbgPrintEx(DPFLTR_IHVDRIVER_ID, level, fmt, args);

	va_end(args);
}

static void adjust_allowed_bits(unsigned int cap_msr, unsigned __int32 *value) {

	union __vmx_true_control_settings_t cap;

	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

	cap.all = __readmsr(cap_msr);

	*value |= cap.allowed.allowed_0_settings;
	*value &= cap.allowed.allowed_1_settings;
}

static void adjust_entry_controls(union __vmx_entry_control_t *entry_control) {
	unsigned int capability_msr;
	union __ia32_vmx_basic_msr basic_msr = { 0 };
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

	basic_msr.control = __readmsr(IA32_VMX_BASIC_MSR);

	capability_msr = basic_msr.bits.true_controls ? IA32_VMX_TRUE_ENTRY_CTLS_MSR : IA32_VMX_ENTRY_CTLS_MSR;
	adjust_allowed_bits(capability_msr, &entry_control->all);
}

static void adjust_exit_controls(union __vmx_exit_control_t *exit_control) {
	unsigned int capability_msr;
	union __ia32_vmx_basic_msr basic_msr = { 0 };
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

	basic_msr.control = __readmsr(IA32_VMX_BASIC_MSR);

	capability_msr = basic_msr.bits.true_controls ? IA32_VMX_TRUE_EXIT_CTLS_MSR : IA32_VMX_EXIT_CTLS_MSR;
	adjust_allowed_bits(capability_msr, &exit_control->all);
}

static void adjust_pinbased_controls(union __vmx_pinbased_control_t *pinbased_control) {
	unsigned int capability_msr;
	union __ia32_vmx_basic_msr basic_msr = { 0 };
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

	basic_msr.control = __readmsr(IA32_VMX_BASIC_MSR);

	capability_msr = basic_msr.bits.true_controls ? IA32_VMX_TRUE_PINBASED_CTLS_MSR : IA32_VMX_PINBASED_CTLS_MSR;
	adjust_allowed_bits(capability_msr, &pinbased_control->all);
}

static void adjust_primary_procbased_controls(union __vmx_primary_processor_based_control_t *procbased_control) {
	unsigned int capability_msr;
	union __ia32_vmx_basic_msr basic_msr = { 0 };
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

	basic_msr.control = __readmsr(IA32_VMX_BASIC_MSR);

	capability_msr = basic_msr.bits.true_controls ? IA32_VMX_TRUE_PROCBASED_CTLS_MSR : IA32_VMX_PROCBASED_CTLS_MSR;
	adjust_allowed_bits(capability_msr, &procbased_control->all);
}

static void adjust_secondary_procbased_controls(union __vmx_secondary_processor_based_control_t *procbased_control) {
	unsigned int capability_msr;
	union __ia32_vmx_basic_msr basic_msr = { 0 };
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

	basic_msr.control = __readmsr(IA32_VMX_BASIC_MSR);

	capability_msr = IA32_VMX_PROCBASED_CTLS2_MSR;
	adjust_allowed_bits(capability_msr, &procbased_control->all);
}



// Grab some contiguous memory out in physical memory space, and map that into virtual memory
// as a page and make that a __vmcs_t for the passed-in vcpu.
int init_vmxon(struct __vcpu_t *vcpu) {
	
	union __ia32_vmx_basic_msr msr = { 0 };
	PHYSICAL_ADDRESS maxAddr;
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

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

int init_vmcs(struct __vmm_context_t *vmm_context, struct __vcpu_t *vcpu, void *guest_rsp, void (*guest_entry_stub)(), int is_pt_allowed, unsigned long procNum) {
	union __ia32_vmx_basic_msr msr = { 0 };
	PHYSICAL_ADDRESS maxAddr;
	union __vmx_entry_control_t entry_control = { 0 };
	union __vmx_exit_control_t  exit_control = { 0 };
	union __vmx_pinbased_control_t pinbased_control = { 0 };
	union __vmx_primary_processor_based_control_t primary_procbased_ctl = { 0 };
	union __vmx_secondary_processor_based_control_t secondary_procbased_ctl = { 0 };
	struct __pseudo_descriptor_48_t gdtr;
	struct __pseudo_descriptor_48_t idtr;

	is_pt_allowed;
	guest_rsp;

	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

	msr.control = __readmsr(IA32_VMX_BASIC_MSR);
	maxAddr.QuadPart = ~0ULL;
	vcpu->vmcs = MmAllocateContiguousMemory(PAGE_SIZE, maxAddr);
	vcpu->vmcs_physical = (MmGetPhysicalAddress(vcpu->vmcs)).QuadPart;

	RtlSecureZeroMemory(vcpu->vmcs, PAGE_SIZE);

	vcpu->vmcs->header.all = (unsigned int)msr.bits.vmcs_revision_identifier;
	vcpu->vmcs->header.bits.shadow_vmcs_indicator = 0;
	
	// init the vmcs
	if (__vmx_vmclear(&vcpu->vmcs_physical) != VMX_OK) {
		logit(DPFLTR_ERROR_LEVEL, "%s: Uh oh. __vmx_vmclear() instrinsic no worky for vcpu %d\n", __FUNCTION__, procNum);
		return 0;
	}

	if (__vmx_vmptrld(&vcpu->vmcs_physical) != VMX_OK) {
		logit(DPFLTR_ERROR_LEVEL, "%s: Uh oh. __vmx_vmptrld() instrinsic no worky for vcpu %d\n", __FUNCTION__, procNum);
		return 0;
	}

	_sgdt(&gdtr);
	__sidt(&idtr);


	entry_control.bits.ia32e_mode_guest = 1;
	adjust_entry_controls(&entry_control);

	exit_control.bits.host_address_space_size = 1;
	adjust_exit_controls(&exit_control);

	adjust_pinbased_controls(&pinbased_control);

	primary_procbased_ctl.bits.active_secondary_controls = 1;
	primary_procbased_ctl.bits.use_msr_bitmaps = 1;
	adjust_primary_procbased_controls(&primary_procbased_ctl);

	secondary_procbased_ctl.bits.enable_invpcid = 1;
	secondary_procbased_ctl.bits.enable_rdtscp = 1;
	secondary_procbased_ctl.bits.enable_xsave_xrstor = 1;
	adjust_secondary_procbased_controls(&secondary_procbased_ctl);

	// Set controls
	__vmx_vmwrite(VmEntryControls, entry_control.all);
	__vmx_vmwrite(VmExitControls, exit_control.all);
	__vmx_vmwrite(PinBasedVmExecutionControls, pinbased_control.all);
	__vmx_vmwrite(PrimaryProcBasedVmExecutionControls, primary_procbased_ctl.all);
	__vmx_vmwrite(SecondaryProcBasedVmExecutionControls, secondary_procbased_ctl.all);

	// Set guest things
	__vmx_vmwrite(GuestCr0, __readcr0());
	__vmx_vmwrite(GuestCr3, __readcr3());
	__vmx_vmwrite(GuestCr4, __readcr4());
	__vmx_vmwrite(GuestDr7, __readdr(7));
	__vmx_vmwrite(GuestRsp, vcpu->guest_rsp);
	__vmx_vmwrite(GuestRip, vcpu->guest_rip);

	__vmx_vmwrite(GuestRflags, __readeflags());
	__vmx_vmwrite(GuestIa32Debugctl, __readmsr(IA32_DEBUGCTL_MSR));
	__vmx_vmwrite(GuestIa32SysenterEsp, __readmsr(IA32_SYSENTER_ESP_MSR));
	__vmx_vmwrite(GuestIa32SysenterEip, __readmsr(IA32_SYSENTER_EIP_MSR));
	__vmx_vmwrite(GuestIa32SysenterCs, __readmsr(IA32_SYSENTER_CS_MSR));
	__vmx_vmwrite(VmcsLinkPointer, MAXUINT64);
	__vmx_vmwrite(GuestFsBase, __readmsr(IA32_FS_BASE_MSR));
	__vmx_vmwrite(GuestGsBase, __readmsr(IA32_GS_BASE_MSR));

	__vmx_vmwrite(Cr0ReadShadow, __readcr0());
	__vmx_vmwrite(Cr4ReadShadow, __readcr4());

	__vmx_vmwrite(GuestEsSelector, __read_es());
	__vmx_vmwrite(GuestCsSelector, __read_cs());
	__vmx_vmwrite(GuestSsSelector, __read_ss());
	__vmx_vmwrite(GuestDsSelector, __read_ds());
	__vmx_vmwrite(GuestFsSelector, __read_fs());
	__vmx_vmwrite(GuestGsSelector, __read_gs());
	__vmx_vmwrite(GuestLdtrSelector, __read_ldtr());
	__vmx_vmwrite(GuestTrSelector, __read_tr());

	__vmx_vmwrite(GuestEsLimit, __segmentlimit(__read_es()));
	__vmx_vmwrite(GuestCsLimit, __segmentlimit(__read_cs()));
	__vmx_vmwrite(GuestSsLimit, __segmentlimit(__read_ss()));
	__vmx_vmwrite(GuestDsLimit, __segmentlimit(__read_ds()));
	__vmx_vmwrite(GuestFsLimit, __segmentlimit(__read_fs()));
	__vmx_vmwrite(GuestGsLimit, __segmentlimit(__read_gs()));
	__vmx_vmwrite(GuestLdtrLimit, __segmentlimit(__read_ldtr()));
	__vmx_vmwrite(GuestTrLimit, __segmentlimit(__read_tr()));

	__vmx_vmwrite(GuestGdtrLimit, gdtr.limit);
	__vmx_vmwrite(GuestIdtrLimit, idtr.limit);
	__vmx_vmwrite(GuestGdtrBase, gdtr.base_address);
	__vmx_vmwrite(GuestIdtrBase, idtr.base_address);

	__vmx_vmwrite(GuestEsAccessRights, read_segment_access_rights(__read_es()));
	__vmx_vmwrite(GuestCsAccessRights, read_segment_access_rights(__read_cs()));
	__vmx_vmwrite(GuestSsAccessRights, read_segment_access_rights(__read_ss()));
	__vmx_vmwrite(GuestDsAccessRights, read_segment_access_rights(__read_ds()));
	__vmx_vmwrite(GuestFsAccessRights, read_segment_access_rights(__read_fs()));
	__vmx_vmwrite(GuestGsAccessRights, read_segment_access_rights(__read_gs()));
	__vmx_vmwrite(GuestLdtrAccessRights, read_segment_access_rights(__read_ldtr()));
	__vmx_vmwrite(GuestTrAccessRights, read_segment_access_rights(__read_tr()));

	__vmx_vmwrite(GuestLdtrBase, get_segment_base(gdtr.base_address, __read_ldtr()));
	__vmx_vmwrite(GuestTrBase, get_segment_base(gdtr.base_address, __read_tr()));

	// Set host things
	// According to 3.26.2.3, you have to mask out the requested priviledge level bits [1:0] and table indicator [2] bit.
	// So our mask will be the 1's compliment of these 3 low bits ~(7) = 0xfff8
	logit(DPFLTR_INFO_LEVEL, "%s: 1\n", __FUNCTION__);
	unsigned short selector_mask = 7u;
	__vmx_vmwrite(HostEsSelector, __read_es() & ~selector_mask);
	logit(DPFLTR_INFO_LEVEL, "%s: 2\n", __FUNCTION__);
	__vmx_vmwrite(HostCsSelector, __read_cs() & ~selector_mask);
	logit(DPFLTR_INFO_LEVEL, "%s: 3\n", __FUNCTION__);
	__vmx_vmwrite(HostSsSelector, __read_ss() & ~selector_mask);
	logit(DPFLTR_INFO_LEVEL, "%s: 4\n", __FUNCTION__);
	__vmx_vmwrite(HostDsSelector, __read_ds() & ~selector_mask);
	logit(DPFLTR_INFO_LEVEL, "%s: 5\n", __FUNCTION__);
	__vmx_vmwrite(HostFsSelector, __read_fs() & ~selector_mask);
	logit(DPFLTR_INFO_LEVEL, "%s: 6\n", __FUNCTION__);
	__vmx_vmwrite(HostGsSelector, __read_gs() & ~selector_mask);
	logit(DPFLTR_INFO_LEVEL, "%s: 7\n", __FUNCTION__);
	__vmx_vmwrite(HostTrSelector, __read_tr() & ~selector_mask);
	logit(DPFLTR_INFO_LEVEL, "%s: 8\n", __FUNCTION__);

	__vmx_vmwrite(HostCr0, __readcr0());
	logit(DPFLTR_INFO_LEVEL, "%s: 9\n", __FUNCTION__);
	__vmx_vmwrite(HostCr3, __readcr3());
	logit(DPFLTR_INFO_LEVEL, "%s: 10\n", __FUNCTION__);
	__vmx_vmwrite(HostCr4, __readcr4());
	logit(DPFLTR_INFO_LEVEL, "%s: 11\n", __FUNCTION__);
	__vmx_vmwrite(HostRsp, &vcpu->vmm_stack.vmm_context);
	logit(DPFLTR_INFO_LEVEL, "%s: 12\n", __FUNCTION__);
	__vmx_vmwrite(HostRip, guest_entry_stub);
	logit(DPFLTR_INFO_LEVEL, "%s: 13\n", __FUNCTION__);

	__vmx_vmwrite(HostTrBase, get_segment_base(gdtr.base_address, __read_tr()));
	__vmx_vmwrite(HostGdtrBase, gdtr.base_address);
	__vmx_vmwrite(HostIdtrBase, idtr.base_address);
	__vmx_vmwrite(HostGsBase, get_segment_base(gdtr.base_address, __read_gs()));
	__vmx_vmwrite(HostFsBase, get_segment_base(gdtr.base_address, __read_fs()));

	vcpu->vmm_stack.vmm_context.msr_bitmap = vmm_context->msr_bitmap;
	vcpu->vmm_stack.vmm_context.processor_count = vmm_context->processor_count;
	vcpu->vmm_stack.vmm_context.vcpu_table = vmm_context->vcpu_table;

	return 1;
}

void adjust_control_registers() {
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);
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

void init_logical_processor(struct __vmm_context_t *vmm, void *guest_rsp) {
	//union __ia32_vmx_misc_msr msr;
	guest_rsp;
	logit(DPFLTR_INFO_LEVEL, "%s: entry\n", __FUNCTION__);

	unsigned long procNum = KeGetCurrentProcessorNumber();

	adjust_control_registers();

	if (!VmHasVmxSupport()) {
		// do something
		logit(DPFLTR_ERROR_LEVEL, "%s: Uh oh. No VMX support for vcpu %d\n", __FUNCTION__, procNum);
		return;
	}

	logit(DPFLTR_ERROR_LEVEL, "%s: vmm->vcpu_table[%d]:%p\n", __FUNCTION__, procNum, vmm->vcpu_table[procNum]);
	if (!init_vmxon(vmm->vcpu_table[procNum])) {
		// do something
		logit(DPFLTR_ERROR_LEVEL, "%s: Uh oh. init_vmxon() no worky for vcpu %d\n", __FUNCTION__, procNum);
		return;
	}

	// call the vmxon instrinsic
	if (__vmx_on(&vmm->vcpu_table[procNum]->vmxon_physical) != 0) {
		// do something
		logit(DPFLTR_ERROR_LEVEL, "%s: Uh oh. __vmx_on() instrinsic no worky for vcpu %d\n", __FUNCTION__, procNum);
		return;
	}

	init_vmcs(vmm, vmm->vcpu_table[procNum], guest_rsp, vmm_entrypoint, 1, procNum);

	int status = __vmx_vmlaunch();
	if (status != 0) {
		unsigned __int32 vmx_error;
		__vmx_vmread(VmInstructionError, &vmx_error);
		logit(DPFLTR_ERROR_LEVEL, "%s: Uh oh. __vmx_vmlaunch() instrinsic no worky for vcpu %d. VXM error: 0x%x\n", __FUNCTION__, procNum, vmx_error);
		return;
	}

	logit(DPFLTR_INFO_LEVEL, "%s: SUCCESS! vcpu %d is in vmxon!\n", __FUNCTION__, procNum);

//_end:
//	KeSignalCallDpcSynchronize();
//	KeSignalCallDpcDone();

}