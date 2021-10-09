//
//  ml_rosaline_mtcp.h
//  Rosaline
//
//  Created by Yida Zhang on 8/8/16.
//  Copyright © 2016 Apple. All rights reserved.
//

#ifndef _ML_ROSALINE_MTCP_H_
#define _ML_ROSALINE_MTCP_H_

/*
 *  revision history
 *  rev1    08/08/16     initial revision
 *  rev2    09/15/16     implement all existing test coverage packets
 *  rev3    09/15/16     add chamber info in tMTCP_payload_SEQU_RSP
 *                       update tMTCP_payload_TSCR_REQ to include chamber and socket info
 *                       update tMTCP_payload_SEQU_RSP to include chamber info
 *                       update tMTCP_payload_INIT_REQ to include lot info
 *                       add FWHM field in tMTCP_payload_VTST_REQ
 *  rev4    09/17/16     correct calypso nvm data write request to tMTCP_payload_DPWR_RSP
 *                       correct the size description for tMTCP_payload_DPWC_REQ
 *                       add peak wavelength and fwhm wavelength in tMTCP_payload_VCAL_REQ
 *  rev5    09/19/16     correct payload length tMTCP_payload_EMCT_REQ, add IMG_SIZE
 *                       add IMG_INDEX in tMTCP_payload_FLDI_REQ, remove MTC_STEADY_STATE field
 *  rev6    09/21/16     tMTCP_data_ADIP add voltage and optical power raw reading
 *                       tMTCP_payload_C1SS_REQ add wavelength, I_cal and forward voltage
 *                       tMTCP_payload_FLDP_REQ add power efficiency and 9 individual LFNU
 *  rev7    09/22/16     tMTCP_payload_VCAL_REQ add Voltage in final result
 *  rev8    09/23/16     tMTCP_payload_EMCT_REQ add drive current and forward voltage
 *                       tMTCP_payload_BMPF_REQ add drive current and forward voltage
 *                       tMTCP_payload_FLDP_REQ add drive current and forward voltage and optical power for each region
 *                       tMTCP_payload_ALPH_REQ add drive current and voltage
 *  rev9    09/26/16     add different port for PreBI, PreDA, PostDA, Alpha
 *  rev10   10/18/16     add tMTCP_payload_YGTS_REQ for Yogi Fault Condition Test, add tMTCP_payload_YGTR_RSP for
 * current request add tMTCP_payload_YGWR_RSP and tMTCP_payload_YGWC_REQ for Yogi NVM Write test rev11   10/23/16 add
 * SOCKET_DCR for tMTCP_payload_TSCR_REQ add Lot size, diffuser lot info and operator info in tMTCP_payload_INIT_REQ
 *                       tMTCP_payload_VTST_REQ add first pulse optical power, remove reverse I/V
 *                       tMTCP_payload_DPCK_REQ extend to have Yogi register field
 *                       tMTCP_payload_VDCR_RSP remove DCR and T_STEADY_STATE field
 *                       tMTCP_payload_YGTS_REQ remove trace Id, add statue pin level and current
 *  rev12   11/09/16     add tMTCP_payload_VTST_PREDA_REQ for PreDA Valencia LIV Test
 *                       add tMTCP_payload_VTST_POSTDA_REQ for PostDA Valencia LIV Test
 *                       updated DPCK request packet to include Yogi test coverage
 *                       rename C1SS to CTSS and rename C1ES to CTES to avoid confusion
 *                       simplify tMTCP_payload_CTSS_REQ
 *                       simplify tMTCP_payload_EMCT_REQ and tMTCP_payload_BMPF_REQ to reflect Test Plan
 *                       simpligy tMTCP_payload_FLDP_REQ
 *  rev13   11/12/16     add more Yogi regster map fileds in tMTCP_payload_YGWC_REQ
 *                       add calibrated current and NVM Reg setting in tMTCP_payload_YGTR_RSP
 *                       add Forward voltage and current field for APLH packet for Alpha measurement
 *  rev20   01/29/17     start to add general result file based structure
 *
 */

#include "ml_mtcp.h"

/*  MTCP server port
 */
#define MTCP_PORT_DEFAULT (61804)
#define MTCP_PORT_PREBI (61805)
#define MTCP_PORT_PREDA (61806)
#define MTCP_PORT_POSTDA (61807)
#define MTCP_PORT_ALPHA (61808)

/*------------------------------------------------------------------------------
 *  Generic Communication Request
 */
#define kMTCP_CTRL_GENL 'genl'
/*  Direction
 *		Tester Controller -> Apple Test SW
 *	payload
 *
 *      +------------------------+-----------------+------+
 *      | tMTCP_payload_GENL_REQ | FILE_SIZE bytes| P_CS |
 *      +------------------------+-----------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_GENL_REQ)
 *                         + FILE_SIZE
 *                         + 1 (P_CS)
 *
 *	Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_GENL_REQ
{
    uint8_t DIRECTION;   // 0 : Request, 1 : Report
    uint8_t FILE_TYPE;   // 0 : csv, 1 : plist
    uint32_t FILE_SIZE;  // file size in bytes
} tMTCP_payload_GENL_REQ, *tMTCP_payload_GENL_REQ_ptr;
#pragma pack()

/*  Generic Communication Response
 *  Direction
 *      Apple Test SW -> Test Controller
 *	payload
 *
 *      +------------------------+----------------+------+
 *      | tMTCP_payload_GENL_RSP | FILE_SIZE bytes| P_CS |
 *      +------------------------+----------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_GENL_RSP)
 *                         + FILE_SIZE
 *                         + 1 (P_CS)
 *
 *	Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_GENL_RSP
{
    uint16_t T_ERRC;     // result error code, 0 is ok
    uint8_t T_ERRS[30];  // error string
    uint8_t FILE_TYPE;   // 0 : csv, 1 : plist
    uint32_t FILE_SIZE;  // file size in bytes
} tMTCP_payload_GENL_RSP, *tMTCP_payload_GENL_RSP_ptr;
#pragma pack()

typedef struct t_groupItem
{
    const char* keyword;
    const char* type;
} t_groupItem, *t_groupItemPtr;

typedef struct t_group
{
    const char* groupName;
    t_groupItem* groupItems;
    uint8_t groupItemsCnt;
} t_group, *t_groupPtr;

#define DECLARE_GINFO_BEGIN(group) static t_groupItem _gTable_##group[] = {
#define DECLARE_GINFO_END \
    }                     \
    ;
#define DECLARE_GINFO(keyword, type) { #keyword, #type },

#define ADD_TO_GROUP_BEGIN static t_group _gTable[] = {
#define ADD_TO_GROUP_END \
    }                    \
    ;
#define ADD_TO_GROUP(name) { #name, _gTable_##name, sizeof(_gTable_##name) / sizeof(_gTable_##name[0]) },
#define ADD_TO_GROUP_SIZE (sizeof(_gTable) / sizeof(_gTable[0]))

#pragma mark -
#pragma mark New Lot Start Group (LOTS)
/*------------------------------------------------------------------------------
 *  New Lot Start Group (LOTS)
 */
DECLARE_GINFO_BEGIN(LOTS)
/*------------------------------------------------------------------------------*/
DECLARE_GINFO(TESTER_NAME, string)  // Tester Name, 31 char max, e.g. TSDI
DECLARE_GINFO(TESTER_ID, uint8_t)   // Tester Id, e.g. 1
DECLARE_GINFO(LOT_NAME, string)     // Production lot name, 31 char max
DECLARE_GINFO(LOT_SIZE, uint32_t)   // Production lot size
DECLARE_GINFO(D_LOT_NAME, string)   // Production diffuser lot name, 31 char max
DECLARE_GINFO(OPERATOR, string)     // Operator badge number
DECLARE_GINFO(AM_SW_VER, string)    // Automation Software Version
DECLARE_GINFO(TEST_MODE, uint8_t)   // Test Mode:
                                    //	0 : FirstTest,
                                    //	1 : Retest
                                    //	2 : GRR,
                                    //	3 : Engineering
/*------------------------------------------------------------------------------*/
DECLARE_GINFO_END

#pragma mark Lot End Group (LOTE)
/*------------------------------------------------------------------------------
 *  Lot End Group (LOTE)
 */
DECLARE_GINFO_BEGIN(LOTE)
/*------------------------------------------------------------------------------*/
DECLARE_GINFO(PASS_BIN_1, uint32_t)   // Number of Bin 1, pass bin
DECLARE_GINFO(PASS_BIN_2, uint32_t)   // Number of Bin 2, pass bin
DECLARE_GINFO(PASS_BIN_3, uint32_t)   // Number of Bin 3, pass bin
DECLARE_GINFO(FAIL_BIN_5, uint32_t)   // Number of Bin 5, fail bin
DECLARE_GINFO(FAIL_BIN_6, uint32_t)   // Number of Bin 6, fail bin
DECLARE_GINFO(FAIL_BIN_7, uint32_t)   // Number of Bin 7, 2D fail bin
DECLARE_GINFO(OVERRALL_YIELD, float)  // Overral yield, e.g. 0.65
DECLARE_GINFO(TOTAL_INPUT, uint32_t)  // Number of total input
/*------------------------------------------------------------------------------*/
DECLARE_GINFO_END

#pragma mark Test Session Creation Group (TSCR)
/*------------------------------------------------------------------------------
 *  Test Session Creation Group (TSCR)
 */
DECLARE_GINFO_BEGIN(TSCR)
/*------------------------------------------------------------------------------*/
DECLARE_GINFO(STATION_TYPE, uint8_t)  // Station Type
                                      //	0	: Loading
                                      //	255 : Unloading
                                      //	1	: Chamber 1 -- Integrating Sphere
                                      //	2	: Chamber 2	-- Emitter Count
                                      //	3	: Chamber 3 -- Beam Profiler
                                      //	4	: Chamber 4 -- Big Screen
                                      //	5	: Chamber 5 -- Gimbal
                                      //	6	: Chamber 4-1 -- Small Screen
                                      //	7	: Chamber 4-2 -- Conoscope
DECLARE_GINFO(STATION_SN, string)     // Serial Number Different Station, e.g. C1-001
DECLARE_GINFO(TM_SW_VER, string)      // Test Manager SoftWare Version
DECLARE_GINFO(FW_SW_VER, string)      // Drive Board Software Version
DECLARE_GINFO(FPGA_SW_VER, string)    // FPGA Software Version
DECLARE_GINFO(VISION_SW_VER, string)  // Vision Software Version
DECLARE_GINFO(MTCP_DLL_VER, string)   // MTCP DLL Version for algorithm
DECLARE_GINFO(MODULE_SN, string)      // Scanned Module Barcode
DECLARE_GINFO(SOCKET_SN, string)      // Scanned Socket Barcode
/*------------------------------------------------------------------------------*/
DECLARE_GINFO_END

#pragma mark Test Session End Group (TSED)
/*------------------------------------------------------------------------------
 *  Test Session End Group (TSED)
 */
DECLARE_GINFO_BEGIN(TSED)
DECLARE_GINFO(SOCKET_SN, string)  // Scanned Socket Barcode
DECLARE_GINFO_END

#pragma mark Valencia Calibrated Current Request Group (VDCR)
/*------------------------------------------------------------------------------
 *  Valencia Calibrated Current Request Group (VDCR)
 */
DECLARE_GINFO_BEGIN(VDCR)
DECLARE_GINFO(I_DR, float)      // Calibrated Current
DECLARE_GINFO(T_SETTLE, float)  // settling time for steady state
DECLARE_GINFO_END

#pragma mark Barcode Scan and Test Initialization Group (SCAN)
/*------------------------------------------------------------------------------
 *  Barcode Scan and Test Initialization Group (SCAN)
 */
DECLARE_GINFO_BEGIN(SCAN)
DECLARE_GINFO(MODULE_SN, string)  // Scanned Module Barcode
DECLARE_GINFO(SOCKET_SN, string)  // Scanned Socket Barcode
DECLARE_GINFO_END

