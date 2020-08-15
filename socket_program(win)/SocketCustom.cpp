#include "SocketCustom.h"

using namespace std;
using namespace cv;

//--------------------Client请求(TCP)---------------------
bool SocketClientTCP::init(string ip, int port)
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		printf("WSAStartup error!\n");
		return false;
	}
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(port);
	clientAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	return true;
}

bool SocketClientTCP::connnect()
{
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		printf("create client error!\n");
		return false;
	}
	if (connect(clientSocket, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
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
		char flag[10];	// 标志位'#'1位 + 数据长度9位
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
	else if (type == PTZINFO_DATA) {	// 发送云台信息
		char control_buffer[50];
		char flag[10];
		sprintf(flag, "*%09d", size); 	// 标志位'*'1位 + 数据长度9位 
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
	else if (type == CTRL_DATA) {	// 发送控制信号
		char control_buffer[50];
		char flag[10];
		sprintf(flag, "@%09d", size);	// 标志位'@'1位 + 数据长度9位
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
	int ret = recv(clientSocket, buffer, buffer_size, 0);
	if (ret > 0) {
		// You can test the received data here.
		return true;
	}
	return false;
}

void SocketClientTCP::cleanup()
{
	WSACleanup();
}

//--------------------Server监听(TCP)---------------------
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
	int len = sizeof(clientAddr);
	connectionSocket = accept(serverSocket, (sockaddr*)&clientAddr, &len);
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
	int ret = send(connectionSocket, data, size, 0);
	if (ret < 0) {
		printf("send control data error!\n");
		return false;
	}
	return true;
}

bool SocketServerTCP::receive(char *buffer, int buffer_size, SOCKETDATATYPE &type, int &size)
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
	else if (buffer[0] == '@') {	//接收到控制信号
		type = CTRL_DATA;
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

//--------------------Client请求(UDP)---------------------
bool SocketClientUDP::init(string ip, int port)
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		printf("WSAStartup error!\n");
		return false;
	}
	clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (clientSocket == INVALID_SOCKET) {
		printf("create client error!\n");
		return false;
	}
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(port);
	clientAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	return true;
}

bool SocketClientUDP::transmit(char * data, int size)
{
	int ret = sendto(clientSocket, data, size, 0, (sockaddr *)&clientAddr, sizeof(clientAddr));
	if (ret < 0) {
		printf("send data error!\n");
		return false;
	}
	return true;
}

bool SocketClientUDP::receive(char *buffer, int buffer_size, int &recv_size)
{
	int addrLen = sizeof(clientAddr);
	recv_size = recvfrom(clientSocket, buffer, 255, 0, (sockaddr *)&clientAddr, &addrLen);
	if (recv_size<0) {
		return false;
	}
	return true;
}

void SocketClientUDP::cleanup()
{
	closesocket(clientSocket);
	WSACleanup();
}

//--------------------Server监听(UDP)---------------------
bool SocketServerUDP::init(int port)
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsdata;
	if (WSAStartup(sockVersion, &wsdata) != 0)
	{
		printf("WSAStartup error!\n");
		return false;
	}
	serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serverSocket == INVALID_SOCKET)
	{
		printf("create server error!\n");
		return false;
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		printf("bind server error!\n");
		closesocket(serverSocket);
		return false;
	}
	return true;
}

bool SocketServerUDP::transmit(char *data, int size)
{
	int addrLen = sizeof(clientAddr);
	int ret = sendto(serverSocket, data, size, 0, (sockaddr *)&clientAddr, sizeof(clientAddr));
	if (ret < 0) {
		printf("send data error!\n");
		return false;
	}
	return true;
}

bool SocketServerUDP::receive(char *buffer, int buffer_size, int &recv_size)
{
	int addrLen = sizeof(clientAddr);
	recv_size = recvfrom(serverSocket, buffer, buffer_size, 0, (sockaddr *)&clientAddr, &addrLen);
	if (recv_size > 0) {
		// You can test the received data here.
		return true;
	}
	return false;
}

void SocketServerUDP::cleanup()
{
	closesocket(serverSocket);
	WSACleanup();
}
