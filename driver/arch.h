#pragma once

#pragma warning(push)
#pragma warning(disable: 4214)
#pragma warning(disable: 4201)
union __cr4_t {
	unsigned int all;
	struct {
		unsigned int
			vme : 1, // Virtual 8086 Mode Extensions
			pvi : 1, // Protected Mode Virtual Interrupts
			tsd : 1, // Time Stamp Disable
			de : 1,  // Debugging Extensions
			pse : 1, // Page Size Extensions
			pae : 1, // PAE
			mce : 1, // Machine Check Enable (enables MCEs)
			pge : 1, // Page Global Enable
			pce : 1, // Perfmon Counter Enable (allows RDPMC to be run from any protection level)
			osfxsr : 1, // OS Support for FXSAVE and FXSTOR instructions
			osxmmexcpt : 1, // OS Support for Unmasked SIMD Floating Point Exceptions
			umip : 1, // User Mode Instruction Prevention (throws #GP if you try to do SGDT, SIDT, SLDT, SMSW, or STR when CPL > 0)
			la57 : 1, // 57 bit Linear Address (see Vol 3: ch 4)
			vmxe : 1, // VMX Enable bit
			smxe : 1, // SMX Enable bit
			: 1,
			fsgsbase : 1, // FSGSBASE Enable bit (enables RDFSBASE, RDGSBASE, WRFSBASE, WRGSBASE - whatever that is)
			pcide : 1, // PCID Enable bit (Process Context Identifiers. See Vol3: 4.10.1. IA-32e mode only)
			osxsave : 1, // XSAVE and Processor Extended States Enable bit
			rsvd1 : 1,
			smep : 1, // SMEP Enable bit (Supervisor Mode Execution Prevention. See Vol3: 4.6)
			smap : 1, // SMAP Enable bit (Supervisor Mode Access Prevention. See Vol3: 4.6)
			pke : 1, // Enable Protection Keys for user mode pages
			cet : 1, // Control-Flow Enforcement Technology
			pks : 1, // Enable protection keys for supervisor-mode pages
			: 7;
	} bits;
};

union __cr0_t {
	unsigned int all;
	struct {
		unsigned int
			pe : 1, // Protection Enable (enables protected mode)
			mp : 1, // Monitor Coprocessor
			em : 1, // Emulation
			ts : 1, // Task Switched
			et : 1, // Extension Type (archaic)
			ne : 1, // Numeric Error
			: 10,
			wp : 1, // Write Protect
			: 1,
			am : 1, // Alignment Mask
			: 10,
			nw : 1, // Not Write-through
			cd : 1, // Cache Disable
			pg : 1; // Paging
	} bits;
};

union __rflags_t {
	unsigned __int64 all;
	struct {
		unsigned __int64
			carry : 1,
			: 1,
			parity : 1,
			: 1,
			aux_carry : 1,
			: 1,
			zero : 1,
			sign : 1,
			trap : 1,
			interrupt_enable : 1,
			direction : 1,
			overflow : 1,
			io_priv_level : 2,
			nested_task : 1,
			: 1,
			resume : 1,
			virt8086_mode : 1,
			alignment_check_access_control : 1,
			virt_interrupt : 1,
			virt_interrupt_pending : 1,
			id : 1,
			: 10,
			: 32;  // RFLAGS is just EFLAGS with the upper 32 bits unused
	} bits;
};

union __interrupt_command_register_t {
	unsigned __int64 full;
	struct {
		unsigned __int64
			vector : 8,
			delivery_mode : 3,
			destination_mode : 1,
			delivery_status : 1,
			: 1,
			level : 1,
			trigger_mode : 1,
			: 2,
			destination_short : 2,
			: 35,
			destination_shorthand : 8;
	} bits;
};

// 3.7.2.3
struct __segment_descriptor_64_t {
	unsigned __int16 segment_limit_low;
	unsigned __int16 base_low;
	union {
		struct {
			unsigned __int32
				base_middle : 8,
				segment_type : 4,
				descriptor_type : 1,
				dpl : 2,
				present : 1,
				segment_limit_high : 4,
				system : 1,
				long_mode : 1,
				default_big : 1,
				granularity : 1,
				base_high : 8;
		};
		unsigned __int32 flags;
	};
	unsigned __int32 base_upper;
	unsigned __int32 reserved;
};

// 3.3.5
#define SEG_DESC_TYPE_TSS_AVAILABLE  9
#define SEG_DESC_TYPE_TSS_BUSY       10

// 3.3.4.5
struct __segment_descriptor_32_t {
	unsigned __int16 segment_limit_low;
	unsigned __int16 base_low;
	union {
		struct {
			unsigned __int32
				base_middle : 8,
				type : 4,
				descriptor_type : 1,
				dpl : 2,
				present : 1,
				segment_limit_high : 4,
				system : 1,
				long_mode : 1,
				default_big : 1,
				granularity : 1,
				base_high : 8;
		};
		unsigned __int32 flags;
	};
};

union __segment_selector_t {
	struct {
		unsigned __int16
			rpl : 2,
			table : 1,
			index : 13;
	};
	unsigned __int16 flags;
};

// 3.24.4.1
union __segment_access_rights_t {
	struct {
		unsigned __int32
			type : 4,
			descriptor_type : 1,
			dpl : 2,
			present : 1,
			rsvd0 : 4,
			available : 1,
			long_mode : 1,
			default_big : 1,
			granularity : 1,
			unusable : 1,
			rsvd1 : 15;
	};
	unsigned __int32 flags;
};

#pragma warning(pop)

#pragma pack(push,1)
struct __pseudo_descriptor_48_t {
	unsigned __int16 limit;
	unsigned __int32 base_address;
};

#pragma pack(pop)






