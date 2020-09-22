/*******************************************************
Socket��װ��

Created by kyn0v.
2020.08.15
********************************************************/

#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>  
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

enum SOCKETDATATYPE
{
	IMAGE_DATA = 1,	// ͼ��֡��Ϣ��־λ#
	PTZINFO_DATA = 2, // ������̨�źű�־λ*����ָ���Ƕ��ƶ���
	CTRL_DATA = 3 // ������̨�źű�־λ%���������ƶ���
};

typedef struct {
	float horz = 0.1;
	float vert = 0.2;
	int zoom = 3;
	int focus = 4;
} PTZInfo;

typedef struct {
	int cmd;
	int stop;
	int speed;
} PTZCtrl;

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
	sockaddr_in clientAddr;
};

class SocketServerTCP
{
public:
	bool init(int port);
	bool connect();
	void disconnect();
	bool transmit(char *data, int size);
	bool receive(char *buffer, int buffer_size, SOCKETDATATYPE &type, int &size, bool is_raw);
	void cleanup();
private:
	SOCKET serverSocket;
	SOCKET connectionSocket;
	sockaddr_in clientAddr;
};

//------------------------- ע��TCP�������ӣ���UDP�������ӵ�-------------------
class SocketClientUDP
{
public:
	bool init(string ip, int port);
	bool transmit(char *data, int size);
	bool receive(char *buffer, int buffer_size, int &recv_size);
	void cleanup();
private:
	SOCKET clientSocket;
	sockaddr_in clientAddr;
};

class SocketServerUDP
{
public:
	bool init(int port);
	bool transmit(char *data, int size);
	bool receive(char *buffer, int buffer_size, int &ret_size);
	void cleanup();
private:
	SOCKET serverSocket;
	sockaddr_in serverAddr;
	sockaddr_in clientAddr;
};

