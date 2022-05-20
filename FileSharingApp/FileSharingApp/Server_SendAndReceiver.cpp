#include"stdafx.h"
#include"Server.h"

bool Server::recvall(int ID, char* data, int totalbytes) {
	int bytesreceived = 0;//holds total bytes received
	while (bytesreceived < totalbytes) {//we still have more bytes to receive
		int RetnCheck = recv(connections[ID].socket, data + bytesreceived, totalbytes - bytesreceived, NULL);//try to receive remaining bytes
		if (RetnCheck == SOCKET_ERROR) { return false; }//failed to receive all bytes

		bytesreceived += RetnCheck;//Add to total bytes  received
	}
	return true;//success
}

bool Server::sendall(int ID, char * data, int totalbytes) {
	int bytesent = 0;//holds total bytes sent

	while (bytesent < totalbytes) {//still to send all data
		int RetnCheck = send(connections[ID].socket, data + bytesent, totalbytes - bytesent, NULL);//try to send remaining data
		if (RetnCheck == SOCKET_ERROR) { return false; }//failed to send all data
		bytesent += RetnCheck;//add to total data sent
	}
	return true;//success
}

bool Server::Sendint32_t(int ID, int32_t _int32_t) {
	_int32_t = htonl(_int32_t);//convert long from host byte order to network byte order
	if (!sendall(ID, (char*)&_int32_t, sizeof(int32_t)))//if int fails to send
		return false;
	return true;//return true if int successufully sent

}

bool Server::Getint32_t(int ID, int32_t & _int32_t) {

	if (!recvall(ID, (char*)&_int32_t, sizeof(int32_t)))
		return false;//fail to receive int
	_int32_t = ntohl(_int32_t);//convert long from network byte order to host byte order
	return true;//return true if int successufully received
}

bool Server::SendPacketType(int ID, PacketType _packettype)
{

	if (!Sendint32_t(ID, (int32_t)_packettype))//if int fails to send
		return false;
	return true;//return true if int successufully sent
}

bool Server::GetPacketType(int ID, PacketType & _packettype)
{
	int packettype;
	if (!Getint32_t(ID, packettype))
		return false;//fail to receive int
	_packettype = (PacketType)packettype;
	return true;//return true if int successufully received

}


bool Server::SendString(int ID, std::string & _string) {

	//PS::ChatMessage outgoingmessage(_string);//create chat message
	//connections[ID].pm.Append(outgoingmessage.toPacket());
	//append to packet manager


	if (!SendPacketType(ID, PacketType::ChatMessage))//send packetmessage"chat message
		return false;//return false :failed to send string

	int32_t bufferlength = _string.size();
	if (!Sendint32_t(ID, bufferlength))//send the length of the buffer
		return false;//failed to send the buffer

	if (!sendall(ID,(char*)_string.c_str(), bufferlength))
		return false;//return false if failed to send string
	return true;//return true if string successufully sent

}


bool Server::GetString(int ID, std::string & _string) {
	int32_t bufferlength;//holds length of the message
	if (!Getint32_t(ID, bufferlength))//getLength of the buffer and store it in variable
		return false;//if get int fails
					 //bufferlength = 20;
	char *buffer = new char[bufferlength + 1];//allocate buffer
	buffer[bufferlength] = '\0';//set last character of buffer to be null

	if (!recvall(ID, buffer, bufferlength))
	{
		delete[] buffer;//delete buffer to prevent memory leak
		return false;
	}

	_string = buffer;//set string to received buffer mwssage
	std::cout << _string << std::endl;
	delete[] buffer;//delocate buffer memory
	return true;//return true if int successufully received

}

bool Server::sendPhoto(int ID,std::string & _string) {

	if (!SendPacketType(ID, PacketType::FileTransferProposal))
		return false;

	int32_t bufferLength = _string.size();
	if (!Sendint32_t(ID, bufferLength))//send size of filename
		return false;//failed to send file size
	if (!sendall(ID, (char*)_string.c_str(), bufferLength))
		return false;
	return true;
}