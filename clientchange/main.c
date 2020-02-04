#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <string.h>
//#include "cJSON.c"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define SA struct sockaddr
#define MAX 80
#define PORT 12345
typedef struct cJSON{
    char* type;
    char* content;
    struct cJSON* before;
    struct cJSON* next;
    char* noe;
    char* valuestring;
} cJSON;
char* cJSON_PrintUnformatted(cJSON* a);
cJSON* cJSON_CreateObject(){
    cJSON* a=calloc(1,sizeof(cJSON));
    a->noe=calloc(1000,sizeof(char));
    a->before=NULL;
    a->type=NULL;
    a->content=NULL;
    a->next=NULL;
    return a;
}
cJSON* cJSON_CreateString(char* str){
    cJSON* a=calloc(1,sizeof(cJSON));
    a->content=calloc(1000,sizeof(char));
    a->type=calloc(1000,sizeof(char));
    char* c;
    c[0]='"';
    c[1]='\0';
    strcpy(a->content,c);
    strcat(a->content,str);
    strcat(a->content,c);
    //strcpy(a->content,str);
    a->before=NULL;
    a->next=NULL;
    a->noe=calloc(1000,sizeof(char));
    strcpy(a->noe,"string");
    return a;
}
void cJSON_AddItemToObject(cJSON* a,char* str,cJSON* b){
     if(strcmp(b->noe,"string")==0){
            if(a->type==NULL && a->content==NULL){
                a->type=calloc(1000,sizeof(char));
                a->content=calloc(1000,sizeof(char));
                strcpy(a->type,str);
                strcpy(a->content,b->content);
            }
            else{
                cJSON* temp=a;
                while(temp->next!=NULL){
                    temp=temp->next;
                }
                b->before=temp;
                temp->next=b;
                b->type=calloc(1000,sizeof(char));
                strcpy(b->type,str);
            }
    }
    else if (strcmp(b->noe,"array")==0){
        char* c=cJSON_PrintUnformatted(b);
        if(a->type==NULL && a->content==NULL){
            a->type=calloc(1000,sizeof(char));
            a->content=calloc(1000,sizeof(char));
            strcpy(a->type,str);
            strcpy(a->content,c);
        }
        else{
            cJSON* temp=a;
            while(temp->next!=NULL){
                temp=temp->next;
            }
            b->before=temp;
            temp->next=b;
            b->type=calloc(1000,sizeof(char));
            strcpy(b->type,str);
            strcpy(b->content,c);
        }
    }
}
char* cJSON_PrintUnformatted(cJSON* a){
    char* finall=calloc(1000,sizeof(char));
    char c='"';
    if (strcmp(a->noe,"string")==0) {
            strcpy(finall,a->content);
    }
    else if(strcmp(a->noe,"array")==0) {
            cJSON* temp=a;
            finall[0]='[';
            char* d=",";
            while(temp->next!=NULL){
                strcat(finall,temp->content);
                strcat(finall,d);
                temp=temp->next;
            }
            finall[strlen(finall)-1]=']';
    }
    else
    {
        sprintf(finall,"{%c%s%c:%s,%c%s%c:%s}",c,a->type,c,a->content,c,(a->next)->type,c,(a->next)->content);
    }
    return finall;
}
cJSON* cJSON_CreateArray(){
    cJSON* a=calloc(1,sizeof(cJSON));
    a->type=calloc(1000,sizeof(char));
    a->content=calloc(1000,sizeof(char));
    a->noe=calloc(50,sizeof(char));
    a->before=NULL;
    strcpy(a->noe,"array");
    a->next=NULL;
    return a;
}
void cJSON_AddItemToArray(cJSON* array,cJSON* add1){
        char* add=cJSON_PrintUnformatted(add1);
        if (array->content[0]==0){
            strcpy(array->content,add);
            cJSON* b=calloc(1,sizeof(cJSON));
            b->type=calloc(1000,sizeof(char));
            b->content=calloc(1000,sizeof(char));
            array->next=b;
            b->before=array;
        }
        else{
            cJSON* temp=array;
            while(temp->next!=NULL){
                temp=temp->next;
            }
            strcpy(temp->content,add);
            cJSON* b=calloc(1,sizeof(cJSON));
            b->type=calloc(1000,sizeof(char));
            b->content=calloc(1000,sizeof(char));
            temp->next=b;
            b->before=temp;

    }
}
int  cJSON_GetArraySize(cJSON* a){
    cJSON* temp=a;
    int counter=0;
    while(temp->next!=NULL){
        counter++;
        temp=temp->next;
    }
    if(counter==0){
    if((a->valuestring)[1]=='{'){
        for(int i=0;i<strlen((a->valuestring));i++){
            if ((a->valuestring[i])=='{') counter++;
        }
    }
    else{
        for(int i=0;i<strlen((a->valuestring));i++){
            if ((a->valuestring[i])==',') counter++;
        }
        counter++;
    }
    }
    return counter;
}
cJSON* cJSON_Parse(char* s1){
        cJSON* a=calloc(1,sizeof(cJSON));
        cJSON* b=calloc(1,sizeof(cJSON));
        a->content=calloc(1000,sizeof(char));
        a->type=calloc(1000,sizeof(char));
        b->content=calloc(1000,sizeof(char));
        b->type=calloc(1000,sizeof(char));
        b->noe=calloc(1000,sizeof(char));
        a->next=b;
        b->before=a;
        b->next=NULL;
        a->before=NULL;
        char c;
        char s[1000];
        strcpy(s,s1);
        int counter1=0,counter3=0;
        int counter2=0,counter4=0;
        int f1=0,f2=0,f3=0,f4=0;
        int flag2=0,flag3=0;
        if(s[2]!='m'){
        for(int i=0;i<strlen(s);i++){
            if(s[i]==' ') counter3++;
            if (s[i]=='"') counter4++;
            if(counter4==7 && flag2==0){
                f3=i;
                flag2=1;
            }
            if(counter4==8 && flag3==0){
                f4=i;
                flag3=1;
            }
            if (s[i]==':' && counter1<2) {
                s[i]=' ';
                counter1++;
            }
            if (s[i]==',' && counter2<1){
                s[i]=' ';
                counter2++;
            }
            if(s[i]=='[') f1=i;
            if(s[i]==']') f2=i;
        }
        if(f1==0 && f2==0 && counter3==0){
                s[strlen(s)-1]='\0';
                sscanf(s,"%c%s %s %s %s",&c,a->type,a->content,(a->next)->type,(a->next)->content);
        }
        else if (f1==0 && f2==0 && counter3>0 && f3!=0 && f4!=0){
            int j=0;
            sscanf(s,"%c%s %s %s",&c,a->type,a->content,(a->next)->type);
            for(int i=f3;i<=f4;i++){
                    ((a->next)->content)[j]=s[i];
                    j++;
            }
            (a->next)->content[j]='\0';
        }
        else {
            int j=0;
            strcpy(b->noe,"array");
            sscanf(s,"%c%s %s %s",&c,a->type,a->content,(a->next)->type);
            for(int i=f1;i<=f2;i++){
                    ((a->next)->content)[j]=s[i];
                    j++;
            }
            ((a->next)->content)[j]='\0';
        }
        }
        if (s[2]=='m'){
            int t1,t2,t3,t4,t5,t6,t7,t8,counter1=0,flagg=0,counter2=0;
            for(int i=0;i<strlen(s);i++){
                if (s[i]=='"' && counter1<2){
                    counter1++;
                    if(counter1==1) t1=i;
                    if (counter1==2) t2=i;
                }
                if(s[i]=='[') t3=i;
                if(s[i]==']') {
                        t4=i;
                        flagg=1;
                }
                if(flagg==1 && s[i]=='"'){
                    counter2++;
                    if(counter2==1) t5=i;
                    if (counter2==2) t6=i;
                    if (counter2==3) t7=i;
                    if (counter2==4) t8=i;
                }
            }
            int j=0;
            for(int i=t7;i<=t8;i++){
                ((a->next)->content)[j]=s[i];
                j++;
            }
            ((a->next)->content)[j]='\0';
            j=0;
            for(int i=t1;i<=t2;i++){
                (a->type)[j]=s[i];
                j++;
            }
            (a->type)[j]='\0';
            j=0;
            for(int i=t3;i<=t4;i++){
                (a->content)[j]=s[i];
                j++;
            }
            (a->content)[j]='\0';
            j=0;
            for(int i=t5;i<=t6;i++){
                ((a->next)->type)[j]=s[i];
                j++;
            }
            ((a->next)->type)[j]='\0';
        }
        printf("%s\n%s\n%s\n%s\n",a->type,a->content,(a->next)->type,((a->next)->content));
        return a;
}
cJSON* cJSON_GetObjectItem(cJSON* a,char *s){
cJSON* b=calloc(1,sizeof(cJSON));
    b->content=calloc(1000,sizeof(char));
    b->type=calloc(1000,sizeof(char));
    b->noe=calloc(1000,sizeof(char));
    b->valuestring=calloc(1000,sizeof(char));
    char c[2];
    char s1[1000];
    c[0]='"';
    c[1]='\0';
    strcpy(s1,c);
    strcat(s1,s);
    strcat(s1,c);
    if(strcmp(a->type,s1)==0) {
        //printf("%s\n",a->content);
        if(a->content[0]='"'){
            int j=0;
            for(int i=1;i<strlen(a->content)-1;i++){
                (b->valuestring)[j]=(a->content)[i];
                j++;
            }
            (b->valuestring[j])='\0';
            //printf("%s\n",b->valuestring);
        }
        else
        strcpy(b->valuestring,a->content);
    }
    if(strcmp((a->next)->type,s1)==0) {
        if((a->next)->content[0]=='"'){
            int j=0;
            for(int i=1;i<strlen((a->next)->content)-1;i++){
                (b->valuestring)[j]=((a->next)->content)[i];
                j++;
            }
            (b->valuestring[j])='\0';
        }
        else
        strcpy(b->valuestring,(a->next)->content);
    }
    return b;
}
cJSON* cJSON_GetArrayItem(cJSON* a,int index){
    char s[1000],s2[1000];
    int counter1=(-1),j=0;
    strcpy(s,a->valuestring);
    if(s[1]=='{'){
        int flag2=0;
        for(int i=1;i<strlen(s)-1;i++){
            if (s[i-1]=='}' && s[i]==','){
                counter1++;
            }
            if(counter1==(index-1)){
                if(s[i]==',' && s[i-1]=='}' && s[i+1]=='{') continue;
                s2[j]=s[i];
                j++;
            }
        }
        s2[j]='\0';
        //printf("%s\n",s2);
        /* cJSON* b=calloc(1,sizeof(cJSON));
        b->content=calloc(1000,sizeof(char));
        b->type=calloc(1000,sizeof(char));
        b->noe=calloc(1000,sizeof(char));
        b->valuestring=calloc(1000,sizeof(char));
        b->next=NULL;
        b->before=NULL; */
        cJSON *b=cJSON_Parse(s2);
        //printf("baba\n");
        return b;
    }
    else{
        counter1=-1;
        for(int i=1;i<(strlen(s)-1);i++){
            if (s[i]==','){
                counter1++;
            }
            if(counter1==(index-1) && s[i]!=','){
                s2[j]=s[i];
                j++;
            }
        }
        s2[j]='\0';
        //printf("salam %s\n",s2);
        char s3[1000];
        if(s2[0]=='"'){
            int j=0;
            for(int i=1;i<strlen(s2)-1;i++){
                s3[j]=s2[i];
                j++;
            }
            (s3[j])='\0';
        }
        //printf("bye %s\n",s3);
        cJSON* b=calloc(1,sizeof(cJSON));
        b->content=calloc(1000,sizeof(char));
        b->type=calloc(1000,sizeof(char));
        b->noe=calloc(1000,sizeof(char));
        b->valuestring=calloc(1000,sizeof(char));
        b->next=NULL;
        b->before=NULL;
        strcpy(b->valuestring,s3);
        return b;
    }
}
int flag1=1;
char autotoken[100];
//char type1[1000],content[1000];
int  soket()
{
    int client_socket, server_socket;
    struct sockaddr_in servaddr, cli;
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    // Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Create and verify socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        //printf("Socket successfully created..\n");

        // Assign IP and port
        memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to server socket
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Connection to the server failed...\n");
        exit(0);
    }
    else
        return client_socket;
    //printf("Successfully connected to the server..\n");
    return 0;
    // Function for chat
    //chat(client_socket);
    // Close the socket
    //closesocket(client_socket);
}
void registerr()
{
    char user[500],pass[500],req[1000]="register ",respond[1000];
    //char* type1=(char*) malloc(10000*sizeof(char));
    //char* content1=(char*) malloc(10000*sizeof(char));
    char chert1;
    char type1[1000];
    char content1[1000];
    system("cls");
    printf("Enter Username:\n");
    scanf("%c",&chert1);
    gets(user);
    //printf("wsdd %s",user);
    printf("Enter Pasword:\n");
    //scanf("%c",&chert1);
    gets(pass);
    //req[] = "register ";
    strcat(req,user);
    strcat(req,", ");
    strcat(req,pass);
    strcat(req,"\n");
    vasl(req,respond,type1,content1);
    printf("----------%s\n",type1);
    printf("-----------%s\n",content1);
    if (*type1=='S')
    {
        system("cls");
        printf(ANSI_COLOR_GREEN  "user registered successfuly\n" ANSI_COLOR_RESET);
        printf("---------------\n");
        return;
    }
    else
    {
        system("cls");
        printf(ANSI_COLOR_RED  "%s\n" ANSI_COLOR_RESET,content1);
        printf("------------------\n");
        return;
    }
}
void vasl(char req[1000],char *respond,char type1[1000],char content1[1000])
{
    int mysocket=soket();
    send(mysocket,req,1000,0);
    memset(respond,0,1000);
    memset(type1,0,1000);
    memset(content1,0,1000);
    recv(mysocket,respond,1000,0);
    //printf("%s\n",respond);
    cJSON* tarjome= cJSON_Parse(respond);
    //printf("ok2");
    cJSON* type=cJSON_GetObjectItem(tarjome,"type");
    //printf("ok3");
    strcpy(type1,type->valuestring);
    //printf("%s\n",type1);
    //printf("ok4");
    cJSON* content=cJSON_GetObjectItem(tarjome,"content");
    //printf("ok5");
    strcpy(content1,content->valuestring);
    //printf("%s\n",type1);
    //printf("ok6");
}
void vaslrefresh(char req[1000],char respond[1000],char type1[1000])
{
    int mysocket=soket();
    char* sender1=(char*) malloc(500*sizeof(char));
    char* payam1=(char*) malloc(3000*sizeof(char));
    send(mysocket,req,1000,0);
    memset(respond,0,1000);
    recv(mysocket,respond,1000,0);
    printf("%s\n",respond);
    cJSON* tarjome= cJSON_Parse(respond);
    cJSON* type=cJSON_GetObjectItem(tarjome,"type");
    strcpy(type1,type->valuestring);
    cJSON* content=cJSON_GetObjectItem(tarjome,"content");
    if(*type1=='L'){
        /* cJSON* payam=NULL;
        cJSON_ArrayForEach(payam,content){

        } */
        int i;
        system("cls");
        for (i = 0 ; i < cJSON_GetArraySize(content) ; i++){
            cJSON * subitem = cJSON_GetArrayItem(content, i);
            cJSON* sender = cJSON_GetObjectItem(subitem, "sender");
            strcpy(sender1,sender->valuestring);
            cJSON* payam = cJSON_GetObjectItem(subitem, "content");
            strcpy(payam1,payam->valuestring);
            if (!strcmp(sender1,"server")) {
                printf(ANSI_COLOR_YELLOW "%s\n",payam1);
            }
            else{
                printf(ANSI_COLOR_YELLOW"%s :",sender1);
                printf(ANSI_COLOR_YELLOW "%s\n",payam1);
            }
        }
        free(sender1);
        free(payam1);
    }
    else {
            system("cls");
            printf(ANSI_COLOR_RED "Try again later\n" ANSI_COLOR_YELLOW);
            free(sender1);
            free(payam1);
    }
}
void vasl_channelmembers(char req[1000],char respond[1000],char type1[1000])
{
    int mysocket=soket();
   // char* members1=(char*) malloc(500*sizeof(char));
    //char* payam1=(char*) malloc(1000*sizeof(char));
    char members1[1000];
    send(mysocket,req,1000,0);
    memset(respond,0,1000);
    recv(mysocket,respond,1000,0);
    cJSON* tarjome= cJSON_Parse(respond);
    cJSON* type=cJSON_GetObjectItem(tarjome,"type");
    strcpy(type1,type->valuestring);
    cJSON* content=cJSON_GetObjectItem(tarjome,"content");
    if(*type1=='L')
    {
        int i;
        system("cls");
        printf("Channel memebers:\n");
        for (i = 0 ; i < cJSON_GetArraySize(content) ; i++)
        {
            cJSON * subitem = cJSON_GetArrayItem(content, i);
            //cJSON* sender = cJSON_GetObjectItem(subitem, "sender");
            strcpy(members1,subitem->valuestring);
            //cJSON* payam = cJSON_GetObjectItem(subitem, "content");
            //strcpy(payam1,payam->valuestring);
            printf(ANSI_COLOR_YELLOW "%s\n",members1);
        }
    }
    else
    {
        system("cls");
        printf(ANSI_COLOR_RED  "Try again later\n" ANSI_COLOR_RESET);
        return;
    }
}
int login()
{
    char user[500],pass[500],req[1000]="login ",respond[1000],type1[1000],content1[1000];
    char chert1;
    system("cls");
    printf("Enter Username\n");
    scanf("%c",&chert1);
    gets(user);
    printf("Enter Password\n");
    gets(pass);
    strcat(req,user);
    strcat(req,", ");
    strcat(req,pass);
    strcat(req,"\n");
    vasl(req,respond,type1,content1);
    if (*type1=='A')
    {
        system("cls");
        printf(ANSI_COLOR_GREEN  "user loged in successfuly\n" ANSI_COLOR_RESET);
        printf("---------------\n");
        strcpy(autotoken,content1);
        flag1=2;
        return;
    }
    else
    {
        system("cls");
        printf(ANSI_COLOR_RED  "%s\n" ANSI_COLOR_RESET,content1);
        printf("------------------\n");
        return;
    }
}
void menuasli()
{
    int s;
    while(1){
        printf("1: Create channel\n");
        printf("2: Join channel\n");
        printf("3: Logout\n");
        scanf("%d",&s);
        if (s==1)
        {
            createchannel();
        }
        if(s==2)
        {
            joinchannel();
        }
        if(s==3)
        {
            logout();
        }
    }
}
void createchannel()
{
    char channel[500],req[1000]="create channel ",respond[1000];
    //char* type1=(char*) malloc(10000*sizeof(char));
    //char* content1=(char*) malloc(10000*sizeof(char));
    char type1[1000],content1[1000];
    char chert1;
    system("cls");
    printf("Enter channel name:\n");
    scanf("%c",&chert1);
    gets(channel);
    //printf("wsdd %s",user);
    //scanf("%c",&chert1);
    //req[] = "register ";
    strcat(req,channel);
    strcat(req,", ");
    strcat(req,autotoken);
    strcat(req,"\n");
    vasl(req,respond,type1,content1);
    //printf("%s\n",type1);
    if (*type1=='S')
    {
        system("cls");
        printf(ANSI_COLOR_GREEN  "channel created successfuly\n" ANSI_COLOR_RESET);
        printf("---------------\n");
        flag1=3;
        return;
    }
    else
    {
        system("cls");
        printf(ANSI_COLOR_RED  "%s\n" ANSI_COLOR_RESET,content1);
        printf("------------------\n");
        return;
    }
}
void joinchannel()
{
    char channel[500],req[1000]="join channel ",respond[1000];
    //char* type1=(char*) malloc(10000*sizeof(char));
    //char* content1=(char*) malloc(10000*sizeof(char));
    char chert1;
    char type1[1000],content1[1000];
    system("cls");
    printf("Enter channel name:\n");
    scanf("%c",&chert1);
    gets(channel);
    //printf("wsdd %s",user);
    //scanf("%c",&chert1);
    //req[] = "register ";
    strcat(req,channel);
    strcat(req,", ");
    strcat(req,autotoken);
    strcat(req,"\n");
    //printf("okkk\n");
    vasl(req,respond,type1,content1);
    //printf("%s\n",type1);
    if (*type1=='S')
    {
        system("cls");
        printf(ANSI_COLOR_GREEN  "user joined successfuly\n" ANSI_COLOR_RESET);
        printf("---------------\n");
        flag1=3;
    }
    else
    {
        system("cls");
        printf(ANSI_COLOR_RED  "%s\n" ANSI_COLOR_RESET,content1);
        printf("------------------\n");
        flag1=2;
        return;
    }
}
void logout()
{
    char req[1000]="logout ",respond[1000];
    //char* type1=(char*) malloc(10000*sizeof(char));
    //char* content1=(char*) malloc(10000*sizeof(char));
    char type1[1000],content1[1000];
    strcat(req,autotoken);
    strcat(req,"\n");
    vasl(req,respond,type1,content1);
    //printf("%s\n",type1);
    if (*type1=='S')
    {
        system("cls");
        printf(ANSI_COLOR_GREEN  "user loged out successfuly\n" ANSI_COLOR_RESET);
        printf("---------------\n");
        flag1=1;
        return;
    }
    else
    {
        system("cls");
        printf(ANSI_COLOR_RED  "%s\n" ANSI_COLOR_RESET,content1);
        printf("------------------\n");
        flag1=2;
        return;
    }
}
void chatroom()
{
    int s;
    while(1)
    {
        printf("1: Send message\n");
        printf("2: Refresh\n");
        printf("3: Channel memebers\n");
        printf("4: Leave channel\n");
        printf("5: Find User\n");
        printf("6: Find word\n");
        scanf("%d",&s);
        if (s==1)
        {
            sendmessage1();
        }
        else if(s==2)
        {
            refresh();
        }
        else if(s==3)
        {
            channelmembers();
        }
        else if(s==4)
        {
            leavechannel();
        }
        else if(s==5)
        {
            find_user();
        }
        else if(s==6)
        {
            find_message();
        }
    }
}
void find_user()
{
    char message1[500],req[1000]="find user ",respond[1000];
    //char* type1=(char*) malloc(10000*sizeof(char));
    //char* content1=(char*) malloc(10000*sizeof(char));
    char type1[1000],content1[1000];
    char chert1;
    system("cls");
    printf("Type user that you want:\n");
    scanf("%c",&chert1);
    gets(message1);
    //printf("wsdd %s",user);
    //scanf("%c",&chert1);
    //req[] = "register ";
    strcat(req,message1);
    strcat(req,", ");
    strcat(req,autotoken);
    strcat(req,"\n");
    vasl(req,respond,type1,content1);
    //printf("%s\n",type1);
    if (*type1=='S')
    {
        printf(ANSI_COLOR_GREEN  "user %s found.\n" ANSI_COLOR_RESET,message1);
        printf("Press enter to Continue\n");
        getchar();
        system("cls");
        flag1=3;
        return;
    }
    else
    {
        printf(ANSI_COLOR_RED  "user %s not found.\n" ANSI_COLOR_RESET,message1);
        printf("Press enter to Continue\n");
        getchar();
        system("cls");
        flag1=3;
        return;
    }
}
void sendmessage1()
{
    char message1[500],req[1000]="send ",respond[1000];
    //char* type1=(char*) malloc(10000*sizeof(char));
    //char* content1=(char*) malloc(10000*sizeof(char));
    char type1[1000],content1[1000];
    char chert1;
    system("cls");
    printf("Type your meesage:\n");
    scanf("%c",&chert1);
    gets(message1);
    //printf("wsdd %s",user);
    //scanf("%c",&chert1);
    //req[] = "register ";
    strcat(req,message1);
    strcat(req,", ");
    strcat(req,autotoken);
    strcat(req,"\n");
    vasl(req,respond,type1,content1);
    //printf("%s\n",type1);
    if (*type1=='S')
    {
        system("cls");
        printf(ANSI_COLOR_GREEN  "message sent successfuly\n" ANSI_COLOR_RESET);
        printf("---------------\n");
        flag1=3;
        return;
    }
    else
    {
        system("cls");
        printf(ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET,content1);
        printf("------------------\n");
        flag1=3;
        return;
    }
}
void find_message(){
    char message1[1000],req[1000]="find message ",respond[1000];
    //char* type1=(char*) malloc(10000*sizeof(char));
    //char* content1=(char*) malloc(10000*sizeof(char));
    char type1[1000],content1[1000];
    char chert1;
    system("cls");
    printf("Type word that you want:\n");
    scanf("%c",&chert1);
    gets(message1);
    //printf("wsdd %s",user);
    //scanf("%c",&chert1);
    //req[] = "register ";
    strcat(req,message1);
    strcat(req,", ");
    strcat(req,autotoken);
    strcat(req,"\n");
    vasl(req,respond,type1,content1);
    //printf("%s\n",type1);
    if (*type1=='S')
    {
        printf(ANSI_COLOR_GREEN "message found.\n" ANSI_COLOR_RESET);
        printf("%s\n",content1);
        printf("Press enter to Continue\n");
        getchar();
        system("cls");
        flag1=3;
        return;
    }
    else
    {
        printf(ANSI_COLOR_RED "message not found.\n" ANSI_COLOR_RESET);
        printf("Press enter to Continue\n");
        getchar();
        system("cls");
        flag1=3;
        return;
    }
}
void refresh()
{
    char req[1000]="refresh ",respond[1000];
    //char* type1=(char*) malloc(1000*sizeof(char));
    char type1[1000];
    char chert1;
    strcat(req,autotoken);
    strcat(req,"\n");
    vaslrefresh(req,respond,type1);
    printf(ANSI_COLOR_RESET "Press enter to Continue\n");
    getchar();
    getchar();
    system("cls");
}
void channelmembers()
{
    char message1[500],req[1000]="channel members ",respond[1000];
    //char* type1=(char*) malloc(10000*sizeof(char));
    //char* content1=(char*) malloc(10000*sizeof(char));
    char type1[1000],content1[1000];
    strcat(req,autotoken);
    strcat(req,"\n");
    vasl_channelmembers(req,respond,type1);
    printf(ANSI_COLOR_RESET "Press enter to Continue\n");
    getchar();
    getchar();
    system("cls");
    flag1=3;
    return;
}
void leavechannel()
{
    char req[1000]="leave ",respond[1000];
    //char* type1=(char*) malloc(10000*sizeof(char));
    //char* content1=(char*) malloc(10000*sizeof(char));
    char type1[1000],content1[1000];
    strcat(req,autotoken);
    strcat(req,"\n");
    vasl(req,respond,type1,content1);
    if (*type1=='S')
    {
        system("cls");
        printf(ANSI_COLOR_GREEN  "user left successfuly\n" ANSI_COLOR_RESET);
        printf("---------------\n");
        flag1=2;
        return;
    }
    else
    {
        system("cls");
        printf(ANSI_COLOR_RED "%s\n ANSI_COLOR_RESET",content1);
        printf("------------------\n");
        flag1=3;
        return;
    }
}
int main()
{
    int n,s;
    while(1)
    {
        if(flag1==1){
            printf("Acount menu\n");
            printf("1: Register\n");
            printf("2: Login\n");
            printf("3: close app\n");
            scanf("%d",&n);
            if (n==1)
            {
                registerr();
                continue;
            }
            if (n==2)
            {
                login();
                continue;
            }
            if(n==3)
            {
                int mysocket=soket();
                send(mysocket,"exit",1000,0);
                system("cls");
                printf(ANSI_COLOR_BLUE  "good bye :)" ANSI_COLOR_RESET);
                exit(0);
            }
        }
         else if(flag1==2){
            printf("1: Create channel\n");
            printf("2: Join channel\n");
            printf("3: Logout\n");
            scanf("%d",&s);
            if (s==1)
            {
                createchannel();
                continue;
            }
            if(s==2)
            {
                joinchannel();
                continue;
            }
            if(s==3)
            {
                logout();
                continue;
            }
        }
        else if(flag1==3){
            printf("1: Send message\n");
            printf("2: Refresh\n");
            printf("3: Channel memebers\n");
            printf("4: Leave channel\n");
            printf("5: Find User\n");
            printf("6: Find word\n");
            scanf("%d",&s);
            if (s==1)
            {
                sendmessage1();
                continue;
            }
            else if(s==2)
            {
                refresh();
                continue;
            }
            else if(s==3)
            {
                channelmembers();
                continue;
            }
            else if(s==4)
            {
                leavechannel();
                continue;
            }
            else if(s==5)
            {
                find_user();
                continue;
            }
            else if(s==6)
            {
                find_message();
                continue;
            }
        }
    }
}
