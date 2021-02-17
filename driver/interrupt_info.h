#pragma once


union __vmentry_interruption_info_t {
	unsigned __int32 all;
	struct {
		unsigned __int32
			vector : 8,
			interruption_type : 3,
			deliver_error_code : 1,
			: 19,
			valid : 1;
	} bits;
};

struct __vmentry_event_information_t {
	union __vmentry_interruption_info_t interrupt_info;
	unsigned __int32 instruction_length;
	unsigned __int64 error_code;
};

enum apic_exception_vectors_e {
	exception_divide_error,
	exception_debug_breakpoint,
	exception_nmi,
	exception_breakpoint,
	exception_overflow,
	exception_bound_range_exceeded,
	exception_undefined_opcode,
	exception_no_math_coprocessor,
	exception_double_fault,
	exception_reserved0,
	exception_invalid_task_segment_selector,
	exception_segment_not_present,
	exception_stack_segment_fault,
	exception_general_protection_fault,
	exception_page_fault,
	exception_reserved1,
	exception_math_fault,
	exception_alignment_check,
	exception_machine_check,
	exception_simd_floating_point_numeric_error,
	exception_virtual_exception
};

enum __interrupt_type_e {
	interrupt_type_external_interrupt,
	interrupt_type_reserved,
	interrupt_type_nmi,
	interrupt_type_hardware_exception,
	interrupt_type_software_interrupt,
	interrupt_type_priviledged_software_exception,
	interrupt_type_software_exception,
	interrupt_type_other
};
