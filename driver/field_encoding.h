#pragma once
enum __vmcs_access_e {
	full = 0,
	high = 1
};

enum __vmcs_type_e {
	control = 0,
	vmexit,
	guest,
	host
};

enum __vmcs_width_e {
	word = 0,
	quadword,
	doubleword,
	natural
};

#define VMCS_ENCODE_COMPONENT(access, type, width, index) \
(unsigned) (\
(unsigned short) access | \
((unsigned short) index << 1) | \
((unsigned short) type << 10) | \
((unsigned short) width << 13) )

#define	VMCS_ENCODE_COMPONENT_FULL(type, width, index)  VMCS_ENCODE_COMPONENT(full, type, width, index)
#define VMCS_ENCODE_COMPONENT_FULL_16(type, index)  VMCS_ENCODE_COMPONENT_FULL(type, word, index)
#define VMCS_ENCODE_COMPONENT_FULL_32(type, index)  VMCS_ENCODE_COMPONENT_FULL(type, doubleword, index)
#define VMCS_ENCODE_COMPONENT_FULL_64(type, index)  VMCS_ENCODE_COMPONENT_FULL(type, quadword, index)

enum __vmcs_encoding_e {

	// Control 16-bit
	Vpid = VMCS_ENCODE_COMPONENT_FULL_16(control, 0),
	PostInterruptNotificationVector = VMCS_ENCODE_COMPONENT_FULL_16(control, 1),
	EtptIndex= VMCS_ENCODE_COMPONENT_FULL_16(control, 2),

	// Control 32-bit
	PinBasedVmExecutionControls = VMCS_ENCODE_COMPONENT_FULL_32(control, 0),
	PrimaryProcBasedVmExecutionControls = VMCS_ENCODE_COMPONENT_FULL_32(control, 1),
	ExecutionBitmap = VMCS_ENCODE_COMPONENT_FULL_32(control, 2),
	PageFaultErrorCodeMask = VMCS_ENCODE_COMPONENT_FULL_32(control, 3),
	PageFaultErrorCodeMatch = VMCS_ENCODE_COMPONENT_FULL_32(control, 4),
	Cr3TargetCount = VMCS_ENCODE_COMPONENT_FULL_32(control, 5),
	VmExitControls = VMCS_ENCODE_COMPONENT_FULL_32(control, 6),
	VmExitMsrStoreCount = VMCS_ENCODE_COMPONENT_FULL_32(control, 7),
	VmExitMsrLoadCount = VMCS_ENCODE_COMPONENT_FULL_32(control, 8),
	VmEntryControls = VMCS_ENCODE_COMPONENT_FULL_32(control, 9),
	VmEntryMsrLoadCount = VMCS_ENCODE_COMPONENT_FULL_32(control, 10),
	VmEntryInterruptionInfoField = VMCS_ENCODE_COMPONENT_FULL_32(control, 11),
	VmEntryExceptionErrorCode = VMCS_ENCODE_COMPONENT_FULL_32(control, 12),
	VmEntryInstructionLength = VMCS_ENCODE_COMPONENT_FULL_32(control, 13),
	TprThreshold = VMCS_ENCODE_COMPONENT_FULL_32(control, 14),
	SecondaryProcBasedVmExecutionControls = VMCS_ENCODE_COMPONENT_FULL_32(control, 15),
	PleGap = VMCS_ENCODE_COMPONENT_FULL_32(control, 16),
	PleWindow = VMCS_ENCODE_COMPONENT_FULL_32(control, 17),
	
