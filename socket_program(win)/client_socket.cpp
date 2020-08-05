#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
using namespace cv;

enum SOCKETDATATYPE {
    IMAGE_DATA = 1,
    CONTROL_DATA = 2
};

class SocketClientTCP {
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

bool SocketClientTCP::init(string ip, int port) {
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0) {
        printf("WSAStartup error!\n");
        return false;
    }

    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons(port);
    sock_in.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
    return true;
}

bool SocketClientTCP::connnect() {
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        printf("create client error!\n");
        return false;
    }
    if (connect(clientSocket, (sockaddr *)&sock_in, sizeof(sock_in)) == SOCKET_ERROR) {
        printf("client connect error!\n");
        return false;
    }
    return true;
}

void SocketClientTCP::disconnect() {
    closesocket(clientSocket);
}

bool SocketClientTCP::transmit(char *data, int size, SOCKETDATATYPE type) {
    if (type == IMAGE_DATA) {
        char image_buffer[100000];
        char flag[10];  // 数据流标志位：1位# + 9位数据长度
        sprintf(flag, "#%09d", size);
        for (int i = 0; i < 10; ++i) {
            image_buffer[i] = flag[i];
        }
        for (int i = 0; i < size; i++) {
            image_buffer[i + 10] = data[i];
        }
        if (send(clientSocket, image_buffer, size + 10, 0) < 0) {
            printf("send image data error!\n");
            return false;
        }
    } else if (type == CONTROL_DATA) {
        char control_buffer[1000];
        char flag[10];
        sprintf(flag, "*%09d", size);
        for (int i = 0; i < 10; ++i) {
            control_buffer[i] = flag[i];
        }
        for (int i = 0; i < size; i++) {
            control_buffer[i + 10] = data[i];
        }
        if (send(clientSocket, control_buffer, size + 10, 0) < 0) {
            printf("send control data error!\n");
            return false;
        }
    } else {
        printf("unknown SOCKETDATATYPE!\n");
        return false;
    }
    return true;
}

bool SocketClientTCP::receive(char *buffer, int buffer_size) {
    //int ret = recv(clientSocket, buffer, buffer_size, 0);
    //if (ret>0) {
    //	buffer[ret] = 0x00;
    //	printf(buffer);
    //	return true;
    //}
    //return false;
}

void SocketClientTCP::cleanup() {
    WSACleanup();
}

int main() {
    SocketClientTCP sc;
    sc.init("192.168.0.105", 8086);
    VideoCapture capture(0);
    Mat image;
    while (true) {
        sc.connnect();

        //char recData[255];
        //sc.receive(recData, sizeof(recData) / sizeof(char));

        capture >> image;
        cv::resize(image, image, Size(1280, 960));
        if (image.empty()) {
            printf("empty image!\n");
            return 0;
        }
        imshow("image", image);
        waitKey(30);
        std::vector<uchar> image_encode;
        std::vector<int> quality;
        quality.push_back(CV_IMWRITE_JPEG_QUALITY);
        quality.push_back(50);  // 压缩率50%
        imencode(".jpg", image, image_encode, quality);

        char *data = reinterpret_cast<char *>(&image_encode[0]);  // uchar转换为char
        int num = image_encode.size();
        sc.transmit(data, num, IMAGE_DATA);
        waitKey(70);
        sc.disconnect();
    }
    sc.cleanup();
    return 0;
}