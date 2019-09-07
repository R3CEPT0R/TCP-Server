//Example TCP SERVER

#include <iostream>
#include <WS2tcpip.h>   //for necessary functions
#pragma comment (lib, "ws2_32.lib")


using namespace std;
void main()
{
	system("color A");
	cout << "-------------------------" << endl;
	cout << "        WELCOME          " << endl;
	cout << "-------------------------" << endl << endl;
	cout << "To start, open up PuTTY, enter 127.0.0.1 as IP address" << endl 
		<< "use port 54000 and choose a \"raw\" connection type" << endl << endl;

	//init winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Cannot initialize winsock! Quitting" << endl;
		return;
	}

	//create the socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Cannot create socket. Quitting" << endl;
		return;
	}

	//bind socket to ip address and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);   //host to network short (for big endian)
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//tell winsock the socket is for listening 
	listen(listening, SOMAXCONN);


	//wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	char host[NI_MAXHOST]; //CLIENT REMOTE NAME
	char service[NI_MAXHOST]; //SERVICE CLIENT IS CONNECTED TO (I.E. PORT)
	 
	ZeroMemory(host, NI_MAXHOST); //SAME AS MEMSET
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}
	cout << "SUCCESS!" << endl << "This is a simple TCP Server adapted from Sloan Kelly's YT video" << endl;
	cout << "Whatever you type will be echoed back to you" << endl;

	//close listening socket
	closesocket(listening);

	//while loop: to echo messages back
	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);

		//wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}
		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			break;
		}
		send(clientSocket, buf, bytesReceived + 1, 0);  //the +1 to send the zero buffer out
		//echo message back to client
	}
	
	//close socket
	closesocket(clientSocket);

	//clean up the winsock
	WSACleanup();
}