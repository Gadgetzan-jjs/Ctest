#include <bits/types/FILE.h>
#include "stdio.h"
#include "stdlib.h"
#include "strings.h"
#include "string.h"
#include "ActionConfig.h"
////
//// Created by mikasa on 2021/10/18.
////
void confconfig(Config * config,char * filepath) {
    FILE *fp;
    if ((fp = fopen(filepath, "r"))==NULL){
        perror("[ERROR]: open file fail\n");
        exit(1);
    }
    char context[101];
    while (fgets(context, sizeof(context),fp)){
        if (!strcmp(context,"server:\n")){
            if (fgets(context,sizeof(context),fp)){
                char * key=strtok(context,":");
                if (!strcmp(key,"    port")){
                    char * value;
                    value=strtok(NULL,key);
                    value=strtok(value,"\n");
                    config->port=value;
                    printf("port %s\n",config->port);
                }
            }
        } else if (!strcmp(context,"webinfo:\n")){
            if (fgets(context, sizeof(context),fp)){
                char * key=strtok(context,":");
                if (!strcmp(key,"    location")){
                    char * afterpart=strtok(NULL,":");
                    char value[101];
                    int valueindex=0;
                    int len=strlen(afterpart);
                    for (int i = 0; i < len; ++i) {
                        if (valueindex<101){
                            if (afterpart[i]=='\n')break;
                            value[valueindex++]=afterpart[i];
                        }
                    }
                    value[valueindex]='\0';
//                    printf("value is isi %s\n",value);
//                    value=strtok(value,"\n");
                    config->conffilepath=value;
//                    printf("location %s\n",value);
                }
            }
        }
    }
}
//
