#include <stdio.h>
//Khai bao thu vien WinSock.h
#include <WinSock2.h>
//Them thu vien ws_32
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main()
{
    //Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    //tao socket tu phia client
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //Khai bao bien va luu dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8000);

    // Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));

    // Chuyen sang trang thai cho ket noi
    listen(listener, 5);

    // Cho ket noi moi
    SOCKET client = accept(listener, NULL, NULL);
    char buf[256];

    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);

        if (ret <= 0)
        {
            if (ret == -1)
                ret = WSAGetLastError();
            break;
        }

        if (ret < sizeof(buf))
            buf[ret] = 0;
        printf("%s", buf);
    }

    printf("\nKet thuc nhan du lieu\n\n");

    // Dong ket noi va giai phong thu vien
    closesocket(client);
    closesocket(listener);
    WSACleanup();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