	// Control 64-bit
	AddrOfIoBitmapA = VMCS_ENCODE_COMPONENT_FULL_64(control, 0),
	AddrOfIoBitmapB = VMCS_ENCODE_COMPONENT_FULL_64(control, 1),
	AddrOfMsrBitmaps = VMCS_ENCODE_COMPONENT_FULL_64(control, 2),
	VmExitMsrStoreAddr = VMCS_ENCODE_COMPONENT_FULL_64(control, 3),
	VmExitMsrLoadAddr = VMCS_ENCODE_COMPONENT_FULL_64(control, 4),
	VmEntryMsrLoadAddr = VMCS_ENCODE_COMPONENT_FULL_64(control, 5),
	ExecutiveVmcsPointer = VMCS_ENCODE_COMPONENT_FULL_64(control, 6),
	PmlAddr = VMCS_ENCODE_COMPONENT_FULL_64(control, 7),
	TscOffset = VMCS_ENCODE_COMPONENT_FULL_64(control, 8),
	VirtualApicAddr = VMCS_ENCODE_COMPONENT_FULL_64(control, 9),
	ApicAccessAddr = VMCS_ENCODE_COMPONENT_FULL_64(control, 10),
	PostedInterruptDescriptorAddr = VMCS_ENCODE_COMPONENT_FULL_64(control, 11),
	VmFunctionControls = VMCS_ENCODE_COMPONENT_FULL_64(control, 12),
	EptPointer = VMCS_ENCODE_COMPONENT_FULL_64(control, 13),
	EoiExitBitmap0 = VMCS_ENCODE_COMPONENT_FULL_64(control, 14),
	EoiExitBitmap1 = VMCS_ENCODE_COMPONENT_FULL_64(control, 15),
	EoiExitBitmap2 = VMCS_ENCODE_COMPONENT_FULL_64(control, 16),
	EoiExitBitmap3 = VMCS_ENCODE_COMPONENT_FULL_64(control, 17),
	EptpListAddr = VMCS_ENCODE_COMPONENT_FULL_64(control, 18),
	VmreadBitmapAddr = VMCS_ENCODE_COMPONENT_FULL_64(control, 19),
	VmwriteBitmapAddr = VMCS_ENCODE_COMPONENT_FULL_64(control, 20),
	VirtualizationExceptionInfoAddr = VMCS_ENCODE_COMPONENT_FULL_64(control, 21),
	XssExitingBitmap = VMCS_ENCODE_COMPONENT_FULL_64(control, 22),
	EnclsExitingBitmap = VMCS_ENCODE_COMPONENT_FULL_64(control, 23),
	SubpagePermissionTablePointer = VMCS_ENCODE_COMPONENT_FULL_64(control, 24),
	TscMultiplier = VMCS_ENCODE_COMPONENT_FULL_64(control, 25),
	//////////////////////////////////////////// wait... where's 26?? <--------------------
	EnclvExitingbitmap = VMCS_ENCODE_COMPONENT_FULL_64(control, 27),

