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
	// control 16-bit
	VPID = VMCS_ENCODE_COMPONENT_FULL_16(control, 0),
	PostInterruptNotificationVector = VMCS_ENCODE_COMPONENT_FULL_16(control, 1),
	ETPTIndex= VMCS_ENCODE_COMPONENT_FULL_16(control, 2),

	// control 32-bit
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
	VmEntryInterruptInfoField = VMCS_ENCODE_COMPONENT_FULL_32(control, 11),
	VmEntryExceptionErrorCode = VMCS_ENCODE_COMPONENT_FULL_32(control, 12),
	VmEntryInstructionLength = VMCS_ENCODE_COMPONENT_FULL_32(control, 13),
	TprThreshold = VMCS_ENCODE_COMPONENT_FULL_32(control, 14),
	SecondaryProcBasedVmExecutionControls = VMCS_ENCODE_COMPONENT_FULL_32(control, 15),
	PleGap = VMCS_ENCODE_COMPONENT_FULL_32(control, 16),
	PleWindow = VMCS_ENCODE_COMPONENT_FULL_32(control, 17),
	
	// control 64-bit
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

	// control natural width
	Cr0GuestHostMask = VMCS_ENCODE_COMPONENT_FULL(control, natural, 0),
	Cr4GuestHostMask = VMCS_ENCODE_COMPONENT_FULL(control, natural, 1),
	Cr0ReadShadow = VMCS_ENCODE_COMPONENT_FULL(control, natural, 2),
	Cr4ReadShadow = VMCS_ENCODE_COMPONENT_FULL(control, natural, 3),
	Cr3TargetValue0 = VMCS_ENCODE_COMPONENT_FULL(control, natural, 4),
	Cr3TargetValue1 = VMCS_ENCODE_COMPONENT_FULL(control, natural, 5),
	Cr3TargetValue2 = VMCS_ENCODE_COMPONENT_FULL(control, natural, 6),
	Cr3TargetValue3 = VMCS_ENCODE_COMPONENT_FULL(control, natural, 7),

	// Guest 16-bit
	GuestEsSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 0),
	GuestCsSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 1),
	GuestSsSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 2),
	GuestDsSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 3),
	GuestFsSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 4),
	GuestGsSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 5),
	GuestLdtrSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 6),
	GuestTrSelector = VMCS_ENCODE_COMPONENT_FULL_16(guest, 7),
	GuestInterruptStatus = VMCS_ENCODE_COMPONENT_FULL_16(guest, 8),
	PmlIndex = VMCS_ENCODE_COMPONENT_FULL_16(guest, 9),

	// Guest 32-bit
	GuestEsLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 0),
	GuestCsLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 1),
	GuestSsLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 2),
	GuestDsLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 3),
	GuestFsLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 4),
	GuestGsLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 5),
	GuestLdtrLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 6),
	GuestTrLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 7),
	GuestGdtrLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 8),
	GuestIdtrLimit = VMCS_ENCODE_COMPONENT_FULL_32(guest, 9),
	GuestEsAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 10),
	GuestCsAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 11),
	GuestSsAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 12),
	GuestDsAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 13),
	GuestFsAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 14),
	GuestGsAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 15),
	GuestLdtrAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 16),
	GuestTrAccessRights = VMCS_ENCODE_COMPONENT_FULL_32(guest, 17),
	GuestInterruptibilityState = VMCS_ENCODE_COMPONENT_FULL_32(guest, 18),
	GuestActivityState = VMCS_ENCODE_COMPONENT_FULL_32(guest, 19),
	GuestSmbase = VMCS_ENCODE_COMPONENT_FULL_32(guest, 20),
	GuestIa32SysenterCs = VMCS_ENCODE_COMPONENT_FULL_32(guest, 21),
		//////////////////////////////////////////// wait... where's 22?? <--------------------
	VmxPreemptionTimerValue = VMCS_ENCODE_COMPONENT_FULL_32(guest, 23),

	// Guest 64-bit
	VmcsLinkpointer = VMCS_ENCODE_COMPONENT_FULL_64(guest, 0),
	GuestIa32Debugctl = VMCS_ENCODE_COMPONENT_FULL_64(guest, 1),
	GuestIa32Pat = VMCS_ENCODE_COMPONENT_FULL_64(guest, 2),
	GuestIa32Efer = VMCS_ENCODE_COMPONENT_FULL_64(guest, 3),
	GuestPerfGlobalCtrl = VMCS_ENCODE_COMPONENT_FULL_64(guest, 4),
	GuestPdpte0 = VMCS_ENCODE_COMPONENT_FULL_64(guest, 5),
	GuestPdpte1 = VMCS_ENCODE_COMPONENT_FULL_64(guest, 6),
	GuestPdpte2 = VMCS_ENCODE_COMPONENT_FULL_64(guest, 7),
	GuestPdpte3 = VMCS_ENCODE_COMPONENT_FULL_64(guest, 8),
	GuestIa32Bindcfgs = VMCS_ENCODE_COMPONENT_FULL_64(guest, 9),
	GuestIa32RtitCtl = VMCS_ENCODE_COMPONENT_FULL_64(guest, 10),
		//////////////////////////////////////////// wait... where's 11?? <--------------------
	GuestIa32Pkrs = VMCS_ENCODE_COMPONENT_FULL_64(guest, 12),

	// Guest natural width
	Cr0GuestHostMask = VMCS_ENCODE_COMPONENT_FULL(guest, natural, ),
};