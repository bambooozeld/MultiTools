#include <stdio.h>
#include <winsock2.h>
#include "Chat.h"
#include "Logger.h"
#include <stdlib.h>


static void InitChat();
static void InitConnection();
static void BuildServer();
static void BuildOrReceiveConn();
static void ChatLogFileForCurrentInstance();

int ChatMain(void){
    SetupFunctionCodes(); //Setup Logger Function Codes
    printf("Starting Chat App...\n");
    ChatLogFileForCurrentInstance();
    Sleep(2500); // for realism, for now.
    printf("Chat App loaded...\n");
    InitChat();
    return 0;
}

static void ChatLogFileForCurrentInstance(void){
    char ChatApp_LoadUp_Text[] = " Chat App loaded...";
    char *firstInitialLogToFile = ChatApp_LoadUp_Text;
    char * time = GetTime();
    strcpy(firstInitialLogToFile, ChatApp_LoadUp_Text);
    char * msg = strcat(time, ChatApp_LoadUp_Text);
    LogMessage(msg);
}

static void InitChat(void){
    BuildOrReceiveConn();
}

static void BuildOrReceiveConn(void){
    int BuildOrReceiveConnInput;
    int valid = 0;
    int Attempts = 1;

    printf("Would you like to initiate(0) a connection or receive(1) a connection? (0/1)\n");
    printf("Input: ");
    scanf_s("%d", &BuildOrReceiveConnInput, 2);

    if (BuildOrReceiveConnInput != 0) {
        if (BuildOrReceiveConnInput != 1) {
            printf("Wrong Input!");
            exit(0);
        } else if (BuildOrReceiveConnInput == 1)
            BuildServer();
    } else InitConnection();
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
                return 0;
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
            return 0;
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

u_long getRemoteUserIP(void){
    int valid = 0;
    char RemoteUserIP[16];
    int Attempts = 1;
    u_long RemoteIP;
    do {
         //Checking Attempts for correct formatting
         printf(Attempts == 1 ? "Please Input the remote IPv4: " : "\nPlease Input the remote IPv4: ");
         Attempts++;

         scanf_s("%s", RemoteUserIP, 16);
         if (strcmp(RemoteUserIP, "") == 0){
             printf("Invalid IPv4 address: invalid character or empty\n");
         }
         else{
             if (validate_ip(RemoteUserIP) == 1) {
                 valid = 1;
             }
         }
    } while (valid != 1);

    RemoteIP = inet_addr(RemoteUserIP);

    printf("Do you want to build a connection with %s ? (y/n) ", RemoteUserIP);
    char *initConnectionCheck = malloc(24);
    scanf("%c", initConnectionCheck);

    if (*initConnectionCheck == 'n'){
        printf("\n");

    }
    else if (*initConnectionCheck == 'y'){
        printf("Ok");
        free(initConnectionCheck);
        return RemoteIP;
    }
    free(initConnectionCheck);
    DWORD err = 0;
    return err;
}

static void BuildServer(void){
    WSADATA wsa;
    SOCKET s, new_socket;
    int c;
    struct sockaddr_in remoteServer, client;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0){
        printf("Error Code: %d", WSAGetLastError());
    }
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    u_long RemoteIP = getRemoteUserIP();
    if (RemoteIP == 0)
        printf("Something went wrong getting the RemoteIP");

    remoteServer.sin_family = AF_INET;
    remoteServer.sin_addr.s_addr = INADDR_ANY;
    remoteServer.sin_port = htons( 8888 );

    if( bind(s ,(struct sockaddr *)&remoteServer , sizeof(remoteServer)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
    }
    listen(s,3);

    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
    new_socket = accept(s , (struct sockaddr *)&client, &c);
    if (new_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d" , WSAGetLastError());
    }

    puts("Connection accepted");

    closesocket(s);
    WSACleanup();
}

static void InitConnection(void){
    WSADATA wsa;
    SOCKET sockD;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        printf("Error initialising WSA: %d", WSAGetLastError());

    sockD = socket(AF_INET , SOCK_STREAM , 0 );
    if(sockD == INVALID_SOCKET)
        printf("Invalid Socket: %d", WSAGetLastError());

    u_long RemoteIP = getRemoteUserIP();
    if (RemoteIP == 0)
        printf("Something went wrong getting the RemoteIP");
    const char * p_RemoteIP = (const char *) &RemoteIP;

    struct sockaddr_in remoteAddr;
    remoteAddr.sin_addr.s_addr = inet_addr(p_RemoteIP);
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(4444);
    printf("Done");
    Sleep(5000);

    int connectStatus = connect(sockD, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr));
    if (connectStatus == -1) {
        printf("Error...\n");
    }
    else {
        char strData[255];
        recv(sockD, strData, sizeof(strData), 0);
        printf("Message: %s\n", strData);
    }
}