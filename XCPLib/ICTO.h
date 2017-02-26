#pragma once
#include <cstdint>
#include "IXCPPacket.h"

enum CTOSlaveToMasterPacketTypes
{
	RES = 0xFF,
	ERR = 0xFE,
	EV = 0xFD,
	SERV = 0xFC,
};

enum CTOMasterToSlaveCommands
{
	CONNECT = 0xFF,
	DISCONNECT = 0xFE,
	GET_STATUS = 0xFD,
	SYNCH = 0xFC,
	GET_COMM_MODE_INFO = 0xFB,
	GET_ID = 0xFA,
	SET_REQUEST = 0xF9,
	GET_SEED = 0xF8,
	UNLOCK = 0xF7,
	SET_MTA = 0xF6,
	UPLOAD = 0xF5,
	SHORT_UPLOAD = 0xF4,
	BUILD_CHECKSUM = 0xF3,
	TRANSPORT_LAYER_CMD = 0xF2,
	USER_CMD = 0xF1
};

class ICTO : public IXCPPacket
{
public:
	ICTO() : IXCPPacket() {}
	virtual ~ICTO() {}
};