#pragma once


struct __cpuid_params_t {
	unsigned __int64
		rax,
		rbx,
		rcx,
		rdx;
};

#define QUERY_CPUID_BIT(x, b)  ((x) & (1 << b))
#define SET_CPUID_BIT(x, b)    ((x) | (1 << b))
#define CLEAR_CPUID_BIT(x, b)  ((x) & ~(1 << b))
