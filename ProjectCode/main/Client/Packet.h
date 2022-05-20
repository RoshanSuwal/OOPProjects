#pragma once
#include"stdafx.h"
class Packet
{
public:
	Packet();
	Packet(char* buffer, int size);//will use existing allocated buffer
	Packet(const Packet &p);//will allocate new buffer but copy buffer from Packet argument
	int size;
	char *buffer;
};
