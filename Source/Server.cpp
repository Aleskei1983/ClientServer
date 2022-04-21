#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cstring>
#include <winsock.h>
#pragma comment (lib, "WCS_32.lib")
using namespace std;

DWORD WINAPI serverReceive(LPVOID lpParam) {                                            //Receive data from client
    char buffer [1024] = {0};                                                           //Buffer for data
    SOCKET client = *(SOCKET*)lpParam;                                                  //Socket for client
    while (true)                                                                        //Cycle of server
    {
        if (recv(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
        {
            cout << "recv function failed with error" << WSAGetLastError() << endl;     //If not successful receive the data, error message and log out 
            return -1;
        }
        if (strcmp(buffer, "exit\n") == 0)
        {
            cout << "Client Disconnected." << endl;                                     //In case client disconnection
            break;
        }
        cout << "Client: " << buffer << endl;                                           //Receive client message from buffer
        memset(buffer, 0, sizeof(buffer));                                              //Clean buffer
    }    
    return 1;
} 

DWORD WINAPI serverSend(LPVOID lpParam){                                                //Send data to client
    char buffer [1024] = {0};
    SOCKET client = *(SOCKET*)lpParam;
    while (true)
    {
      if (send(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
      {
          cout << "send failed with error " << WSAGetLastError() << endl;
          return -1;
      }
      if (strcmp(buffer, "exit\n") == 0)
      {
          cout << "Thank you for using the application" << endl;
          break;
      }      
    }
    return 1;
}

int main(){
    WSADATA WSAData;                                                                    //Data
    SOCKET server, client;                                                              //Client and server sockets
    SOCKADDR_IN serverAddr, clientAddr;                                                 //Sockets address
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);                                           //Server creation

    if (bind(server,(SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        cout << "Bind function failed with error: " << WSAGetLastError() << endl;
        return -1;
    }
    if (listen(server, 0) == SOCKET_ERROR)                                              //Receive is not accepted
    {
        cout << "Listen function failed with error:" << WSAGetLastError() <<endl;
        return -1;
    }
    
    cout << "Listening for incoming connection..." << endl;

    char buffer[1024];                                                                      //Buffer created
    int clientAddrSize = sizeof(clientAddr);                                                //Client addres initialization
    if ((client = accept(server, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)      
    {                                                                                        //Successful connection
        cout << "Client connected!" << endl;
        cout << "Now you can use our live chat application." << "Enter \"exit\" to disconnect" << endl;

        DWORD tid;                                                                              //Ident
        HANDLE t1 = CreateThread(NULL, 0, serverReceive, &client, 0, &tid);                     //Flow data for receiving data
        if (t1 == NULL)
        {
            cout << "Thread Creation Error: " << WSAGetLastError() << endl;
        }
        HANDLE t2 = CreateThread(NULL, 0, serverSend, &client, 0, &tid);                        //Flow data for sending
        if (t2 == NULL)
        {
            cout << "Thread Creation Error: " << WSAGetLastError() << endl;
        }

        WaitForSingleObject(t1, INFINITE);
        WaitForSingleObject(t2, INFINITE);

        closesocket(client);                                                                    //Close socket
        if (closesocket(server) == SOCKET_ERROR)                                                //Close socket error
        {
            cout << "Close socket failed with error" << WSAGetLastError() << endl;
            return -1;
        }
        WSACleanup();
        
    }
    
    
}
