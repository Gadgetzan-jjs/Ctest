

#include "stdlib.h"
#include "stdio.h"
#include "strings.h"
#include "string.h"
#include <pthread.h>
#include "MyAnalysisNew.h"
#define Controller 1;
#define Context 2;
#define Define 3;
#define Val 4;
#define Operator 5;
#define Varibale 6;
#define Fin 13;
#define Operator_Equal 501
#define Interger 100
#define String 101
//#define Interger[] 102
typedef struct StringList{
    char * array;
    struct StringList *next;
}Node;
typedef struct ResultList{
    const char * constarray;
    void * array;
    struct ResultList *next;
    int valuetype;
};
typedef struct WordList{
    char * array;
    struct WordList *next;
    struct WordList *left;
    struct WordList * right;
    int type;
    char * msg;
    int bit;
}Word;
typedef struct Grammar{
    char * array;
    char * key;
    const char * constarray;
    void * value;
    struct Grammar *left;
    struct Grammar *right;
    int type;
    int bit;
    int valuetype;
    int operatortype;
};
typedef struct preAttribute{
    char * key;
    void * value;
};
preAttribute * preAttributes[20];
int indexpAttr=0;



Grammar * vareax[20];
char * operatoreax[10];
void * eax[20];
Grammar * roots[10];
WordList * Wlist[20];
StringList * List[20];

int varindex=0;
int eaxindex=0;
int operatoreaxindex=0;
int trees=0;
int listlen=0;
int wlistlen=0;
int numofpart=0;
int operatortype=0;
int istrue=0;
int ifbodyindex=0;

StringList * headsl=(StringList *)malloc(sizeof(StringList));
WordList * headctl=(WordList *)malloc(sizeof(WordList));
ResultList * replacelisthead=(ResultList *)malloc(sizeof (ResultList));
ResultList * replacelisttail=replacelisthead;

void bodydeal(FILE * fp);
void docon(WordList * segment);
void doval(WordList * segment);
void docontext(WordList * segement);
void createoperatertree(Grammar * tree);

char * thisfilepath=(char *)malloc(128);
char * thisuri=(char *)malloc(32);
char * thispath=(char *)malloc(96);
char temphtml[11]="Temp.html\0";
char * ifbody[10];

const char * ptrarr;
/**
 *  将解析之后的结果重新写在当前静态资源当中
 *
 *  {@param array}执行结果的链表
 *
 * */
void replacehtml(ResultList * array){
    FILE *fpSrc=NULL;
    FILE *fpDst=NULL;
    if ((fpSrc=fopen(thisfilepath,"r"))==NULL){
        perror("file open fail");
        exit(1);
    }
    int len=strlen(thispath);
    char temp[128];
    int i=0;
    for (int j=0; i < len+10; ++i) {
        if (i>len){
            temp[i-1]=temphtml[j++];
            continue;
        }
        temp[i]=thispath[i];
    }
    temp[i-1]='\0';
    printf("%s\n",temp);
    if ((fpDst=fopen(temp,"w"))==NULL)
    {
        perror("error!!!");
        exit(0);
    }
    char context[101];
    int atbody=0;
    while (fgets(context,sizeof(context),fpSrc)){
        if(!strcmp(context,"{{jinja2}}\n")){
            continue;
        }
        if (!strcmp(context,"<body>\n")){
            fputs(context,fpDst);
            while (array->array!=NULL||array->constarray!=NULL) {
                if (array->valuetype==100){
                    fprintf(fpDst,"%d",*((int *)(array->array)));
                }else {
                    if (array->array==NULL){
                        fputs(array->constarray,fpDst);
                        fputs("\n",fpDst);
                        array=array->next;
                        continue;
                    }
                    fputs((char *) array->array, fpDst);
                    fputs("\n",fpDst);
                }
                array=array->next;
            }
            atbody=1;
            continue;
        }
        if (atbody){
            if (!strcmp(context,"</body>\n")){
                fputs("\n",fpDst);
                fputs(context,fpDst);
                atbody=0;
                continue;
            }
            continue;
        }
        fputs(context,fpDst);
    }
    if (fpSrc!=NULL)
    {
        fclose(fpSrc);
    }
    if (fpDst!=NULL)
    {
        fclose(fpDst);
    }
    rename(temp,thisfilepath);
    return;
}
/**
 *  替换定义的变量。
 *
 *  {@param key}变量名
 *  {@param value}变量值
 *  1 int
 *  2 char *
 *  3 int[]
 *
 *
 *
 * */
