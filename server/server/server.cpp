#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include "FordFalkerson.h"
#include <string>
using namespace std;

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996);

SOCKET Connections[100];
int Counter = 0;

enum Packet {
    P_GraphMaxFlowRequest,
    P_ChatMessage,
    P_Test,
};

bool ProcessPacket(int index, Packet packetType) {
    switch (packetType) {
        case P_ChatMessage: {
            int msg_size;
            recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
            char* msg = new char[msg_size + 1];
            msg[msg_size] = '\0';

            recv(Connections[index], msg, msg_size, NULL);

            for (int i = 0; i < Counter; i++) {
                if (i == index) {
                    continue;
                }

                //data for demo how algorithm works
                int** matrix = new int*[40];
                int mtrx[][6] {
                {0, 16, 0, 0, 12, 0},
                {0, 0, 12, 0, 6, 0},
                {0, 0, 0, 0, 9, 20},
                {0, 0, 7, 0, 0, 4},
                {0, 0, 0, 14, 0, 0},
                {0, 0, 0, 0, 0, 0} 
                };

                //init int**
                for (int i = 0; i < 6; i++)
                { 
                    matrix[i] = new int[40];
                    for (int j = 0; j < 6; j++)
                    {
                        matrix[i][j] = mtrx[i][j];
                    }
                }

                FordFalkerson ff = FordFalkerson();
                cout << "Flow: " << ff.getMaxFlow(6, 0, 5, matrix) << endl;

                Packet msgtype = P_ChatMessage;
                send(Connections[i], (char*)&msgtype, sizeof(Packet), NULL);
                send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
                send(Connections[i], msg, msg_size, NULL);
            }

            delete[] msg;
            break;
        }
        case P_GraphMaxFlowRequest:{
            int pnt_cn;
            int start_pnt;
            int end_pnt;
            int array_to_parce_size;

            //read ints from client
            recv(Connections[index], (char*)&pnt_cn, sizeof(int), NULL);
            recv(Connections[index], (char*)&start_pnt, sizeof(int), NULL);
            recv(Connections[index], (char*)&end_pnt, sizeof(int), NULL);
            recv(Connections[index], (char*)&array_to_parce_size, sizeof(int), NULL);

            //read string array from client

            char* arr = new char[array_to_parce_size + 1];
            arr[array_to_parce_size] = '\0';

            recv(Connections[index], arr, array_to_parce_size, NULL);
            

            // return result to client
            for (int i = 0; i < Counter; i++) {
                if (i == index) {
                    int** matrix = ParceArrayFromClient(pnt_cn, arr);

                    FordFalkerson ff = FordFalkerson();
                    int maxFlowValue = ff.getMaxFlow(6, 0, 5, matrix);
                    cout << "Calculated from server: " << maxFlowValue << endl;

                    //TODO: send data back
                }
            }

            delete[] arr;
            break;
        }
        default: {
            cout << "Unrecognized packet: " << packetType << endl;
            return false;
        }
    }

    return true;
}

void ClientHandler(int index) {
    Packet packetType;

    while (true) {
        recv(Connections[index], (char*)&packetType, sizeof(Packet), NULL);

        if (!ProcessPacket(index, packetType)) {
            cout << "BREAK" << endl;
            break;
        }
    }

    closesocket(Connections[index]);
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

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    SOCKET newConnection;

    for (int i = 0; i < 100; i++) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

        if (newConnection == 0) {
            cout << "ERROR: Failed to create connection" << endl;
        } else {
            cout << "Client CONNECTED!" << endl;
            string msg = "Hello. It's my first network program!";
            int msg_size = msg.size();
            Packet msgtype = P_ChatMessage;

            send(newConnection, (char*)&msgtype, sizeof(Packet), NULL);
            send(newConnection, (char*)&msg_size, sizeof(int), NULL);
            send(newConnection, msg.c_str(), msg_size, NULL);

            Connections[i] = newConnection;
            Counter++;

            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);

            Packet testPacket = P_Test;
            send(newConnection, (char*)&testPacket, sizeof(Packet), NULL);
        }
    }

    system("pause");
    return 0;
}
