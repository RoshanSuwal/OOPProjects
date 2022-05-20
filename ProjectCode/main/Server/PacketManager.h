#pragma once
#include"stdafx.h"
#include"Packet.h"
#include<queue>
#include<mutex>

class PacketManager
{
private:
	std::queue<Packet> queue_packets;
	std::mutex mutes_packets;
public:
	bool HasPendingPackets();
	void Append(Packet P);
	Packet Retreive();

};