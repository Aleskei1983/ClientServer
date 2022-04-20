#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cstring>
#include <winsock.h>
#pragma comment (lib, "WCS_32.lib")
using namespace std;

DWORD WINAPI serverReceive(LPVOID lpParam) {
    char buffer [1024] = {0};
    SOCKET client = *(SOCKET*)lpParam;
    while (true)
    {
        if (recv(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
        {
            cout << "recv function failed with error" << WSAGetLastError() << endl;
            return -1;
        }
        if (strcmp(buffer, "exit\n") == 0)
        {
            cout << "Client Disconnected." << endl;
            break;
        }
        cout << "Client: " << buffer << endl;
        memset(buffer, 0, sizeof(buffer));
    }    
    return 1;
} 

DWORD WINAPI serverSend(LPVOID lpParam){
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
    
}
