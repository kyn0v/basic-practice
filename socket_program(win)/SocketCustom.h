#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>  
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

enum SOCKETDATATYPE
{
	IMAGE_DATA = 1,	// 图像帧标志位#
	PTZINFO_DATA = 2 // 云台信息标志位*
};

typedef struct {
	float horz = 0.1;
	float vert = 0.2;
	int zoom = 3;
	int focus = 4;
} PTZInfo;

class SocketClientTCP
{
public:
	bool init(string ip, int port);
	bool connnect();
	void disconnect();
	bool transmit(char *data, int size, SOCKETDATATYPE type);
	bool receive(char *buffer, int buffer_size);
	void cleanup();
private:
	SOCKET clientSocket;
	sockaddr_in sock_in;
};

class SocketServerTCP
{
public:
	bool init(int port);
	bool connect();
	void disconnect();
	bool transmit(char *data, int size);
	bool receive(char buffer[], int buffer_size, SOCKETDATATYPE &type, int &size);
	void cleanup();
private:
	SOCKET serverSocket;
	SOCKET connectionSocket;
};

