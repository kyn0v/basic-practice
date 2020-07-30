# 注：
* `server_socket.cpp` :serverSocket在服务结束时才可关闭，connectionSocket在每次数据传输的过程中，经历建立和销毁。

* `client_socket.cpp` :clientSocket在每次传输过程中，经历建立和销毁。