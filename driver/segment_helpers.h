#pragma once
#include "FieldEncoding.h"
#include "vmm_intrin.h"
#include "arch.h"


unsigned __int32 read_segment_access_rights(unsigned __int16 segment_selector);
unsigned __int64 get_segment_base(unsigned __int64 gdt_base, unsigned __int16 ss_in);
