#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "stdafx.h"
#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<cstdio>
#include<WS2tcpip.h>
#include<iostream>
#include<string>
#include<list>
#include"FileTransferData.h"
#include"PacketType.h"



struct Connection {
	SOCKET socket;
	FileTransferData file;
	
};
class Server
{
public:
	Server(int PORT, bool BroadcastPublically = false);
	bool ListenForNewConnection();
	bool sendNext = true;
	void addToSend(std::string &filename, std::string &filePath);
	void convertToCompatible(std::string &name);

	std::string s_sendingFileName;
	double s_totalsize=0;
	double s_sentfileSize=0;
	bool newFileRequest = false;
	bool fileNotsentYet = false;

private:
	bool sendPhoto(int ID,std::string &fileName);

	bool sendall(int ID, char *data, int totalbytes);
	bool recvall(int ID, char *data, int totalbytes);

	bool Sendint32_t(int ID, int32_t _int32_t);
	bool Getint32_t(int ID, int32_t & _int32_t);

	bool SendPacketType(int ID, PacketType _packettype);
	bool GetPacketType(int ID, PacketType & _packettype);

	bool SendString(int ID, std::string & _string);
	bool GetString(int ID, std::string & _string);

	bool ProcessPacket(int ID, PacketType _packettype);
	bool HandleSendFile(int ID);

	static void ClientHandlerThread(int ID);
	static void PacketSenderThread();

	 std::list<std::string> fileList;
	 std::list<std::string> filePath;


private:
	Connection connections[100];
	int TotalConnections = 0;

	std::list<std::string>::iterator p_filepath;
	sockaddr_in addr;//Address  that we will bind our listening socket to
	int addrlen = sizeof(addr);//length of the address(required to accept the call)
	SOCKET sListen;
};

static Server *serverptr;//need server pointer so that static method can get its instance and use its methods
