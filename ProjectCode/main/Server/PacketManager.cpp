#include"stdafx.h"
#include"PacketManager.h"


bool PacketManager::HasPendingPackets() {

	return (queue_packets.size() > 0);//return true if size >0

}

void PacketManager::Append(Packet p) {
	std::lock_guard<std::mutex> lock(mutes_packets);
	queue_packets.push(p);//add packets to queue

}

Packet PacketManager::Retreive() {
	std::lock_guard<std::mutex> lock(mutes_packets);
	Packet p = queue_packets.front();//get packet from front of queue
	queue_packets.pop();//remove packet from front of the queue
	return p;//return packet that was removed from the queue
}