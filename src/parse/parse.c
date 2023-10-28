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
*   Created by wuhuua on 2023/10/28
*/

#include "./parse.h"

char ctable[12][10] = {"create", "delete", "cd", "close", "read", "write", "password", "format", "exit", "login", "logout", "ls"};

void parse(const char* message,ext2_inode* currentdir,char* response){
    char command[10], var1[10], var2[128], path[10];
    ext2_inode temp;
    int i, j;
    char tempStr[50];
    (void)sscanf(message,"%s %s %s",command,var1,var2);
    int lenVar1=strlen(var1);
    int lenVar2=strlen(var2);
    for (i = 0; i < 12; i++)
        if (!strcmp(command, ctable[i]))
            break;
    if (i == 0 || i == 1) // 创建，删除 文件/目录
    {
        if(lenVar1==0){
            strcat(response,"the first variant must be [f/d]\n");
            return;
        }
        if (var1[0] == 'f')
            j = 1; // 创建文件
        else if (var1[0] == 'd')
            j = 2; // 创建目录
        else
        {
            strcat(response,"the first variant must be [f/d]\n");
            return;
        }
        if(lenVar2==0){
            strcat(response,"no enough agrs for create/delete, need both type and name\n");
            return;
        }
        if (i == 0)
        {
            if (Create(j, currentdir, var2) == 1){
                memset(tempStr,0,sizeof(tempStr));
                sprintf(tempStr,"Failed! %s can't be created\n", var2);
                strcat(response,tempStr);
            }
            else{
                memset(tempStr,0,sizeof(tempStr));
                sprintf(tempStr,"Congratulations! %s is created\n", var2);
                strcat(response,tempStr);
            }
        }
        else
        {
            if (Delet(j, currentdir, var2) == 1){
                memset(tempStr,0,sizeof(tempStr));
                sprintf(tempStr,"Failed! %s can't be deleted!\n", var2);
                strcat(response,tempStr);
            }

            else{
                memset(tempStr,0,sizeof(tempStr));
                sprintf(tempStr,"Congratulations! %s is deleted!\n", var2);
                strcat(response,tempStr);
            }
        }
    }
    else if (i == 2) // open == cd change dir
    {
        if(lenVar1==0){
            strcat(response,"Directory is empty\n");
            return;
        }
        i = 0;
        j = 0;
        temp = *currentdir;
        while (1)
        {
            path[i] = var1[j];
            if (path[i] == '/')
            {
                if (j == 0)
                    initialize(currentdir);
                else if (i == 0)
                {
                    strcat(response,"path input error!\n");
                    break;
                }
                else
                {
                    path[i] = '\0';
                    if (Open(currentdir, path) == 1)
                    {
                        strcat(response,"path input error!\n");
                        *currentdir = temp;
                    }
                }
                i = 0;
            }
            else if (path[i] == '\0')
            {
                if (i == 0)
                    break;
                if (Open(currentdir, path) == 1)
                {
                    strcat(response,"path input error!\n");
                    *currentdir = temp;
                }
                break;
            }
            else
                i++;
            j++;
        }
    }
    else if (i == 3) // close
    {
        if(lenVar1==0){
            strcat(response,"No layer input, use close {layer} instead\n");
            return;
        }
        /*imagine the second variable suply number of layers to get out of*/
        int layer=atoi(var1);
        for (j = 0; j < layer; j++){
            if (Close(currentdir) == 1)
            {
                memset(tempStr,0,sizeof(tempStr));
                sprintf(tempStr,"Warning! the number %d is too large\n", layer);
                strcat(response,tempStr);
                break;
            }
        }
    }
    else if (i == 4) // read
    {
        if(lenVar1==0){
            strcat(response,"No file input\n");
            return;
        }
        if (quietRead(currentdir, var1,response) == 1)
            strcat(response,"Failed! The file can't be read\n");
    }
    else if (i == 5) // write
    {
        sscanf(message,"%s %s %[^\\]",command,var1,var2);
        printf("%s\n",message);
        if(lenVar1==0){
            strcat(response,"No file input\n");
            return;
        }
        if(strlen(var2)==0){
            strcat(response,"No content to write\n");
            return;
        }
        int res=quietWrite(currentdir,var1,var2);
        if (res == 1)
            strcat(response,"Failed! The file can't be written\n");
        if (res == 2)
            strcat(response,"There isn't this file,please create it first\n");
    }
    else if (i == 6) // password
        strcat(response,"Permission denied!\n");
    else if (i == 7) // format
    {
        strcat(response,"Permission denied!\n");
    }
    else if (i == 8) // exit
    {
        strcat(response,"Client should use shutdown instead!\n");
        // code should not get here
    }
    else if (i == 9) // login
        strcat(response,"You are already login!\n");
    else if (i == 10) // logout
    {
        strcat(response,"Client should use exit instead!\n");
    }
    else if (i == 11) // ls
        quietLs(currentdir,response);
    else
        printf("Failed! Command not available\n");
}