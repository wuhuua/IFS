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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./start_page.h"

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