#pragma mark Basic DCR Test Group (DCRT)
/*------------------------------------------------------------------------------
 *  Basic DCR Test Group (DCRT)
 */
DECLARE_GINFO_BEGIN(DCRT)
DECLARE_GINFO(V_ATEST2AVSS_MV, float)  // Measured Voltage across ATEST to AVSS as V1
DECLARE_GINFO(V_AVDD2ATEST_MV, float)  // Measured Voltage across AVDD to ATEST as V2
DECLARE_GINFO(I_AVDD2ATEST_MA, float)  // Measured VDD current as I2
DECLARE_GINFO(R_AVSS_MOHM, float)      // V1 / 12.01mA
DECLARE_GINFO(R_AVDD_MOHM, float)      // V2 / I2
DECLARE_GINFO_END

#pragma mark Valencia Reverse Voltage Measurement Group (REVV)
/*------------------------------------------------------------------------------
 *  Valencia Reverse Voltage Measurement (REVV)
 */
DECLARE_GINFO_BEGIN(REVV)
DECLARE_GINFO(I_REVERSE_UA, float)  // Current drive on Anode Pin, -10uA
DECLARE_GINFO(V_REVERSE_V, float)   // Measured Voltage on Anode Pin with -10uA drive
DECLARE_GINFO_END

#pragma mark HV Leak Test Group (LEAK)
/*------------------------------------------------------------------------------
 *  HV Leak Test Group (LEAK)
 */
DECLARE_GINFO_BEGIN(LEAK)
DECLARE_GINFO(I_HV_LEAK_V, float)   // Voltage driven on HV pin during test
DECLARE_GINFO(I_HV_LEAK_UA, float)  // Current on HV Pin given 8V
DECLARE_GINFO_END

#pragma mark Calypso & Yogi Communication Check Group (DPCK)
/*------------------------------------------------------------------------------
 *  Calypso & Yogi Communication Check Group (DPCK)
 */
DECLARE_GINFO_BEGIN(DPCK)
DECLARE_GINFO(C_I_IDLE_LOW_UA, float)     // Calypso Low Idle Current
DECLARE_GINFO(C_NVM_DATA, hexArray)       // 128 bytes Calypso NVM Dump
DECLARE_GINFO(C_NVM_I_CAL_VAL, uint16_t)  // Calibrated Current raw byte
DECLARE_GINFO(C_NVM_I_CAL_A, float)       // Calibrated Current
DECLARE_GINFO(Y_NVM_DATA, hexArray)       // 61 bytes Yogi NVM Dump, from 0x00 to 0x3D
DECLARE_GINFO(Y_I_ACTIVE, float)          // Yogi Active Mode Current
DECLARE_GINFO_END

#pragma mark Valencia Current Calibration Group (VCAL)
/*------------------------------------------------------------------------------
 *  Valencia Current Calibration Group (VCAL)
 */
DECLARE_GINFO_BEGIN(VCAL)
DECLARE_GINFO(SOCKET_DCR_MOHM, float)            // Socket DCR Value
DECLARE_GINFO(NTC_INIT_C, float)                 // NTC Temperature before calibration
DECLARE_GINFO(IF_1A_A_RAW, floatArray)           // Measured current with 1A, 10 pulses
DECLARE_GINFO(VF_UNCOMP_1A_V_RAW, floatArray)    // Measured voltage with 1A, 10 pulses
DECLARE_GINFO(PEAK_OPWR_1A_MWP_RAW, floatArray)  // Measured peak optical power with 1A, 10 pulses
DECLARE_GINFO(CENT_WL_1A_NM_RAW, floatArray)     // Measured centroid wavlength with 1A, calculated by DLL
DECLARE_GINFO(PEAK_WL_1A_NM_RAW, floatArray)     // Measured peak wavlength with 1A, calculated by DLL
DECLARE_GINFO(MAX_WL_1A_NM_RAW, floatArray)      // Measured max wavlength with 1A, calculated by DLL
DECLARE_GINFO(MIN_WL_1A_NM_RAW, floatArray)      // Measured min wavlength with 1A, calculated by DLL
DECLARE_GINFO(1_WL_1A_NM_RAW, floatArray)        // Measured wavelength 1 with 1A, calculated by DLL
DECLARE_GINFO(2_WL_1A_NM_RAW, floatArray)        // Measured wavelength 2 with 1A, calculated by DLL
DECLARE_GINFO(WL_WIDTH_1A_NM_RAW, floatArray)    // Measured wavelength width with 1A, calculated by DLL
DECLARE_GINFO(FWHM_1A_NM_RAW, floatArray)        // Measured FWHM with 1A, calculated by DLL
DECLARE_GINFO(PEAK_OPWR_1A_MWP, float)           // Average of PEAK_OPWR_1A_MWP_RAW
DECLARE_GINFO(IF_1A_A, float)                    // Average of IF_1A_A_RAW
DECLARE_GINFO(VF_UNCOMP_1A_V, float)             // Average of VF_UNCOMP_1A_A_RAW
DECLARE_GINFO(VF_1A_V, float)                    // VF_UNCOMP_1A_V with DCR compensation
DECLARE_GINFO(PEAK_WL_1A_NM, float)              // Average of PEAK_WL_1A_NM_RAW
DECLARE_GINFO(CENT_WL_1A_NM, float)              // Average of CENT_WL_1A_NM_RAW
DECLARE_GINFO(FWHM_1A_NM, float)                 // Average of FWHM_1A_NM_RAW
DECLARE_GINFO(WL_WIDTH_1A_NM, float)             // Average of WL_WIDTH_1A_NM_RAW
DECLARE_GINFO(MAX_WL_1A_NM, float)               // Average of MAX_WL_1A_NM_RAW
DECLARE_GINFO(MIN_WL_1A_NM, float)               // Average of MIN_WL_1A_NM_RAW
DECLARE_GINFO(1_WL_1A_NM, float)                 // Average of 1_WL_1A_NM_RAW
DECLARE_GINFO(2_WL_1A_NM, float)                 // Average of 2_WL_1A_NM_RAW

DECLARE_GINFO(IF_0 .9A_A_RAW, floatArray)           // Measured current with 0.9A, 10 pulses
DECLARE_GINFO(VF_UNCOMP_0 .9A_V_RAW, floatArray)    // Measured voltage with 0.9A, 10 pulses
DECLARE_GINFO(PEAK_OPWR_0 .9A_MWP_RAW, floatArray)  // Measured peak optical power with 0.9A, 10 pulses
DECLARE_GINFO(CENT_WL_0 .9A_NM_RAW, floatArray)     // Measured centroid wavlength with 0.9A, calculated by DLL
DECLARE_GINFO(PEAK_WL_0 .9A_NM_RAW, floatArray)     // Measured peak wavlength with 0.9A, calculated by DLL
DECLARE_GINFO(MAX_WL_0 .9A_NM_RAW, floatArray)      // Measured max wavlength with 0.9A, calculated by DLL
DECLARE_GINFO(MIN_WL_0 .9A_NM_RAW, floatArray)      // Measured min wavlength with 0.9A, calculated by DLL
DECLARE_GINFO(1_WL_0.9A_NM_RAW, floatArray)         // Measured wavlength 1 with 0.9A, calculated by DLL
DECLARE_GINFO(2_WL_0.9A_NM_RAW, floatArray)         // Measured wavlength 2 with 0.9A, calculated by DLL
DECLARE_GINFO(WL_WIDTH_0 .9A_NM_RAW, floatArray)    // Measured wavlength width with 0.9A, calculated by DLL
DECLARE_GINFO(FWHM_0 .9A_NM_RAW, floatArray)        // Measured FWHM with 0.9A, calculated by DLL

DECLARE_GINFO(IF_0 .8A_A_RAW, floatArray)           // Measured current with 0.8A, 10 pulses
DECLARE_GINFO(VF_UNCOMP_0 .8A_V_RAW, floatArray)    // Measured voltage with 0.8A, 10 pulses
DECLARE_GINFO(PEAK_OPWR_0 .8A_MWP_RAW, floatArray)  // Measured peak optical power with 0.8A, 10 pulses
DECLARE_GINFO(CENT_WL_0 .8A_NM_RAW, floatArray)     // Measured centroid wavlength with 0.8A, calculated by DLL
DECLARE_GINFO(PEAK_WL_0 .8A_NM_RAW, floatArray)     // Measured peak wavlength with 0.8A, calculated by DLL
DECLARE_GINFO(MAX_WL_0 .8A_NM_RAW, floatArray)      // Measured max wavlength with 0.8A, calculated by DLL
DECLARE_GINFO(MIN_WL_0 .8A_NM_RAW, floatArray)      // Measured min wavlength with 0.8A, calculated by DLL
DECLARE_GINFO(1_WL_0.8A_NM_RAW, floatArray)         // Measured wavlength 1 with 0.8A, calculated by DLL
DECLARE_GINFO(2_WL_0.8A_NM_RAW, floatArray)         // Measured wavlength 2 with 0.8A, calculated by DLL
DECLARE_GINFO(WL_WIDTH_0 .8A_NM_RAW, floatArray)    // Measured wavlength width with 0.8A, calculated by DLL
DECLARE_GINFO(FWHM_0 .8A_NM_RAW, floatArray)        // Measured FWHM with 0.8A, calculated by DLL

DECLARE_GINFO(IF_0 .7A_A_RAW, floatArray)           // Measured current with 0.7A, 10 pulses
DECLARE_GINFO(VF_UNCOMP_0 .7A_V_RAW, floatArray)    // Measured voltage with 0.7A, 10 pulses
DECLARE_GINFO(PEAK_OPWR_0 .7A_MWP_RAW, floatArray)  // Measured peak optical power with 0.7A, 10 pulses
DECLARE_GINFO(CENT_WL_0 .7A_NM_RAW, floatArray)     // Measured centroid wavlength with 0.7A, calculated by DLL
DECLARE_GINFO(PEAK_WL_0 .7A_NM_RAW, floatArray)     // Measured peak wavlength with 0.7A, calculated by DLL
DECLARE_GINFO(MAX_WL_0 .7A_NM_RAW, floatArray)      // Measured max wavlength with 0.7A, calculated by DLL
DECLARE_GINFO(MIN_WL_0 .7A_NM_RAW, floatArray)      // Measured min wavlength with 0.7A, calculated by DLL
DECLARE_GINFO(1_WL_0.7A_NM_RAW, floatArray)         // Measured wavlength 1 with 0.7A, calculated by DLL
DECLARE_GINFO(2_WL_0.7A_NM_RAW, floatArray)         // Measured wavlength 2 with 0.7A, calculated by DLL
DECLARE_GINFO(WL_WIDTH_0 .7A_NM_RAW, floatArray)    // Measured wavlength width with 0.7A, calculated by DLL
DECLARE_GINFO(FWHM_0 .7A_NM_RAW, floatArray)        // Measured FWHM with 0.7A, calculated by DLL

DECLARE_GINFO(IF_0 .6A_A_RAW, floatArray)           // Measured current with 0.6A, 10 pulses
DECLARE_GINFO(VF_UNCOMP_0 .6A_V_RAW, floatArray)    // Measured voltage with 0.6A, 10 pulses
DECLARE_GINFO(PEAK_OPWR_0 .6A_MWP_RAW, floatArray)  // Measured peak optical power with 0.6A, 10 pulses
DECLARE_GINFO(CENT_WL_0 .6A_NM_RAW, floatArray)     // Measured centroid wavlength with 0.6A, calculated by DLL
DECLARE_GINFO(PEAK_WL_0 .6A_NM_RAW, floatArray)     // Measured peak wavlength with 0.6A, calculated by DLL
DECLARE_GINFO(MAX_WL_0 .6A_NM_RAW, floatArray)      // Measured max wavlength with 0.6A, calculated by DLL
DECLARE_GINFO(MIN_WL_0 .6A_NM_RAW, floatArray)      // Measured min wavlength with 0.6A, calculated by DLL
DECLARE_GINFO(1_WL_0.6A_NM_RAW, floatArray)         // Measured wavlength 1 with 0.6A, calculated by DLL
DECLARE_GINFO(2_WL_0.6A_NM_RAW, floatArray)         // Measured wavlength 2 with 0.6A, calculated by DLL
DECLARE_GINFO(WL_WIDTH_0 .6A_NM_RAW, floatArray)    // Measured wavlength width with 0.6A, calculated by DLL
DECLARE_GINFO(FWHM_0 .6A_NM_RAW, floatArray)        // Measured FWHM with 0.6A, calculated by DLL

