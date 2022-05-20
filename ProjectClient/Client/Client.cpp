#include<iostream>
#include<WS2tcpip.h>
#include<string>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

void main()
{
	//initiaise winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0)
	{
		cerr << "connot initialize winsock ! quiting" << endl;
		//return;
	}

	//create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "cannot create a socket" << endl;
		//return;
	}
	//bind the  an ip address and port to socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;  //could also use inet_pton....

	bind(listening,(sockaddr*) &hint, sizeof(hint));

	//tell winsock the socket is for listening 
	listen(listening, SOMAXCONN);
	//wait for connection
	sockaddr_in client;
	int clientsize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsize);
	
	char host[NI_MAXHOST];
	char service[NI_MAXHOST];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << "connected on port" << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port" << ntohs(client.sin_port) << endl;
	}

	//close listening socket
	closesocket(listening);
	//while loop:accept and echo messsage back to client
	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);
		//wait for client to send data
		int byteReceived = recv(clientSocket, buf, 4096, 0);
		if (byteReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv() . quiting" << endl;
			break;
		}

		if (byteReceived == 0)
		{
			cout << "client disconnected" << endl;
			break;
		}

		cout<< "client : "<< string(buf, 0, byteReceived) << endl;
		//echo message back to client
		send(clientSocket, buf, byteReceived + 1, 0);

	}
	//close the socket
	closesocket(clientSocket);
	//cleanup winsock
	WSACleanup();

}