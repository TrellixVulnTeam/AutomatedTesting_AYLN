//////////////////////////////////////////////////////////////////////////
//
// File Name		: MTCProto.h
//
// Description		: Defines the exported functions for the DLL application.
//
// Author			: James.Gu
//
// Date				: 09/25/2016
//
//////////////////////////////////////////////////////////////////////////
#ifndef __MTCPROTOCOL__H__
#define __MTCPROTOCOL__H__

#include "ml_mtcp.h"
#include "MTCP_Rosaline.h"

#define IN	
#define OUT
#define INOUT

#define MP_SUCCESSFUL		0					// successfult

#define MP_FAILED			-999				// failed
#define MP_MTCP_HOST_FAILED	-998				// socket failed
#define MP_SOCKET_FAILED	-997				// socket failed
#define MP_SOCKET_TIMEOUT	-996				// socket timeout

#if (_WIN32)
#define MTCP_EXPORT		__declspec(dllexport)
#else //
// ...
#endif //

typedef unsigned int		uint32_t;
typedef unsigned short		uint16_t;
typedef unsigned char		uint8_t;

typedef int			int32_t;
typedef short		int16_t;


//////////////////////////////////////////////////////////////////////////
#endif //__MTCPROTOCOL__H__
//////////////////////////////////////////////////////////////////////////


