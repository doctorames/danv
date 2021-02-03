#pragma once

#define VMM_TAG '_VMM'
#define VMM_STACK_SIZE (0x1000 * 6)

struct __vmcs_t {
	union {
		unsigned int all;
		struct {
			unsigned int revision_identifier : 31;
			unsigned int shadow_vmcs_indicator : 1;
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



