#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <string>
#include <iostream>
using namespace std;

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996);

SOCKET Connection;

enum Packet {
    P_ChatMessage,
    P_Test,
};

bool createConnection();
void ClientHandler();
bool ProcessPacket(Packet packetType);

// Варианты 7 и 13
// ЗАПУСК КЛИЕНТА ЧЕРЕЗ КОМАНДНУЮ СТРОКУ с указанием имени файла с заданной конфигурацией сетевого графа. Конфигурация графа матричная
int main(int argc, char* argv[]) {
    //cout << "Have " << argc << " arguments:" << endl;

    //for (int i = 0; i < argc; ++i) {
    //    cout << argv[i] << endl;
    //}

    //return 0;

    // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

    //[
    //    [0, 0, 0, 0, 0, 0],
    //    [0, 0, 0, 0, 0, 0],
    //    [0, 0, 0, 0, 0, 0],
    //    [0, 0, 0, 0, 0, 0],
    //    [0, 0, 0, 0, 0, 0],
    //    [0, 0, 0, 0, 0, 12],
    //]

    if (!createConnection()) {
        exit(1);
    }

    cout << "Connection Created" << endl;

    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

    string msg1;
    while (true) {
        getline(cin, msg1);
        int msg_size = msg1.size();
        Packet packetType = P_ChatMessage;

        send(Connection, (char*)&packetType, sizeof(Packet), NULL);
        send(Connection, (char*)&msg_size, sizeof(int), NULL);
        send(Connection, msg1.c_str(), msg_size, NULL);
        Sleep(10);
    }

    system("pause");
    return 0;
}

bool createConnection() {
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);

    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        cout << "ERROR: Unable to start WSA" << endl;
        return false;
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
        cout << "ERROR: Failed to connect to server" << endl;
        return false;
    }

    return true;
}

void ClientHandler() {
    Packet packetType;

    while (true) {
        recv(Connection, (char*)&packetType, sizeof(Packet), NULL);

        if (!ProcessPacket(packetType)) {
            break;
        }
    }

    closesocket(Connection);
}

bool ProcessPacket(Packet packetType) {
    switch (packetType) {
        case P_ChatMessage: {
            int msg_size;
            recv(Connection, (char*)&msg_size, sizeof(int), NULL);

            char* msg = new char[msg_size + 1];
            msg[msg_size] = '\0';
            recv(Connection, msg, msg_size, NULL);

            cout << msg << endl;

            delete[] msg;
            break;
        }
        case P_Test: {
            cout << "TEST PACKET" << endl;
            break;
        }
        default: {
            cout << "Unrecognized packet: " << packetType << endl;
            return false;
        }
    }

    return true;
}
