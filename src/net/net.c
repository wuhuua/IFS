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

#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "./net.h"
#include "../core/ifs.h"
#include "../parse/parse.h"

extern    ext2_group_desc group_desc;        // 组描述符
extern    ext2_inode inode;
extern    ext2_dir_entry dir;                // 目录体
extern    FILE *f;                           /*文件指针*/
extern    unsigned int last_allco_inode;     // 上次分配的索引节点号
extern    unsigned int last_allco_block;     // 上次分配的数据块号

int handle_request(userInfo *user,const char* message,char* response,int buffersize){
    char* content=(char*)malloc(buffersize);
    memset(content,0,buffersize);
    // 处理初始化
    if(user->status<LOGIN){
        if(user->status==START){
            user->status=START;
        }
        else if(strcmp(message,"Y")==0||strcmp(message,"y")==0){
            user->status=ACCEPTINIT;
        }
        else if(strcmp(message,"N")==0||strcmp(message,"n")==0){
            user->status=REFUSEINIT;
        }else{
            user->status=INITUNKNOWN;
        }
        if(quietConnect(user,content)!=0){
            strcat(content,"\nfile system init error\n");
        }
        if(strlen(content)!=0){
            strcat(response,content);
            memset(content,0,buffersize);
        }
        if(user->status==LOGIN){
            strcat(response,"please input the password(init:123):");
            free(content);
            return 0;
        }
    }

    // 处理登录
    if(user->status==LOGIN){
        quietLogin(user,message,content);
        if(strlen(content)!=0){
            strcat(response,content);
            memset(content,0,buffersize);
        }
        if(user->status==INTERN){
            char dirName[20];
            getstring(dirName,user->currentdir);
            sprintf(content,"\n%s=># ",dirName);
            strcat(response,content);
            free(content);
            return 0;
        }
    }

    // 处理文件系统内操作
    if(user->status==INTERN){
        if(strlen(message)==0){
            // do nothing
        }
        else if(strcmp(message,"shutdown")==0){
            strcpy(response,"IFS server shutdown, bye!\n");
            free(content);
            return -2;
        }
        else{
            parse(message,&user->currentdir,content);
            if(strlen(content)!=0){
                strcat(response,content);
                memset(content,0,buffersize);
            }
        }
    }

    if(user->status==INTERN){
        memset(content,0,buffersize);
        char dirName[20];
        getstring(dirName,user->currentdir);
        sprintf(content,"\n%s=># ",dirName);
        strcat(response,content);
    }

    free(content);

    // 处理退出
    if(user->status==QUIT){
        return -1;
    }

    return 0;
}

int start_server(int port,int max_clients,int buffer_size){
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[buffer_size];
    memset(buffer,0,sizeof(buffer));
    char* response=(char*)malloc(buffer_size);
    const char* errorInfo="server error, cannot resolve your request.";

    // 创建 socket 文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return INIT_ERROR;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // 设置可重用性
    int on = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    // 将 socket 绑定到指定的 IP 地址和端口上
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return BINDING_ERROR;
    }

    // 监听来自客户端的连接请求
    if (listen(server_fd, max_clients) < 0) {
        perror("listen failed");
        return LISTEN_FAILED;
    }

    // 创建客户端套接字数组
    int client_sockets[max_clients];
    userInfo users[max_clients];
    memset(client_sockets, 0, sizeof(client_sockets));
    int max_fd = server_fd;

    printf("Server init over, port:%d, max_clients:%d, buffer_size:%d\n",port,max_clients,buffer_size);
    printf("Waiting for connections...\n");

    while (1) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);

        // 将客户端套接字添加到集合中
        for (int i = 0; i < max_clients; i++) {
            int client_socket = client_sockets[i];
            if (client_socket > 0) {
                FD_SET(client_socket, &read_fds);
            }
        }

        // 通过 select 函数等待套接字上的读取事件
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("select failed");
            return SELECT_FAILED;
        }

        // 检查是否有新的连接请求
        if (FD_ISSET(server_fd, &read_fds)) {
            // 接受新的连接
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("accept failed");
                return ACCEPT_FAILED;
            }

            // 将新连接的套接字设置为非阻塞模式
            int new_flags = fcntl(new_socket, F_GETFL, 0);
            fcntl(new_socket, F_SETFL, new_flags | O_NONBLOCK);

            printf("New connection accepted\n");

            // 将新连接的套接字添加到客户端套接字数组中
            for (int i = 0; i < max_clients; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    initUserInfo(&users[i]);
                    break;
                }
            }

            // 更新最大文件描述符
            if (new_socket > max_fd) {
                max_fd = new_socket;
            }
        }

        // 处理已建立连接的套接字
        for (int i = 0; i < max_clients; i++) {
            int client_socket = client_sockets[i];
            if (FD_ISSET(client_socket, &read_fds)) {
                // 从客户端读取数据
                valread = read(client_socket, buffer, buffer_size);

                if (valread <= 0) {
                    // 客户端关闭连接或发生错误
                    printf("Connection closed\n");
                    close(client_socket);
                    client_sockets[i] = 0;
                } else {
                    // 处理客户端发送的数据
                    int res=handle_request(&users[i],buffer,response,buffer_size);
                    // 返回信息
                    if(res==0){
                        send(client_socket,response, strlen(response), 0);
                    }else if(res==-1){
                        send(client_socket, response, strlen(response), 0);
                        close(client_socket);
                        client_sockets[i] = 0;
                    }else if(res==-2){
                        send(client_socket, response, strlen(response), 0);
                        // 关闭所有连接
                        for (int i = 0; i < max_clients; i++) {
                            if (client_sockets[i] != 0) {
                                close(client_sockets[i]);
                            }
                        }
                        close(server_fd);
                        printf("shutdown by client\n");
                        return 0;
                    }
                    else{
                        send(client_socket, errorInfo, strlen(errorInfo), 0);
                    }
                    printf("Response sent\n");

                    // 清空缓冲区
                    memset(buffer, 0, sizeof(buffer));
                    memset(response, 0, buffer_size);
                }
            }
        }
    }
    // 关闭所有连接
    for (int i = 0; i < max_clients; i++) {
        if (client_sockets[i] != 0) {
            close(client_sockets[i]);
        }
    }
    close(server_fd);
    free(response);
    return 0;
}