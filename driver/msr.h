#pragma once
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
			excecute_disable : 1,
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
#define IA32_VMX_PINBASED_CTLS_MSR  0x481
#define IA32_VMX_TRUE_PINBASED_CTLS_MSR  0x48d


// 0x482
#define IA32_VMX_PROCBASED_CTLS_MSR  0x482
#define IA32_VMX_TRUE_PROCBASED_CTLS_MSR  0x48e

// 0x48b
#define IA32_VMX_PROCBASED_CTLS2_MSR  0x48b

// 0x483
#define IA32_VMX_EXIT_CTLS_MSR  0x483
#define IA32_VMX_TRUE_EXIT_CTLS_MSR   0x48f

// 0x484
#define IA32_VMX_ENTRY_CTLS_MSR  0x484
#define IA32_VMX_TRUE_ENTRY_CTLS_MSR  0x490



// 0x1d9
#define IA32_DEBUGCTL_MSR  0x1d9
#define IA32_SYSENTER_CS_MSR      0x174
#define IA32_SYSENTER_ESP_MSR     0x175
#define IA32_SYSENTER_EIP_MSR     0x176

#define IA32_FS_BASE_MSR          0xc0000100
#define IA32_GS_BASE_MSR          0xc0000101

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