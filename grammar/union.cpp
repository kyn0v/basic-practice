// union 用于测试CPU是大端模式/小端模式
#include <iostream>
using namespace std;

void checkCPU() {
    union MyUnion {
        int a;
        char c;
    } test;
    test.a = 1;
    if (test.c == 1)
        cout << "little endian" << endl;
    else
        cout << "big endian" << endl;
}

int main() {
    checkCPU();
    system("pause");
    return 0;
}