void setAttribute(char key[],void * target){
    if(indexpAttr>20){
        return;
    }
    preAttribute * pre=(preAttribute *) malloc(sizeof(preAttribute));
    pre->key=key;
    pre->value=((Attribute *)target)->context;
    preAttributes[indexpAttr++]=pre;
    printf("setAAAA\n");
    printf("%s\n",(pre->value));

}
int nowtreetype=0;
int ifnum=0;
/**
 *  执行语法树。
 *
 *  {@param t}语法树的根节点
 * */
void InOrder_traverse(Grammar *t)
{
    if (t) {
        printf("ptrarr is %s\n",ptrarr);
        InOrder_traverse(t->left);

        printf("inorder %s\n",t->array);
        switch (t->type) {
            case 1:
                if (!strcmp(t->array, "if")) {
                    ifnum=1;
                    switch (operatortype) {
                        case 501://等于号
                            if (!strcmp((char *)eax[0], (char *)eax[1])) {
                                istrue++;
                            }
                            break;
                    }
                }break;
            case 2:
                replacelisttail->valuetype=String;
                replacelisttail->constarray=t->constarray;
                printf("replace list tail %s\n",replacelisttail->constarray);
                replacelisttail->next=(ResultList *) malloc(sizeof(ResultList));
                replacelisttail=replacelisttail->next;
                if (!ifnum)
                istrue++;
                break;
            case 4:
                eax[eaxindex++] = t->array;
                if (nowtreetype==4)
                istrue++;
                break;
            case 5:
                operatoreax[operatoreaxindex] = t->array;
                if (!strcmp(operatoreax[operatoreaxindex++], "==")) {
                    operatortype = Operator_Equal;
                }
                break;
            case 6:
                if (t->value!=NULL&&t->bit==200) {
                    if (t->valuetype == 100) {
                        replacelisttail->valuetype=Interger;
                    }else if (t->valuetype=101){
                        replacelisttail->valuetype=String;
                    }
                    replacelisttail->array =t->value;
                    replacelisttail->next = (ResultList *) malloc(sizeof(ResultList));
                    replacelisttail = replacelisttail->next;
                }
                istrue++;
                break;
        }

        InOrder_traverse(t->right);
        if (istrue==numofpart) {
            printf("before replace %s\n",t->array);
            replacelisttail->array=t->array;
            replacelisttail->next=(ResultList *)malloc(sizeof (ResultList));
            replacelisttail=replacelisttail->next;
        }
    }
}
/**
 *  打开要访问的静态资源文件,并读取配置文件，当读取到<body>的内容之后
 *  调用{@method bodydeal(fp)}函数处理
 *
 *  {@param filepath}静态资源的路径
 *  {@param uri}用户请求的uri路径
 *  {@param path}用户通过配置文件配置的静态资源所在路径
 */
void open(char * filepath,char * uri,char * path) {
    thisuri = uri;
    thisfilepath = filepath;
    thispath = path;
}
/**
 *  回收malloc之后的资源
 *
 * */
void *freeAll(void *args) {
        printf("ready to free the lists\n");
        for (int i = 0; i < 20; ++i) {
            if (vareax[i] != NULL)
                free(vareax[i]);
        }
        printf("vareax is free done\n");

        for (int i = 0; i < 20; ++i) {
            if (Wlist[i] != NULL)
                free(Wlist[i]);
        }
        printf("Wlist is free done\n");

        for (int i = 0; i < 20; ++i) {
            if (List[i] != NULL)
                free(List[i]);
        }

        printf("roots is free done\n");
        printf("List is free done\n");
        printf("all free is done\n");
}
/**
 *  执行解析页面的工作。
 *
 * */
