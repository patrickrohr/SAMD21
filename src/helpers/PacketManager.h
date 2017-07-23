/*
* BufferManager.h
*
* Created: 6/12/2017 8:31:12 PM
*  Author: Patrick Rohr
*/ 

#ifndef __PACKET_MANAGER_H__
#define __PACKET_MANAGER_H__

#include "BufferPool.h"


Packet_t * packet_manager_get_packet();
void packet_manager_free_packet(Packet_t * packet_ptr);

#endif