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

#ifndef IFS_H
#define IFS_H

#define blocks 4611          // 1+1+1+512+4096,总块数
#define blocksiz 512         // 每块字节数
#define inodesiz 64          // 索引长度
#define data_begin_block 515 // 数据开始块
#define dirsiz 32            // 目录体长度
#define EXT2_NAME_LEN 15     // 文件名长度
#define PATH "vdisk"         // 文件系统

#include "time.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>          /* for STDIN_FILENO */

typedef struct ext2_group_desc // 组描述符 68 字节
{

    char bg_volume_name[16];  // 卷名
    int bg_block_bitmap;      // 保存块位图的块号
    int bg_inode_bitmap;      // 保存索引结点位图的块号
    int bg_inode_table;       // 索引结点表的起始块号
    int bg_free_blocks_count; // 本组空闲块的个数
    int bg_free_inodes_count; // 本组空闲索引结点的个数
    int bg_used_dirs_count;   // 本组目录的个数
    char psw[16];
    char bg_pad[24]; // 填充(0xff)
} ext2_group_desc;

typedef struct ext2_inode // 索引节点 64 字节
{
    int i_mode;     // 文件类型及访问权限
    int i_blocks;   // 文件的数据块个数
    int i_size;     // 大小(字节)
    time_t i_atime; // 访问时间
    time_t i_ctime; // 创建时间
    time_t i_mtime; // 修改时间
    time_t i_dtime; // 删除时间
    int i_block[8]; // 指向数据块的指针
    char i_pad[24]; // 填充 1(0xff)
} ext2_inode;

typedef struct ext2_dir_entry // 目录体 32 字节
{
    int inode;                // 索引节点号
    int rec_len;              // 目录项长度
    int name_len;             // 文件名长度
    int file_type;            // 文件类型(1:普通文件，2:目录...)
    char name[EXT2_NAME_LEN]; // 文件名
    char dir_pad;             // 填充
} ext2_dir_entry;

// 用户状态机，维护当前用户的状态
typedef enum USERSTATUS{
    START,
    INIT,
    ACCEPTINIT,
    REFUSEINIT,
    INITUNKNOWN,
    LOGIN,
    INTERN,
    QUIT
}USERSTATUS;

typedef struct userInfo       // 用户信息
{
    ext2_inode currentdir;             // 当前目录
    USERSTATUS status;                 // 交互状态
}userInfo;

extern    ext2_group_desc group_desc;        // 组描述符
extern    ext2_inode inode;
extern    ext2_dir_entry dir;                // 目录体
extern    FILE *f;                           /*文件指针*/
extern    unsigned int last_allco_inode;     // 上次分配的索引节点号
extern    unsigned int last_allco_block;     // 上次分配的数据块号

void initUserInfo(userInfo *info);
int initfs(ext2_inode *cu);
int login();
void exitdisplay();
void shellloop(ext2_inode currentdir);
void getstring(char *cs, ext2_inode node);
int quietConnect(userInfo* info,char* output);
void quietLs(ext2_inode *current,char* output);
void quietLogin(userInfo *info,const char* psw,char* output);
int quietRead(ext2_inode *current, char *name,char* output);
int quietWrite(ext2_inode *current, char *name,char* content);
int Create(int type, ext2_inode *current, char *name);
int Delet(int type, ext2_inode *current, char *name);
int initialize(ext2_inode *cu);
int Open(ext2_inode *current, char *name);
int Close(ext2_inode *current);
#endif