DECLARE_GINFO(I_CAL_A, float)  // Calculated Calibrated Current by DLL

DECLARE_GINFO(IF_CAL_A_RAW, floatArray)           // Measured current with CAL, 10 pulses
DECLARE_GINFO(VF_UNCOMP_CAL_V_RAW, floatArray)    // Measured voltage with CAL, 10 pulses
DECLARE_GINFO(PEAK_OPWR_CAL_MWP_RAW, floatArray)  // Measured peak optical power with CAL, 10 pulses
DECLARE_GINFO(CENT_WL_CAL_NM_RAW, floatArray)     // Measured centroid wavlength with CAL, calculated by DLL
DECLARE_GINFO(PEAK_WL_CAL_NM_RAW, floatArray)     // Measured peak wavlength with CAL, calculated by DLL
DECLARE_GINFO(MAX_WL_CAL_NM_RAW, floatArray)      // Measured max wavlength with CAL, calculated by DLL
DECLARE_GINFO(MIN_WL_CAL_NM_RAW, floatArray)      // Measured min wavlength with CAL, calculated by DLL
DECLARE_GINFO(1_WL_CAL_NM_RAW, floatArray)        // Measured wavlength 1 with CAL, calculated by DLL
DECLARE_GINFO(2_WL_CAL_NM_RAW, floatArray)        // Measured wavlength 2 with CAL, calculated by DLL
DECLARE_GINFO(WL_WIDTH_CAL_NM_RAW, floatArray)    // Measured wavlength width with CAL, calculated by DLL
DECLARE_GINFO(FWHM_CAL_NM_RAW, floatArray)        // Measured FWHM with CAL, calculated by DLL

DECLARE_GINFO(PEAK_OPWR_CAL_MWP, float)  // Average of PEAK_OPWR_CAL_MWP_RAW
DECLARE_GINFO(IF_CAL_A, float)           // Average of IF_CAL_A_RAW
DECLARE_GINFO(VF_UNCOMP_CAL_V, float)    // Average of VF_UNCOMP_CAL_V_RAW
DECLARE_GINFO(VF_CAL_V, float)           // VF_UNCOMP_CAL_V with DCR compensation
DECLARE_GINFO(PEAK_WL_CAL_NM, float)     // Average of PEAK_WL_CAL_NM_RAW
DECLARE_GINFO(CENT_WL_CAL_NM, float)     // Average of CENT_WL_CAL_NM_RAW
DECLARE_GINFO(FWHM_CAL_NM, float)        // Average of FWHM_CAL_NM_RAW
DECLARE_GINFO(WL_WIDTH_CAL_NM, float)    // Average of WL_WIDTH_CAL_NM_RAW
DECLARE_GINFO(MAX_WL_CAL_NM, float)      // Average of MAX_WL_CAL_NM_RAW
DECLARE_GINFO(MIN_WL_CAL_NM, float)      // Average of MIN_WL_CAL_NM_RAW
DECLARE_GINFO(1_WL_CAL_NM, float)        // Average of 1_WL_CAL_NM_RAW
DECLARE_GINFO(2_WL_CAL_NM, float)        // Average of 2_WL_CAL_NM_RAW

DECLARE_GINFO_END

#pragma mark Calypso NVM Write Group (DPWC)
/*------------------------------------------------------------------------------
 *  Calypso NVM Write Group (DPWC)
 */
DECLARE_GINFO_BEGIN(DPWC)
DECLARE_GINFO(C_ADDR_TO_WRITE, hexArray)     // NVM Address to be written, generated via DLL
DECLARE_GINFO(C_DATA_TO_WRITE, hexArray)     // NVM Data to write, generated via DLL
DECLARE_GINFO(C_NVM_BEFORE_WRITE, hexArray)  // 128 bytes Calypso NVM Dump before writing
DECLARE_GINFO(C_NVM_AFTER_WRITE, hexArray)   // 128 bytes Calypso NVM Dump after writing
DECLARE_GINFO_END

#pragma mark Valencia Emitter Count Check Group (EMCT)
/*------------------------------------------------------------------------------
 *  Valencia Emitter Count Check Group (EMCT)
 */
DECLARE_GINFO_BEGIN(EMCT)
DECLARE_GINFO(SOCKET_DCR_MOHM, float)           // Socket DCR Value
DECLARE_GINFO(IF_CAL_A_RAW, floatArray)         // Measured current with CAL, 10 pulses
DECLARE_GINFO(VF_UNCOMP_CAL_V_RAW, floatArray)  // Measured voltage with CAL, 10 pulses
DECLARE_GINFO(IF_CAL_A, float)                  // Average of IF_CAL_A_RAW
DECLARE_GINFO(VF_UNCOMP_CAL_V, float)           // Average of VF_UNCOMP_CAL_V_RAW
DECLARE_GINFO(VF_CAL_V, float)                  // VF_UNCOMP_CAL_V with DCR compensation
DECLARE_GINFO(VALID_EMITTER_CNT, uint16_t)      // Valid emitter counts
DECLARE_GINFO(EMITTER_CENTER_X, uint16_tArray)  // X position of each emitter center, in pixel
DECLARE_GINFO(EMITTER_CENTER_Y, uint16_tArray)  // Y position of each emitter center, in pixel
DECLARE_GINFO(EMITTER_SIZE, uint16_tArray)      // Number of pixels in each emitter
DECLARE_GINFO_END

#pragma mark Beam Profile Test Group (BMPF)
/*------------------------------------------------------------------------------
 *  Beam Profile Test Group (BMPF)
 */
DECLARE_GINFO_BEGIN(BMPF)
DECLARE_GINFO(SOCKET_DCR_MOHM, float)           // Socket DCR Value
DECLARE_GINFO(IF_CAL_A_RAW, floatArray)         // Measured current with CAL, 10 pulses
DECLARE_GINFO(VF_UNCOMP_CAL_V_RAW, floatArray)  // Measured voltage with CAL, 10 pulses
DECLARE_GINFO(IF_CAL_A, float)                  // Average of IF_CAL_A_RAW
DECLARE_GINFO(VF_UNCOMP_CAL_V, float)           // Average of VF_UNCOMP_CAL_V_RAW
DECLARE_GINFO(VF_CAL_V, float)                  // VF_UNCOMP_CAL_V with DCR compensation
DECLARE_GINFO(NUMERIC_APERTURE, float)          // Numeric Aperture via DLL
DECLARE_GINFO(AE_MWP, float)                    // Peak Accessible Emission via DLL
DECLARE_GINFO(AF, float)                        // Accessible Fraction via DLL
DECLARE_GINFO(HOTSPOT_LOC_X, float)             // Hot Spot Location X via DLL
DECLARE_GINFO(HOTSPOT_LOC_Y, float)             // Hot Spot Location Y via DLL
DECLARE_GINFO(CENTER_OF_MASS_X, float)          // Center of Mass X via DLL
DECLARE_GINFO(CENTER_OF_MASS_Y, float)          // Center of Mass Y via DLL
DECLARE_GINFO(CIRCULARITY_PCT, float)           // Circulariry via DLL
DECLARE_GINFO_END

#pragma mark Yogi Fault Test for PreDA Group (YPRE)
/*------------------------------------------------------------------------------
 *  Yogi Fault Test for PreDA Group (YPRE)
 */
DECLARE_GINFO_BEGIN(YPRE)
DECLARE_GINFO(I_CAL_A, float)         // Calibrated Current
DECLARE_GINFO(I_PULSEMAX_B1, float)   // I_pulseMax_B1 from DLL
DECLARE_GINFO(I_PULSEMAX_B2, float)   // I_pulseMax_B2 from DLL
DECLARE_GINFO(I_PULSEMAX_B3, float)   // I_pulseMax_B3 from DLL
DECLARE_GINFO(SEL_I_PULSEMAX, hex)    // SEL_I_PULSEMAX from DLL
DECLARE_GINFO(SEL_I_SEVERE, hex)      // SEL_I_SEVERE from DLL
DECLARE_GINFO(Y_I_ACTIVE_UA, float)   // Yogi Active Mode Current
DECLARE_GINFO(Y_I_STANDBY_UA, float)  // Yogi Standby Mode Current
DECLARE_GINFO(I_FAULT_A_UA, float)    // Current after Fault A triggered
DECLARE_GINFO(I_FAULT_B_UA, float)    // Current after Fault B triggered
DECLARE_GINFO(I_FAULT_B1_UA, float)   // Current after Fault B1 triggered
DECLARE_GINFO(I_FAULT_B2_UA, float)   // Current after Fault B2 triggered
DECLARE_GINFO(I_FAULT_B3_UA, float)   // Current after Fault B3 triggered
DECLARE_GINFO(I_FAULT_C_UA, float)    // Current after Fault C triggered
DECLARE_GINFO(I_FAULT_D_UA, float)    // Current after Fault D triggered
DECLARE_GINFO(I_FAULT_E_UA, float)    // Current after Fault E triggered
DECLARE_GINFO(I_FAULT_E1_UA, float)   // Current after Fault E1 triggered
DECLARE_GINFO(I_FAULT_E2_UA, float)   // Current after Fault E2 triggered
DECLARE_GINFO(STATUS_FAULT_A, hex)    // Fault A Reg 0x24 Status
DECLARE_GINFO(STATUS_FAULT_B, hex)    // Fault B Reg 0x24 Status
DECLARE_GINFO(STATUS_FAULT_B1, hex)   // Fault B1 Reg 0x24 Status
DECLARE_GINFO(STATUS_FAULT_B2, hex)   // Fault B2 Reg 0x24 Status
DECLARE_GINFO(STATUS_FAULT_B3, hex)   // Fault B3 Reg 0x24 Status
DECLARE_GINFO(STATUS_FAULT_C, hex)    // Fault C Reg 0x24 Status
DECLARE_GINFO(STATUS_FAULT_D, hex)    // Fault D Reg 0x24 Status
DECLARE_GINFO(STATUS_FAULT_E, hex)    // Fault E Reg 0x24 Status
DECLARE_GINFO(STATUS_FAULT_E1, hex)   // Fault E1 Reg 0x24 Status
DECLARE_GINFO(STATUS_FAULT_E2, hex)   // Fault E2 Reg 0x24 Status
DECLARE_GINFO_END

#pragma mark Yogi Fault Test for PostDA Group (YPST)
/*------------------------------------------------------------------------------
 *  Yogi Fault Test for PostDA Group (YPST)
 */
DECLARE_GINFO_BEGIN(YPST)
DECLARE_GINFO(Y_I_ACTIVE_UA, float)       // Yogi Active Mode Current
DECLARE_GINFO(Y_I_STANDBY_UA, float)      // Yogi Standby Mode Current
DECLARE_GINFO(I_FAULT_A_UA, float)        // Current after Fault A triggered
DECLARE_GINFO(STATUS_FAULT_A, uint8_t)    // Fault A Reg 0x24 Status
DECLARE_GINFO(VF_STATUS, float)           // forward voltage with 10mA drive, in V
DECLARE_GINFO(IF_STATUS, float)           // forward current with 10mA drive, in A
DECLARE_GINFO(LEVEL_STATUS_PIN, uint8_t)  // Status pin level, PostDA only
DECLARE_GINFO(Y_STATUS_L_IF_UA, float)    // IF when Yogi status is low, in uA
DECLARE_GINFO(Y_STATUS_L_VF_V, float)     // VF when Yogi status is low, in V
DECLARE_GINFO_END

