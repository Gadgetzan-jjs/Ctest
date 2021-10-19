//
// Created by mikasa on 2021/10/19.
//

#ifndef UNTITLED3_ACTIONCONFIG_H
#define UNTITLED3_ACTIONCONFIG_H

#endif //UNTITLED3_ACTIONCONFIG_H
typedef struct ConfigStruct{
    const char * port;
    char * conffilepath;
}Config;
void confconfig(Config * config,char * filepath);