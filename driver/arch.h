#pragma once

#pragma warning(push)
#pragma warning(disable: 4214)
union __cr4_t {
	unsigned int all;
	struct {
		unsigned int vme : 1; // Virtual 8086 Mode Extensions
		unsigned int pvi : 1; // Protected Mode Virtual Interrupts
		unsigned int tsd : 1; // Time Stamp Disable
		unsigned int de : 1;  // Debugging Extensions
		unsigned int pse : 1; // Page Size Extensions
		unsigned int pae : 1; // PAE
		unsigned int mce : 1; // Machine Check Enable (enables MCEs)
		unsigned int pge : 1; // Page Global Enable
		unsigned int pce : 1; // Perfmon Counter Enable (allows RDPMC to be run from any protection level)
		unsigned int osfxsr : 1; // OS Support for FXSAVE and FXSTOR instructions
		unsigned int osxmmexcpt : 1; // OS Support for Unmasked SIMD Floating Point Exceptions
		unsigned int umip : 1; // User Mode Instruction Prevention (throws #GP if you try to do SGDT, SIDT, SLDT, SMSW, or STR when CPL > 0)
		unsigned int la57 : 1; // 57 bit Linear Address (see Vol 3: ch 4)
		unsigned int vmxe : 1; // VMX Enable bit
		unsigned int smxe : 1; // SMX Enable bit
		unsigned int rsvd0 : 1;
		unsigned int fsgsbase : 1; // FSGSBASE Enable bit (enables RDFSBASE, RDGSBASE, WRFSBASE, WRGSBASE - whatever that is)
		unsigned int pcide : 1; // PCID Enable bit (Process Context Identifiers. See Vol3: 4.10.1. IA-32e mode only)
		unsigned int osxsave : 1; // XSAVE and Processor Extended States Enable bit
		unsigned int rsvd1 : 1;
		unsigned int smep : 1; // SMEP Enable bit (Supervisor Mode Execution Prevention. See Vol3: 4.6)
		unsigned int smap : 1; // SMAP Enable bit (Supervisor Mode Access Prevention. See Vol3: 4.6)
		unsigned int pke : 1; // Enable Protection Keys for user mode pages
		unsigned int cet : 1; // Control-Flow Enforcement Technology
		unsigned int pks : 1; // Enable protection keys for supervisor-mode pages
		unsigned int rsvd2 : 7;
	} bits;
};

union __cr0_t {
	unsigned int all;
	struct {
		unsigned int pe : 1; // Protection Enable (enables protected mode)
		unsigned int mp : 1; // Monitor Coprocessor
		unsigned int em : 1; // Emulation
		unsigned int ts : 1; // Task Switched
		unsigned int et : 1; // Extension Type (archaic)
		unsigned int ne : 1; // Numeric Error
		unsigned int rsvd0 : 10;
		unsigned int wp : 1; // Write Protect
		unsigned int rsvd1 : 1;
		unsigned int am : 1; // Alignment Mask
		unsigned int rsvd2 : 10;
		unsigned int nw : 1; // Not Write-through
		unsigned int cd : 1; // Cache Disable
		unsigned int pg : 1; // Paging
	} bits;
};

union __rflags_t {
	unsigned __int64 all;
	struct {
		unsigned __int64 carry : 1;
		unsigned __int64 rsvd0 : 1;
		unsigned __int64 parity : 1;
		unsigned __int64 rsvd1 : 1;
		unsigned __int64 aux_carry : 1;
		unsigned __int64 rsvd2 : 1;
		unsigned __int64 zero : 1;
		unsigned __int64 sign : 1;
		unsigned __int64 trap : 1;
		unsigned __int64 interrupt_enable : 1;
		unsigned __int64 direction : 1;
		unsigned __int64 overflow : 1;
		unsigned __int64 io_priv_level : 2;
		unsigned __int64 nested_task : 1;
		unsigned __int64 rsvd3 : 1;
		unsigned __int64 resume : 1;
		unsigned __int64 virt8086_mode : 1;
		unsigned __int64 alignment_check_access_control : 1;
		unsigned __int64 virt_interrupt : 1;
		unsigned __int64 virt_interrupt_pending : 1;
		unsigned __int64 id : 1;
		unsigned __int64 rsvd4 : 10;
		unsigned __int64 rsvd5 : 32;  // RFLAGS is just EFLAGS with the upper 32 bits unused
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