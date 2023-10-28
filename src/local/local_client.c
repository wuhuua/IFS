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

#include "./local_client.h"
#include "../core/ifs.h"
#include "../common/start_page.h"

extern    ext2_group_desc group_desc;        // 组描述符
extern    ext2_inode inode;
extern    ext2_dir_entry dir;                // 目录体
extern    FILE *f;                           /*文件指针*/
extern    unsigned int last_allco_inode;     // 上次分配的索引节点号
extern    unsigned int last_allco_block;     // 上次分配的数据块号

int cli(){
    ext2_inode currentdir;
    printStartPage();
    printf("Hello! Welcome to Ext2_like file system!\n");
    if (initfs(&currentdir) == 1)
        return 0;
    if (login() != 0) /***************/
    {
        printf("Wrong password!It will terminate right away.\n");
        exitdisplay();
        return 0;
    }
    shellloop(currentdir);
    exitdisplay();
    return 0;
}

void server_help(){
    printf("Help:\n");
    printf("no args: start as server mode\n");
    printf("-i:      start as server mode\n");
    printf("-p:      set port\n");
    printf("-b:      set server input buffer\n");
    printf("-c:      set the max client num\n");
    printf("-h:      print this help\n");
}