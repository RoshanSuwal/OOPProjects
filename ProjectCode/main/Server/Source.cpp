// Server.cpp : Defines the entry point for the console application.
//
#include"stdafx.h"
#include"Server.h"

int main()
{
	Server myServer(9998);
	for (int i = 0; i < 100; i++) {//upto 100 times
		myServer.ListenForNewConnection();//Accept new connections
	}

	system("pause");
    return 0;
}
