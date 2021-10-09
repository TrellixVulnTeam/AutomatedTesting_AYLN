//
//  MTCP.h
//
//  Created by Joon heup Kwon on 11/9/13.
//  Copyright 2013 Apple. All rights reserved.
//

#ifndef _MTCP_H_
#define _MTCP_H_
#include <cstdint>


// #include <AssertMacros.h>
// #include <pthread.h>
// #include "ml_socket.h"

#pragma mark -
#pragma mark MTCP protocol definition
/*  MTCP packet format 
 
 *** MTCP Header (16-byte) ***
 
     0       1       2       3       4       5       6       7       8       9       10      11      12      13      14      15
 +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
 |          MTCP_PREAMBLE        |      ERRC     |             CTRL              |              PLEN             | SQCN  | H_CS  |
 +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
 
 Byte0~3:    MTCP:   preamble, MTCP_PREAMBLE
 Byte4~5:    ERRC:   valid only for backward packet
 Byte6~9:    CTRL:   control id, cannot be zero
 Byte10~13:  PLEN:   payload data length (include 1-byte payload checksum, P_CS)
 Byte14:     SQCN:   sequence number (0x00 for the request that doesn't require response)
 Byte15:     H_CS:   header checksum, 1's complement of sum of Byte0~12
 
 Payload follows packet header when PLEN>0
 
 *** MTCP Payload ***
when PLEN>1, the payload data includes 1-byte checksum, P_CS.
 
     0       ~     PLEN-2  PLEN-1
 +-------+   ~   +-------+-------+
 |      payload_data     | P_CS  |   
 +-------+   ~   +-------+-------+
 
 payload_data:      Byte0 ~ Byte[PLEN-2], CTRL specific data
 P_CS:              Byte[PLEN-1] the checksum of payload_data, 
                    1's complement of sum of Byte0~Byte[PLEN-2], see below _MTCP_calculateCheckSum
 
 * note *
 - the payload content format is CTRL specific, and to be defined per-CTRL basis
 - P_CS is the checksum of "unscrambled payload data" if the payload is encoded/scrambled
 */


/*  MTCP checksum function
 */
static inline uint8_t _MTCP_calculateCheckSum (uint8_t* buff, uint32_t buffLeng){
	uint8_t rtn = 0;
	for (uint32_t i=0;i<buffLeng;i++) rtn += buff[i];
	rtn = ~rtn;
	return rtn;
}

/*  MTCP packet header 
 */
#define MTCP_PREAMBLE   'MTCP'

#pragma pack(1)
typedef struct tMTCP_header{
	uint32_t    MTCP;   //  MTCP_PREAMBLE
    uint16_t    ERRC;   //  valid only for backward packet, tMTCP_ERR code (defined below)
	uint32_t    CTRL;   //  packet control id (defined below)
	uint32_t    PLEN;   //  payload length (if any), CTRL specific
	uint8_t     SEQN;   //  forward packet sequence number
	uint8_t     H_CS;   //  header checksum, 1's complement of sum of Byte0~6
} tMTCP_header;
#pragma pack()
#ifndef __cplusplus
check_compile_time(sizeof(tMTCP_header)==16);
#endif

#pragma pack(1)
typedef struct tMTCP_packet{
	tMTCP_header	header;
	uint8_t			payload[];
} tMTCP_packet;
#pragma pack()
#ifndef __cplusplus
check_compile_time(sizeof(tMTCP_packet)==16);
#endif

/*------------------------------------------------------------------------------
 *  Error Codes
 *  
 *  Note:
 *      [CLOSE ON ERROR] 
 *          it indicates that the Apple SW will close the connection on error
 */
typedef enum{
	// 0~19     Generic Error
	kMTCP_ERR_OK                        = 0,
	kMTCP_ERR_unknown                   = 1,    // [CLOSE ON ERROR]
	kMTCP_ERR_invalid_preamble          = 2,    // [CLOSE ON ERROR]
	kMTCP_ERR_checksum_error            = 3,
	kMTCP_ERR_unknown_ctrl_code         = 4,
	kMTCP_ERR_invalid_param             = 5,
	kMTCP_ERR_test_evaluation_error     = 6,
	kMTCP_ERR_memory_allocation_failed  = 7,
	kMTCP_ERR_unexpected_payload_size   = 8,
	kMTCP_ERR_invalid_test_condition    = 9,
	kMTCP_ERR_invalid_pheader           = 10,   // [CLOSE ON ERROR]
	// 20~29    test session creation
	kMTCP_ERR_test_session_already_exist= 20,   // [CLOSE ON ERROR]
	kMTCP_ERR_invalid_handler_name      = 21,   // [CLOSE ON ERROR]
	kMTCP_ERR_invalid_tester_name       = 22,   // [CLOSE ON ERROR]
	kMTCP_ERR_invalid_tester_id         = 23,   // [CLOSE ON ERROR]
	kMTCP_ERR_duplicated_tester_id      = 24,   // [CLOSE ON ERROR]
	kMTCP_ERR_invalid_sw_version        = 25,   // [CLOSE ON ERROR]
	kMTCP_ERR_invalid_hw_version        = 26,   // [CLOSE ON ERROR]
	kMTCP_ERR_invalid_serial_number     = 27,   // [CLOSE ON ERROR]
	kMTCP_ERR_duplicated_serial_number  = 28,   // [CLOSE ON ERROR]
	
	//kMTCP_ERR_max                       = 29,
	kMTCP_ERR_max = 32,
} tMTCP_ERR;

static inline bool _MTCP_isCloseOnError_errorCode(int err){
	return (    err == kMTCP_ERR_unknown
			||  err == kMTCP_ERR_invalid_preamble
			||  err == kMTCP_ERR_invalid_pheader
			||  err == kMTCP_ERR_test_session_already_exist
			||  err == kMTCP_ERR_invalid_handler_name
			||  err == kMTCP_ERR_invalid_tester_name
			||  err == kMTCP_ERR_invalid_tester_id
			||  err == kMTCP_ERR_duplicated_tester_id
			||  err == kMTCP_ERR_invalid_sw_version
			||  err == kMTCP_ERR_invalid_hw_version
			||  err == kMTCP_ERR_invalid_serial_number
			||  err == kMTCP_ERR_duplicated_serial_number
			);
}

#endif//ifndef _MTCP_H_
