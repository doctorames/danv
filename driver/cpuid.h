#pragma once


struct __cpuid_params_64_t {
	unsigned __int64
		rax,
		rbx,
		rcx,
		rdx;
};

struct __cpuid_params_32_t {
	unsigned __int32
		eax,
		ebx,
		ecx,
		edx;
};

#define QUERY_CPUID_BIT(x, b)  ((x) & (1 << b))
#define SET_CPUID_BIT(x, b)    ((x) | (1 << b))
#define CLEAR_CPUID_BIT(x, b)  ((x) & ~(1 << b))
