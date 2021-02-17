#pragma once
#include <xmmintrin.h>
#include "arch.h"

#define VMM_TAG '_VMM'
#define VMM_STACK_SIZE (0x1000 * 6)
#define VMX_OK  0

typedef  unsigned __int32 vmexit_status_t;
#define VMEXIT_HANDLED  0
#define VMEXIT_ERROR    1




enum __vmexit_reason_e {
	vmexit_nmi = 0,
	vmexit_ext_int,
	vmexit_triple_fault,
	vmexit_init_sugnal,
	vmexit_sipi,
	vmexit_smi,
	vmexit_other_smi,
	vmexit_interrupt_window,
	vmexit_nmi_window,
	vmexit_task_switch,
	vmexit_cpuid,
	vmexit_getsec,
	vmexit_hlt,
	vmexit_invd,
	vmexit_invlpg,
	vmexit_rdpmc,
	vmexit_rdtsc,
	vmexit_rsm,
	vmexit_vmcall,
	vmexit_vmclear,
	vmexit_vmlaunch,
	vmexit_vmptrld,
	vmexit_vmptrst,
	vmexit_vmread,
	vmexit_vmresume,
	vmexit_vmwrite,
	vmexit_vmxoff,
	vmexit_vmxon,
	vmexit_control_register_access,
	vmexit_mov_dr,
	vmexit_io_intruction,
	vmexit_rdmsr,
	vmexit_wrmsr,
	vmexit_vmentry_failure_due_to_guest_state,
	vmexit_vmentry_failure_due_to_msr_loading,
	vmexit_mwait = 36,
	vmexit_monitor_trap_flag,
	vmexit_monitor = 39,
	vmexit_pause,
	vmexit_vmentry_failure_due_to_machine_check_event,
	vmexit_tpr_below_threshold = 43,
	vmexit_apic_access,
	vmexit_virtualized_eoi,
	vmexit_access_to_gdtr_or_idtr,
	vmexit_access_to_ldtr_or_tr,
	vmexit_ept_violation,
	vmexit_ept_misconfiguration,
	vmexit_invept,
	vmexit_rdtscp,
	vmexit_vmx_preemption_timer_expired,
	vmexit_invvpid,
	vmexit_wbinvd,
	vmexit_xsetbv,
	vmexit_apic_write,
	vmexit_rdrand,
	vmexit_invpcid,
	vmexit_vmfunc,
	vmexit_encls,
	vmexit_rdseed,
	vmexit_pml_full,
	vmexit_xsaves,
	vmexit_xrstors
};

struct __vmcs_t {
	union {
		unsigned int all;
		struct {
			unsigned int
				revision_identifier : 31,
				shadow_vmcs_indicator : 1;
		} bits;
	} header;
	unsigned int abort_indicator;
	char data[0x1000 - 2*sizeof(unsigned int)];
};

struct __vmm_context_t {
	unsigned __int64 processor_count;
	__declspec(align(4096)) struct __vcpu_t **vcpu_table;
	__declspec(align(4096)) void *msr_bitmap;
};

struct __vmm_stack_t {
	unsigned char limit[VMM_STACK_SIZE - sizeof(struct __vmm_context_t)];
	struct __vmm_context_t vmm_context;
};

struct __vcpu_t {

	unsigned __int64 status;
	unsigned __int64 guest_rsp;
	unsigned __int64 guest_rip;

	struct __vmcs_t *vmcs;
	unsigned __int64 vmcs_physical;

	struct __vmcs_t *vmxon;
	unsigned __int64 vmxon_physical;

	//void *msr_bitmap;
	//unsigned __int64 msr_bitmap_physical;

	//struct __vmm_context_t *vmm_context;

	__declspec(align(4096)) struct __vmm_stack_t vmm_stack;
};

#pragma warning(push)
#pragma warning(disable: 4214)


// This controls how asynchronous events are handled in the guest. (Like nmi's and the like)
// Basically, what should/shouldn't cause a vmexit
union __vmx_pinbased_control_t {
	unsigned __int32 all;
	struct {
		unsigned __int32
			external_interrupt_exiting : 1,
			: 1,
			nmi_exiting : 1,
			: 1,
			virtual_nmis : 1,
			vmx_preemption_timer : 1,
			process_posted_interrupts : 1;
	} bits;
};

