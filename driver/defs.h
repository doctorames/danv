#pragma once

union __cpuid_t {
	unsigned int cpu_info[4];
	struct {
		struct {
			unsigned int steppingId : 4;
			unsigned int model : 4;
			unsigned int familyId : 4;
			unsigned int processorType : 2;
			unsigned int rsvd1 : 2;
			unsigned int extModelId : 4;
			unsigned int extFamilyId : 8;
			unsigned int rsvd2 : 4;
		} eax;
		struct {
			unsigned int brandIndex : 8;
			unsigned int cflushLineSize : 8;
			unsigned int maxAddressableIds : 8;
			unsigned int initialApicId : 8;
		} ebx;
		struct {
			unsigned int sse3 : 1;
			unsigned int carrilessMult : 1;
			unsigned int dsArea64Bit : 1;
			unsigned int monitorMwait : 1;
			unsigned int cplQualifiedDebugStore : 1;
			unsigned int vmx : 1;
			unsigned int smx : 1;
			unsigned int eist : 1;
			unsigned int thermalMonitor2 : 1;
			unsigned int ssse3 : 1;
			unsigned int l1ContextId : 1;
			unsigned int sdbg : 1;
			unsigned int fusedMultiplyAdd : 1;
			unsigned int cmpxchg16b : 1;
			unsigned int xtprUpdateControl : 1;
			unsigned int perfDbgCapMsr : 1;
			unsigned int rsvd1 : 1;
			unsigned int processContextIds : 1;
			unsigned int dca : 1;
			unsigned int sse4_1 : 1;
			unsigned int sse4_2 : 1;
			unsigned int x2Apic : 1;
			unsigned int movbe : 1;
			unsigned int popcnt : 1;
			unsigned int tscDeadline : 1;
			unsigned int aes : 1;
			unsigned int xsave : 1;
			unsigned int osxsave : 1;
			unsigned int avx : 1;
			unsigned int f16c : 1;
			unsigned int rdrand : 1;
			unsigned int rsvd2 : 1;
		} ecx;
		struct {
			unsigned int fpu : 1;
			unsigned int vme : 1;
			unsigned int de : 1;
			unsigned int pse : 1;
			unsigned int tsc : 1;
			unsigned int msr : 1;
			unsigned int pae : 1;
			unsigned int mce : 1;
			unsigned int cx8 : 1;
			unsigned int apic : 1;
			unsigned int rsvd1 : 1;
			unsigned int sep : 1;
			unsigned int mtrr : 1;
			unsigned int pge : 1;
			unsigned int mca : 1;
			unsigned int cmov : 1;
			unsigned int pat : 1;
			unsigned int pse36 : 1;
			unsigned int psn : 1;
			unsigned int clflush : 1;
			unsigned int rsvd2 : 1;
			unsigned int ds : 1;
			unsigned int acpi : 1;
			unsigned int mmx : 1;
			unsigned int fxsr : 1;
			unsigned int sse : 1;
			unsigned int sse2 : 1;
			unsigned int ss : 1;
			unsigned int htt : 1;
			unsigned int tm : 1;
			unsigned int rsvd3 : 1;
			unsigned int pbe : 1;
		}edx;
	} regs;
};

// 0xc0000080
union __ia32_efer_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64 syscall_enable : 1;
		unsigned __int64 rsvd0 : 7;
		unsigned __int64 long_mode_enable : 1;
		unsigned __int64 rsvd1 : 1;
		unsigned __int64 long_mode_active : 1;
		unsigned __int64 ececute_disable : 1;
		unsigned __int64 rsvd2 : 52;
	} bits;
};

// 0x3a
union __ia32_feature_control_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64 lock : 1;
		unsigned __int64 vmxon_inside_smx : 1;
		unsigned __int64 vmxon_outside_smx : 1;
		unsigned __int64 rsvd0 : 5;
		unsigned __int64 senter_local : 6;
		unsigned __int64 senter_global : 1;
		unsigned __int64 rsvd1;
		unsigned __int64 sgx_launch_control_enable : 1;
		unsigned __int64 sgx_global_enable : 1;
		unsigned __int64 rsvd2 : 1;
		unsigned __int64 lmce : 1;
		unsigned __int64 system_rsvd : 42;
	} bits;
};

// 0x485
union __ia32_vmx_misc_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64 vmx_preemption_tsc_rate : 5;
		unsigned __int64 store_lma_in_vmentry_control : 1;
		unsigned __int64 activate_state_bitmap : 3;
		unsigned __int64 rsvd0 : 5;
		unsigned __int64 pt_in_vmx : 1;
		unsigned __int64 rdmsr_in_smm : 1;
		unsigned __int64 cr3_target_value_count : 9;
		unsigned __int64 max_msr_mvexit : 3;
		unsigned __int64 allow_smi_blocking : 3;
		unsigned __int64 vmwrite_to_any : 1;
		unsigned __int64 interrupt_mod : 1;
		unsigned __int64 rsvd1 : 1;
		unsigned __int64 mseg_revision_identifier : 32;
	} bits;
};

// 0x480
union __ia32_vmx_basic_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64 vmcs_revision_idetifier : 31;
		unsigned __int64 always_0 : 1;
		unsigned __int64 vmxon_region_size : 13;
		unsigned __int64 rsvd0 : 3;
		unsigned __int64 vmxon_physical_address_width : 1;
		unsigned __int64 dual_monitor_smi : 1;
		unsigned __int64 memory_type : 4;
		unsigned __int64 io_instruction_reporting : 1;
		unsigned __int64 true_controls : 1;
	} bits;
};

