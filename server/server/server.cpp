#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include "FordFalkerson.h"
using namespace std;

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996);

void ClientHandler(SOCKET connection);
int** ParceArrayFromClient(int vertexes, char* data);

int main(int argc, char* argv[]) {
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
    bind(connection, (SOCKADDR*)&addr, sizeof(addr));
    listen(connection, SOMAXCONN);

    for (int i = 0; i < 100; i++) {
        SOCKET newConnection = accept(connection, (SOCKADDR*)&addr, &sizeofaddr);

        if (newConnection == 0) {
            cout << "ERROR: Failed to create connection" << endl;
            continue;
        }

        cout << "Client connected!" << endl;
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(newConnection), NULL, NULL);
    }

    system("pause");
    return 0;
}

void ClientHandler(SOCKET connection) {
    while (true) {
        int pointsCount;
        int startPoint;
        int endPoint;
        int arraySize;

        int checkSocket = recv(connection, (char*)&pointsCount, sizeof(int), NULL);
        recv(connection, (char*)&startPoint, sizeof(int), NULL);
        recv(connection, (char*)&endPoint, sizeof(int), NULL);
        recv(connection, (char*)&arraySize, sizeof(int), NULL);

        char* array = new char[arraySize + 1];
        array[arraySize] = '\0';
        recv(connection, array, arraySize, NULL);

        if (checkSocket == -1) {
            break;
        }

        int** matrix = ParceArrayFromClient(pointsCount, array);
        FordFalkerson ff = FordFalkerson();
        int maxFlowValue = ff.getMaxFlow(pointsCount, startPoint, endPoint, matrix);
        send(connection, (char*)&maxFlowValue, sizeof(int), NULL);

        delete[] array;
    }

    closesocket(connection);
}

int** ParceArrayFromClient(int vertexes, char* data) {

    int** result = new int* [40];
    for (int i = 0; i < vertexes; i++)
    {
        result[i] = new int[40];
    }

    string s = data;
    int arrayRow = 0;
    int arrayCol = 0;

    //remove first '[' and last ']'
    s.replace(s.find("["), 1, "");
    s.replace(s.find_last_of("]"), 1, "");

    //while we have braces
    while (s.find("[") != std::string::npos)
    {
        //take first pair of braces
        int firstOpenBrace = s.find("[");
        int firstCloseBrace = s.find("]");

        string dataInBraces =
            s.substr(firstOpenBrace + 1, (firstCloseBrace - firstOpenBrace) - 1);

        //add data from dataInBraces in array
        string delimiter = ",";

        size_t pos = 0;
        string token;
        while ((pos = dataInBraces.find(delimiter)) != std::string::npos)
        {
            token = dataInBraces.substr(0, pos);
            result[arrayRow][arrayCol] = stoi(token);
            dataInBraces.erase(0, pos + delimiter.length());
            arrayCol++;
        }

        result[arrayRow][arrayCol] = stoi(dataInBraces);

        //remove braces
        s.replace(firstOpenBrace, 1, "");
        s.replace(firstCloseBrace - 1, 1, "");

        //increase counters to set data to array
        arrayRow++;
        arrayCol = 0;
    }

    //code below can be used for debugging this method

    for (int i = 0; i < vertexes; i++)
    {

        for (int j = 0; j < vertexes; j++)
        {
            cout << "Arr: " << result[i][j] << endl;
        }
        cout << endl;
    }

    return result;
}
