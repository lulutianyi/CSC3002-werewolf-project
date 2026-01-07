#define _WIN32_WINNT 0x0600
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <thread>
#include <string>
#include <winsock2.h>
#include<Windows.h>
#include <ws2tcpip.h>
#include<map>
#include<random>
#include<ctime>

#pragma comment(lib, "ws2_32.lib")
using namespace std;
map<int,string>ErrorMsg;


void receive_messages(SOCKET sock){
    char buffer[1024];
    while (true){
        int DataSize = recv(sock,buffer,sizeof(buffer),0);
        if (DataSize > 0){
            string msg = string(buffer,DataSize);
            if(msg.substr(0,3) == "///")cout << msg <<"\n";
        }
    }
}
int CreateRandomId(int randint){
    return randint > 0 ? (randint % 9999) : ((0 - randint) % 9999);
}
void DefiningErrorMsg(){
    ErrorMsg[10004] = "阻塞操作被中断\n";
    ErrorMsg[10009] = "无效的文件描述符\n";
    ErrorMsg[10013] = "权限被拒绝\n";
    ErrorMsg[10014] = "错误的地址（非法指针或缓冲区不足）\n";
    ErrorMsg[10022] = "参数非法";
    ErrorMsg[10024] = "打开的套接字过多，超过系统限制\n";
    ErrorMsg[10035] = "操作会阻塞（非阻塞套接字无法立即完成）\n";
    ErrorMsg[10036] = "操作正在进行中\n";
    ErrorMsg[10037] = "操作已在进行中\n";
    ErrorMsg[10038] = "对象不是一个套接字\n";
    ErrorMsg[10048] = "地址已被使用（端口冲突）\n";
    ErrorMsg[10049] = "地址不可用（IP 地址错误）\n";
    ErrorMsg[10050] = "网络不可用\n";
    ErrorMsg[10051] = "网络不可达\n";
    ErrorMsg[10052] = "网络连接被重置\n";
    ErrorMsg[10053] = "连接被中止\n";
    ErrorMsg[10054] = "连接被对方重置\n";
    ErrorMsg[10056] = "套接字已连接\n";
    ErrorMsg[10057] = "套接字未连接\n";
    ErrorMsg[10058] = "套接字已关闭，无法发送\n";
    ErrorMsg[10060] = "连接超时（服务器未响应）\n";
    ErrorMsg[10061] = "连接被拒绝（服务器未启动或端口未监听）\n";
    ErrorMsg[10065] = "主机不可达\n";

}
int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    mt19937 mt(time(nullptr));
    DefiningErrorMsg();
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
        cerr << "WSAStartup 初始化失败！" << endl;
        return 1;
    }

    SOCKET sock = socket(AF_INET,SOCK_STREAM,0);
    if (sock == INVALID_SOCKET) {
        cerr << "创建 socket 失败，错误码: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }
    string ip;
    int port = 8080;
    string nickname;
    cout << "请输入昵称：";
    cin >> nickname;
    int RandomId = CreateRandomId(mt());
    cout << "已为您生成服务器用户名: " << nickname << "#" << RandomId << "\n\n";
    string identity = "///User_Create:Name = " + nickname + "#" + to_string(RandomId);
    cout << "请输入服务器IP地址: ";
    cin >> ip;
    cout << "请输入端口号: ";
    cin >> port;

    sockaddr_in ServerAddress{};
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(port);

    if (inet_pton(AF_INET,ip.c_str(),&ServerAddress.sin_addr) <= 0) {
        cerr << "IP 地址格式错误！" << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (connect(sock,(sockaddr*)&ServerAddress,sizeof(ServerAddress)) == SOCKET_ERROR){
        int ErrorCode = WSAGetLastError();
        cerr << "\n连接服务器失败，错误码: " << ErrorCode << endl;
        cerr << endl << ErrorMsg[ErrorCode] << endl;
        system("pause");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "已成功连接到服务器 " << ip << ":" << port << endl;

    thread(receive_messages,sock).detach();

    send(sock,identity.c_str(),identity.size(),0);
    Sleep(300);
    string msg;
    cin.ignore(); // 清理输入缓冲区

    //string msg0 = "[! 广播 !]：" + nickname + "加入了服务器";
    nickname = nickname + "：";
    //send(sock,msg0.c_str(),msg0.size(),0);
    while (true){
        getline(cin,msg);
        //msg = nickname + msg;
        if (!msg.empty()) {
            send(sock,msg.c_str(),msg.size(),0);
            Sleep(300);
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