#pragma mark Valencia LIV Test for PostDA Group (VTST)
/*------------------------------------------------------------------------------
 *  Valencia LIV Test for PostDA Group (VTST)
 */
DECLARE_GINFO_BEGIN(VTST)
DECLARE_GINFO(SOCKET_DCR_MOHM, float)             // Socket DCR Value
DECLARE_GINFO(IF_CAL_A_RAW, floatArray)           // Measured current with CAL, 10 pulses
DECLARE_GINFO(VF_UNCOMP_CAL_V_RAW, floatArray)    // Measured voltage with CAL, 10 pulses
DECLARE_GINFO(PEAK_OPWR_CAL_MWP_RAW, floatArray)  // Measured peak optical power with CAL, 10 pulses
DECLARE_GINFO(CENT_WL_CAL_NM_RAW, floatArray)     // Measured centroid wavlength with CAL, calculated by DLL
DECLARE_GINFO(PEAK_WL_CAL_NM_RAW, floatArray)     // Measured peak wavlength with CAL, calculated by DLL
DECLARE_GINFO(MAX_WL_CAL_NM_RAW, floatArray)      // Measured max wavlength with CAL, calculated by DLL
DECLARE_GINFO(MIN_WL_CAL_NM_RAW, floatArray)      // Measured min wavlength with CAL, calculated by DLL
DECLARE_GINFO(1_WL_CAL_NM_RAW, floatArray)        // Measured wavlength 1 with CAL, calculated by DLL
DECLARE_GINFO(2_WL_CAL_NM_RAW, floatArray)        // Measured wavlength 2 with CAL, calculated by DLL
DECLARE_GINFO(WL_WIDTH_CAL_NM_RAW, floatArray)    // Measured wavlength width with CAL, calculated by DLL
DECLARE_GINFO(FWHM_CAL_NM_RAW, floatArray)        // Measured FWHM with CAL, calculated by DLL

DECLARE_GINFO(PEAK_OPWR_CAL_MWP, float)  // Average of PEAK_OPWR_CAL_MWP_RAW
DECLARE_GINFO(IF_CAL_A, float)           // Average of IF_CAL_A_RAW
DECLARE_GINFO(VF_UNCOMP_CAL_V, float)    // Average of VF_UNCOMP_CAL_V_RAW
DECLARE_GINFO(VF_CAL_V, float)           // VF_UNCOMP_CAL_V with DCR compensation
DECLARE_GINFO(PEAK_WL_CAL_NM, float)     // Average of PEAK_WL_CAL_NM_RAW
DECLARE_GINFO(CENT_WL_CAL_NM, float)     // Average of CENT_WL_CAL_NM_RAW
DECLARE_GINFO(FWHM_CAL_NM, float)        // Average of FWHM_CAL_NM_RAW
DECLARE_GINFO(WL_WIDTH_CAL_NM, float)    // Average of WL_WIDTH_CAL_NM_RAW
DECLARE_GINFO(MAX_WL_CAL_NM, float)      // Average of MAX_WL_CAL_NM_RAW
DECLARE_GINFO(MIN_WL_CAL_NM, float)      // Average of MIN_WL_CAL_NM_RAW
DECLARE_GINFO(1_WL_CAL_NM, float)        // Average of 1_WL_CAL_NM_RAW
DECLARE_GINFO(2_WL_CAL_NM, float)        // Average of 2_WL_CAL_NM_RAW
DECLARE_GINFO_END

#pragma mark Flood Performance Test Group (FLDP)
/*------------------------------------------------------------------------------
 *	Flood Performance Test Group (FLDP)
 */
DECLARE_GINFO_BEGIN(FLDP)
DECLARE_GINFO(SOCKET_DCR_MOHM, float)           // Socket DCR Value
DECLARE_GINFO(IF_CAL_A_RAW, floatArray)         // Measured current with CAL, 10 pulses
DECLARE_GINFO(VF_UNCOMP_CAL_V_RAW, floatArray)  // Measured voltage with CAL, 10 pulses
DECLARE_GINFO(IF_CAL_A, float)                  // Average of IF_CAL_A_RAW
DECLARE_GINFO(VF_UNCOMP_CAL_V, float)           // Average of VF_UNCOMP_CAL_V_RAW
DECLARE_GINFO(VF_CAL_V, float)                  // VF_UNCOMP_CAL_V with DCR compensation
DECLARE_GINFO(HFNU_PCT, float)                  // HFNU
DECLARE_GINFO(, LFNU_C_PCT float)               // LFNU C region
DECLARE_GINFO(LFNU_N_PCT, float)                // LFNU N region
DECLARE_GINFO(LFNU_NE_PCT, float)               // LFNU NE region
DECLARE_GINFO(LFNU_E_PCT, float)                // LFNU E region
DECLARE_GINFO(LFNU_SE_PCT, float)               // LFNU SE region
DECLARE_GINFO(LFNU_S_PCT, float)                // LFNU S region
DECLARE_GINFO(LFNU_SW_PCT, float)               // LFNU	SW region
DECLARE_GINFO(LFNU_W_PCT, float)                // LFNU W region
DECLARE_GINFO(LFNU_NW_PCT, float)               // LFNU NW region
DECLARE_GINFO(PWR_IN_FOV_MW, float)             // Power in FOV
DECLARE_GINFO(WINDOW_EFFI_PCT, float)           // Window efficiency
DECLARE_GINFO_END

#pragma mark Class 1 Laser Compliance - Accessible Emission (CTES)
/*------------------------------------------------------------------------------
 *	Class 1 Laser Compliance - Accessible Emission (CTES)
 */
DECLARE_GINFO_BEGIN(CTES)
DECLARE_GINFO(AE_MAX_MWP, float)       // Accessible Emission, Max Power
DECLARE_GINFO(AE_AVG_MWA, float)       // Accessible Emission, Average Power
DECLARE_GINFO(HOTSPOT_THETA_D, float)  // Hot Spot location, theta
DECLARE_GINFO(HOTSPOT_PHI_D, float)    // Hot Spot location, phi
DECLARE_GINFO(TOTAL_POWER_MW, float)   // total power
DECLARE_GINFO_END

#pragma mark Yogi Programming Group (YGWC)
/*------------------------------------------------------------------------------
 *	Yogi Programming Group (YGWC)
 */
DECLARE_GINFO_BEGIN(YGWC)

DECLARE_GINFO(Y_NVM_1, hexArray)   // Initial NVM Reading from 0x12 to 0x23
DECLARE_GINFO(Y_NVM_2, hexArray)   // NVM Reading after writing, from 0x12 to 0x23
DECLARE_GINFO(Y_NVM_3, hexArray)   // NVM Reading after programming, from 0x12 to 0x23
DECLARE_GINFO(PROG0_REG, uint8_t)  // PROG0_REG register value after programming
DECLARE_GINFO_END

#pragma mark Class 1 Laser Compliance - Alpha Information Group (ALPR)
/*------------------------------------------------------------------------------
 *	Class 1 Laser Compliance - Alpha Information Group (ALPR)
 */
DECLARE_GINFO_BEGIN(ALPR)
DECLARE_GINFO(HOTSPOT_THETA, float)      // Hot spot location Theta
DECLARE_GINFO(HOTSPOT_PHI, float)        // Hot spot location Phi
DECLARE_GINFO(OBJ_DISTANCE, float)       // objDistance for DLL
DECLARE_GINFO(WAVELENGTH, float)         // Wavelength for DLL
DECLARE_GINFO(TFRAME, float)             // Tframe for DLL
DECLARE_GINFO(TOTFRAME, float)           // TOTframe for DLL
DECLARE_GINFO(EFRAME, float)             // Eframe for DLL
DECLARE_GINFO(PITCH, float)              // Pitch for DLL
DECLARE_GINFO(CROPSHIFT_X, float)        // cropshift_x for DLL
DECLARE_GINFO(CROPSHIFT_Y, float)        // cropshift_y for DLL
DECLARE_GINFO(CROPFACTOR_X, float)       // cropfactor_x for DLL
DECLARE_GINFO(CROPFACTOR_Y, float)       // cropfactor_y for DLL
DECLARE_GINFO(PADDING_FRAME_VER, float)  // Padding_frameVer for DLL
DECLARE_GINFO(PADDING_FRAME_HOR, float)  // Padding_frameHor for DLL
DECLARE_GINFO_END

#pragma mark Class 1 Laser Compliance Group - Alpha (ALPH)
/*------------------------------------------------------------------------------
 *	Class 1 Laser Compliance Group - Alpha (ALPH)
 */
