#include"stdafx.h"
#include"PacketStruct.h"
#include<Windows.h>

namespace PS
{
	ChatMessage::ChatMessage(std::string msg) {
		message = msg;
	}

	Packet ChatMessage::toPacket() {
		const int packetsize = sizeof(int32_t) * 2 + message.size();//total size of buffer
		char *buffer = new char[packetsize];//create buffer big enough to hold all info of  message ...
		int32_t packettype = htonl((int32_t)PacketType::ChatMessage);//convert packet from host to network byte order
		int32_t messagesize = htonl(message.size());//convert message size from host to network byte order
		memcpy(buffer, &packettype, sizeof(int32_t));//copy packet type to buffer]
		memcpy(buffer + sizeof(int32_t), &messagesize, sizeof(int32_t));
		memcpy(buffer + sizeof(int32_t) * 2, message.c_str(), message.size() * sizeof(char));

		Packet p(buffer, packetsize);//create packet to return
		return p;
	}
}