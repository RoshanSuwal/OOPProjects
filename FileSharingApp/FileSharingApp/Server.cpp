#include"stdafx.h"
#include"Server.h"
#include<list>

Server::Server(int PORT, bool BroadcastPublically) {//port=port to boardcast on

													//winsock Startup
	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 2);
	if (WSAStartup(DllVersion, &wsaData) != 0)//if WSAStartup returns nothing other than 0 then an error has occured
	{
		//MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		//exit(0);
	}

	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");//boardcast locally
	addr.sin_port = htons(PORT);//port
	addr.sin_family = AF_INET;//ipv4 socket

	if (BroadcastPublically)//if server is open pubically
		inet_pton(AF_INET, INADDR_ANY, &addr.sin_addr);
	else//if server for is only for one router
		inet_pton(AF_INET, "192.168.43.182", &addr.sin_addr);//boardcast locally

	sListen = socket(AF_INET, SOCK_STREAM, NULL);//create socket to listen for new connection

	if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {//bind the address to the socket
		std::string ErrorMsg = "Failed to bind the address to our listening socket.WinsockError" + std::to_string(WSAGetLastError());
		//MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		//exit(1);
	}
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) {//place the socket in the state of listening the incommenig connection
		std::string ErrorMsg = "Failed to listen on listening socket.Winsock Error" + std::to_string(WSAGetLastError());
		//MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		//exit(1);
	}
	serverptr = this;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PacketSenderThread, NULL, NULL, NULL);//create a thread to handle the outgoing packets


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
		
		
	
		serverptr->p_filepath = serverptr->filePath.begin();

		
		connections[ID].file.infileStream.open(*(serverptr->p_filepath), std::ios::binary | std::ios::ate);//open file to read and send
		if (!connections[ID].file.infileStream.is_open())
		{
			std::cout << "Client" << ID << "requested file :" << *p_filepath << "but that file doenont open" << std::endl;
			std::string Errmsg = "Requested file doesnot exist";
			SendString(ID, Errmsg);
			return true;
		}
		//set file struct value
	
		std::cout << *(serverptr->p_filepath) << std::endl;

		connections[ID].file.fileName = *(serverptr->p_filepath);
		connections[ID].file.filesize = (int)(connections[ID].file.infileStream.tellg());//getfile size
		connections[ID].file.infileStream.seekg(0);//set cursor in file back to offset 0
		connections[ID].file.fileOffset = 0;

		//windows form output
		serverptr->s_totalsize = connections[ID].file.filesize;
		serverptr->s_sentfileSize = 0;
		serverptr->s_sendingFileName = connections[ID].file.fileName;
		serverptr->newFileRequest = true;
		serverptr->fileNotsentYet = true;
		std::cout << "Size=" << serverptr->s_totalsize << std::endl;

		if (!HandleSendFile(ID)) {//attempt send byte buffer from file if fails
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
		serverptr->s_sentfileSize += connections[ID].file.buffersize;
	}
	else {
		connections[ID].file.infileStream.read(connections[ID].file.buffer, connections[ID].file.remainingBytes);//read remaining bytes
		if (!Sendint32_t(ID, connections[ID].file.remainingBytes))//send int for remaining bytes
			return false;
		if (!sendall(ID, connections[ID].file.buffer, connections[ID].file.remainingBytes))//send bytes
			return false;
		connections[ID].file.fileOffset += connections[ID].file.remainingBytes;//increament in fileoffset by buffer size
		serverptr->s_sentfileSize = serverptr->s_totalsize;
	}

	if (connections[ID].file.fileOffset == connections[ID].file.filesize) {//if we are at end of the file
		if (!SendPacketType(ID, PacketType::FileTransferEndofFile))//send end of the file packet
			return false;

		connections[ID].file.fileName = "";
		connections[ID].file.filesize = 0;//getfile size
		connections[ID].file.bytesWritten = 0;
		connections[ID].file.infileStream.seekg(0);//set cursor in file back to offset 0
		connections[ID].file.fileOffset = 0;
		connections[ID].file.infileStream.clear();
		connections[ID].file.infileStream.close();

		serverptr->fileList.pop_front();
		serverptr->filePath.pop_front();

		//windows form
		serverptr->fileNotsentYet = false;
		serverptr->newFileRequest = false;
		//serverptr->s_totalsize = 0;
		//serverptr->s_sentfileSize = 0;
		serverptr->sendNext = true;

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

		//std::cout << "Processing Packets" << std::endl;
		if (!serverptr->ProcessPacket(ID, packettype))
		{
			break;
		}
	}

	std::cout << "lost connection to the client Id:" << ID << std::endl;
	closesocket(serverptr->connections[ID].socket);
}

void Server::PacketSenderThread() {//thread for all outgoing packets
	while (true) {
		for (int i=0; i < serverptr->TotalConnections; i++) {
			if (serverptr->fileList.size() > 0 && serverptr->sendNext) {
				std::list<std::string>::iterator p_fileName;
				p_fileName = serverptr->fileList.begin();
				std::string name = *p_fileName;
				std::cout <<"Sending :"<< name << std::endl;
				if (serverptr->sendPhoto(i,name)) {
					serverptr->sendNext = false;
				}
				else {
					std::cout << "failed to send file" << std::endl;
				}

			}
		}

		Sleep(5);
	}
}

void Server::addToSend(std::string &filename, std::string &filePath) {
	serverptr->fileList.push_back(filename);
	serverptr->filePath.push_back(filePath);
	std::cout << serverptr->fileList.size() << std::endl;
}


void Server::convertToCompatible(std::string &name) {

	std::cout<<name.find("\\")<<std::endl;
}