void close() {
        FILE *fp;
        char context[101];
        if ((fp = fopen(thisfilepath, "r")) == NULL) {
            perror("file open fail");
            exit(1);
        }
        int jinjaexist = 0;
        while (fgets(context, sizeof(context), fp)) {
            if (!strcmp(context, "{{jinja2}}\n")) {
                jinjaexist = 1;
                break;
            }
        }
        printf("open file success \n");
        if (jinjaexist) {
            printf("use jinja2\n");
            while (fgets(context, sizeof(context), fp)) {
                if (!strcmp(context, "<body>\n")) {
                    bodydeal(fp);
                    printf("body 执行完毕\n");
                    break;
                }
            }
            printf("the trees is %d\n",trees);
            for (int i = 0; i < 10; ++i) {
                if (roots[i] != NULL) {
                    eaxindex = 0;
                    nowtreetype=roots[i]->type;
                    InOrder_traverse(roots[i]);
                    ifnum=0;
                }
            }
            replacehtml(replacelisthead);
            pthread_t tid;
            int iRet = pthread_create(&tid, NULL, freeAll, NULL);
            if (iRet) {
                printf("error: iRet=%d\n", iRet);
                return;
            }
        } else {
            printf("unuse jinja2\n");
            return;
        };
    }
/**
 * 处理<body>里的内容，将符合jinja语法的内容进行解析
 *
 * {@param fp}读取静态资源的函数指针
 *
 * */
void bodydeal(FILE *fp) {
        char context[101];
        char jinja[1025];
        char pln[] = "\n";
        bzero(jinja, 1025);
        while (fgets(context, sizeof(context), fp)) {
            if (!strcmp(context, "</body>\n")) break;
            strcat(jinja, context);
        }
        char *line = NULL;
        line = strtok(jinja, pln);
        char *lines[100];
        StringList *tail = headsl;
        bzero(lines, 100);
        while (line != NULL) {
            StringList *node = (StringList *) malloc(sizeof(StringList));
            tail->next = node;
            node->array = line;
            tail = node;
            List[listlen++] = node;
            line = strtok(NULL, pln);
        }
        tail = headsl->next;
        WordList *tailwd = headctl;
        char pspace[] = " ";
        char *word = NULL;
        while (tail != NULL) {
            word = strtok(tail->array, pspace);
            while (tail != NULL && word != NULL) {
                WordList *wordnode = (WordList *) malloc(sizeof(WordList));
                tailwd->next = wordnode;
                wordnode->array = word;
                tailwd = wordnode;
                Wlist[wlistlen++] = wordnode;
                word = strtok(NULL, pspace);
            }
            tail = tail->next;
            WordList *pln = (WordList *) malloc(sizeof(WordList));
            tailwd->next = pln;
            pln->array = "nextLine";
            tailwd = pln;
            tailwd->msg = NULL;
            Wlist[wlistlen++] = pln;
        }
        tailwd->bit = 133;
        tailwd = headctl->next;
        int typeint = 0;
    char * contexttemp=(char *)malloc(8);//当执行到html语法时，获取该语法的起括号的关键字
    int Duplicatenum=0;
    int isendofHTML=1;
    while (tailwd != NULL) {
            if (tailwd->msg != NULL) {
                tailwd->msg = NULL;
            }
            if (tailwd->bit != NULL) {
                if (tailwd->bit != 133||typeint!=0) {
                    tailwd->bit = NULL;
                } else {
                    return;
                }
            }
       if (typeint) {
           if (tailwd->next==NULL||tailwd->next->next == NULL||tailwd->next->next->next==NULL) {
               numofpart++;
               tailwd->msg = "part";
           } else {
               switch (typeint) {
                   case 1:
                       if (tailwd->next->next->next->array[0] == 'e' && tailwd->next->next->next->array[1] == 'n' &&
                           tailwd->next->next->next->array[2] == 'd') {
                            numofpart++;
                            tailwd->msg="part";
                       }break;
                   case 2:
                       if (!strcmp(tailwd->array,contexttemp)){
                           Duplicatenum++;
                       }
                       if (tailwd->array[1]=='/'){
                           int contexttemplen=strlen(contexttemp);
                           for (int i = 1; i < contexttemplen; ++i) {
                               if (contexttemp[i]!=tailwd->array[i+1]){
                                   isendofHTML=0;
                                   break;
                               }
                           }
                       }
                       if (isendofHTML){
                           Duplicatenum--;
                           if (Duplicatenum<=0) {
                               numofpart++;
                               tailwd->msg = "part";
                           }
                       }
                       break;
                   case 3:
                       if (!strcmp(tailwd->next->array,"nextLine")){
                           numofpart++;
                        tailwd->msg="part";
                       }
                       break;
                   case 4:
                       if (!strcmp(tailwd->next->array,"nextLine")){
                           numofpart++;
                           tailwd->msg="part";
                       }
                       break;
               }
           }
       }
       isendofHTML=1;
       Duplicatenum=0;
//            if (typeint) {
//                if (tailwd->next->next == NULL) {
//                    numofpart++;
//                    tailwd->msg = "part";
//                } else {
//                    if (tailwd->next->next->array != NULL) {
//                        if (!strcmp(tailwd->next->next->array, "{%")) {
//                            numofpart++;
//                            tailwd->msg = "part";
//                        }
//                    }
//                }
//            }
            if (!typeint) {
                if (!strcmp(tailwd->array, "nextLine")) {
                    tailwd = tailwd->next;
                    continue;
                }
                if (!strcmp(tailwd->array, "{%")) {
                    typeint = Controller;
                    tailwd = tailwd->next;
                    headctl = tailwd;
                    continue;
                } else if (!strcmp(tailwd->array, "{{")) {
                    typeint = Val;
                    tailwd = tailwd->next;
                    continue;
                } else if (!strcmp(tailwd->array, "{#")) {
                    typeint = Define;
                    tailwd = tailwd->next;
                    continue;
                } else if (tailwd->array[0]=='<'){
                    contexttemp=tailwd->array;
                    printf("context is is is **** %s\n",tailwd->array);
                    typeint = Context;
                    tailwd = tailwd->next;
                    continue;
                }
            } else {
                if (tailwd->msg != NULL && !strcmp(tailwd->msg, "part")) {
                    tailwd->type = Fin;
                    switch (typeint) {
                        case 1:
                            docon(headctl);
                            tailwd=tailwd->next->next->next->next->next->next;
                            break;
                        case 2:
                            docontext(headctl);
                            tailwd = tailwd->next;
                            break;
                        case 3:
                            break;
                        case 4:
                            doval(headctl);
                            tailwd = tailwd->next;
                            break;
                    }
                    typeint = 0;
                    headctl = tailwd;
                    continue;
                }
                if (!strcmp(tailwd->array, "==")) {
                    tailwd->type = Operator;
                    tailwd = tailwd->next;
                    continue;
                }
            }
            tailwd = tailwd->next;
        }
    }