// This controls how SYNChronous events are handled. That is, events that happen as a matter of code execution. (Like cpuid, rdmsr, and the like)
// 
union __vmx_primary_processor_based_control_t {
	unsigned __int32 all;
	struct {
		unsigned __int32
			:2,
			interrupt_window_exiting : 1,
			use_tsc_offsetting : 1,
			: 3,
			hlt_exiting : 1,
			: 1,
			invalidpage_exiting : 1,
			mwait_exiting : 1,
			rdpmc_exiting : 1,
			rdtsc_exiting : 1,
			: 2,
			cr3_load_exiting : 1,
			cr3_store_exiting : 1,
			: 2,
			cr8_load_exiting : 1,
			cr8_store_exiting : 1,
			use_tpr_shadow : 1,
			nmi_window_exiting : 1,
			mov_dr_exiting : 1,
			unconditional_io_exiting : 1,
			use_io_bitmaps : 1,
			: 1,
			monitor_trap_flag : 1,
			use_msr_bitmaps : 1,
			monitor_exiting : 1,
			pause_exiting : 1,
			active_secondary_controls : 1;
	} bits;
};

union __vmx_secondary_processor_based_control_t {
	unsigned __int32 all;
	struct {
		unsigned __int32
			virtualize_apic_access : 1,
			enable_ept : 1,
			descriptor_table_exiting : 1,
			enable_rdtscp : 1,
			virtualize_x2apic : 1,
			enable_vpid : 1,
			wbinvd_exiting : 1,
			unrestricted_guest : 1,
			apic_register_virtualization : 1,
			virtual_interrupt_delivery : 1,
			pause_loop_exiting : 1,
			rdrand_exiting : 1,
			enable_invpcid : 1,
			enable_vmfunc : 1,
			vmcs_shadowing : 1,
			enable_encls_exiting : 1,
			rdseed_exiting : 1,
			enable_pml : 1,
			use_virtualization_exception : 1,
			conceal_vmx_from_pt : 1,
			enable_xsave_xrstor : 1,
			: 1,
			mode_based_execute_control_ept : 1,
			: 2,
			use_tsc_scaling : 1;
	} bits;
};

union __vmx_exit_control_t {
	unsigned __int32 all;
	struct {
		unsigned __int32
			:2,
			save_dbg_controls : 1,
			: 6,
			host_address_space_size : 1,
			: 2,
			load_ia32_perf_global_control : 1,
			: 2,
			ack_interrupt_on_exit : 1,
			: 2,
			save_ia32_pat : 1,
			load_ia32_pat : 1,
			save_ia32_efer : 1,
			load_ia32_efer : 1,
			save_vmx_preeemption_timer_value : 1,
			clear_ia32_bndcfgs : 1,
			conceal_vmx_from_pt : 1,
			clear_ia32_rtit_ctl : 1,
			: 2,
			load_cet_state : 1,
			load_pkrs : 1;
	} bits;
};

union __vmx_entry_control_t {
	unsigned __int32 all;
	struct {
		unsigned __int32
			:2,
			load_dbg_controls : 1,
			: 6,
			ia32e_mode_guest : 1,
			entry_to_smm : 1,
			deactivate_dual_monitor_treatment : 1,
			: 1,
			load_ia32_perf_global_control : 1,
			load_ia32_pat : 1,
			load_ia32_efer : 1,
			load_ia32_bndcfgs : 1,
			conceal_vmx_from_pt : 1,
			load_ia32_rtit_ctl : 1,
			: 1,
			load_cet_state : 1,
			: 1,
			load_pkrs : 1;
	} bits;

};

union __vmx_exit_reason_field_t {
	unsigned __int64 all;
	struct {
		unsigned __int64
			basic_exit_reason : 16,
			must_be_zero_1 : 11,
			was_in_enclave_mode : 1,
			pending_mtf_exit : 1,
			exit_from_vmx_root : 1,
			must_be_zero_2 : 1,
			vm_entry_failure : 1;
	} bits;
};

#pragma warning(pop)

struct __vmexit_guest_registers_t {
	unsigned __int64
		rax,
		rbx,
		rcx,
		rdx,
		rbp,
		rsi,
		rdi,
		r8,
		r9,
		r10,
		r11,
		r12,
		r13,
		r14,
		r15;
};

union __vmx_true_control_settings_t {
	unsigned __int64 all;
	struct {
		unsigned __int32 allowed_0_settings;
		unsigned __int32 allowed_1_settings;
	} allowed;
};




struct __guest_registers_t {
	__m128 xmm[6];
	void *padding;
	unsigned __int64
		r15,
		r14,
		r13,
		r12,
		r11,
		r10,
		r9,
		r8,
		rdi,
		rsi,
		rbp,
		rdx,
		rcx,
		rbx,
		rax;
};

struct __vmm_exit_stack_t {
	struct __guest_registers_t guest_registers;
	struct __vmm_context_t vmm_context;
};

struct __ext_registers_t {
	unsigned __int64 rip;
	unsigned __int64 rsp;
	union __rflags_t rflags;
};

struct __gcpu_context_t {
	void *vcpu;
	struct __ext_registers_t ext_registers;
	struct __guest_registers_t guest_registers;
};