DECLARE_GINFO_BEGIN(ALPH)
DECLARE_GINFO(SOCKET_DCR_MOHM, float)           // Socket DCR Value
DECLARE_GINFO(IF_CAL_A_RAW, floatArray)         // Measured current with CAL, 10 pulses
DECLARE_GINFO(VF_UNCOMP_CAL_V_RAW, floatArray)  // Measured voltage with CAL, 10 pulses
DECLARE_GINFO(IF_CAL_A, float)                  // Average of IF_CAL_A_RAW
DECLARE_GINFO(VF_UNCOMP_CAL_V, float)           // Average of VF_UNCOMP_CAL_V_RAW
DECLARE_GINFO(VF_CAL_V, float)                  // VF_UNCOMP_CAL_V with DCR compensation
// Following Items are generated with Hot Spot image via DLL
DECLARE_GINFO(PIXELS_PITCH_MM, float)
DECLARE_GINFO(OBJDIST_MM, float)
DECLARE_GINFO(IMDIST_MM, float)
DECLARE_GINFO(WAVELENGTH_MM, float)
DECLARE_GINFO(EFRAME_J, float)
DECLARE_GINFO(TFRAME_SEC, float)
DECLARE_GINFO(TOTFRAME_SEC, float)
DECLARE_GINFO(PLR_SINGLE_RATIO, float)
DECLARE_GINFO(PLR_SINGLE_ED3_RATIO, float)
DECLARE_GINFO(PLR_SPT_RATIO, float)
DECLARE_GINFO(PLR_SPT_ED3_RATIO, float)
DECLARE_GINFO(PLR_TOTP_RATIO, float)
DECLARE_GINFO(PLR_C5_RATIO, float)
DECLARE_GINFO(PLR_ED2_RATIO, float)
DECLARE_GINFO(CONDITION_ED2, float)
DECLARE_GINFO(PLR_ED3_RATIO, float)
DECLARE_GINFO(CONDITION_ED3, float)
DECLARE_GINFO(Q_COND1_RATIO, float)
DECLARE_GINFO(Q_COND1_ED3_RATIO, float)
DECLARE_GINFO(Q_COND2_RATIO, float)
DECLARE_GINFO(Q_COND2_ED3_RATIO, float)
DECLARE_GINFO(Q_COND3_TOTP_RATIO, float)
DECLARE_GINFO(Q_COND3_C5_RATIO, float)
DECLARE_GINFO(E_T2_COND1_J, float)
DECLARE_GINFO(E_T2_COND1_ED3_J, float)
DECLARE_GINFO(E_T2_COND2_J, float)
DECLARE_GINFO(E_T2_COND2_ED3_J, float)
DECLARE_GINFO(E_T2_COND3_TOTP_J, float)
DECLARE_GINFO(E_T2_COND3_C5_J, float)
DECLARE_GINFO(AE_T2_COND1_J, float)
DECLARE_GINFO(AE_T2_COND1_ED3_J, float)
DECLARE_GINFO(AE_T2_COND2_J, float)
DECLARE_GINFO(AE_T2_COND2_ED3_J, float)
DECLARE_GINFO(AE_T2_COND3_TOTP_J, float)
DECLARE_GINFO(AE_T2_COND3_C5_J, float)
DECLARE_GINFO(AEL_SINGLE_J, float)
DECLARE_GINFO(AEL_SINGLE_ED3_J, float)
DECLARE_GINFO(AEL_SPT_J, float)
DECLARE_GINFO(AEL_SPT_ED3_J, float)
DECLARE_GINFO(AEL_TOTP_J, float)
DECLARE_GINFO(AEL_C5_J, float)
DECLARE_GINFO(ALPHA_COND1_MRAD, float)
DECLARE_GINFO(ALPHA_COND1_ED3_MRAD, float)
DECLARE_GINFO(ALPHA_COND2_MRAD, float)
DECLARE_GINFO(ALPHA_COND2_ED2_MRAD, float)
DECLARE_GINFO(ALPHA_COND3_TOTP_MRAD, float)
DECLARE_GINFO(ALPHA_COND3_C5_MRAD, float)
DECLARE_GINFO(ALPHAEFF_COND1_MRAD, float)
DECLARE_GINFO(ALPHAEFF_COND1_ED3_MRAD, float)
DECLARE_GINFO(ALPHAEFF_COND2_MRAD, float)
DECLARE_GINFO(ALPHAEFF_COND2_ED2_MRAD, float)
DECLARE_GINFO(ALPHAEFF_COND3_TOTP_MRAD, float)
DECLARE_GINFO(ALPHAEFF_COND3_C5_MRAD, float)
DECLARE_GINFO(ALPHAX_COND1_MRAD, float)
DECLARE_GINFO(ALPHAX_COND1_ED3_MRAD, float)
DECLARE_GINFO(ALPHAX_COND2_MRAD, float)
DECLARE_GINFO(ALPHAX_COND2_ED2_MRAD, float)
DECLARE_GINFO(ALPHAX_COND3_TOTP_MRAD, float)
DECLARE_GINFO(ALPHAX_COND3_C5_MRAD, float)
DECLARE_GINFO(ALPHAY_COND1_MRAD, float)
DECLARE_GINFO(ALPHAY_COND1_ED3_MRAD, float)
DECLARE_GINFO(ALPHAY_COND2_MRAD, float)
DECLARE_GINFO(ALPHAY_COND2_ED2_MRAD, float)
DECLARE_GINFO(ALPHAY_COND3_TOTP_MRAD, float)
DECLARE_GINFO(ALPHAY_COND3_C5_MRAD, float)
DECLARE_GINFO(ALPHA_MAX_MRAD, float)
DECLARE_GINFO(ALPHA_MAX_ED3_MRAD, float)
DECLARE_GINFO(PIXELSX_COND1_PX, uint16_t)
DECLARE_GINFO(PIXELSX_COND1_ED3_PX, uint16_t)
DECLARE_GINFO(PIXELSX_COND2_PX, uint16_t)
DECLARE_GINFO(PIXELSX_COND2_ED2_PX, uint16_t)
DECLARE_GINFO(PIXELSX_COND3_TOTP_PX, uint16_t)
DECLARE_GINFO(PIXELSX_COND3_C5_PX, uint16_t)
DECLARE_GINFO(PIXELSY_COND1_PX, uint16_t)
DECLARE_GINFO(PIXELSY_COND1_ED3_PX, uint16_t)
DECLARE_GINFO(PIXELSY_COND2_PX, uint16_t)
DECLARE_GINFO(PIXELSY_COND2_ED2_PX, uint16_t)
DECLARE_GINFO(PIXELSY_COND3_TOTP_PX, uint16_t)
DECLARE_GINFO(PIXELSY_COND3_C5_PX, uint16_t)
DECLARE_GINFO(CENTERX_COND1_PX, uint16_t)
DECLARE_GINFO(CENTERX_COND1_ED3_PX, uint16_t)
DECLARE_GINFO(CENTERX_COND2_PX, uint16_t)
DECLARE_GINFO(CENTERX_COND2_ED2_PX, uint16_t)
DECLARE_GINFO(CENTERX_COND3_TOTP_PX, uint16_t)
DECLARE_GINFO(CENTERX_COND3_C5_PX, uint16_t)
DECLARE_GINFO(CENTERY_COND1_PX, uint16_t)
DECLARE_GINFO(CENTERY_COND1_ED3_PX, uint16_t)
DECLARE_GINFO(CENTERY_COND2_PX, uint16_t)
DECLARE_GINFO(CENTERY_COND2_ED2_PX, uint16_t)
DECLARE_GINFO(CENTERY_COND3_TOTP_PX, uint16_t)
DECLARE_GINFO(CENTERY_COND3_C5_PX, uint16_t)
DECLARE_GINFO(T2_COND1_SEC, float)
DECLARE_GINFO(T2_COND1_ED3_SEC, float)
DECLARE_GINFO(T2_COND2_SEC, float)
DECLARE_GINFO(T2_COND2_ED2_SEC, float)
DECLARE_GINFO(T2_COND3_TOTP_SEC, float)
DECLARE_GINFO(T2_COND3_C5_SEC, float)
DECLARE_GINFO(C4_WAVELENGTH, float)
DECLARE_GINFO(C5_ALPHAMAX, float)
DECLARE_GINFO(C6_COND1, float)
DECLARE_GINFO(C6_COND1_ED3, float)
DECLARE_GINFO(C6_COND2, float)
DECLARE_GINFO(C6_COND2_ED2, float)
DECLARE_GINFO(C6_COND3_TOTP, float)
DECLARE_GINFO(C6_COND3_C5, float)
DECLARE_GINFO(C7_WAVELENGTH, float)
DECLARE_GINFO(SCRIPT_REVISION, uint8_t)
// Following Items are generated with On Axis image via DLL
DECLARE_GINFO(OA_PIXELS_PITCH_MM, float)
DECLARE_GINFO(OA_OBJDIST_MM, float)
DECLARE_GINFO(OA_IMDIST_MM, float)
DECLARE_GINFO(OA_WAVELENGTH_MM, float)
DECLARE_GINFO(OA_EFRAME_J, float)
DECLARE_GINFO(OA_TFRAME_SEC, float)
DECLARE_GINFO(OA_TOTFRAME_SEC, float)
DECLARE_GINFO(OA_PLR_SINGLE_RATIO, float)
DECLARE_GINFO(OA_PLR_SINGLE_ED3_RATIO, float)
DECLARE_GINFO(OA_PLR_SPT_RATIO, float)
DECLARE_GINFO(OA_PLR_SPT_ED3_RATIO, float)
DECLARE_GINFO(OA_PLR_TOTP_RATIO, float)
DECLARE_GINFO(OA_PLR_C5_RATIO, float)
DECLARE_GINFO(OA_PLR_ED2_RATIO, float)
DECLARE_GINFO(OA_CONDITION_ED2, float)
DECLARE_GINFO(OA_PLR_ED3_RATIO, float)
DECLARE_GINFO(OA_CONDITION_ED3, float)
DECLARE_GINFO(OA_Q_COND1_RATIO, float)
DECLARE_GINFO(OA_Q_COND1_ED3_RATIO, float)
DECLARE_GINFO(OA_Q_COND2_RATIO, float)
DECLARE_GINFO(OA_Q_COND2_ED3_RATIO, float)
DECLARE_GINFO(OA_Q_COND3_TOTP_RATIO, float)
DECLARE_GINFO(OA_Q_COND3_C5_RATIO, float)
DECLARE_GINFO(OA_E_T2_COND1_J, float)
DECLARE_GINFO(OA_E_T2_COND1_ED3_J, float)
DECLARE_GINFO(OA_E_T2_COND2_J, float)
DECLARE_GINFO(OA_E_T2_COND2_ED3_J, float)
DECLARE_GINFO(OA_E_T2_COND3_TOTP_J, float)
DECLARE_GINFO(OA_E_T2_COND3_C5_J, float)
DECLARE_GINFO(OA_AE_T2_COND1_J, float)
DECLARE_GINFO(OA_AE_T2_COND1_ED3_J, float)
DECLARE_GINFO(OA_AE_T2_COND2_J, float)
DECLARE_GINFO(OA_AE_T2_COND2_ED3_J, float)
DECLARE_GINFO(OA_AE_T2_COND3_TOTP_J, float)
DECLARE_GINFO(OA_AE_T2_COND3_C5_J, float)
DECLARE_GINFO(OA_AEL_SINGLE_J, float)
DECLARE_GINFO(OA_AEL_SINGLE_ED3_J, float)
DECLARE_GINFO(OA_AEL_SPT_J, float)
DECLARE_GINFO(OA_AEL_SPT_ED3_J, float)
DECLARE_GINFO(OA_AEL_TOTP_J, float)
DECLARE_GINFO(OA_AEL_C5_J, float)
DECLARE_GINFO(OA_ALPHA_COND1_MRAD, float)
DECLARE_GINFO(OA_ALPHA_COND1_ED3_MRAD, float)
DECLARE_GINFO(OA_ALPHA_COND2_MRAD, float)
DECLARE_GINFO(OA_ALPHA_COND2_ED2_MRAD, float)
DECLARE_GINFO(OA_ALPHA_COND3_TOTP_MRAD, float)
DECLARE_GINFO(OA_ALPHA_COND3_C5_MRAD, float)
DECLARE_GINFO(OA_ALPHAEFF_COND1_MRAD, float)
DECLARE_GINFO(OA_ALPHAEFF_COND1_ED3_MRAD, float)
DECLARE_GINFO(OA_ALPHAEFF_COND2_MRAD, float)
DECLARE_GINFO(OA_ALPHAEFF_COND2_ED2_MRAD, float)
DECLARE_GINFO(OA_ALPHAEFF_COND3_TOTP_MRAD, float)
DECLARE_GINFO(OA_ALPHAEFF_COND3_C5_MRAD, float)
DECLARE_GINFO(OA_ALPHAX_COND1_MRAD, float)
DECLARE_GINFO(OA_ALPHAX_COND1_ED3_MRAD, float)
DECLARE_GINFO(OA_ALPHAX_COND2_MRAD, float)
DECLARE_GINFO(OA_ALPHAX_COND2_ED2_MRAD, float)
DECLARE_GINFO(OA_ALPHAX_COND3_TOTP_MRAD, float)
DECLARE_GINFO(OA_ALPHAX_COND3_C5_MRAD, float)
DECLARE_GINFO(OA_ALPHAY_COND1_MRAD, float)
DECLARE_GINFO(OA_ALPHAY_COND1_ED3_MRAD, float)
DECLARE_GINFO(OA_ALPHAY_COND2_MRAD, float)
DECLARE_GINFO(OA_ALPHAY_COND2_ED2_MRAD, float)
DECLARE_GINFO(OA_ALPHAY_COND3_TOTP_MRAD, float)
DECLARE_GINFO(OA_ALPHAY_COND3_C5_MRAD, float)
DECLARE_GINFO(OA_ALPHA_MAX_MRAD, float)
DECLARE_GINFO(OA_ALPHA_MAX_ED3_MRAD, float)
DECLARE_GINFO(OA_PIXELSX_COND1_PX, uint16_t)
DECLARE_GINFO(OA_PIXELSX_COND1_ED3_PX, uint16_t)
DECLARE_GINFO(OA_PIXELSX_COND2_PX, uint16_t)
DECLARE_GINFO(OA_PIXELSX_COND2_ED2_PX, uint16_t)
DECLARE_GINFO(OA_PIXELSX_COND3_TOTP_PX, uint16_t)
DECLARE_GINFO(OA_PIXELSX_COND3_C5_PX, uint16_t)
DECLARE_GINFO(OA_PIXELSY_COND1_PX, uint16_t)
DECLARE_GINFO(OA_PIXELSY_COND1_ED3_PX, uint16_t)
DECLARE_GINFO(OA_PIXELSY_COND2_PX, uint16_t)
DECLARE_GINFO(OA_PIXELSY_COND2_ED2_PX, uint16_t)
DECLARE_GINFO(OA_PIXELSY_COND3_TOTP_PX, uint16_t)
DECLARE_GINFO(OA_PIXELSY_COND3_C5_PX, uint16_t)
DECLARE_GINFO(OA_CENTERX_COND1_PX, uint16_t)
DECLARE_GINFO(OA_CENTERX_COND1_ED3_PX, uint16_t)
DECLARE_GINFO(OA_CENTERX_COND2_PX, uint16_t)
DECLARE_GINFO(OA_CENTERX_COND2_ED2_PX, uint16_t)
DECLARE_GINFO(OA_CENTERX_COND3_TOTP_PX, uint16_t)
DECLARE_GINFO(OA_CENTERX_COND3_C5_PX, uint16_t)
DECLARE_GINFO(OA_CENTERY_COND1_PX, uint16_t)
DECLARE_GINFO(OA_CENTERY_COND1_ED3_PX, uint16_t)
DECLARE_GINFO(OA_CENTERY_COND2_PX, uint16_t)
DECLARE_GINFO(OA_CENTERY_COND2_ED2_PX, uint16_t)
DECLARE_GINFO(OA_CENTERY_COND3_TOTP_PX, uint16_t)
DECLARE_GINFO(OA_CENTERY_COND3_C5_PX, uint16_t)
DECLARE_GINFO(OA_T2_COND1_SEC, float)
DECLARE_GINFO(OA_T2_COND1_ED3_SEC, float)
DECLARE_GINFO(OA_T2_COND2_SEC, float)
DECLARE_GINFO(OA_T2_COND2_ED2_SEC, float)
DECLARE_GINFO(OA_T2_COND3_TOTP_SEC, float)
DECLARE_GINFO(OA_T2_COND3_C5_SEC, float)
DECLARE_GINFO(OA_C4_WAVELENGTH, float)
DECLARE_GINFO(OA_C5_ALPHAMAX, float)
DECLARE_GINFO(OA_C6_COND1, float)
DECLARE_GINFO(OA_C6_COND1_ED3, float)
DECLARE_GINFO(OA_C6_COND2, float)
DECLARE_GINFO(OA_C6_COND2_ED2, float)
DECLARE_GINFO(OA_C6_COND3_TOTP, float)
DECLARE_GINFO(OA_C6_COND3_C5, float)
DECLARE_GINFO(OA_C7_WAVELENGTH, float)
DECLARE_GINFO(OA_SCRIPT_REVISION, uint8_t)
DECLARE_GINFO_END

