﻿#include <stdio.h>
//Khai bao thu vien WinSock.h
#include <WinSock2.h>
//Them thu vien ws_32
#include <windows.h>
#include <iostream>
#include <lmcons.h>
#include <codecvt>
#include <sstream>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

using namespace std;

int main()
{
    //Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    //tao socket tu phia client
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //Khai bao bien va luu dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8000);

    system("pause");

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("Ket noi khong thanh cong - %d\n", ret);
        return 1;
    }

    //Gui thong tin ve may tinh ve phia server
    // Gửi thông tin tên máy tới server
    send(client, "Ten may: ", strlen("Ten may: "), 0);

    // Lấy tên máy
    TCHAR computerName[UNCLEN + 1];
    DWORD computer_len = UNCLEN + 1;
    GetComputerName((TCHAR*)computerName, &computer_len);

    // Convert TCHAR sang char
    char msg[500];
    wcstombs(msg, computerName, wcslen(computerName) + 1);

    //Gửi tên máy
    send(client, msg, strlen(msg), 0);

    system("pause");

    //Gửi danh sách ổ đĩa và dung lượng của nó
    send(client, "\n\nDanh sach o dia va kich thuoc cua tung o dia:\n", strlen("\n\nDanh sach o dia va kich thuoc cua tung o dia:\n"), 0);

    //===
    char* drives = new char[MAX_PATH]();
    string nameDisk = "C:\\\\";
    ULARGE_INTEGER FreeBytesAvailable, TotalNumberOfBytes, TotalNumberOfFreeBytes;
    wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    GetLogicalDriveStringsA(MAX_PATH, drives);

    for (int i = 0; i < 100; i += 4)
        if (drives[i] != (char)0)
        {
            nameDisk[0] = drives[i];    //Thay ten o dia (C:\\ ==> D:\\ ...)
            send(client, nameDisk.substr(0, 2).c_str(), strlen(nameDisk.substr(0, 2).c_str()), 0);

            // Convert string --> LPCWST
            wstring wst_nameDisk = converter.from_bytes(nameDisk);
            LPCWSTR nameDisk_convert = wst_nameDisk.c_str();

            GetDiskFreeSpaceEx(nameDisk_convert, &FreeBytesAvailable, &TotalNumberOfBytes, &TotalNumberOfFreeBytes);

            string totalSize = "\nTong dung luong tren o dia: " + to_string(TotalNumberOfBytes.QuadPart / 1024.0 / 1024 / 1024) + " GB";
            send(client, totalSize.c_str(), strlen(totalSize.c_str()), 0);

            string freeAvailable = "\nTong dung luong trong co san: " + to_string(FreeBytesAvailable.QuadPart / 1024.0 / 1024 / 1024) + " GB";
            send(client, freeAvailable.c_str(), strlen(freeAvailable.c_str()), 0);

            string totalFree = "\nDung luong trong tren o dia: " + to_string(TotalNumberOfFreeBytes.QuadPart / 1024.0 / 1024 / 1024) + " GB\n\n";
            send(client, totalFree.c_str(), strlen(totalFree.c_str()), 0);

        }

    closesocket(client);
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


