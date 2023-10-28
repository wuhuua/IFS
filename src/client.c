/*
Copyright (c) 2023 UnloadHome and/or its affiliates. All rights reserved.
UnloadKV is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
 */

/*
*   Created by wuhuua on 2023/10/26
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <termios.h>

char title[]    ="*                     IFS                        *";
char copyright[]="*           Copyright ©️ 2023 wuhuua              *";
char helpKids[] ="*            help poor kids in Uganda            *";
char support[]  ="*     contact us through wuuhuua@163.com         *";
char emptyLine[]="*                                                *";
char line[]     ="**************************************************";

void printAds() {


    srand(time(NULL));
    int randomNum = rand() % 4;
    if (randomNum == 0) {
        printf("%s\n", helpKids);
    } else {
        printf("%s\n", support);
    }
}

void printStartPage() {
    printf("%s\n", line);
    printf("%s\n", emptyLine);
    printf("%s\n", title);
    printf("%s\n", emptyLine);
    printf("%s\n", copyright);
    printf("%s\n", emptyLine);
    printAds();
    printf("%s\n", emptyLine);
    printf("%s\n", line);
}

typedef struct ConnectInfo{
    int clientSocket;
    int bufferSize;
}ConnectInfo;

int connectIFS(const char* ip,int port){
    int clientSocket=-1;
    struct sockaddr_in serverAddress;

    // 创建客户端套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Failed to create socket");
        return -1;
    }

    // 设置服务器地址
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &(serverAddress.sin_addr)) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    // 连接到服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Connection failed");
        return -1;
    }
    printf("Successfully connect to server, ip:%s, port:%d \n",ip,port);
    return clientSocket;
}


void client_help(){
    printf("Help:\n");
    printf("port and server ip address is required to start a client\n");
    printf("-p:      set port (required)\n");
    printf("-b:      set server input buffer\n");
    printf("-t:      set the ip address of targer server (required)\n");
    printf("-h:      print this help\n");
}

int send2IFS(int clientSocket,
              const char* message,int messageSize,const char* reply, int replySize){
    if(clientSocket==-1){
        perror("Unable to send message");
        return -1;
    }
    // 发送消息给服务器
    if (send(clientSocket, message, messageSize, 0) < 0) {
        perror("Failed to send message");
        return -1;
    }
    // 接收服务器的响应
    memset((void*)reply, 0, replySize);
    if (recv(clientSocket, (void*)reply, replySize, 0) < 0) {
        perror("Failed to receive message");
        return -1;
    }
    return 0;
}

int CliInit(const ConnectInfo* info){
    char* reply=(char*)malloc(info->bufferSize);
    if(send2IFS(info->clientSocket,"init",4,reply,info->bufferSize)!=0){
        free(reply);
        return -1;
    }
    printf("%s",reply);
    free(reply);
    return 0;
}

int getch()
{
    int ch;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void writeMultipleLineText(char* content,int size){
    memset((void*)content,0,size);
    char str;
    int i=0;
    str = getch();
    while (str != 27)
    {
        printf("%c", str);
        if (str == 0x0d){
            str=0x0a;
            printf("%c", str);
        }

        if(i<size)
            content[i++] = str;
        str = getch();
        if (str == 27)
            break;
    }
    if(i>=size){
        printf("write more than buffer size, send %d bytes",size);
    }
}

int writeFunc(char* message,int size){
    char cmd[10];
    char fileName[20];
    int contentSize=size-strlen(message)-5;
    char* content=(char*)malloc(contentSize);
    memset((void*)cmd,0,10);
    memset((void*)fileName,0,20);
    memset((void*)content,0,contentSize);
    sscanf(message,"%s %s %s",cmd,fileName,content);
    if(strcmp(cmd,"write")!=0){
        printf("%s\n",cmd);
        free(content);
        return 0;
    }
    if(strlen(fileName)==0){
        free(content);
        return 1;
    }
    if(strlen(content)!=0){
        printf("%d\n",(int)strlen(content));
        free(content);
        return 0;
    }
    writeMultipleLineText(content,contentSize);
    strcat(message," ");
    strcat(message,content);
    free(content);
    return 0;
}


void CliInput(const ConnectInfo* info){
    char* reply=(char*)malloc(info->bufferSize);
    char* message=(char*)malloc(info->bufferSize);
    while(1){
        memset((void*)message,0,info->bufferSize);
        memset((void*)reply,0,info->bufferSize);
        fgets(message, info->bufferSize, stdin);
        message[strlen(message)-1]='\0';
        if(strcmp(message,"exit")==0){
            free(reply);
            free(message);
            return;
        }
        if(writeFunc(message,info->bufferSize)!=0){
            free(reply);
            free(message);
            printf("write func needs file name");
            return;
        }
        if(send2IFS(info->clientSocket,message,strlen(message)+1,reply,info->bufferSize)!=0){
            free(reply);
            free(message);
            printf("Connection error\n");
            return;
        }
        printf("%s",reply);
        if(strcmp(message,"shutdown")==0){
            free(reply);
            free(message);
            return;
        }
        memset((void*)reply,0,info->bufferSize);
    }
}

int main(int argc, char *argv[]) {
    char ip[20];
    memset(ip,0,20);
    int opt,
    port=-1,
    need_help=0,
    buffer_size=1024;
    while(-1 != (opt = getopt(argc, argv, "b:t:p:h"))){
        switch(opt) {
            case 'p':
                port = atoi(optarg);
                break;
            case 't':
                memcpy(ip,optarg,strlen(optarg)+1);
                break;
            case 'b':
                buffer_size = atoi(optarg);
                break;
            case 'h':
                need_help=1;
                break;
            default:
                printf("wrong argument\n");
                break;
        }
    }
    if(need_help){
        client_help();
        return 0;
    }
    if(strlen(ip)==0){
        printf("ip is required\n");
        client_help();
        return 0;
    }
    if(port==-1){
        printf("port is required\n");
        client_help();
        return 0;
    }
    int clientSocket=connectIFS(ip,port);
    if(clientSocket==-1){
        printf("connect failed\n");
    }else{
        printStartPage();
    }
    ConnectInfo info;
    info.bufferSize=buffer_size;
    info.clientSocket=clientSocket;
    if(CliInit(&info)==0){
        CliInput(&info);
    }
    printf("client exit\n");
    close(clientSocket);
}