#pragma mark Data Post and Test Wrap Up Group (POST)
/*------------------------------------------------------------------------------
 *  Data Post and Test Wrap Up Group (POST)
 */
DECLARE_GINFO_BEGIN(POST)
DECLARE_GINFO(TEST_BIN, uint8_t)        // Test Bin, 1 char,
                                        //	Bin '1','2','3' are Pass Bin
                                        //	Bin '5','6','7' are Fail Bin
DECLARE_GINFO(COMPLIANCE_BIN, uint8_t)  // Compliance Bin
DECLARE_GINFO_END

ADD_TO_GROUP_BEGIN
ADD_TO_GROUP(LOTS)
ADD_TO_GROUP(LOTE)
ADD_TO_GROUP(DCRT)
ADD_TO_GROUP(REVV)
ADD_TO_GROUP(LEAK)
ADD_TO_GROUP(DPCK)
ADD_TO_GROUP(VCAL)
ADD_TO_GROUP(DPWC)
ADD_TO_GROUP(EMCT)
ADD_TO_GROUP(BMPF)
ADD_TO_GROUP(YPRE)
ADD_TO_GROUP(VTST)
ADD_TO_GROUP(FLDP)
ADD_TO_GROUP(CTES)
ADD_TO_GROUP(ALPH)
ADD_TO_GROUP(YGWC)
ADD_TO_GROUP(POST)
ADD_TO_GROUP(TSCR)
ADD_TO_GROUP(TSED)
ADD_TO_GROUP_END

#pragma mark -

/*------------------------------------------------------------------------------
 *  Generic Test Result Response Packet
 *  Direction
 *      Apple Test SW -> Tester Controller
 *  Payload
 *      tMTCP_payload_TEST_RSP
 *      . P_CS (1-byte)
 *  Note
 *      CTRL of the header is the transaction specific.
 *      field description below
 *      Tester Controller will evaluate the information:
 *          T_ERRC      the error code of the evaluation
 *          T_ERRS      the error string corresponding ERRC
 */
#pragma pack(1)
typedef struct tMTCP_payload_TEST_RSP
{
    uint16_t T_ERRC;     // result error code, 0 is ok
    uint8_t T_ERRS[30];  // error string
} tMTCP_payload_TEST_RSP, *tMTCP_payload_TEST_RSP_ptr;
#pragma pack()

/*------------------------------------------------------------------------------
 *  Test Session Creation
 */

#define kMTCP_CTRL_TSCR 'tscr'
/*  Tester Connect Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  payload
 *      none
 *  Note
 *      at the beginning, tester controller send this packet to
 *      request connection to Apple Test SW
 *
 */

#pragma pack(1)
typedef struct tMTCP_payload_TSCR_REQ
{
    uint8_t SOCKET_ID;          // dut id on socket, position 0, 1, 2
    int8_t SOCKET_SN[32];       // socket serial number
    float SOCKET_DCR;           // DCR value for current socket, in mOhm
    uint8_t STATION_ID;         // Test Chamber ID, 0 for loading/unloading
    int8_t STATION_SW_VER[32];  // Test Chamber SW Version, 31 char max
    int8_t DUT_SN[32];          // dut serial number, 31 char max
    uint8_t TEST_MODE;          // Mode to Run Test, 0 : Normal, 1 : GRR, 2 : Audit
} tMTCP_payload_TSCR_REQ, *tMTCP_payload_TSCR_REQ_ptr;
#pragma pack()

/*
 *  Tester Connect Response
 *  Direction
 *      Apple Test SW -> Tester Controller
 *  Payload
 *      . tMTCP_payload_TSCR_RSP
 *      . P_CS (1-byte)
 *  Timeout
 *      Apple Test SW will wait maximum 10s for this response
 *
 *----------------------------------------------------------------------------*/

#pragma pack(1)
typedef struct tMTCP_payload_TSCR_RSP
{
    uint8_t HOST_SW_VERSION[32];  // apple test sw version, 31 char max
} tMTCP_payload_TSCR_RSP, *tMTCP_payload_TSCR_RSP_ptr;
#pragma pack()

/*------------------------------------------------------------------------------
 *  Test Session Wrap Up
 */
#define kMTCP_CTRL_TSED 'tsed'

/*  Tester Disconnect Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 *  Note
 *      at the end of the test session, tester send this packet to Apple Test SW
 */

/*  Tester Disconnect Response
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_TEST_RSP
 *      . P_CS (1-byte)
 *  Timeout
 *      Test Software will wait maximum 2s for this response
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Test Sequence Information
 */
#define kMTCP_CTRL_SEQU 'sequ'
/*  Test Sequence Information Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 */

/*  Test Sequence Information Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_ALPR_RSP
 *      . P_CS (1-byte)
 *
 *  Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_SEQU_RSP
{
    uint8_t TEST_ITEM_CNT;         // number of total test items
    uint32_t TEST_CTRL_CODE[256];  // list of test to be executed, label with CTRL
    uint8_t CHAMBER_CNT;           // number of chambers
    uint8_t CHAMBER_TO_RUN[8];     // list of chambers to run 1,2,3,4,5
} tMTCP_payload_SEQU_RSP, *tMTCP_payload_SEQU_RSP_ptr;
#pragma pack()

/*------------------------------------------------------------------------------
 *  Test Initialization
 */
#define kMTCP_CTRL_INIT 'init'
/*  Test Initialization Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  payload
 *		. tMTCP_payload_INIT_REQ
 *      . P_CS (1-byte)
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_INIT_REQ
{
    int8_t TSTC_NAME[32];
    uint8_t TSTC_ID;
    int8_t TSTC_SW_VER[32];
    int8_t LOT_NAME[32];
    uint32_t LOT_SIZE;
    int8_t DIFFUSER_LOT_ID[32];
    int8_t OPERATOR_ID[32];
} tMTCP_payload_INIT_REQ, *tMTCP_payload_INIT_REQ_ptr;
#pragma pack()

/*  Test Initialization Response
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_TEST_RSP
 *      . P_CS (1-byte)
 *  Timeout
 *      Test Software will wait maximum 2s for this response
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Valencia LIV
 */
#define kMTCP_CTRL_VTST 'vtst'
/*  Valencia LIV Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *	  For PreDA and PreDA Audit, use struct of tMTCP_payload_VTST_PREDA_REQ
 *
 *      +-----------------------------+--------------+---+------------------------+------+
 *      |tMTCP_payload_VTST_PREDA_REQ | WL Reading 1 | ~ | WL Reading WL_DATA_CNT | P_CS |
 *      +-----------------------------+--------------+---+------------------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_VTST_PREDA_REQ)
 *                           + sizeof(tMTCP_data_TTDP) x WL_LEN x WL_DATA_CNT
 *                           + 1 (P_CS)
 *
 *
 *	  For PostDA and Flex level, use struct of tMTCP_payload_VTST_POSTDA_REQ
 *
 *      +------------------------------+--------------+---+------------------------+------+
 *      |tMTCP_payload_VTST_POSTDA_REQ | WL Reading 1 | ~ | WL Reading WL_DATA_CNT | P_CS |
 *      +------------------------------+--------------+---+------------------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_VTST_POSTDA_REQ)
 *                           + sizeof(tMTCP_data_TTDP) x WL_LEN x WL_DATA_CNT
 *                           + 1 (P_CS)
 *
 */

#pragma pack(1)
typedef struct tMTCP_payload_VTST_PREDA_REQ
{
    uint8_t MAGIC;                  // 0 for PreDA
    float NTC_INIT;                 // NTC read at beginning, in C
    float SOCKET_DCR;               // Socket DCR, in mOhm
    float PEAK_OPTICAL_POWER[10];   // 10 pulse peak optical power, in mW
    float FORWARD_V[10];            // 10 pulse forward voltage, in V
    float FORWARD_I[10];            // 10 pulse forward current, in A
    float CENTROID_WAVELENGTH[10];  // 10 pulse peak wavelength, in nm
    float FWHM_WAVELENGTH[10];      // 10 pulse wavelength FWHM, in nm
    uint8_t WL_DATA_CNT;            // spectrometer reading times
    uint16_t WL_LEN;                // data count from one spectrometer reading
} tMTCP_payload_VTST_PREDA_REQ, *tMTCP_payload_VTST_PREDA_REQ_ptr;
#pragma pack()

#pragma pack(1)
typedef struct tMTCP_payload_VTST_POSTDA_REQ
{
    uint8_t MAGIC;                 // 1 for PostDA
    float SOCKET_DCR;              // Socket DCR, in mOhm
    float PEAK_OPTICAL_POWER[10];  // 10 pulse peak optical power, in mW
    float PEAK_OPTICAL_POWER_0;
    float FORWARD_V[10];            // 10 pulse forward voltage, in V
    float FORWARD_I[10];            // 10 pulse forward current, in A
    float CENTROID_WAVELENGTH[10];  // 10 pulse peak wavelength, in nm
    float FWHM_WAVELENGTH[10];      // 10 pulse wavelength FWHM, in nm
    float CENTROID_WAVELENHTH_0;    // first pulse wavelength, in nm
    float FWHM_WAVELENGTH_0;        // first pulse wavelength width, in nm
    float NTC_STEADY_STATE;         // NTC read when DUT reach steady state, in C
    uint8_t WL_DATA_CNT;            // spectrometer reading times
    uint16_t WL_LEN;                // data count from one spectrometer reading
} tMTCP_payload_VTST_POSTDA_REQ, *tMTCP_payload_VTST_POSTDA_REQ_ptr;
#pragma pack()

/*  Spectrometer Reading structure
 *
 *      +---------------------+---+---------------------------+
 *      | tMTCP_data_TTDP (0) | ~ | tMTCP_data_TTDP (WL_LEN-1)|
 *      +---------------------+---+---------------------------+
 *
 *      +       WL_LEN x sizeof(tMTCP_data_TTDP)
 *     Total one spctrometer reading data length = sizeof(tMTCP_data_TTDP) x WL_LEN;
 *
 */
