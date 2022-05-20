#pragma once
#include"stdafx.h"
#include"PacketType.h"
#include"Packet.h"
#include<string>

namespace PS//packet structure namespace
{
	class ChatMessage
	{
	public:
		ChatMessage(std::string);
		Packet toPacket();//Convert chat message to packet
	private:
		std::string message;

	};
}