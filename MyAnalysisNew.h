//
// Created by mikasa on 2021/8/9.
//
extern void open(char * filepath,char * uri,char * path);
//extern struct Attribute;
extern void setAttribute(char key[],void * value);
extern void close();
struct Attribute{
    void * context;
    int type;
};
#define Interger 100;
#define String 101;

