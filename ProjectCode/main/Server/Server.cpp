#include"stdafx.h"
#include"Server.h"

Server::Server(int PORT, bool BroadcastPublically) {//port=port to boardcast on

	//winsock Startup
	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 2);
	if (WSAStartup(DllVersion, &wsaData) != 0)//if WSAStartup returns nothing other than 0 then an error has occured
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

							   //addr.sin_addr.s_addr = inet_addr("127.0.0.1");//boardcast locally
	addr.sin_port = htons(PORT);//port
	addr.sin_family = AF_INET;//ipv4 socket

	if(BroadcastPublically)//if server is open pubically
		inet_pton(AF_INET,INADDR_ANY, &addr.sin_addr);
	else//if server for is only for one router
		inet_pton(AF_INET, "192.168.43.182", &addr.sin_addr);//boardcast locally

	sListen = socket(AF_INET, SOCK_STREAM, NULL);//create socket to listen for new connection
	
	if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {//bind the address to the socket
		std::string ErrorMsg = "Failed to bind the address to our listening socket.WinsockError" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) {//place the socket in the state of listening the incommenig connection
		std::string ErrorMsg = "Failed to listen on listening socket.Winsock Error" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	serverptr = this;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PacketSenderThread,NULL, NULL, NULL);//create a thread to handle the outgoing packets

	
}

bool Server::ListenForNewConnection() {

	SOCKET newConnection;//socket to hold client connetion

		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);//accept a new connection
		if (newConnection == 0)//if accepting client connection failed
		{
			std::cout << "failed to accept the client connection" << std::endl;
			return false;
		}
		else//client connection properly accepted
		{
			std::cout << "Client connected" << std::endl;
			connections[TotalConnections].socket = newConnection;
			
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(TotalConnections), NULL, NULL);//create a thread to handle the client.the index in  the socket array for this thred is value (i)
			//std::string MOTD = "this is the message of the day";//create buffer with message of the day
			//SendString(TotalConnections, MOTD);
			TotalConnections += 1;
			return true;
		}
}

bool Server::ProcessPacket(int ID, PacketType _packettype) {

	switch (_packettype)
	{
	case PacketType::ChatMessage:
	{
		std::string message;//string to store message we received
		if (!GetString(ID, message))//get chat message and store string in the variable
			return false;// if we dont get the message properly

		
		

		for (int i = 0; i < TotalConnections; i++)
		{
			if (i == ID)
				continue;
			else {
				SendString(i, message);//send message to connection i
			}
			
		}

	
		std::cout << "Processed chat message packet from user ID" << ID << std::endl;
		break;

	}

	case PacketType::FileTransferRequestFile://when client request file from the server
	{
		std::string FileName;
		if (!GetString(ID, FileName))//gettting file name
			return false;

		connections[ID].file.infileStream.open(FileName, std::ios::binary | std::ios::ate);//open file to read and send
		if (!connections[ID].file.infileStream.is_open())
		{
			std::cout << "Client" << ID << "requested file :" << FileName << "but that file doenont open" << std::endl;
			std::string Errmsg = "Requested file doesnot exist";
			SendString(ID, Errmsg); 
			return true;
		}
		//set file struct value
		connections[ID].file.fileName = FileName;
		connections[ID].file.filesize = connections[ID].file.infileStream.tellg();//getfile size
		connections[ID].file.infileStream.seekg(0);//set cursor in file back to offset 0
		connections[ID].file.fileOffset = 0;
		
		if (!HandleSendFile(ID)){//attempt send byte buffer from file if fails
			return false;
		}
		break;
	}
	case PacketType::FileTransferRequestNextBuffer://when client request next buffer for current file
	{
		if (!HandleSendFile(ID)) {//attempt send byte buffer from file if fails
			return false;
		}
		break;
	}

	default:
		std::cout << "unrecognised packets" << (int32_t)_packettype << std::endl;
		break;
	}
	return true;
}

bool Server::HandleSendFile(int ID) {
	if (connections[ID].file.fileOffset >= connections[ID].file.filesize) //if end of file reached
		return true;
	
	if (!SendPacketType(ID, PacketType::FileTransferByteBuffer))//send packet type for file transfer byte buffer
		return false;

	connections[ID].file.remainingBytes = connections[ID].file.filesize - connections[ID].file.fileOffset;//calculate remaining bytes
	if (connections[ID].file.remainingBytes > connections[ID].file.buffersize) {//if remaining bytes >buffer size
		connections[ID].file.infileStream.read(connections[ID].file.buffer, connections[ID].file.buffersize);
		if (!Sendint32_t(ID, connections[ID].file.buffersize))//send int of buffer size
			return false;
		if (!sendall(ID, connections[ID].file.buffer, connections[ID].file.buffersize))//send bytes
			return false;
		connections[ID].file.fileOffset += connections[ID].file.buffersize;//increament in fileoffset by buffer size

	}
	else {
		connections[ID].file.infileStream.read(connections[ID].file.buffer, connections[ID].file.remainingBytes);//read remaining bytes
		if (!Sendint32_t(ID, connections[ID].file.remainingBytes))//send int for remaining bytes
			return false;
		if (!sendall(ID, connections[ID].file.buffer, connections[ID].file.remainingBytes))//send bytes
			return false;
		connections[ID].file.fileOffset += connections[ID].file.remainingBytes;//increament in fileoffset by buffer size

	}

	if (connections[ID].file.fileOffset == connections[ID].file.filesize) {//if we are at end of the file
		if (!SendPacketType(ID, PacketType::FileTransferEndofFile))//send end of the file packet
			return false;
		std::cout << "file sent successfully" << std::endl;
	}
	return true;
}

void Server::ClientHandlerThread(int ID) {

	PacketType packettype;
	while (true)
	{
		//first get packet type
		if (!serverptr->GetPacketType(ID, packettype))//get packet type
			break;//if there an issue in getting packet
		if (!serverptr->ProcessPacket(ID, packettype))
		{
			break;
		}
	}

	std::cout << "lost connection to the client Id:" << ID << std::endl;
	closesocket(serverptr->connections[ID].socket);
}

void Server::PacketSenderThread() {//thread for all outgoing packets
	while (true){
		for (int i = 0; i < serverptr->TotalConnections; i++) {//for  each client
			if (serverptr->connections[i].pm.HasPendingPackets())
			{
				Packet p = serverptr->connections[i].pm.Retreive();//retreive packet
				if (!serverptr->sendall(i, p.buffer, p.size)) {//send packet
					std::cout << "failed to send packet to id:" << i << std::endl;

				}
				delete p.buffer;//clean up buffer from packet p
			}

		}
		Sleep(5);
	}
}
