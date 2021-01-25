#include <ntddk.h>
#include "defs.h"
#include "VmxHeader.h"

#include <stdarg.h>

unsigned int VmHasVmxSupport();
void logit(ULONG level, const char *fmt, ...);

NTSTATUS
DriverEntry(
	PDRIVER_OBJECT DriverObject,
	PUNICODE_STRING RegistryPath
)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	DbgPrint("hey its me!!");
	DbgPrint("VMX is %s", VmHasVmxSupport() == 1u ? "SUPPORTED" : "NOT SUPPORTED");
//
//#define DPFLTR_ERROR_LEVEL 0
//#define DPFLTR_WARNING_LEVEL 1
//#define DPFLTR_TRACE_LEVEL 2
//#define DPFLTR_INFO_LEVEL 3

	logit(DPFLTR_ERROR_LEVEL, "%s", "DPFLTR_ERROR_LEVEL");
	logit(DPFLTR_WARNING_LEVEL, "%s", "DPFLTR_WARNING_LEVEL");
	logit(DPFLTR_TRACE_LEVEL, "%s", "DPFLTR_TRACE_LEVEL");
	logit(DPFLTR_INFO_LEVEL, "%s", "DPFLTR_INFO_LEVEL");

	return STATUS_SUCCESS;
}


unsigned int VmHasVmxSupport() {
	union __cpuid_t cpuid = {0};

	__cpuid(cpuid.cpu_info, 1);

	return cpuid.regs.ecx.vmx;
}

void logit(ULONG level, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	vDbgPrintEx(DPFLTR_IHVDRIVER_ID, level, fmt, args);

	va_end(args);
}