/**
*  处理controller结构的函数。
*
*  {@param segment}controller部分的链表
*
* */
void docon(WordList * segment){
    int isbody=0;
    char arr[41];
    int arrindex=0;
    int isbreak=0;
    eaxindex=0;
    if (!strcmp(segment->array,"if")){
        Grammar * tree=(Grammar *)malloc(sizeof(Grammar));
        tree->array=segment->array;
        tree->type=Controller;
        segment=segment->next;
        roots[trees++]=tree;
        while (1){
            if (isbody){
                int i = 0;
                int len=strlen(segment->array);
                for (;i<len;++i) {
                    arr[arrindex++]=segment->array[i];
                }
                if (isbreak!=1){
                    arr[arrindex++]=' ';
                }
                segment=segment->next;
                if (isbreak){
                    arr[arrindex++]='\0';
                    break;
                }
                if (segment->type==13){
                    isbreak=1;
                }
                continue;
            }
            if (!strcmp(segment->array,"}}")){
                segment=segment->next;
                continue;
            }
            if (segment->type==5) {
                operatoreax[operatoreaxindex++]=segment->array;
                segment=segment->next;
                continue;
            }
            if (!strcmp(segment->array,"nextLine")){
                isbody=1;
                segment=segment->next;
                continue;
            }
            if (segment->array[0]==':'){
                for (int i = 0; i < 20; ++i) {
                    if (!strcmp(segment->array,vareax[i]->key)){
                        if (vareax[i]->value==NULL){
                            break;
                        }
                        eax[eaxindex++]=(char *)(vareax[i]->value);
                        printf("the value %s\n",(char *)vareax[i]->value);
                        segment=segment->next;
                        break;
                    }
                }
                continue;
            }
            eax[eaxindex++]=segment->array;
            segment=segment->next;
            if (isbreak){
                break;
            }
            if (segment->type==13){
                isbreak=1;
            }
        }
        Grammar * treerightchild=(Grammar *)malloc(sizeof(Grammar));
        printf("the arr is %s\n",arr);
        printf("the place of arr %0xl \n",*arr);
        ptrarr=arr;
//        ifbody[ifbodyindex]=ptrarr;
        treerightchild->constarray=ptrarr;
        treerightchild->type=2;
        printf("the right child %s\n",treerightchild->constarray);
        tree->right=treerightchild;
        tree->right->bit=202;
        printf("******************\n");
        printf("the tree tree right child %s\n",tree->right->constarray);
        if (!strcmp(operatoreax[0],"==")){
            printf("eax 0 %s\n",eax[0]);
            printf("eax 1 %s\n",eax[1]);
            if (!strcmp((char *)eax[0],(char *)eax[1])){
                createoperatertree(tree);
            }else {
                return;
            }
        }
    } else if (!strcmp(segment->array,"for")){

    }
//    arr[arrindex]='\0';
}
/**
 *  处理定义变量部分的函数。
 *
 *  {@param segment}变量所在链表
 * */
