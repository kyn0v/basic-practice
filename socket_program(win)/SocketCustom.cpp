#include "SocketCustom.h"

using namespace std;
using namespace cv;

//--------------------Client请求---------------------
bool SocketClientTCP::init(string ip, int port)
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		printf("WSAStartup error!\n");
		return false;
	}

	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(port);
	sock_in.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	return true;
}

bool SocketClientTCP::connnect()
{
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		printf("create client error!\n");
		return false;
	}
	if (connect(clientSocket, (sockaddr*)&sock_in, sizeof(sock_in)) == SOCKET_ERROR) {
		printf("client connect error!\n");
		return false;
	}
	return true;
}

void SocketClientTCP::disconnect()
{
	closesocket(clientSocket);
}

bool SocketClientTCP::transmit(char * data, int size, SOCKETDATATYPE type)
{
	if (type == IMAGE_DATA) {	// 发送图像数据
		char image_buffer[100000];
		char flag[10];	// 标志位：# + 数据长度
		sprintf(flag, "#%09d", size);
		for (int i = 0; i < 10; ++i) {
			image_buffer[i] = flag[i];
		}
		for (int i = 0; i < size; i++)
		{
			image_buffer[i + 10] = data[i];
		}
		if (send(clientSocket, image_buffer, size + 10, 0) < 0) {
			printf("send image data error!\n");
			return false;
		}
	}
	else if (type == PTZINFO_DATA) {	// 发送控制信号数据
		char control_buffer[1000];
		char flag[10];	// 标志位：* + 数据长度
		sprintf(flag, "*%09d", size);
		for (int i = 0; i < 10; ++i) {
			control_buffer[i] = flag[i];
		}
		for (int i = 0; i < size; i++)
		{
			control_buffer[i + 10] = data[i];
		}
		if (send(clientSocket, control_buffer, size + 10, 0) < 0) {
			printf("send control data error!\n");
			return false;
		}
	}
	else {
		printf("unknown SOCKETDATATYPE!\n");
		return false;
	}
	return true;
}

bool SocketClientTCP::receive(char *buffer, int buffer_size)
{
	//int ret = recv(clientSocket, buffer, buffer_size, 0);
	//if (ret>0) {
	//	buffer[ret] = 0x00;
	//	printf(buffer);
	//	return true;
	//}
	//return false;
	return false;
}

void SocketClientTCP::cleanup()
{
	WSACleanup();
}

//--------------------Server监听---------------------
bool SocketServerTCP::init(int port)
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsdata;
	if (WSAStartup(sockVersion, &wsdata) != 0)
	{
		printf("WSAStartup error!\n");
		return false;
	}
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		printf("create server error!\n");
		return false;
	}
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(port);
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(serverSocket, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
		printf("bind server error!\n");
		return false;
	}
	if (listen(serverSocket, 10) == SOCKET_ERROR) {
		printf("create server listenner error!\n");
		return false;
	}
	return true;
}

bool SocketServerTCP::connect()
{
	sockaddr_in client_sin;
	int len = sizeof(client_sin);
	connectionSocket = accept(serverSocket, (sockaddr*)&client_sin, &len);
	if (connectionSocket == INVALID_SOCKET) {
		printf("create connection error!\n");
		return false;
	}
	return true;
}

void SocketServerTCP::disconnect()
{
	closesocket(connectionSocket);
}

bool SocketServerTCP::transmit(char * data, int size)
{
	if (send(connectionSocket, data, size, 0) < 0) {
		printf("send control data error!\n");
		return false;
	}
	return true;
}

bool SocketServerTCP::receive(char buffer[], int buffer_size, SOCKETDATATYPE &type, int &size)
{
	int ret = recv(connectionSocket, buffer, buffer_size, 0);
	char size_str[9];
	strncpy(size_str, buffer + 1, 9);
	size = atoi(size_str);
	int rest_size = (size + 10) - ret;
	while (rest_size>0) {
		ret = recv(connectionSocket, buffer + ret, buffer_size - ret, 0);
		rest_size -= ret;
	}
	if (size < 0) {
		printf("server receive data failed!\n");
		return false;
	}
	if (buffer[0] == '#') {	//接收到图片
		type = IMAGE_DATA;
		for (int i = 0; i < size; i++) {
			buffer[i] = buffer[i + 10];
		}
	}
	else if (buffer[0] == '*') {	//接收到控制信号
		type = PTZINFO_DATA;
		for (int i = 0; i < size; i++) {
			buffer[i] = buffer[i + 10];
		}
	}
	else {
		printf("unknown SOCKETDATATYPE!\n");
		return false;
	}
	return true;
}

void SocketServerTCP::cleanup()
{
	closesocket(serverSocket);
	WSACleanup();
}