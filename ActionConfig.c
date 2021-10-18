#include <bits/types/FILE.h>
#include "stdio.h"
#include "stdlib.h"
#include "strings.h"
#include "string.h"
////
//// Created by mikasa on 2021/10/18.
////
void split(char *src,const char *separator,char **dest,int *num) {
    char *pNext;
    int count = 0;
    if (src == NULL || strlen(src) == 0)
        return;
    if (separator == NULL || strlen(separator) == 0)
        return;
    pNext = strtok(src,separator);
    while(pNext != NULL) {
        *dest++ = pNext;
        ++count;
        pNext = strtok(NULL,separator);
    }
    *num = count;
}
void confconfig(char * filepath) {
    FILE *fp;
    if ((fp = fopen(filepath, "r"))==NULL){
        perror("[ERROR]: open file fail\n");
        exit(1);
    }
    char context[101];
    while (fgets(context, sizeof(context),fp)){
        if (!strcmp(context,"server:\n")){
            if (fgets(context,sizeof(context),fp)){

            }
        }
    }


}
//
