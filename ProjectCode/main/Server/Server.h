#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "stdafx.h"
#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<cstdio>
#include<WS2tcpip.h>
#include<iostream>
#include<string>
#include"FileTransferData.h"
#include"PacketType.h"
#include"PacketManager.h"
#include"PacketStruct.h"

struct Connection {
	SOCKET socket;
	FileTransferData file;
	PacketManager pm;
};
class Server
{
public:
	Server(int PORT, bool BroadcastPublically = false);
	bool ListenForNewConnection();

private:
	bool sendall(int ID, char *data, int totalbytes);
	bool recvall(int ID, char *data, int totalbytes);

	bool Sendint32_t(int ID, int32_t _int32_t);
	bool Getint32_t(int ID, int32_t & _int32_t);

	bool SendPacketType(int ID, PacketType _packettype);
	bool GetPacketType(int ID, PacketType & _packettype);

	void SendString(int ID,std::string & _string);
	bool GetString(int ID, std::string & _string);

	bool ProcessPacket(int ID,PacketType _packettype);
	bool HandleSendFile(int ID);

	static void ClientHandlerThread(int ID);
	static void PacketSenderThread();


private:
	Connection connections[100];
	int TotalConnections = 0;

	sockaddr_in addr;//Address  that we will bind our listening socket to
	int addrlen = sizeof(addr);//length of the address(required to accept the call)
	SOCKET sListen;
};

static Server *serverptr;//need server pointer so that static method can get its instance and use its methods
