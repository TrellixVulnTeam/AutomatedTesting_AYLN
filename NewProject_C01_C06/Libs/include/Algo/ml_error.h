//
//	ml_error.h
//
//  Created by Joon Kwon on 8/27/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#ifndef _ML_ERROR_H_
#define _ML_ERROR_H_

//#ifdef __ML_USE_AS_EMB__
#include <ml_typedef.h>
//#else
//#include <ml_typedef.h>
//#endif

/** @addtogroup ml_error
*  @{
*/

#if !defined(ML_ERROR_TABLE_BEGIN)
#	define ML_ERROR_TABLE_BEGIN enum
#endif

#if !defined(ML_ERROR_TABLE_END)
#	define ML_ERROR_TABLE_END ;
#endif

#if !defined(ML_ERROR_DECLARE)
#	define ML_ERROR_DECLARE(err, code, exception_code) err = code,
#endif

/*------------------------------------------------------------------------------
 *	Error Codes
 */
typedef enum {
	kHandling_unknown = -1,
	kHandling_normal,
	kHandling_applePass,
} t_mlErrorHandlingRule;

ML_ERROR_TABLE_BEGIN{// Error Definition							Error Code	handling rule
	ML_ERROR_DECLARE(kMLErr_OK,										0x0000,		kHandling_normal)
	// ERROR Code Range 0x0001 to 0x0FFF are saved for inherited framework or projects //
	
	//	0xE000 ~ 0xE03F	-	Generic Error Codes
	//	parameter check
	ML_ERROR_DECLARE(kMLErr_invalidPointer,							0xE001,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidBufferSize,						0xE002,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidParam,							0xE003,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidDataSize,						0xE004,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidChecksum,						0xE005,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidData,							0xE03D,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidDataId,							0xE006,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidDeviceIndex,						0xE007,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidDataType,						0xE008,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidFormat,							0xE009,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_memoryAllocationFailed,					0xE00A,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_objectCreationFailed,					0xE00B,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_threadCreationFailed,					0xE00C,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_threadAlreadyRunning,					0xE00E,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_internalErr,							0xE00F,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_previousRequestInProgress,				0xE010,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_unkownProbeZPos,						0xE011,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_noDataAvailable,						0xE012,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_testAborted,							0xE013,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lapackError,							0xE015,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_OOS,									0xE016,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_objectNotAvailable,						0xE017,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_actionFailed,							0xE019,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_notEnoughFreeSpace,						0xE01A,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_noTestCtrlBrd,							0xE01C,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_noTestFixture,							0xE01D,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidTestScript,						0xE01F,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidValue_NAN,						0xE020,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidValue_INF,						0xE021,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidObject,							0xE022,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_duplicatedSymbol,						0xE023,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_imageCreationException,					0xE024,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_objectException,						0xE025,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_specCheck_missed,						0xE026,		kHandling_normal)

	ML_ERROR_DECLARE(kMLErr_notSupported,							0xE027,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidCondition,						0xE028,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidStatus,							0xE029,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_timeout,								0xE030,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_dataCheckError,							0xE031,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_INCOMPLETE_TEST,						0xE032,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_specOverridingFailed,					0xE033,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidI2cResponse,						0xE034,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_invalidTestCondition,					0xE035,		kHandling_normal)
	//	0xE030 ~ 0xE04F	-	file
	ML_ERROR_DECLARE(kMLErr_archiveExtractFailed,					0xE036,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_archiveCreateFailed,					0xE037,		kHandling_normal)
	
	ML_ERROR_DECLARE(kMLErr_testRecordError,						0xE038,		kHandling_normal)
	//	0xE030 ~ 0xE035	- 	Process Control
	ML_ERROR_DECLARE(kMLErr_processCreationFailed,					0xE039,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_processExitWithError,					0xE03A,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_processExitWithUncaughtSignal,			0xE03B,		kHandling_normal)
	//
	//	0xE030 ~ 0xE04F	-	file
	ML_ERROR_DECLARE(kMLErr_fileNotExist,							0xE040,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pathNotExist,							0xE041,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileSizeGetFailed,						0xE042,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileOpenFailed,							0xE043,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileWriteFailed,						0xE044,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileReadFailed,							0xE045,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pipeOpenFailed,							0xE046,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fcntlFailed,							0xE047,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_dirOpenFailed,							0xE048,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_dirReadFailed,							0xE049,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileRemoveFailed,						0xE04A,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileRenameFailed,						0xE04B,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_mkdirFailed,							0xE04C,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileChecksum_failed,					0xE04D,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_chdirFailed,							0xE04E,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_commandNotExist,						0xE04F,		kHandling_normal)
	//	0xE050 ~ 0xE068 -	ioreg
	ML_ERROR_DECLARE(kMLErr_ioMasterPort,							0xE050,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ioServiceGetMatchingService,			0xE051,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ioServiceOpen,							0xE052,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ioServiceMatching,						0xE053,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ioServiceGetMatchingServices,			0xE054,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ioServiceAddMatchingNotification,		0xE055,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ioServiceAddInterestNotification,		0xE056,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ioCreatePlugInInterfaceForService,		0xE057,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ioRegistryEntryCreateCFProperty,		0xE058,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_queryInterface,							0xE059,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_usbDeviceRequest,						0xE05A,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_FailedToGetData,						0xE05B,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_FailedToSetData,						0xE05C,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ioConnectCallMethod,					0xE05D,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ioNotificationPortCreate,				0xE05E,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ioNotificationPortGetRunLoopSource,		0xE05F,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_cfDataGetTypeID,						0xE060,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_cfRunLoopGetCurrent,					0xE061,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hidDeviceCreate,						0xE062,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hidDeviceOpen,							0xE063,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hidDeviceGetReport,						0xE064,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hidDeviceSetReport,						0xE065,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_usbInterfaceRequest,					0xE066,		kHandling_normal)

	//	0xE068 ~ 0xE06F -	lockdown service
	ML_ERROR_DECLARE(kMLErr_amCreateDeviceList,						0xE067,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_amDeviceConnect,						0xE068,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_amDevicePair,							0xE069,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_amDeviceStartSession,					0xE06A,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_amDeviceSecureStartService,				0xE06B,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_amDeviceNotificationSubscribe,			0xE06C,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_amDeviceCopyDeviceLocation,				0xE06D,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_amDServiceConnectionGetSocket,			0xE06E,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_secureLockdownCheckinFailed,			0xE06F,		kHandling_normal)
	//	0xE070 ~ 0xE07F -	USB
	ML_ERROR_DECLARE(kMLErr_cannotFindUsbDevice,					0xE070,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_usbRemoveNotifySetupFailed,				0xE071,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_usbGetReportError,						0xE072,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_usbDevicePowerOffFailed,				0xE073,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_usbDevicePowerOnFailed,					0xE074,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_usbSetReportError,						0xE075,		kHandling_normal)
	//	0xE080 ~ 0xE08F -	serial port
	ML_ERROR_DECLARE(kMLErr_serialCannotFindDevice,					0xE080,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_serialOpenError,						0xE081,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_serialCtrlError,						0xE082,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_serialWriteError,						0xE083,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_serialWriteAbnormal,					0xE084,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_serialReadError,						0xE085,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_serialReadAbnormal,						0xE086,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_serialReadTimeout,						0xE087,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_serialCmdFailed,						0xE088,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_serialAlreadyOpen,						0xE089,		kHandling_normal)
	//	0xE090 ~ 0xE09F -	socket
	ML_ERROR_DECLARE(kMLErr_socketOpenError,						0xE090,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketOptionSetError,					0xE091,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketBindError,						0xE092,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketListenError,						0xE093,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketSelectError,						0xE094,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketPollError,						0xE095,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketAcceptError,						0xE096,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketInvalidAddress,					0xE097,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketConnectionFailed,					0xE098,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketRecvFailed,						0xE099,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketRecvTimeoout,						0xE09A,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketSendFailed,						0xE09B,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketClosed,							0xE09C,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketInvalidPort,						0xE09D,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketAppendError,						0xE09E,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_socketSendTimeout,						0xE09F,		kHandling_normal)

	//	0xE0A0 ~ 0xE0AF - 	OpenGL Error
	ML_ERROR_DECLARE(kMLErr_glShaderCreationFailed,					0xE0A0,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_glShaderCompilationFailed,				0xE0A1,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_glProgramCreationFailed,				0xE0A2,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_glShaderLinkFailed,						0xE0A3,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_glAttributeFindingFailed,				0xE0A4,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_glUniformFindingFailed,					0xE0A5,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_glGenBuffersFailed,						0xE0A6,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_glMapBufferFailed, 						0xE0A7, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_glFontLoadingFailed,					0xE0A8, 	kHandling_normal)
	//	0xE0B0 ~ 0xE0BF - 	Module Tracking Error
	ML_ERROR_DECLARE(kMLErr_tstrIdErr_duplicated, 					0xE0B8,		kHandling_normal)
	//	0xE100 ~ 0xE10F -	MTCP Error
	ML_ERROR_DECLARE(kMLErr_mtcp_communicationError,				0xE100,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_mtcp_invalidPreamble,					0xE101,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_mtcp_invalidPheader,					0xE102,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_mtcp_checksumError,						0xE103,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_mtcp_unknownCtrlCode,					0xE104,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_mtcp_unexpectedPayloadSize,				0xE105,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_mtcp_memoryAllocationFailed,			0xE106,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_mtcp_test_evaluation_error,				0xE107,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_mtcp_packet_not_acknowledged,			0xE108,		kHandling_normal)
	//
	ML_ERROR_DECLARE(kMLErr_snErr_invalidLength,					0xE1B4, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_snErr_unknownPlantCode,					0xE1B5, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_snErr_unknownEEEECode,					0xE1B7, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_snErr_invalidYearCode,					0xE1B8, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_snErr_invalidWeekCode,					0xE1B9, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_snErr_invalidDayCode,					0xE1BA, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_snErr_invalidCharacter,					0xE1BB, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_snErr_invalidColorCode,					0xE1BC, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_snErr_invalidCheckDigit,				0xE1BD, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_snErr_invalidRevCode,					0xE1BE, 	kHandling_normal)
	//	0xE800 ~ 0xE80F	-	fixture
	ML_ERROR_DECLARE(kMLErr_fixtLoadCellNotAvailable,				0xE7FE,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtMotorNotAvailable,					0xE7FF,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtCommunicationError,					0xE800,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtModBus_crc_error,					0xE801,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtAlarmActivated,						0xE802,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtNotInitialized,						0xE803,		kHandling_normal)	
	ML_ERROR_DECLARE(kMLErr_fixtUnknownFixtureStat,					0xE804,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtActionFailed,						0xE805,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtActionTimeout,						0xE806,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtThreadCreationFail,					0xE807,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtInvalidCondition,					0xE808,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtInvalidRequest,						0xE809,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtProbeNotReady,						0xE80A,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtInAction,							0xE80B,		kHandling_normal)
    ML_ERROR_DECLARE(kMLErr_fixtInvalidRegAddr,						0xE80C,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtInvalidRspLeng,						0xE80D,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtInvalidRspCrc,						0xE80E,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fixtInvalidRspFormat,					0xE80F,		kHandling_normal)
	//	0xE810 ~ 0xE81F - 	FDR Error
	ML_ERROR_DECLARE(kMLErr_fdrDataAllocation,						0xE810, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdrCreateInstance,						0xE811, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdrAppendPermissionsString,				0xE812, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdrCreation,							0xE813, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdr_dataCheckFailed,					0xE814, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdr_SET,								0xE815, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdr_GET,								0xE816, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdr_DEL,								0xE817, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdr_DEL_confirm,						0xE818, 	kHandling_normal)
	
	ML_ERROR_DECLARE(kMLErr_fdr_connection_timeout,					0xE81D, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdr_duplicatedEntry,					0xE81F,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdr_no_entry,							0xE820, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdr_internal_issue,						0xE821, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdr_trustObj_issue,						0xE822, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdr_EXPORT,								0xE823, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fdr_getPermission,						0xE824, 	kHandling_normal)

	//	0xE830 ~ 0xE83F	-	syscfg
	ML_ERROR_DECLARE(kMLErr_syscfgNoKey,							0xE830,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_syscfgNoSpace,							0xE831,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_syscfgAddKeyFailed,						0xE832,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_syscfgDeleteKeyFailed,					0xE833,		kHandling_normal)
	//	0xE840 ~ 0xE85F	-	instant pudding
	ML_ERROR_DECLARE(kMLErr_IP_UUTStart_failed,						0xE840,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_UUTDone_failed,						0xE841,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_UUTCommit_failed,					0xE842,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_UUTNotStarted,						0xE843,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_addAttribute_failed,					0xE844,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_addBlob_failed,						0xE845,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_addResult_failed,					0xE846,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_testSpec_setTestName_failed,			0xE847,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_testSpec_setSubTestName_failed,		0xE848,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_testSpec_setPriority_failed,			0xE849,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_testSpec_setUnit_failed,				0xE84A,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_testSpec_setLimits_failed,			0xE84B,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_testResult_setValue_failed,			0xE84C,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_testResult_setResult_failed,			0xE84D,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_testResult_setMessage_failed,		0xE84E,		kHandling_normal)
    ML_ERROR_DECLARE(kMLErr_IP_setStartTime_failed,                 0xE84F,     kHandling_normal)
    ML_ERROR_DECLARE(kMLErr_IP_setStopTime_failed,                  0xE850,     kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_validateSerialNumber, 				0xE851, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IPm_serialNumberNotInit, 				0xE852, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_getGHStationInfoFailed, 				0xE853, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_AmIOkCallFailed,		 				0xE854, 	kHandling_normal)
	//
	ML_ERROR_DECLARE(kMLErr_writeControlBitFailed,					0xE855, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_readControlBitFailed,					0xE856, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pdcaSessionStartFailed,					0xE857, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_networkCBFailed,						0xE858, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pdcaFatalError,							0xE859, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pdcaUploadFailed,						0xE85A, 	kHandling_normal)
	
	ML_ERROR_DECLARE(kMLErr_sfcQueryFailed,							0xE85B, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_sfcAddAttrFailed,						0xE85C, 	kHandling_normal)
	//
	ML_ERROR_DECLARE(kMLErr_IP_UnitOutOfProcess,		 			0xE85D, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IPm_handlerExisted, 					0xE85E, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IPm_handlerRunning, 					0xE85F, 	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_setDutPos_failed,					0xE860,		kHandling_normal)
	//
	ML_ERROR_DECLARE(kMLErr_IP_ctsSetup_fomMissing,					0xE861,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_ctsPost_notSupported,				0xE862,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_ctsPost_blobMissing,					0xE863,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_ctsPost_fomMissing,					0xE864,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_ctsPost_fomValidInvalid,				0xE865,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_IP_ctsPost_fomNotComplete,				0xE866,		kHandling_normal)

	ML_ERROR_DECLARE(kMLErr_GH_InfoNotExist,						0xE870,		kHandling_normal)
	// FOX
	ML_ERROR_DECLARE(kMLErr_foxMap_staticFieldMissing,				0xE880,		kHandling_normal)
	
	// CTS
	ML_ERROR_DECLARE(kMLErr_ctsBlob_invalidCs,						0xE890,	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ctsBlob_invalidEncode,					0xE891,	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ctsBlob_invalidTar,						0xE892,	kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_ctsBlob_invalidFileCnt,					0xE893,	kHandling_normal)

	// SQL
	ML_ERROR_DECLARE(kMLErr_sqlCtrl_dbNotReady,						0xE900,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_sqlCtrl_scriptError,					0xE901,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_sqlCtrl_dbOpenError,					0xE902,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_sqlCtrl_dbCloseError,					0xE903,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_sqlCtrl_tableNotExist,					0xE904,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_sqlCtrl_recordNotExist,					0xE905,		kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_sqlCtrl_emptyStorageTable,				0xE906,		kHandling_normal)
	
	ML_ERROR_DECLARE(kMLErr_lotDB_emptyStorageTable,				0xE910, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotDB_numStrFormat,						0xE911, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotDB_binNotSupported,					0xE912, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotDB_noStorageFilePath,				0xE913, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotDB_missRequiredInfo,					0xE914, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotDB_emptyLot,							0xE915, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotDB_existed,							0xE916, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotDB_NotExist,							0xE917, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotDB_storageTableExisted,				0xE918, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotCtrl_emptyLot,						0xE919, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotCtrl_emptyStorageTable,				0xE91A, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotCtrl_numStrFormat,					0xE91B, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotCtrl_binNotSupported,				0xE91C, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotCtrl_noStorageFilePath,				0xE91D, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotCtrl_missRequiredInfo,				0xE91E, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotCtrl_emptyRecord,					0xE91F, kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_lotCtrl_emptySubLot,					0xE920, kHandling_normal)

	// MTCP Pivot
	ML_ERROR_DECLARE(kMLErr_pivotCsv_saveFailed,					0xE930	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_fileOpen,						0xE931	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_fileType,						0xE932	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_fileSize,						0xE933	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_fileDir,						0xE934	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_groupSize,						0xE935	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_tscrPair,						0xE936	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_lotsPair,						0xE937	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_reqNotSupported,				0xE938	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_groupNotSupported,				0xE939	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_missRequiredItem,				0xE93A	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_itemCnt,						0xE93B	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_numFormat,						0xE93C	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_titleColInvalid,				0xE93D	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_colNumMismatch,				0xE93E	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_requestNotSupported,			0xE93F	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_pivotCsv_reportNotSupported,			0xE940	,kHandling_normal)

	ML_ERROR_DECLARE(kMLErr_archive_invalidDB,						0xE960	,kHandling_normal)
	
	// JSON
	ML_ERROR_DECLARE(kMLErr_json_objectNotFound,					0xE970	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_json_invalidQueryType,					0xE971	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_json_queryTypeNotMatch,					0xE972	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_json_containNonStringMember,			0xE973	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_json_emptyKey,							0xE974	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_json_parseError,						0xE975	,kHandling_normal)
	
	// HostHelper
	ML_ERROR_DECLARE(kMLErr_hh_connect_failed,						0xE980	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hh_encodeMsg_failed,					0xE981	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hh_decodeMsg_failed,					0xE982	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hh_addTaskPdca_failed,					0xE983	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hh_addTaskBobcat_failed,				0xE984	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hh_getGhLiveFile_failed,				0xE985	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hh_addFileToAutoDel_failed,				0xE986	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hh_saveBitStream_failed,				0xE987	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hh_saveArchiveFile_failed,				0xE988	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hh_loadArchiveFile_failed,				0xE989	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hh_not_available,						0xE98A	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_hh_timeout,								0xE98B	,kHandling_normal)

	// file share
	ML_ERROR_DECLARE(kMLErr_fileShare_Download_failed,				0xE990	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileShare_Upload_failed,				0xE991	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileShare_getStatus_failed,				0xE992	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileShare_ListDir_failed,				0xE993	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileShare_MakeDir_failed,				0xE994	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileShare_Delete_failed,				0xE995	,kHandling_normal)
	ML_ERROR_DECLARE(kMLErr_fileShare_checkExisted_failed,				0xE996	,kHandling_normal)

	//	0xFFFF - unknown error
	ML_ERROR_DECLARE(kMLErr_unknown, 								0xFFFF, 	kHandling_normal)
} ML_ERROR_TABLE_END;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/**
		@brief	This method return the size of whole error table

		@return	size of error table
	*/
	ML_API uint16_t mlError_getErrorTableSize();

	/**
		@brief	Get the actual predefined MesaLibrary Error code given error table order

		@param[in]	tableOrder	numberic order in predefined error table
		@return	MesaLibrary t_mlError defined in ml_error.h
	*/
	ML_API t_mlError mlError_getErrorCodeFromOrder(uint16_t tableOrder);
	
	/**
		@brief	Get the string information for certain Error code

		@param[in]	err	predefined Error code
		@return	string information
	*/
	ML_API const char* mlError_getErrorString(t_mlError err);

	/**
		@brief	Get the handling rule for certain Error code

		@param[in]	err	predefined Error code
		@return	t_mlErrorHandlingRule
	*/
	ML_API t_mlErrorHandlingRule mlError_getHandlingRule(t_mlError err);

	/**
		@brief	expand the existing MesaLibrary error code; overwrite if error code is the same
	 
		@discussion Caller can use this method to define project specific error code, but still use the defined APIs

		@param[in]	errStr			error code string
		@param[in]	errCode			error code
		@param[in]	handlingRule	error code handling rule
	*/
	ML_API void mlError_expandErrorCodes(const char* errStr, uint16_t errCode, t_mlErrorHandlingRule handlingRule);
#ifdef __cplusplus
}
#endif // __cplusplus

/** @} */

#endif//ifndef _ML_ERROR_H_
