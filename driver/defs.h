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