#pragma pack(1)
typedef struct tMTCP_data_TTDP
{
    float wavelength;  // wavelength in nm
    float measure;     // photo counts
} tMTCP_data_TTDP;
#pragma pack()

/*  Valencia LIV Test Request Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Calypso & Yogi Communication Check
 */
#define kMTCP_CTRL_DPCK 'dpck'
/*  Calypso Communication Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      . tMTCP_payload_DPCK_REQ
 *      . P_CS (1-byte)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_DPCK_REQ
{
    float C_I_IDLE_LOW;        // low power idle mode current, in uA
    uint8_t C_NVM_DATA[128];   // NVM reading raw bytes
    uint8_t Y_DEVICE_ID_H;     // High Byte for Device Id, Addr 0x00
    uint8_t Y_DEVICE_ID_L;     // Low  Byte for Device Id, Addr 0x01
    uint8_t Y_REVISION_MAJOR;  // High Byte for Revision, Addr 0x02
    uint8_t Y_REVISION_MINOR;  // Low	Byte for Revision, Addr 0x03
    uint8_t Y_TRACE_ID[4];     // Trace Id, Addr 0x04 to 0x07
    float Y_I_ACTIVE;          // Yogi Active state current, in uA
} tMTCP_payload_DPCK_REQ, *tMTCP_payload_DPCK_REQ_ptr;
#pragma pack()

/*  Calypso Communication Check Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Calypso NVM Data Write Request
 */
#define kMTCP_CTRL_DPWR 'dpwr'
/*  Calypso NVM Data Write Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 *
 */

/*  Calypso NVM Data Write Request Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_DPWR_RSP
 *      . P_CS (1-byte)
 *
 *  Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_DPWR_RSP
{
    uint8_t CNT;        // Count of bytes to write
    uint8_t ADDR[128];  // Address of bytes to write
    uint8_t DATA[128];  // data of bytes to write
} tMTCP_payload_DPWR_RSP, *tMTCP_payload_DPWR_RSP_ptr;
#pragma pack()
/*-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Calypso NVM Data Write Check
 */
#define kMTCP_CTRL_DPWC 'dpwc'
/*  Calypso NVM Data Write Check Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      . tMTCP_payload_DPWC_REQ
 *      . P_CS (1-byte)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_DPWC_REQ
{
    uint8_t NVM_DATA[128];  // NVM reading raw bytes
} tMTCP_payload_DPWC_REQ, *tMTCP_payload_DPWC_REQ_ptr;
#pragma pack()

/*  Calypso NVM Data Write Check Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Valencia Drive Current Calibration
 */
#define kMTCP_CTRL_VCAL 'vcal'
/*  Valencia Drive Current Calibration Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *      +-------+---------------+------------+------------------+------+
 *      |tMTCP_payload_VCAL_REQ | WL Reading | Calibration Data | P_CS |
 *      +-------+---------------+------------+------------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_VCAL_REQ)
 *                           + WL Reading structure size
 *                           + Calibration data structure size
 *                           + 1 (P_CS)
 *
 *  Note
 *
 */

/*  WL Reading structure
 *
 *      +---------------------+---+---------------------------+
 *      | tMTCP_data_TTDP (0) | ~ | tMTCP_data_TTDP (WL_LEN-1)|
 *      +---------------------+---+---------------------------+
 *
 *      +       WL_LEN x sizeof(tMTCP_data_TTDP)
 *     Total one spctrometer reading data length = sizeof(tMTCP_data_TTDP) x WL_DATA_LEN;
 *
 */

/*  Calibration data structure
 *
 *      +---------------------+---+-----------------------------+
 *      | tMTCP_data_ADIP (0) | ~ | tMTCP_data_ADIP (ITER_CNT-1)|
 *      +---------------------+---+-----------------------------+
 *
 *      +       ITER_CNT x sizeof(tMTCP_data_ADIP)
 *     Total calibration data length = ITER_CNT x sizeof(tMTCP_data_ADIP);
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_VCAL_REQ
{
    float NTC_STEADY_STATE;          // NTC read when DUT reach steady state, in C
    float T_STEADY_STATE;            // Time to reach steady state, in ms
    float PEAK_OPTICAL_POWER_FINAL;  // final peak optical power, in mW
    float I_PULSE_FINAL;             // final pulse current, in A
    float V_PULSE_FINAL;             // final pulse voltage, in V
    float PEAK_WAVELENGTH;           // peak wavelength, in nm
    float FWHM_WAVELENGTH;           // wavelength FWHM, in nm
    uint16_t WL_DATA_LEN;            // data count from one spectrometer reading
    uint16_t ITER_CNT;               // number of adjust current iternations
} tMTCP_payload_VCAL_REQ, *tMTCP_payload_VCAL_REQ_ptr;
#pragma pack()

#pragma pack(1)
typedef struct tMTCP_data_ADIP
{
    float I_pulse;        // I_pulse at this step
    float V_pulse;        // forward voltage at this step
    float optical_power;  // peak optical power with I_pulse
    float V_pulse_raw[10];
    float optical_power_row[340];
} tMTCP_data_ADIP;
#pragma pack()

/*  Valencia Drive Current Calibration Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Request Valencia Calibated Drive Current
 */
#define kMTCP_CTRL_VDCR 'vdcr'
/*  Valencia Calibrated Drive Current Request Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 */

/*  Valencia Calibrated Drive current Request Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_VDCR_RSP
 *      . P_CS (1-byte)
 *
 *  Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_VDCR_RSP
{
    float I_DR;      // valencia calibrated drive current
    float T_SETTLE;  // time to reach steady state
} tMTCP_payload_VDCR_RSP, *tMTCP_payload_VDCR_RSP_ptr;
#pragma pack()
/*
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Class 1 Skin Compliance Test
 */
#define kMTCP_CTRL_CTSS 'ctss'
/*  Class 1 Skin Compliance Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *    Total payload length = sizeof(tMTCP_payload_CTSS_REQ)
 *                           + 1 (P_CS)
 *
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_CTSS_REQ
{
    float NTC_STEADY_STATE;        // NTC read when DUT reach steady state, in C
    float I_FORWARD[10];           // measured drive current, in A
    float V_FORWARD[10];           // forward voltage for 10 pulses, in V
    float PEAK_OPTICAL_POWER[10];  // total optical power for 10 pulses, in mW
} tMTCP_payload_CTSS_REQ, *tMTCP_payload_CTSS_REQ_ptr;
#pragma pack()

/*  Class 1 Skin Compliance Test Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  VSCEL Emitter Test
 */
#define kMTCP_CTRL_EMCT 'emct'
/*  VSCEL Emitter Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *      +-------+---------------+------------+------+
 *      |tMTCP_payload_EMCT_REQ | Image Data | P_CS |
 *      +-------+---------------+------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_EMCT_REQ)
 *                           + IMG_SIZE in bytes
 *                           + 1 (P_CS)
 *
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_EMCT_REQ
{
    uint16_t IMG_WIDTH;              // Image Width;
    uint16_t IMG_HEIGHT;             // Image Height;
    uint32_t IMG_SIZE;               // Image File Size
    float CLUSTER_THRESHOLD;         // Threshold for Clustering, percentage of max pixel intensity
    uint8_t VALID_EMITTER_CNT;       // valid emitter counts
    uint16_t EMITTER_CENTER_X[256];  // X position of each emitter center, in pixel
    uint16_t EMITTER_CENTER_Y[256];  // Y position of each emitter center, in pixel
    uint16_t EMITTER_SIZE[256];      // Number of pixels in each emitter
    float UNIF_MAX;                  // max optical power of each emitter
    float UNIF_MIN;                  // min optical power of each emitter
    float UNIF_STD;                  // optical power stanard deviation of each emitter
    float SOCKET_DCR;                // Socket DCR, in mOhm
    float FORWARD_V[10];             // 10 pulse forward voltage, in V
    float FORWARD_I[10];             // 10 pulse forward current, in A
} tMTCP_payload_EMCT_REQ, *tMTCP_payload_EMCT_REQ_ptr;
#pragma pack()

/*  VSCEL Emitter Test Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  VSCEL Mode Profile Test
 */
#define kMTCP_CTRL_BMPF 'bmpf'
/*  VSCEL Emitter Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *      +-------+---------------+----------------+------+
 *      |tMTCP_payload_BMPF_REQ | Image Raw Data | P_CS |
 *      +-------+---------------+----------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_EMCT_REQ)
 *                           + IMG_SIZE
 *                           + 1 (P_CS)
 *
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_BMPF_REQ
{
    uint16_t IMG_WIDTH;         // Image Width;
    uint16_t IMG_HEIGHT;        // Image Height;
    uint32_t IMG_SIZE;          // Image Binary Size;
    float TOTAL_OPTICAL_POWER;  // total optical power
    float BEAM_SIZE;            // beam size
    float HOT_SPOT_POWER;       // max optical power in aperture scan
    uint16_t HOT_SPOT_LOC_X;    // hot spot position in X
    uint16_t HOT_SPOT_LOC_Y;    // hot spot position in Y
    float SOCKET_DCR;           // Socket DCR, in mOhm
    float FORWARD_V[10];        // 10 pulse forward voltage, in V
    float FORWARD_I[10];        // 10 pulse forward current, in A
} tMTCP_payload_BMPF_REQ, *tMTCP_payload_BMPF_REQ_ptr;
#pragma pack()

/*  VSCEL Mode Profile Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Flood Performance Test
 */
#define kMTCP_CTRL_FLDP 'fldp'
/*  Flood Performance Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      . tMTCP_payload_FLDP_REQ
 *      . P_CS (1-byte)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_FLDP_REQ
{
    uint8_t IMG_CNT;             // number of images
    float SCREEN_REFLECTANCE;    // calibrated screen reflectance
    float POWER_IN_FOV;          // power in FOV, in mW
    float POWER_EFFICIENCY;      // optical power efficiency, in %
    float HFNU;                  // in %
    float LFNU_P_C[9];           // P1C1, P1C2, P1C3, P2C1, P2C2, P2C3, P3C1, P3C2, P3C3, in %
    float OPTICAL_POWER_MAX[9];  // Optical power max for each region
    float OPTICAL_POWER_MIN[9];  // optical power min for each region
    float OPTICAL_POWER_AVG[9];  // optical power avg for each region
    float SOCKET_DCR;            // Socket DCR, in mOhm
    float FORWARD_V[10];         // 10 pulse forward voltage, in V
    float FORWARD_I[10];         // 10 pulse forward current, in A
} tMTCP_payload_FLDP_REQ, *tMTCP_payload_FLDP_REQ_ptr;
#pragma pack()

#define kMTCP_CTRL_FLDI 'fldi'
/*  Flood Performance Image Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *      +-------+---------------+----------------------+----------------+------+
 *      |tMTCP_payload_FLDI_REQ | Flat Field Cal Array | Image Raw Data | P_CS |
 *      +-------+---------------+----------------------+----------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_FLDI_REQ)
 *                           + IMG_WIDTH x IMG_HEIGHT x sizeof(float)
 *                           + IMAGE_SIZE
 *                           + 1 (P_CS)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_FLDI_REQ
{
    uint8_t IMG_INDEX;           // Image Index;
    uint16_t IMG_WIDTH;          // Image Width;
    uint16_t IMG_HEIGHT;         // Image Height;
    uint32_t IMG_SIZE;           // Image size;
    int16_t POS_X;               // camera coordinate X
    int16_t POS_Y;               // camera coordicate Y
    float POWER_TO_PIXEL_RATIO;  // power to pixel ratio
} tMTCP_payload_FLDI_REQ, *tMTCP_payload_FLDI_REQ_ptr;
#pragma pack()

#pragma pack(1)
typedef struct tMTCP_payload_FLDI_FLX_REQ
{
    uint8_t IMG_INDEX;           // Image Index;
    uint16_t IMG_WIDTH;          // Image Width;
    uint16_t IMG_HEIGHT;         // Image Height;
    uint32_t IMG_SIZE;           // Image size;
    int16_t POS_X;               // camera coordinate X
    int16_t POS_Y;               // camera coordicate Y
    float POWER_TO_PIXEL_RATIO;  // power to pixel ratio
    float NTC_STEADY_STATE;      // NTC read when DUT reach steady state, in C
} tMTCP_payload_FLDI_FLX_REQ, *tMTCP_payload_FLDI_FLX_REQ_ptr;
#pragma pack()

/*  Flood Performance Test Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Class 1 Laser Compliance Test
 */
