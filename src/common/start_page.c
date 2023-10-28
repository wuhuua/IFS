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