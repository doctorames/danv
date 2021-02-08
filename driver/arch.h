#pragma once

#pragma warning(push)
#pragma warning(disable: 4214)
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

#pragma warning(pop)











/*

typedef struct _M128A
{
	UINT64 Low;
	INT64 High;
} M128A, *PM128A;

typedef struct _XSAVE_FORMAT
{
	UINT16 ControlWord;
	UINT16 StatusWord;
	UINT8 TagWord;
	UINT8 Reserved1;
	UINT16 ErrorOpcode;
	UINT32 ErrorOffset;
	UINT16 ErrorSelector;
	UINT16 Reserved2;
	UINT32 DataOffset;
	UINT16 DataSelector;
	UINT16 Reserved3;
	UINT32 MxCsr;
	UINT32 MxCsr_Mask;
	M128A FloatRegisters[8];
	M128A XmmRegisters[16];
	UINT8 Reserved4[96];
} XSAVE_FORMAT, *PXSAVE_FORMAT;
typedef XSAVE_FORMAT XMM_SAVE_AREA32, *PXMM_SAVE_AREA32;

typedef struct _CONTEXT
{
	UINT64 P1Home;
	UINT64 P2Home;
	UINT64 P3Home;
	UINT64 P4Home;
	UINT64 P5Home;
	UINT64 P6Home;
	UINT32 ContextFlags;
	UINT32 MxCsr;
	UINT16 SegCs;
	UINT16 SegDs;
	UINT16 SegEs;
	UINT16 SegFs;
	UINT16 SegGs;
	UINT16 SegSs;
	UINT32 EFlags;
	UINT64 Dr0;
	UINT64 Dr1;
	UINT64 Dr2;
	UINT64 Dr3;
	UINT64 Dr6;
	UINT64 Dr7;
	UINT64 Rax;
	UINT64 Rcx;
	UINT64 Rdx;
	UINT64 Rbx;
	UINT64 Rsp;
	UINT64 Rbp;
	UINT64 Rsi;
	UINT64 Rdi;
	UINT64 R8;
	UINT64 R9;
	UINT64 R10;
	UINT64 R11;
	UINT64 R12;
	UINT64 R13;
	UINT64 R14;
	UINT64 R15;
	UINT64 Rip;
	union
	{
		XMM_SAVE_AREA32 FltSave;
		struct
		{
			M128A Header[2];
			M128A Legacy[8];
			M128A Xmm0;
			M128A Xmm1;
			M128A Xmm2;
			M128A Xmm3;
			M128A Xmm4;
			M128A Xmm5;
			M128A Xmm6;
			M128A Xmm7;
			M128A Xmm8;
			M128A Xmm9;
			M128A Xmm10;
			M128A Xmm11;
			M128A Xmm12;
			M128A Xmm13;
			M128A Xmm14;
			M128A Xmm15;
		};
	};
	M128A VectorRegister[26];
	UINT64 VectorControl;
	UINT64 DebugControl;
	UINT64 LastBranchToRip;
	UINT64 LastBranchFromRip;
	UINT64 LastExceptionToRip;
	UINT64 LastExceptionFromRip;
} CONTEXT, *PCONTEXT;
*/