// 0x481
union __ia32_pinbased_ctl_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64 external_interrupt_exiting : 1;
		unsigned __int64 rsvd0 : 2;
		unsigned __int64 nmi_exiting : 1;
		unsigned __int64 rsvd1 : 1;
		unsigned __int64 virtual_nmis : 1;
		unsigned __int64 vmx_preemption_timer : 1;
		unsigned __int64 process_posted_interrupts : 1;
	} bits;
};

// 0x 482
union __ia32_vmx_primary_processor_based_ctl_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64 rsvd0 : 2;
		unsigned __int64 interrupt_window_exiting : 1;
		unsigned __int64 use_tsc_offsetting : 1;
		unsigned __int64 rsvd1 : 3;
		unsigned __int64 htl_exiting : 1;
		unsigned __int64 rsvd2 : 1;
		unsigned __int64 invldpg_exiting : 1;
		unsigned __int64 mwait_exiting : 1;
		unsigned __int64 rdpmc_exiting : 1;
		unsigned __int64 rdtsc_exiting : 1;
		unsigned __int64 rsvd3 : 2;
		unsigned __int64 cr3_load_exiting : 1;
		unsigned __int64 cr3_store_exiting : 1;
		unsigned __int64 rsvd4 : 2;
		unsigned __int64 cr8_load_exiting : 1;
		unsigned __int64 cr8_store_exiting : 1;
		unsigned __int64 use_tpr_shadow : 1;
		unsigned __int64 nmi_window_exiting : 1;
		unsigned __int64 mov_dr_exiting : 1;
		unsigned __int64 unconditional_io_exiting : 1;
		unsigned __int64 use_io_bitmaps : 1;
		unsigned __int64 rsvd5 : 1;
		unsigned __int64 monitor_trap_flags : 1;
		unsigned __int64 use_msr_bitmaps : 1;
		unsigned __int64 monitor_exiting : 1;
		unsigned __int64 pause_exiting : 1;
		unsigned __int64 active_secondary_controls : 1;
	} bits;
};

// 0x48b
union __ia32_vmx_secondary_processor_based_ctl_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64 virtualize_apic_accesses : 1;
		unsigned __int64 enable_ept : 1;
		unsigned __int64 descriptor_table_exiting : 1;
		unsigned __int64 enable_rdtscp : 1;
		unsigned __int64 virtualize_x2apic : 1;
		unsigned __int64 enable_vpid : 1;
		unsigned __int64 wbinvd_exiting : 1;
		unsigned __int64 unrestricted_guest : 1;
		unsigned __int64 apic_register_virtualization : 1;
		unsigned __int64 virtual_interrupt_delivery : 1;
		unsigned __int64 pause_loop_exiting : 1;
		unsigned __int64 rdrand_exiting : 1;
		unsigned __int64 enable_invpcid : 1;
		unsigned __int64 enable_vmfunc : 1;
		unsigned __int64 vmcs_shadowing : 1;
		unsigned __int64 enable_encls_exiting : 1;
		unsigned __int64 rdseed_exiting : 1;
		unsigned __int64 enable_pml : 1;
		unsigned __int64 use_virtualization_exception : 1;
		unsigned __int64 conceal_vmx_from_pt : 1;
		unsigned __int64 enable_xsave_xrstor : 1;
		unsigned __int64 rsvd0 : 2;
		unsigned __int64 use_tsc_scaling : 1;
	} bits;
};

// 0x483
union __ia32_vmx_exit_ctl_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64 rsvd0 : 2;
		unsigned __int64 save_dbg_controls : 1;
		unsigned __int64 rsvd1 : 6;
		unsigned __int64 host_address_space_size : 1;
		unsigned __int64 rsvd2 : 2;
		unsigned __int64 load_ia32_perf_global_control : 1;
		unsigned __int64 rsvd3 : 2;
		unsigned __int64 ack_interrupt_on_exit : 1;
		unsigned __int64 rsvd4 : 2;
		unsigned __int64 save_ia32_pat : 1;
		unsigned __int64 load_ia32_pat : 1;
		unsigned __int64 save_ia32_efer : 1;
		unsigned __int64 load_ia32_efer : 1;
		unsigned __int64 save_vmx_preemption_timer_value : 1;
		unsigned __int64 clear_ia32_bndconfigs : 1;
		unsigned __int64 conceal_vmx_from_pt : 1;
	} bits;
};

// 0x484
union __ia32_vmx_entry_ctl_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64 rsvd0 : 2;
		unsigned __int64 load_dbg_controls : 1;
		unsigned __int64 rsvd1 : 6;
		unsigned __int64 ia32e_mode_guest : 1;
		unsigned __int64 entry_to_smm : 1;
		unsigned __int64 deactivate_dual_monitor_treatment : 1;
		unsigned __int64 rsvd3 : 1;
		unsigned __int64 load_ia32_perf_global_control : 1;
		unsigned __int64 load_ia32_pat : 1;
		unsigned __int64 load_ia32_efer : 1;
		unsigned __int64 load_ia32_bndconfigs : 1;
		unsigned __int64 conceal_vmx_from_pt : 1;
	} bits;
};