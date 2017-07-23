/*
* TextPacket.c
*
* Created: 7/22/2017 9:59:40 PM
*  Author: Patrick Rohr
*/ 

#include "PacketType.h"
#include "GlobalConfig.h"
#include <string.h>
#include <stdbool.h>

const int max_text_packet_data_length = MAX_PACKET_DATA_LENGTH - offsetof(TextPacket_t, text);

// pushes character on the back of a text packet and returns true once the packet is full or a carriage return was detected
bool text_packet_push_char(TextPacket_t * self, char c)
{
	strcpy((char *)(&(self->text) + self->length), &c);
	if (c == 0x0D) // carriage return ends text packet
	{
		return true;
	}

	return ++(self->length) >= max_text_packet_data_length;
}