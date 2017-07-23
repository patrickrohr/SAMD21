/*
* TextPacket.h
*
* Created: 7/22/2017 9:59:31 PM
*  Author: Patrick Rohr
*/ 


#ifndef __PACKET_TYPE_H__
#define __PACKET_TYPE_H__

#include "GlobalConfig.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum {PACKET_TYPE_TEXT, PACKET_TYPE_HDLC} PacketType_enum;

typedef struct {
	uint32_t length;
	char text;
} TextPacket_t;

// TODO
typedef struct {
	uint32_t length;
	char text;
} HDLCPacket_t;

typedef struct {
	uint8_t arr[MAX_PACKET_DATA_LENGTH];
} DebugPacketOverlay_t;

bool text_packet_push_char(TextPacket_t * self, char c);

#endif