#define kMTCP_CTRL_CTES 'ctes'
/*  Flood Performance Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      . tMTCP_payload_FLDP_REQ
 *      . P_CS (1-byte)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_CTES_REQ
{
    uint8_t IMG_CNT;           // number of images
    float SCREEN_REFLECTANCE;  // calibrated screen reflectance
    float HOT_SPOT_THETA;      // theta angle for hot spot
    float HOT_SPOT_PHI;        // phi angle for hot spot
    float AE_POWER;            // energy level for AE
    float OPTICAL_POWER;       //
} tMTCP_payload_CTES_REQ, *tMTCP_payload_CTES_REQ_ptr;
#pragma pack()

/*  Calss 1 Laser Compliance Test Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Class 1 Alpha Measurement Position Request
 */
#define kMTCP_CTRL_ALPR 'alpr'
/*  Class 1 Alpha Measurement Position Request Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 */

/*  Class 1 Alpha Measurement Position Request Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_ALPR_RSP
 *      . P_CS (1-byte)
 *
 *  Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_ALPR_RSP
{
    float HOT_SPOT_THETA;  // theta angle for hot spot
    float HOT_SPOT_PHI;    // phi angle for hot spot
} tMTCP_payload_ALPR_RSP, *tMTCP_payload_ALPR_RSP_ptr;
#pragma pack()
/*
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Class 1 Alpha Measurement Test
 */
#define kMTCP_CTRL_ALPH 'alph'
/*  Class 1 Alpha measurement Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *      +-------+---------------+----------------+---+----------------------+------+
 *      |tMTCP_payload_ALPH_REQ | Image Binary 1 | ~ | Image Binary IMG_CNT | P_CS |
 *      +-------+---------------+----------------+---+----------------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_ALPH_REQ)
 *                           + IMG_SIZE x IMG_CNT;
 *                           + 1 (P_CS)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_ALPH_REQ
{
    uint8_t IMG_CNT;      // number of images
    uint16_t IMG_WIDTH;   // image width
    uint16_t IMG_HEIGHT;  // image height
    uint32_t IMG_SIZE;    // image binary size for each image
    float SOCKET_DCR;     // Socket DCR, in mOhm
    float FORWARD_V[10];  // 10 pulse forward voltage, in V
    float FORWARD_I[10];  // 10 pulse forward current, in A
} tMTCP_payload_ALPH_REQ, *tMTCP_payload_ALPH_REQ_ptr;
#pragma pack()

#pragma pack(1)
typedef struct tMTCP_payload_ALPH_FLX_REQ
{
    float NTC_STEADY_STATE;  // NTC read when DUT reach steady state, in C
    uint8_t IMG_CNT;         // number of images
    uint16_t IMG_WIDTH;      // image width
    uint16_t IMG_HEIGHT;     // image height
    uint32_t IMG_SIZE;       // image binary size for each image
    float SOCKET_DCR;        // Socket DCR, in mOhm
    float FORWARD_V[10];     // 10 pulse forward voltage, in V
    float FORWARD_I[10];     // 10 pulse forward current, in A
} tMTCP_payload_ALPH_FLX_REQ, *tMTCP_payload_ALPH_FLX_REQ_ptr;
#pragma pack()

/*------------------------------------------------------------------------------
 *  Data Posting to Panda
 */
#define kMTCP_CTRL_POST 'post'
/*  Data Posting to Panda Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 */

/*  Data Posting to Panda Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_POST_RSP
 *      . P_CS (1-byte)
 *
 *  Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_POST_RSP
{
    uint16_t T_ERRC;     // result error code, 0 is ok
    uint8_t T_ERRS[30];  // error string
    uint8_t T_BIN;       // test binning information
} tMTCP_payload_POST_RSP, *tMTCP_payload_POST_RSP_ptr;
#pragma pack()

/*------------------------------------------------------------------------------
 *  Yogi Fault Condition Test
 */
#define kMTCP_CTRL_YGTS 'ygts'
/*  Yogi Fault Condition Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *   PreDA
 *      . tMTCP_payload_YGTS_PREDA_REQ
 *      . P_CS (1-byte)
 *
 *
 *   PostDA
 *      . tMTCP_payload_YGTS_POSTDA_REQ
 *      . P_CS (1-byte)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_YGTS_PREDA_REQ
{
    float Y_I_ACTIVE;          // Yogi Active state current, in uA
    float Y_I_STANDBY;         // Yogi standby state current, in uA
    float I_CONDITION_A;       // Fault A current measurement, in A
    uint8_t FAULT_STATUS_A;    // Fault A Reg 0x24
    float I_CONDITION_B1;      // Fault B1 current measurement, in A
    uint8_t FAULT_STATUS_B1;   // Fault B1 Reg 0x24
    float I_CONDITION_B2;      // Fault B2 current measurement, in A
    uint8_t FAULT_STATUS_B2;   // Fault B2 Reg 0x24
    float I_CONDITION_B3;      // Fault B3 current measurement, in A
    uint8_t FAULT_STATUS_B3;   // Fault B3 Reg 0x24
    float I_CONDITION_C;       // Fault C current measurement, in A
    uint8_t FAULT_STATUS_C;    // Fault C Reg 0x24
    float I_CONDITION_D;       // Fault D current measurement, in A
    uint8_t FAULT_STATUS_D;    // Fault D Reg 0x24
    float I_CONDITION_E;       // Fault E current measurement, in A
    uint8_t FAULT_STATUS_E_1;  // Fault E1 Reg 0x24
    uint8_t FAULT_STATUS_E_2;  // Fault E2 Reg 0x24
    uint8_t LEVEL_STATUS_PIN;  // Status pin level, PostDA only
    float I_STATUS_PIN;        // Status pin current
} tMTCP_payload_YGTS_PREDA_REQ, *tMTCP_payload_YGTS_PREDA_REQ_ptr;
#pragma pack()

#pragma pack(1)
typedef struct tMTCP_payload_YGTS_FLX_REQ
{
    float Y_I_ACTIVE;          // Yogi Active state current, in uA
    float Y_I_STANDBY;         // Yogi standby state current, in uA
    float I_CONDITION_A;       // Fault A current measurement, in uA
    uint8_t FAULT_STATUS_A;    // Fault A Reg 0x24
    float I_CONDITION_B1;      // Fault B1 current measurement, in uA
    uint8_t FAULT_STATUS_B1;   // Fault B1 Reg 0x24
    float I_CONDITION_B2;      // Fault B2 current measurement, in uA
    uint8_t FAULT_STATUS_B2;   // Fault B2 Reg 0x24
    float I_CONDITION_B3;      // Fault B3 current measurement, in uA
    uint8_t FAULT_STATUS_B3;   // Fault B3 Reg 0x24
    float I_CONDITION_C;       // Fault C current measurement, in uA
    uint8_t FAULT_STATUS_C;    // Fault C Reg 0x24
    float I_CONDITION_D;       // Fault D current measurement, in uA
    uint8_t FAULT_STATUS_D;    // Fault D Reg 0x24
    float I_CONDITION_E;       // Fault E current measurement, in uA
    uint8_t FAULT_STATUS_E_1;  // Fault E1 Reg 0x24
    uint8_t FAULT_STATUS_E_2;  // Fault E2 Reg 0x24
    uint8_t LEVEL_STATUS_PIN;  // Status pin level, PostDA only
    float I_STATUS_PIN;        // Status pin current
    float Y_STATUS_L_IF_UA;    // IF when Yogi status is low, in uA
    float Y_STATUS_L_VF_V;     // VF when Yogi status is low, in V
} tMTCP_payload_YGTS_FLX_REQ, *tMTCP_payload_YGTS_FLX_REQ_ptr;
#pragma pack()

#pragma pack(1)
typedef struct tMTCP_payload_YGTS_POSTDA_REQ
{
    float Y_I_ACTIVE;          // Yogi Active state current, in uA
    float Y_I_STANDBY;         // Yogi standby state current, in uA
    float I_CONDITION_A;       // Fault A current measurement, in A
    uint8_t FAULT_STATUS_A;    // Fault A Reg 0x24
    float V_FORWARD_STATUS;    // forward voltage with 10mA drive, in V
    float I_FORWARD_STATUS;    // forward current with 10mA drive, in A
    uint8_t LEVEL_STATUS_PIN;  // Status pin level, PostDA only
    float I_STATUS_PIN;        // Status pin current
} tMTCP_payload_YGTS_POSTDA_REQ, *tMTCP_payload_YGTS_POSTDA_REQ_ptr;
#pragma pack()

/*  Yogi Fault Condition Test Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Yogi Calibrated Current Request
 */
#define kMTCP_CTRL_YGTR 'ygtr'
/*  Yogi Calibrated Current Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 *
 */

/*  Yogi Calibrated Current Request Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_YGTR_RSP
 *      . P_CS (1-byte)
 *
 *  Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_YGTR_RSP
{
    float I_CAL;
    float I_PULSEMAX_B1;     // Yogi calibrated drive current for B1 Fault
    float I_PULSEMAX_B2;     // Yogi calibrated drive current for B2 Fault
    float I_PULSEMAX_B3;     // Yogi calibrated drive current for B3 Fault
    uint8_t SEL_I_PULSEMAX;  // Reg setting for I_pulse
    uint8_t SEL_I_SEVERE;    // Reg setting for I_severe
} tMTCP_payload_YGTR_RSP, *tMTCP_payload_YGTR_RSP_ptr;
#pragma pack()
/*-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Yogi NVM Write Data Request
 */
#define kMTCP_CTRL_YGWR 'ygwr'
/*  Yogi NVM Write Data Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 *
 */

/*  Yogi NVM Write Data Request Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_YGWR_RSP
 *      . P_CS (1-byte)
 *
 *  Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_YGWR_RSP
{
    uint8_t CNT;       // Count of bytes to write
    uint8_t ADDR[16];  // Address of bytes to write
    uint8_t DATA[16];  // data of bytes to write
} tMTCP_payload_YGWR_RSP, *tMTCP_payload_YGWR_RSP_ptr;
#pragma pack()
/*-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Yogi NVM Data Write Check Check
 */
#define kMTCP_CTRL_YGWC 'ygwc'
/*  Yogi NVM Data Write Check Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      . tMTCP_payload_YGWC_REQ
 *      . P_CS (1-byte)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_YGWC_REQ
{
    uint8_t CONFIG_CRC_H;       // CRC_H from TEST_CONFIG_CRC_H Register
    uint8_t CONFIG_CRC_L;       // CRC_L from TEST_CONFIG_CRC_L Register
    uint8_t CONFIG_CRC_H_CALC;  // calculated CRC_H from 0x12 to 0x21
    uint8_t CONFIG_CRC_L_CALC;  // calculated CRC_L from 0x12 to 0x21
    uint8_t PROG0_REG;          //
    uint8_t Y_NVM_1[18];        // Yogi NVM from 0x12 to 0x23, Step 1
    uint8_t Y_NVM_2[18];        // Yogi NVM from 0x12 to 0x23, Step 2
    uint8_t Y_NVM_3[18];        // Yogi NVM from 0x12 to 0x23, Step 3
} tMTCP_payload_YGWC_REQ, *tMTCP_payload_YGWC_REQ_ptr;
#pragma pack()

/*  Yogi NVM Data Write Check Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

#endif /* _ML_ROSALINE_MTCP_H_ */
