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

#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./net/net.h"
#include "./local/local_client.h"

#define SERVER_START 0

int main(int argc, char *argv[]){
    // 默认模式，不开启web服务
    if(argc==1){
        cli();
        return 0;
    }
    else{
        int opt,
        port=8080,
        max_clients=10,
        buffer_size=1024,
        server_start=0,
        need_help=0;
        while(-1 != (opt = getopt(argc, argv, "b:c:p:ih"))){
            switch(opt) {
                case 'p':
                    port = atoi(optarg);
                    break;
                case 'i':
                    server_start=1;
                    break;
                case 'b':
                    buffer_size = atoi(optarg);
                    break;
                case 'c':
                    max_clients = atoi(optarg);
                    break;
                case 'h':
                    need_help=1;
                    break;
                default:
                    printf("wrong argument\n");
                    server_help();
                    break;
            }
        }
        if(need_help){
            server_help();
            return 0;
        }
        if(server_start){
            int res=start_server(port,max_clients,buffer_size);
            if(res!=0){
                printf("server terminated\n");
            }
            return 0;
        }
    }
}