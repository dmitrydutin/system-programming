#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
using namespace std;

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)

bool startsWith(const string& str, const string& prefix);
string getConsoleParam(int argc, char* argv[], string startMarker);
SOCKET createConnection();
void ClientHandler(SOCKET connection);

int main(int argc, char* argv[]) {
    const int pointsCount = stoi(getConsoleParam(argc, argv, "--points-count="));
    const int startPoint = stoi(getConsoleParam(argc, argv, "--start-point="));
    const int endPoint = stoi(getConsoleParam(argc, argv, "--end-point="));
    const string array = getConsoleParam(argc, argv, "--array=");

    SOCKET connection = createConnection();
    cout << "Connection created" << endl;
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(connection), NULL, NULL);

    send(connection, (char*)&pointsCount, sizeof(int), NULL);
    send(connection, (char*)&startPoint, sizeof(int), NULL);
    send(connection, (char*)&endPoint, sizeof(int), NULL);

    int arraySize = array.size();
    send(connection, (char*)&arraySize, sizeof(int), NULL);
    send(connection, array.c_str(), arraySize, NULL);

    system("pause");
    return 0;
}

bool startsWith(const string& str, const string& prefix) {
    return str.rfind(prefix, 0) == 0;
}

string getConsoleParam(int argc, char* argv[], string startMarker) {
    for (int i = 0; i < argc; i++) {
        string arg = string(argv[i]);

        if (startsWith(arg, startMarker)) {
            return arg.substr(startMarker.length());
        }
    }

    cout << "ERROR: Param " << startMarker << " not found" << endl;
    exit(1);
}

SOCKET createConnection() {
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);

    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        cout << "ERROR: Unable to start WSA" << endl;
        exit(1);
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET connection = socket(AF_INET, SOCK_STREAM, NULL);

    if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
        cout << "ERROR: Failed to connect to server" << endl;
        exit(1);
    }

    return connection;
}

void ClientHandler(SOCKET connection) {
    while (true) {
        int result;
        int checkSocket = recv(connection, (char*)&result, sizeof(int), NULL);

        if (checkSocket == -1) {
            break;
        }

        cout << "Result: " << result << endl;
    }

    closesocket(connection);
}
