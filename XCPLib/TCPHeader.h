#pragma once
#include <cstdint>
#include "IXCPHeader.h"

class TCPHeader : public IXCPHeader
{
private:
	uint16_t m_Length;
	uint16_t m_Ctr;
public:
	TCPHeader(uint16_t len, uint16_t ctr);
	virtual ~TCPHeader();
	//virtual uint8_t* GetRawBytes();
	void SetLength(uint16_t len);
	uint16_t GetLength();
	void SetCtr(uint16_t ctr);
	uint16_t GetCtr();
	virtual void Serialize(std::vector<uint8_t>& out_Binary);
	virtual uint8_t GetSize();
};