void doval(WordList * segment){
    int num=0;
    Grammar *variable=(Grammar *) malloc(sizeof (Grammar));
    Grammar * tail=NULL;
    while (segment->array==NULL)segment=segment->next;
    while (strcmp(segment->array,"nextLine")){
        if (segment->msg!=NULL&&!strcmp(segment->msg,"part"))break;
        if (segment->array[0]==':'){//处理定义的变量
            if (num==0){
             printf("%s\n",segment->array);
             variable->key=segment->array;
             variable->type=6;
             vareax[varindex++]=variable;
             num++;
             tail=variable;
             segment=segment->next;
             continue;
            }
            Grammar * var=(Grammar *) malloc(sizeof (Grammar));
            var->key=segment->array;
            var->type=6;
            tail->left=var;
            tail->right=NULL;
            tail=var;
            vareax[varindex++]=var;
        }
        segment=segment->next;
    }
    if (indexpAttr!=0) {
        for (int i = 0; i < varindex; ++i) {
            for (int j = 0; j < indexpAttr; ++j) {
                if (!strcmp(vareax[i]->key,preAttributes[j]->key)){
                    vareax[i]->value=preAttributes[j]->value;
                    vareax[i]->bit=200;
                }
            }
        }
    }
    roots[trees++]=variable;
    printf("vareax len %d\n",varindex);
    printf("var0 key%s\n",vareax[0]->key);
}
/**
 *  处理html原文的函数。
 *
 *  {@param segment}html原文所在的部分链表
 * */
void docontext(WordList * segement){
    int isfrist=0;
    Grammar * contextnode = (Grammar *) malloc(sizeof(Grammar));
    Grammar * ptr;
    while (1) {
        if (segement->msg != NULL && !strcmp(segement->msg, "part")) {
            roots[trees++]=contextnode;
            return;
        }
        if (!isfrist) {
            contextnode->constarray = segement->array;
            contextnode->type = Context;
            ptr=contextnode;
            segement=segement->next;
            isfrist=1;
            continue;
        }
        Grammar * node=(Grammar *)malloc(sizeof(Grammar));
        node->constarray=segement->array;
        node->type=Context;
        ptr->left=node;
        ptr=node;
        segement=segement->next;
    }
}
/**
 *  创建操作数的子树。
 *
 *  {@param tree}操作数的紫薯
 *
 * */
void createoperatertree(Grammar * tree){
    Grammar * operatornode=(Grammar *)malloc(sizeof(Grammar));
    operatornode->array=operatoreax[0];
    operatornode->type=Operator;
    tree->left=operatornode;
    Grammar * leftnode=(Grammar *)malloc(sizeof(Grammar));
    leftnode->array=(char *)eax[0];
    leftnode->type=Val;
    leftnode->left=NULL;
    leftnode->right=NULL;
    operatornode->left=leftnode;
    Grammar * rightnode=(Grammar *)malloc(sizeof(Grammar));
    rightnode->array=(char *)eax[1];
    rightnode->type=Val;
    rightnode->left=NULL;
    rightnode->right=NULL;
    operatornode->right=rightnode;
    bzero(operatoreax,10);
    bzero(eax,10);
}