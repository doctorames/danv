#pragma once

union __cpuid_t {
	unsigned int cpu_info[4];
	struct {
		struct {
			unsigned int
				steppingId : 4,
				model : 4,
				familyId : 4,
				processorType : 2,
				: 2,
				extModelId : 4,
				extFamilyId : 8,
				: 4;
		} eax;
		struct {
			unsigned int
				brandIndex : 8,
				cflushLineSize : 8,
				maxAddressableIds : 8,
				initialApicId : 8;
		} ebx;
		struct {
			unsigned int
				sse3 : 1,
				carrilessMult : 1,
				dsArea64Bit : 1,
				monitorMwait : 1,
				cplQualifiedDebugStore : 1,
				vmx : 1,
				smx : 1,
				eist : 1,
				thermalMonitor2 : 1,
				ssse3 : 1,
				l1ContextId : 1,
				sdbg : 1,
				fusedMultiplyAdd : 1,
				cmpxchg16b : 1,
				xtprUpdateControl : 1,
				perfDbgCapMsr : 1,
				: 1,
				processContextIds : 1,
				dca : 1,
				sse4_1 : 1,
				sse4_2 : 1,
				x2Apic : 1,
				movbe : 1,
				popcnt : 1,
				tscDeadline : 1,
				aes : 1,
				xsave : 1,
				osxsave : 1,
				avx : 1,
				f16c : 1,
				rdrand : 1,
				: 1;
		} ecx;
		struct {
			unsigned int
				fpu : 1,
				vme : 1,
				de : 1,
				pse : 1,
				tsc : 1,
				msr : 1,
				pae : 1,
				mce : 1,
				cx8 : 1,
				apic : 1,
				: 1,
				sep : 1,
				mtrr : 1,
				pge : 1,
				mca : 1,
				cmov : 1,
				pat : 1,
				pse36 : 1,
				psn : 1,
				clflush : 1,
				: 1,
				ds : 1,
				acpi : 1,
				mmx : 1,
				fxsr : 1,
				sse : 1,
				sse2 : 1,
				ss : 1,
				htt : 1,
				tm : 1,
				: 1,
				pbe : 1;
		}edx;
	} regs;
};

#pragma warning(push)
#pragma warning(disable: 4214)

// 0xc0000080
#define IA32_EFER_MSR  0xc0000080
union __ia32_efer_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64
			syscall_enable : 1,
			: 7,
			long_mode_enable : 1,
			: 1,
			long_mode_active : 1,
			ececute_disable : 1,
			: 52;
	} bits;
};

// 0x3a
#define IA32_FEATURE_CONTROL_MSR  0x3a
union __ia32_feature_control_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64
			lock : 1,
			vmxon_inside_smx : 1,
			vmxon_outside_smx : 1,
			: 5,
			senter_local : 6,
			senter_global : 1,
			: 1,
			sgx_launch_control_enable : 1,
			sgx_global_enable : 1,
			: 1,
			lmce : 1,
			system_rsvd : 42;
	} bits;
};

// 0x485
#define IA32_VMX_MISC_MSR  0x485
union __ia32_vmx_misc_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64
			vmx_preemption_tsc_rate : 5,
			store_lma_in_vmentry_control : 1,
			activate_state_bitmap : 3,
			: 5,
			pt_in_vmx : 1,
			rdmsr_in_smm : 1,
			cr3_target_value_count : 9,
			max_msr_mvexit : 3,
			allow_smi_blocking : 3,
			vmwrite_to_any : 1,
			interrupt_mod : 1,
			: 1,
			mseg_revision_identifier : 32;
	} bits;
};

// 0x480
#define IA32_VMX_BASIC_MSR  0x480
union __ia32_vmx_basic_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64
			vmcs_revision_identifier : 31,
			always_0 : 1,
			vmxon_region_size : 13,
			: 3,
			vmxon_physical_address_width : 1,
			dual_monitor_smi : 1,
			memory_type : 4,
			io_instruction_reporting : 1,
			true_controls : 1;
	} bits;
};

// 0x481
#define IA32_PINBASED_CTL_MSR  0x481
union __ia32_pinbased_ctl_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64
			external_interrupt_exiting : 1,
			: 2,
			nmi_exiting : 1,
			: 1,
			virtual_nmis : 1,
			vmx_preemption_timer : 1,
			process_posted_interrupts : 1;
	} bits;
};

// 0x482
#define IA32_VMX_PRIMARY_PROCESSOR_BASED_CTL_MSR  0x482
union __ia32_vmx_primary_processor_based_ctl_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64
			: 2,
			interrupt_window_exiting : 1,
			use_tsc_offsetting : 1,
			: 3,
			htl_exiting : 1,
			: 1,
			invldpg_exiting : 1,
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
			monitor_trap_flags : 1,
			use_msr_bitmaps : 1,
			monitor_exiting : 1,
			pause_exiting : 1,
			active_secondary_controls : 1;
	} bits;
};

// 0x48b
#define IA32_VMX_SECONDARY_PROCESSOR_BASED_CTL_MSR  0x48b
union __ia32_vmx_secondary_processor_based_ctl_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64
			virtualize_apic_accesses : 1,
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
			: 2,
			use_tsc_scaling : 1;
	} bits;
};

// 0x483
#define IA32_VMX_EXIT_CTL_MSR  0x483
union __ia32_vmx_exit_ctl_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64
			: 2,
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
			save_vmx_preemption_timer_value : 1,
			clear_ia32_bndconfigs : 1,
			conceal_vmx_from_pt : 1;
	} bits;
};

// 0x484
#define IA32_VMX_ENTRY_CTL_MSR  0x484
union __ia32_vmx_entry_ctl_msr {
	unsigned __int64 control;
	struct {
		unsigned __int64
			: 2,
			load_dbg_controls : 1,
			: 6,
			ia32e_mode_guest : 1,
			entry_to_smm : 1,
			deactivate_dual_monitor_treatment : 1,
			: 1,
			load_ia32_perf_global_control : 1,
			load_ia32_pat : 1,
			load_ia32_efer : 1,
			load_ia32_bndconfigs : 1,
			conceal_vmx_from_pt : 1;
	} bits;
};

#pragma warning(pop)

#define IA32_VMX_CR0_FIXED0 0x486
#define IA32_VMX_CR0_FIXED1 0x487

#define IA32_VMX_CR4_FIXED0 0x488
#define IA32_VMX_CR4_FIXED1 0x489

union __ia32_vmx_cr_fixed_msr {
	struct {
		unsigned long low;
		long high;
	} split;
	long long all;
};



