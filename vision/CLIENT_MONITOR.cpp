#define _WIN32_WINNT 0x0600
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <thread>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <map>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

map<int,string> ErrorMsg;

// ✅ 接收服务器消息（唯一作用：显示）
void receive_messages(SOCKET sock){
    char buffer[1024];
    while (true) {
        int DataSize = recv(sock,buffer,sizeof(buffer),0);
        if (DataSize > 0){
            cout << "\n[MONITOR RECEIVED] "
                 << string(buffer,DataSize) << endl;
        }
    }
}

void DefiningErrorMsg(){
    ErrorMsg[10004] = "阻塞操作被中断\n";
    ErrorMsg[10009] = "无效的文件描述符\n";
    ErrorMsg[10013] = "权限被拒绝\n";
    ErrorMsg[10014] = "错误的地址\n";
    ErrorMsg[10022] = "参数非法\n";
    ErrorMsg[10024] = "打开的套接字过多\n";
    ErrorMsg[10035] = "操作会阻塞\n";
    ErrorMsg[10036] = "操作正在进行中\n";
    ErrorMsg[10037] = "操作已在进行中\n";
    ErrorMsg[10038] = "不是一个套接字\n";
    ErrorMsg[10048] = "端口被占用\n";
    ErrorMsg[10049] = "地址不可用\n";
    ErrorMsg[10050] = "网络不可用\n";
    ErrorMsg[10051] = "网络不可达\n";
    ErrorMsg[10052] = "网络连接被重置\n";
    ErrorMsg[10053] = "连接被中止\n";
    ErrorMsg[10054] = "连接被对方强制关闭\n";
    ErrorMsg[10056] = "套接字已连接\n";
    ErrorMsg[10057] = "套接字未连接\n";
    ErrorMsg[10058] = "套接字已关闭\n";
    ErrorMsg[10060] = "连接超时\n";
    ErrorMsg[10061] = "连接被拒绝\n";
    ErrorMsg[10065] = "主机不可达\n";
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    DefiningErrorMsg();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
        cerr << "WSAStartup 初始化失败！" << endl;
        return 1;
    }

    SOCKET sock = socket(AF_INET,SOCK_STREAM,0);
    if (sock == INVALID_SOCKET) {
        cerr << "创建 socket 失败，错误码: "
             << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    string ip;
    int port;

    cout << "请输入服务器 IP: ";
    cin >> ip;
    cout << "请输入端口: ";
    cin >> port;

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET,ip.c_str(),
                  &server_addr.sin_addr) <= 0) {
        cerr << "IP 地址格式错误！" << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (connect(sock,(sockaddr*)&server_addr,
                sizeof(server_addr)) == SOCKET_ERROR){

        int ErrorCode = WSAGetLastError();
        cerr << "\n连接服务器失败，错误码: "
             << ErrorCode << endl;
        cerr << ErrorMsg[ErrorCode] << endl;

        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "✅ 已成功连接到服务器 "
         << ip << ":" << port << endl;

    // ✅【关键新增 1】告诉 server：我是一个 monitor
    send(sock, "///Monitor", 10, 0);
    Sleep(100);

    // ✅【关键新增 2】绑定指定 client
    string bindname;
    cout << "请输入你要绑定的 client 用户名（如 张三#1234）：";
    cin >> bindname;

    string bindcmd = "///Bind:" + bindname;
    send(sock, bindcmd.c_str(), bindcmd.size(), 0);

    cout << "✅ 已绑定到 client："
         << bindname << endl;

    // ✅ 启动接收线程
    thread(receive_messages,sock).detach();

    // ✅ monitor 不发消息，只保持进程存活
    string msg;
    cin.ignore();
    while (true){
        msg = msg;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
