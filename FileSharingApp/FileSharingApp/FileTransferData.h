#pragma once
#include<fstream>

struct FileTransferData
{
	static const int buffersize = 8912;
	std::string fileName;
	int bytesWritten;//for when receiving file
	int fileOffset;//reading file
	int filesize;//for when reading a file
	int remainingBytes;//for when reading a file
	std::ifstream infileStream;//for reading a file that has been sent
	std::ofstream outfileStream;//for writing a file that has been received
	char buffer[buffersize];//buffer used for sending or receiving to optimise reallocable

};