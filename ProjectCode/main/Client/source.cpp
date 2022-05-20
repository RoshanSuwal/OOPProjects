// Client.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Client.h"




int main()
{
	Client myClient("192.168.43.182", 9998);

	if (!myClient.Connect()) { //if fails to connect
		std::cout << "Failed to connect to the server..." << std::endl;
		system("pause"); 
		return 1;
	}
	//myClient.RequestFile("roshan.jpg");


	std::string userinput;//holds user chat message

	while (true)
	{
		std::getline(std::cin,userinput);//get line input from user in string
		if (!myClient.SendString(userinput))//send string user input
			break;//if send string failed



		Sleep(10);
	}

	system("pause");
    return 0;
}