
#include "asm_helpers.h"


 unsigned __int32 read_segment_access_rights(unsigned __int16 segment_selector) {
	union __segment_selector_t selector;
	union __segment_access_rights_t vmx_ar = { 0 };

	selector.flags = segment_selector;

	// Make sure the passed-in selector aint the null selector
	if (selector.index == 0 && selector.table == 0) {
		vmx_ar.unusable = 1;
		return vmx_ar.flags;
	}

	// It aint the null selector
	vmx_ar.flags = __load_ar(segment_selector) >> 8;
	vmx_ar.unusable = 0;
	vmx_ar.rsvd0 = 0;
	vmx_ar.rsvd1 = 0;
	return vmx_ar.flags;
}

unsigned __int64 get_segment_base(unsigned __int64 gdt_base, unsigned __int16 ss_in) {
	 
	 union __segment_selector_t selector = { 0 };
	 struct __segment_descriptor_32_t *descriptor_table;
	 struct __segment_descriptor_32_t *descriptor;
	 struct __segment_descriptor_64_t *descriptor_64;
	 unsigned __int64 segment_base = 0;
	 
	 
	 selector.flags = ss_in;

	 // Check for null selector
	 if (selector.index == 0 && selector.table == 0) return 0;

	 descriptor_table = (struct __segment_descriptor_32_t*)gdt_base;
	 descriptor = &descriptor_table[selector.index];

	 // Get the base from the descriptor
	 segment_base = (unsigned __int64) ((descriptor->base_high << 24) | (descriptor->base_middle << 16) | descriptor->base_low);

	 if (descriptor->system == 0 && (descriptor->type == SEG_DESC_TYPE_TSS_AVAILABLE || descriptor->type == SEG_DESC_TYPE_TSS_BUSY)) {
		 // This is a 64 bit descriptor, so pull in the upper 32 bits
		 descriptor_64 = (struct __segment_descriptor_64_t*) descriptor;
		 segment_base |= ((unsigned __int64)descriptor_64->base_upper) << 32;
	 }
	 return segment_base;
 }

