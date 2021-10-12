//#include "MyAnalysis.h"
////#include "Homework/MyCMS.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "pthread.h"
////#include "Web/Myserver.h"
//#include "strings.h"
//#include "string.h"
//#define Controll 1;
//#define Context 2;
//
//typedef struct StringList{
//    char * array;
//    struct StringList *next;
//}Node;
//typedef struct WordList{
//    char * array;
//    struct WordList *next;
//    int type;
//}Word;
//
//StringList * headsl=(StringList *)malloc(sizeof(StringList));
//WordList * headctl=(WordList *)malloc(sizeof(WordList));
//WordList * tailctl=headctl;
//WordList * Wlist[50];
//StringList * List[50];
//int listlen=0;
//int wlistlen=0;
//
//void bodydeal(FILE * fp);
//WordList* perfectthewordlist(char str[],int type);
//StringList* controlldeal(StringList * node,int index);
//
////void open(char * filepath){
////    FILE *fp;
////    char context[101];
////    if ((fp=fopen(filepath,"r"))==NULL){
////        perror("file open fail");
////        exit(1);
////    }
//////    printf("%s\n",headctl->array);
////    while (fgets(context,sizeof(context),fp)){
////        if (!strcmp(context,"<body>\n")){
////            bodydeal(fp);
////            printf("body 执行完毕\n");
////        }
////    }
////    printf("遍历链表\n");
////    printf("%s\n",headctl->array);
////}
//void bodydeal(FILE * fp){
//    char context[101];
//    char jinja[1025];
//    char pln[]="\n";
//    int istake=0;
//    bzero(jinja,1025);
//    while (fgets(context,sizeof(context),fp)){
//        if (!strcmp(context,"</body>\n")) break;
//        strcat(jinja,context);
//    }
//    char * line=NULL;
//    line=strtok(jinja,pln);
//    char * lines[100];
//    StringList *tail=headsl;
//    bzero(lines,100);
////    int index=0;
//    while (line!=NULL) {
//        StringList * node=(StringList *)malloc(sizeof(StringList));
//        tail->next=node;
//        node->array=(char *)malloc(sizeof(line));
//        node->array=line;
//        tail=node;
//        printf("%s\n",node->array);
//        List[listlen++]=node;
//        line=strtok(NULL,pln);
//    }
//    StringList * position=headsl->next;
//    wlistlen=0;
//    while (position!=NULL) {
//            int i=0;
//            while (position!=NULL&&position->array[i]!='\0'){
//            if (position->array[i]==' ') {
//                i++;
//                continue;
//            };
//            if (position->array[i]=='{'&&position->array[i+1]=='%'){//判断是否为控制结构
////                printf("is controll\n");
//                int index=i+1;
//                position=controlldeal(position,index);
////                if (wordandStringList->slhead!=NULL)
////                position=wordandStringList->slhead;
////                else position=NULL;
//                i=0;
//                continue;
//            } else if (position->array[i]=='{'&&position->array[i+1]=='{'){
//                int index=i+1;
//                position=position->next;
//                i++;
//                continue;
//            } else if (position->array[i]=='{'&&position->array[i+1]=='#'){
//                int index=i+1;
//                continue;
//            }else { //如果不是符合语法的句子就保留
//            //不在语法内容的句子保留
//            }
//        }
////            if (position->next!=NULL){position=position->next;};
//    }
////    printf(",.,.%s\n",wordandStringList->wlhead->next->array);
//
//}
//StringList* controlldeal(StringList * node,int index){//处理控制结构
////    WordandStringList * result=(WordandStringList *)malloc(sizeof(WordandStringList));
//    char str[101];
//    bzero(str,101);
//    int strindex=0;
//        int i=index+1;
//        int istrue=0;
//        int iscon=1;
//        int isval=0;
//        while (node->array[i]!='%'&&node->array[i+1]!='}'){//确认是否为控制结构，此处为一行的数据
//        if (node->array[i]==' ') { i++;
//        continue; }
//        else{
//            str[strindex++]=node->array[i];
////            printf("%c\n",str[strindex]);
//            if (iscon) {//判断时默认语法控制的关键字是第一个字段，判断完第一个字段之后
//                if (!strcmp(str, "if")) {
//                    perfectthewordlist(str,1);//如果是if的话创建一个节点
//                    istrue = 1;
//
//                } else if (!strcmp(str, "for")) {
//                    perfectthewordlist(str,1);
//                    istrue = 1;
//
//                } else if (!strcmp(str, "endif")) {
//                    perfectthewordlist(str,1);
//                    istrue = 0;
//                    break;
//                } else if (!strcmp(str, "endfor")) {
//                    perfectthewordlist(str,1);
//                    istrue = 0;
//                    break;
//                }
//                iscon = 0;
//                bzero(str, 101);
//                strindex=0;
//            } else {
//                if (isval){
//                    if (str[strindex-1]=='\''){
//                        perfectthewordlist(str,1);
//                        isval=0;
//                        bzero(str,101);
//                        strindex=0;
//                    }
//                }
//                if (str[strindex-1]=='\''){
//                    isval=1;
//                    i++;
//                    continue;
//                }
//                if (!strcmp(str,"==")){
//                    perfectthewordlist(str,1);
//                    bzero(str,101);
//                    strindex=0;
//                }else if (!strcmp(str,"!=")){
//                    perfectthewordlist(str,1);
//                    bzero(str,101);
//                    strindex=0;
//                }
//            }
//        }
//        i++;
//    }
//        int j;
//        while (istrue){
//            j=0;
//            if (node->next==NULL) return NULL;
//            node=node->next;
//            while (node->array[j]==' ')j++;
//            if (node->array[j]=='{'&&node->array[j+1]=='%'){
//                return node;
//                } else
//                    if (node->array[j]=='{'&&node->array[j+1]=='{') {
//                        return node;
//                    }
//                    perfectthewordlist(node->array,2);
//                    printf("isture while %s\n",node->array);
//            }
//    if (node->next==NULL) { return NULL;}
////    result->slhead=node;
//    return node;
//}
//WordList* perfectthewordlist(char str[],int type){
//    WordList * word=(WordList *)malloc(sizeof(WordList));
//    memset(word,0,sizeof(word));
//    word->array=str;
//    word->type=Controll;
//    tailctl->next=word;
//    tailctl=tailctl->next;
//    Wlist[wlistlen++]=word;
//    printf("%s\n",word->array);
//    WordList * wd=headctl;
//    while (wd!=NULL){
//        printf("word: %s\n",wd->array);
//        wd=wd->next;
//    }
//    return headctl;
//}
