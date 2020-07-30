#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
using namespace cv;

enum SOCKETDATATYPE {
    IMAGE_DATA = 1,   // 图像数据
    CONTROL_DATA = 2  // 控制数据
};

class SocketServerTCP {
   public:
    bool init(int port);
    bool connect();
    bool receive(char *buffer, int buffer_size, SOCKETDATATYPE &type, int &size);
    void disconnect();
    void cleanup();

   private:
    SOCKET serverSocket;
    SOCKET connectionSocket;
};

bool SocketServerTCP::init(int port) {
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsdata;
    if (WSAStartup(sockVersion, &wsdata) != 0) {
        printf("WSAStartup error!\n");
        return false;
    }
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        printf("create server error!\n");
        return false;
    }
    sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(serverSocket, (sockaddr *)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
        printf("bind server error!\n");
        return false;
    }
    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        printf("create server listenner error!\n");
        return false;
    }
    return true;
}

bool SocketServerTCP::connect() {
    sockaddr_in client_sin;
    int len = sizeof(client_sin);
    connectionSocket = accept(serverSocket, (sockaddr *)&client_sin, &len);
    if (connectionSocket == INVALID_SOCKET) {
        printf("create connection error!\n");
        return false;
    }
    return true;
}

void SocketServerTCP::disconnect() {
    closesocket(connectionSocket);
}

bool SocketServerTCP::receive(char *buffer, int buffer_size, SOCKETDATATYPE &type, int &size) {
    size = recv(connectionSocket, buffer, buffer_size, 0);
    if (size < 0) {
        printf("server receive data failed!\n");
        return false;
    }
    if (buffer[0] == '#') {
        type = IMAGE_DATA;
        size--;
        for (int i = 0; i < size; i++) {
            buffer[i] = buffer[i + 1];
        }
    } else if (buffer[0] == '*') {
        type = CONTROL_DATA;
        size--;
        for (int i = 0; i < size; i++) {
            buffer[i] = buffer[i + 1];
        }
    } else {
        printf("unknown SOCKETDATATYPE!\n");
        return false;
    }
    return true;
}

void SocketServerTCP::cleanup() {
    closesocket(serverSocket);
    WSACleanup();
}

int main() {
    SocketServerTCP ss;
    ss.init(8086);
    Mat image;
    while (true) {
        ss.connect();
		// 数据接收缓冲区
        char buffer[100000];
        SOCKETDATATYPE type;
        int size;
        ss.receive(buffer, sizeof(buffer), type, size);
        if (type == IMAGE_DATA) {
            std::vector<uchar> image_decode;
            int idx = 0;
            while (idx < size) {
                image_decode.push_back(uchar(buffer[idx++]));
            }
            Mat image = imdecode(image_decode, CV_LOAD_IMAGE_COLOR);
            imshow("image", image);
            waitKey(30);
        } else if(type == CONTROL_DATA){
			for(int i=0; i< size; i++){
				printf("%s", buffer[i]);
			}
		} else {
            printf("unknown SOCKETDATATYPE!");
        }
        ss.disconnect();
    }
    ss.cleanup();
    return 0;
}