	// Control natural width
	Cr0GuestHostMask = VMCS_ENCODE_COMPONENT_FULL(control, natural, 0),
	Cr4GuestHostMask = VMCS_ENCODE_COMPONENT_FULL(control, natural, 1),
	Cr0ReadShadow = VMCS_ENCODE_COMPONENT_FULL(control, natural, 2),
	Cr4ReadShadow = VMCS_ENCODE_COMPONENT_FULL(control, natural, 3),
	Cr3TargetValue0 = VMCS_ENCODE_COMPONENT_FULL(control, natural, 4),
	Cr3TargetValue1 = VMCS_ENCODE_COMPONENT_FULL(control, natural, 5),
	Cr3TargetValue2 = VMCS_ENCODE_COMPONENT_FULL(control, natural, 6),
	Cr3TargetValue3 = VMCS_ENCODE_COMPONENT_FULL(control, natural, 7),

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Guest 16-bit
	GuestEsSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 0), // register
	GuestCsSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 1), // register
	GuestSsSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 2), // register
	GuestDsSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 3), // register
	GuestFsSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 4), // register
	GuestGsSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 5), // register
	GuestLdtrSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 6), // register
	GuestTrSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 7), // register
	GuestInterruptStatus = VMCS_ENCODE_COMPONENT_FULL_16(guest, 8),
	PmlIndex = VMCS_ENCODE_COMPONENT_FULL_16(guest, 9),

	// Guest 32-bit
	GuestEsLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 0), // register
	GuestCsLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 1), // register
	GuestSsLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 2), // register
	GuestDsLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 3), // register
	GuestFsLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 4), // register
	GuestGsLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 5), // register
	GuestLdtrLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 6), // register
	GuestTrLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 7), // register
	GuestGdtrLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 8), // register
	GuestIdtrLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 9), // register
	GuestEsAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 10), // register
	GuestCsAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 11), // register
	GuestSsAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 12), // register
	GuestDsAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 13), // register
	GuestFsAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 14), // register
	GuestGsAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 15), // register
	GuestLdtrAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 16), // register
	GuestTrAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 17), // register
	GuestInterruptibilityState = VMCS_ENCODE_COMPONENT_FULL_32(guest, 18),
	GuestActivityState = VMCS_ENCODE_COMPONENT_FULL_32(guest, 19),
	GuestSmbase = VMCS_ENCODE_COMPONENT_FULL_32(guest, 20), // register
	GuestIa32SysenterCs = VMCS_ENCODE_COMPONENT_FULL_32(guest, 21), // register
		//////////////////////////////////////////// wait... where's 22?? <--------------------
	VmxPreemptionTimerValue = VMCS_ENCODE_COMPONENT_FULL_32(guest, 23),

	// Guest 64-bit
	VmcsLinkPointer = VMCS_ENCODE_COMPONENT_FULL_64(guest, 0), // register
	GuestIa32Debugctl = VMCS_ENCODE_COMPONENT_FULL_64(guest, 1), // register
	GuestIa32Pat = VMCS_ENCODE_COMPONENT_FULL_64(guest, 2), // register
	GuestIa32Efer = VMCS_ENCODE_COMPONENT_FULL_64(guest, 3), // register
	GuestPerfGlobalCtrl = VMCS_ENCODE_COMPONENT_FULL_64(guest, 4), // register
	GuestPdpte0 = VMCS_ENCODE_COMPONENT_FULL_64(guest, 5),
	GuestPdpte1 = VMCS_ENCODE_COMPONENT_FULL_64(guest, 6),
	GuestPdpte2 = VMCS_ENCODE_COMPONENT_FULL_64(guest, 7),
	GuestPdpte3 = VMCS_ENCODE_COMPONENT_FULL_64(guest, 8),
	GuestIa32Bindcfgs = VMCS_ENCODE_COMPONENT_FULL_64(guest, 9), // register
	GuestIa32RtitCtl = VMCS_ENCODE_COMPONENT_FULL_64(guest, 10),
		//////////////////////////////////////////// wait... where's 11?? <--------------------
	GuestIa32Pkrs = VMCS_ENCODE_COMPONENT_FULL_64(guest, 12),

	// Guest natural width
	GuestCr0 = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 0), // register
	GuestCr3 = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 1), // register
	GuestCr4 = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 2), // register
	GuestEsBase = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 3), // register
	GuestCsBase = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 4), // register
	GuestSsBase = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 5), // register
	GuestDsBase = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 6), // register
	GuestFsBase = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 7), // register
	GuestGsBase = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 8), // register
	GuestLdtrBase = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 9), // register
	GuestTrBase = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 10), // register
	GuestGdtrBase = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 11), // register
	GuestIdtrBase = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 12), // register
	GuestDr7 = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 13), // register
	GuestRsp = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 14), // register
	GuestRip = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 15), // register
	GuestRflags = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 16), // register
	GuestPendingDebugExceptions = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 17),
	GuestIa32SysenterEsp = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 18), // register
	GuestIa32SysenterEip = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 19), // register
	GuestIa32SCet = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 20),
	GuestSsp = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 21),
	GuestIa32InterruptSspTableAddr = VMCS_ENCODE_COMPONENT_FULL(guest, natural, 22),

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Host 16-bit
	HostEsSelector = VMCS_ENCODE_COMPONENT_FULL_16(host, 0), // register
	HostCsSelector = VMCS_ENCODE_COMPONENT_FULL_16(host, 1), // register
	HostSsSelector = VMCS_ENCODE_COMPONENT_FULL_16(host, 2), // register
	HostDsSelector = VMCS_ENCODE_COMPONENT_FULL_16(host, 3), // register
	HostFsSelector = VMCS_ENCODE_COMPONENT_FULL_16(host, 4), // register
	HostGsSelector = VMCS_ENCODE_COMPONENT_FULL_16(host, 5), // register
	HostTrSelector = VMCS_ENCODE_COMPONENT_FULL_16(host, 6), // register

	// Host 32-bit
	HostIa32SysenterCs = VMCS_ENCODE_COMPONENT_FULL_32(host, 0), // register

	// Host 64-bit
	HostIa32Pat = VMCS_ENCODE_COMPONENT_FULL_64(host, 0), // register
	HostIa32Efer = VMCS_ENCODE_COMPONENT_FULL_64(host, 1), // register
	HostPerfGlobalCtrl = VMCS_ENCODE_COMPONENT_FULL_64(host, 2), // register
	HostIa32Pkrs = VMCS_ENCODE_COMPONENT_FULL_64(host, 3),

	// Host natural width
	HostCr0 = VMCS_ENCODE_COMPONENT_FULL(host, natural, 0), // register
	HostCr3 = VMCS_ENCODE_COMPONENT_FULL(host, natural, 1), // register
	HostCr4 = VMCS_ENCODE_COMPONENT_FULL(host, natural, 2), // register
	HostFsBase = VMCS_ENCODE_COMPONENT_FULL(host, natural, 3), // register
	HostGsBase = VMCS_ENCODE_COMPONENT_FULL(host, natural, 4), // register
	HostTrBase = VMCS_ENCODE_COMPONENT_FULL(host, natural, 5), // register
	HostGdtrBase = VMCS_ENCODE_COMPONENT_FULL(host, natural, 6), // register
	HostIdtrBase = VMCS_ENCODE_COMPONENT_FULL(host, natural, 7), // register
	HostIa32SysenterEsp = VMCS_ENCODE_COMPONENT_FULL(host, natural, 8), // register
	HostIa32SysenterEip = VMCS_ENCODE_COMPONENT_FULL(host, natural, 9), // register
	HostRsp = VMCS_ENCODE_COMPONENT_FULL(host, natural, 10), // register
	HostRip = VMCS_ENCODE_COMPONENT_FULL(host, natural, 11), // register
	HostIa32SCet = VMCS_ENCODE_COMPONENT_FULL(host, natural, 12),
	HostSsp = VMCS_ENCODE_COMPONENT_FULL(host, natural, 13),
	HostIa32InterruptSspTableAddr = VMCS_ENCODE_COMPONENT_FULL(host, natural, 14),

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Read only (vmexit) 16-bit
	// --- none ---

	// Read only (vmexit) 32-bit
	VmInstructionError = VMCS_ENCODE_COMPONENT_FULL_32(vmexit, 0),
	ExitReason = VMCS_ENCODE_COMPONENT_FULL_32(vmexit, 1),
	VmExitInterruptionInfo = VMCS_ENCODE_COMPONENT_FULL_32(vmexit, 2),
	VmExitInterruptionErrorCode = VMCS_ENCODE_COMPONENT_FULL_32(vmexit, 3),
	IdtVectoringInfoField = VMCS_ENCODE_COMPONENT_FULL_32(vmexit, 4),
	IdtVectoringErrorCode = VMCS_ENCODE_COMPONENT_FULL_32(vmexit, 5),
	VmExitInstructionLength = VMCS_ENCODE_COMPONENT_FULL_32(vmexit, 6),
	VmExitInstructionInfo = VMCS_ENCODE_COMPONENT_FULL_32(vmexit, 7),

	// Read only (vmexit) 64-bit
	GuestPhysicalAddr = VMCS_ENCODE_COMPONENT_FULL_64(vmexit, 0),

	// Read only (vmexit) natural width
	ExitQualification = VMCS_ENCODE_COMPONENT_FULL(vmexit, natural, 0),
	IoRcx = VMCS_ENCODE_COMPONENT_FULL(vmexit, natural, 1),
	IoRsi = VMCS_ENCODE_COMPONENT_FULL(vmexit, natural, 2),
	IoRdi = VMCS_ENCODE_COMPONENT_FULL(vmexit, natural, 3),
	IoRip = VMCS_ENCODE_COMPONENT_FULL(vmexit, natural, 4),
	GuestlinearAddr = VMCS_ENCODE_COMPONENT_FULL(vmexit, natural, 5)
};