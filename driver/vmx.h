#pragma once

#define VMM_TAG '_VMM'
#define VMM_STACK_SIZE (0x1000 * 6)

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

struct __vcpu_t {
	struct __vmcs_t *vmcs;
	unsigned __int64 vmcs_physical;

	struct __vmcs_t *vmxon;
	unsigned __int64 vmxon_physical;

	struct __vmm_context_t *vmm_context;
};


struct __vmm_context_t {
	unsigned __int64 processor_count;
	struct __vcpu_t **vcpu_table;
	char *stack;
};


#pragma warning(push)
#pragma warning(disable: 4214)


// This controls how asynchronous events are handled in the guest. (Like nmi's and the like)
// Basically, what should/shouldn't cause a vmexit
union __vmx_pinbased_control_msr {
	unsigned __int64 all;
	struct {
		unsigned __int64
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
	unsigned __int64 all;
	struct {
		unsigned __int64
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
	unsigned __int64 all;
	struct {
		unsigned __int64
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
	unsigned __int64 all;
	struct {
		unsigned __int64
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
			conceal_vmx_from_pt : 1;
	} bits;
};

union __vmx_entry_control_t {
	unsigned __int64 all;
	struct {
		unsigned __int64
			:2,
			load_dbg_controls : 1,
			: 6,
			ia32_mode_guest : 1,
			entry_to_smm : 1,
			deactivate_dual_monitor_treatment : 1,
			: 1,
			load_ia32_perf_global_control : 1,
			load_ia32_pat : 1,
			load_ia32_efer : 1,
			load_ia32_bndcfgs : 1,
			conceal_vmx_from_pt : 1;
	} bits;

};


#pragma warning(pop)