#include <stdio.h>
#include <winsock2.h>
#include "Chat.h"
#include <stdlib.h>

#pragma comment(lib,"ws2_32.lib")

static void InitChat();
static void InitConnection();

int ChatMain(){
    printf("Starting Chat App...\n");
    Sleep(2500);
    printf(" --> Done\n");
    InitChat();
    return 0;
}

static void InitChat(){
    InitConnection();
}

int validate_number(char * str) {
    while ( * str) {
        if (!isdigit( * str)) { //if the character is not a number, return false
            return 0;
        }
        str++; //point to next character
    }
    return 1;
}

int validate_ip(char * RemoteUserIP) { //check whether the IP is valid or not

    // Check if the input is a valid IPv4 address
    int octet_count = 0;
    int dot_count = 0;
    for (int i = 0; i < strlen(RemoteUserIP); i++)
    {
        if (isdigit(RemoteUserIP[i]))
        {
            // Ensure that each octet is within the range 0-255
            int octet = atoi(RemoteUserIP + i);
            if (octet < 0 || octet > 255)
            {
                printf("Invalid IPv4 address: octet out of range\n");
                return 1;
            }

            // Skip over any subsequent digits in the octet
            while (isdigit(RemoteUserIP[i + 1]))
            {
                i++;
            }
            octet_count++;
        }
        else if (RemoteUserIP[i] == '.')
        {
            dot_count++;
        }
        else
        {
            // Invalid character in the input
            printf("Invalid IPv4 address: invalid character\n");
            return 1;
        }
    }

    // Ensure that there are exactly four octets and three dots in the input
    if (octet_count != 4 || dot_count != 3)
    {
        printf("Invalid IPv4 address: incorrect number of octets or dots\n");
        return 0;
    }

    // Input is a valid IPv4 address
    printf("Valid IPv4 address: %s\n", RemoteUserIP);
    return 1;
}

char* getRemoteUserIP(){ /* Returns a validated IPv4 that the User wants to connect to */
    printf("Please Input the remote IPv4: ");
    char* RemoteUserIP = malloc(16 * sizeof(char));
    scanf_s("%s", RemoteUserIP, 16);
    if (validate_ip(RemoteUserIP) != 1){
        printf("ERROR: something is wrong with this IPv4: %c", *RemoteUserIP);
    }
    return RemoteUserIP;
}

static void InitConnection(){
    WSADATA wsa;
    SOCKET sockD;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        printf("Error initialising WSA: %d", WSAGetLastError());

    sockD = socket(AF_INET , SOCK_STREAM , 0 );
    if(sockD == INVALID_SOCKET)
        printf("Invalid Socket: %d", WSAGetLastError());

    char *p_RemoteIP = getRemoteUserIP();

    struct sockaddr_in servAddr;
    servAddr.sin_addr.s_addr = (unsigned long) (unsigned char) (*p_RemoteIP);
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(4444);

    int connectStatus = connect(sockD, (struct sockaddr*)&servAddr, sizeof(servAddr));
    if (connectStatus == -1) {
        printf("Error...\n");
    }
    else {
        char strData[255];
        recv(sockD, strData, sizeof(strData), 0);
        printf("Message: %s\n", strData);
    }
}