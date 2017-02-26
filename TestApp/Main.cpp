#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef XCPLIB_EXPORTS
#define XCP_API    __declspec(dllexport)
#else
#define XCP_API    __declspec(dllimport)
#endif



#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <memory>

#include <vld.h>

#include <XCPMaster.h>
#include <ResponsePacket.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "5555"


using XCPMsgPtr = std::unique_ptr<IXCPMessage>;

int main()
{
	WSADATA wsa;
	SOCKET s;
	uint8_t message[] = { 0x02, 0x00, 0x00, 0x00,0xff, 0x00 };
	//char server_reply[2000];
	int recv_size;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(5555);

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");
	Sleep(500);
	XCPMaster master(TransportLayer::ETHERNET);
	XCPMsgPtr connect_message = master.CreateConnectMessage(ConnectMode::NORMAL);
	XCPMsgPtr disconnect_message = master.CreateDisconnectMessage();
	XCPMsgPtr GetStatus = master.CreateGetStatusMessage();
	XCPMsgPtr Synch = master.CreateSynchMessage();
	

	std::vector<uint8_t> bytes;
	connect_message->Serialize(bytes);
	send(s, (const char*)bytes.begin()._Ptr, bytes.size(), 0);
	bytes.clear();
	bytes.resize(2000);
	master.AddSentMessage(connect_message.get());

	recv_size = recv(s, (char*)&bytes[0], 2000, 0);
	for (int i = 0; i < recv_size; i++)
	{
		std::cout << std::hex << (int)(bytes[i] & 0xff) << " ";
	}
	std::cout << "\n";
	XCPMsgPtr asd = master.DeserializeMessage(bytes);
	bytes.clear();

	GetStatus->Serialize(bytes);
	send(s, (const char*)bytes.begin()._Ptr, bytes.size(), 0);
	bytes.clear();
	bytes.resize(2000);
	master.AddSentMessage(GetStatus.get());

	recv_size = recv(s, (char*)&bytes[0], 2000, 0);
	for (int i = 0; i < recv_size; i++)
	{
		std::cout << std::hex << (int)(bytes[i] & 0xff) << " ";
	}
	std::cout << "\n";
	asd = master.DeserializeMessage(bytes);
	bytes.clear();


	Synch->Serialize(bytes);
	send(s, (const char*)bytes.begin()._Ptr, bytes.size(), 0);
	bytes.clear();
	bytes.resize(2000);
	master.AddSentMessage(Synch.get());

	recv_size = recv(s, (char*)&bytes[0], 2000, 0);
	for (int i = 0; i < recv_size; i++)
	{
		std::cout << std::hex << (int)(bytes[i] & 0xff) << " ";
	}
	std::cout << "\n";
	asd = master.DeserializeMessage(bytes);
	bytes.clear();
	

	
	disconnect_message->Serialize(bytes);
	send(s, (const char*)bytes.begin()._Ptr, bytes.size(), 0);
	bytes.clear();
	master.AddSentMessage(disconnect_message.get());
	bytes.resize(2000);
	recv(s, (char*)&bytes[0], 2000, 0);
	master.DeserializeMessage(bytes);
	bytes.clear();

	//delete connect_message;
	//delete disconnect_message;

	system("pause");
	closesocket(s);
	WSACleanup();


	system("pause");
	return 0;
}