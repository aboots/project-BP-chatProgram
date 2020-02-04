#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>
//#include "cJSON.c"
#include <conio.h>
#include <dir.h>
#include <process.h>
#include <dirent.h>
#include <errno.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
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
        //printf("%s\n%s\n%s\n%s\n",a->type,a->content,(a->next)->type,((a->next)->content));
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
typedef struct users{
    char name[100];
    char pass[100];
    char token[32];
    bool loginn;
    int refreshh;
    char channel_name[100];
} user;
user users[100];
int counter_user=0;
int global_flag=0;
int soket(int server_socket){
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    struct sockaddr_in server, client;
    int client_socket;
     //server_socket = socket(AF_INET, SOCK_STREAM, 6);
    // Accept the data packet from client and verify
    int len = sizeof(client);
    client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        exit(0);
    }
    else {
        printf("Server acccepted the client..\n");
        return client_socket;
    }
    // Function for chatting between client and server
    //chat(client_socket);
    // Close the socket
  //  closesocket(server_socket);
}
void load_users(){
    struct dirent *dp;
    char path[100]="resources\\users";
    DIR *dir = opendir("resources\\users");
    char info[500],user2[100],pass2[100];
    // Unable to open directory stream
    if (!dir)
        return;
    while ((dp = readdir(dir)) != NULL)
    {
        if (!strcmp (dp->d_name, "."))
            continue;
        if (!strcmp (dp->d_name, ".."))
            continue;
        strcpy(path,"resources\\users\\");
        strcat(path,dp->d_name);
        FILE* fptr1=fopen(path,"r");
        fscanf(fptr1,"%s",info);
        //printf("%s\n",info);
        cJSON* tarjome= cJSON_Parse(info);
        //printf("%s\n%s\n",tarjome->type,tarjome->content);
        cJSON* type=cJSON_GetObjectItem(tarjome,"username");
        strcpy(users[counter_user].name,type->valuestring);
        cJSON* content=cJSON_GetObjectItem(tarjome,"password");
        strcpy(users[counter_user].pass,content->valuestring);
        counter_user++;
       // printf("%s\n", dp->d_name);
    }
    // Close directory stream
    closedir(dir);
}
void request(int server_socket){
    int client_socket;
    client_socket = soket(server_socket);
    char req[1000],part1[100],part2[100],part3[100],part4[100];
    char * out,*finall;
    cJSON *reqq , *type , *content;
    memset(req, 0, 1000);
    recv(client_socket,req, 1000, 0);
    printf("%s\n",req);
    if (strncmp("register", req, 8) == 0){
            int counter=0;
            for(int i=0;i<strlen(req);i++){
                if (req[i]==',') req[i]=' ';
                else if (req[i]==' ') counter++;
                if (counter>2){
                    sendd(client_socket,"Error","invalid user name");
                    return 0;
                }
            }
            sscanf(req,"%s %s  %s",part1,part2,part3);
            //printf("%s\n",part1);
            //printf("%s\n",part2);
            //printf("%s\n",part3);
            int flag1=find_user(part2);
            //printf("%d",flag1);
            if (flag1==0){
                    sendd(client_socket,"Error","User already exists.");
                    return 0;
            }
            registerr(client_socket,part2,part3);
        }
     else if (strncmp("login", req, 5) == 0){
            int counter=0;
            for(int i=0;i<strlen(req);i++){
                if (req[i]==',') req[i]=' ';
                else if (req[i]==' ') counter++;
                if (counter>2){
                    sendd(client_socket,"Error","invalid command");
                    return 0;
                }
            }
            sscanf(req,"%s %s  %s",part1,part2,part3);
            int flag1=find_user(part2);
            if (flag1==1){
                    sendd(client_socket,"Error","user not found");
                    return 0;
            }
            int i=find_user_in_struct(part2);
            if (users[i].loginn==true){
                    sendd(client_socket,"Error","user already logged in.");
                    return 0;
            }
            int flag2=check_pass(part2,part3);
            if(flag2==0){
                sendd(client_socket,"Error","wrong password");
                return 0;
            }
            if (i>=0) create_token(users[i].token);
            //printf("%s\n",users[i].token);
            if (i>=0) users[i].loginn=true;
            sendd(client_socket,"AuthToken",users[i].token);
        }
     else if (strncmp("create channel", req, 14) == 0)
        {
            int counter=0;
            for(int i=0;i<strlen(req);i++){
                if (req[i]==',') req[i]=' ';
                else if (req[i]==' ') counter++;
                if (counter>3){
                    sendd(client_socket,"Error","invalid command");
                    return 0;
                }
            }
            sscanf(req,"%s %s %s  %s",part1,part2,part3,part4);
            int i=find_token(part4);
            if (i==(-1)){
                    sendd(client_socket,"Error","token not found.");
                    return 0;
            }
            int flag2=find_channel(part3);
            if (flag2==0){
                    sendd(client_socket,"Error","channel already exists.");
                    return 0;
            }
            create_channel(i,client_socket,part3);
        }
        else if (strncmp("join channel", req, 12) == 0)
        {
            int counter=0;
            for(int i=0;i<strlen(req);i++){
                if (req[i]==',') req[i]=' ';
                else if (req[i]==' ') counter++;
                if (counter>3){
                    sendd(client_socket,"Error","invalid command");
                    return 0;
                }
            }
            sscanf(req,"%s %s %s  %s",part1,part2,part3,part4);
            int i=find_token(part4);
            //printf("%s\n",part4);
            if (i==(-1)){
                    sendd(client_socket,"Error","token not found.");
                    return 0;
            }
            int flag2=find_channel(part3);
            if (flag2==1){
                    sendd(client_socket,"Error","channel not found.");
                    return 0;
            }
            join_channel(i,client_socket,part3);
        }
        else if (strncmp("send", req, 4) == 0)
        {
            int counter=0,endd;
            for(int i=0;i<strlen(req);i++){
                if (req[i]==',') {
                        req[i]=' ';
                        endd=i;
                }
            }
            int j=0;
            for(int i=5;i<endd;i++){
                    part2[j]=req[i];
                    j++;
            }
            part2[j]='\0';
            j=0;
            for(int i=endd+2;i<endd+33;i++){
                part4[j]=req[i];
                j++;
            }
            part4[31]='\0';
           // printf("%s\n",part4);
            int i=find_token(part4);
            if (i==(-1)){
                    sendd(client_socket,"Error","token not found.");
                    return 0;
            }
            if (users[i].channel_name[0]==0){
                sendd(client_socket,"Error","you didn't join any channel");
                return 0;
            }
            send_message(i,client_socket,part2);
        }
        else if (strncmp("refresh", req, 7) == 0)
        {
            sscanf(req,"%s %s",part1,part2);
            int i=find_token(part2);
            if (i==(-1)){
                    sendd(client_socket,"Error","token not found.");
                    return 0;
            }
            if (users[i].channel_name[0]==0){
                sendd(client_socket,"Error","you didn't join any channel");
                return 0;
            }
            refresh(i,client_socket);
        }
        else if (strncmp("channel members", req, 15) == 0)
        {
            sscanf(req,"%s %s %s",part1,part2,part3);
            int i=find_token(part3);
            if (i==(-1)){
                    sendd(client_socket,"Error","token not found.");
                    return 0;
            }
            if (users[i].channel_name[0]==0){
                sendd(client_socket,"Error","you didn't join any channel");
                return 0;
            }
            channel_members(i,client_socket);
        }
        else if (strncmp("leave", req, 5) == 0)
        {
            sscanf(req,"%s %s",part1,part2);
            int i=find_token(part2);
            if (i==(-1)){
                    sendd(client_socket,"Error","token not found.");
                    return 0;
            }
            if (users[i].channel_name[0]==0){
                sendd(client_socket,"Error","you didn't join any channel");
                return 0;
            }
            leave_channel(i,client_socket);
        }
        else if (strncmp("logout", req, 6) == 0)
        {
            sscanf(req,"%s %s",part1,part2);
            int i=find_token(part2);
            if (i==(-1)){
                    sendd(client_socket,"Error","token not found.");
                    return 0;
            }
            logout(i,client_socket);
        }
        else if (strncmp("find user", req, 9) == 0)
        {
            int counter=0;
            for(int i=0;i<strlen(req);i++){
                if (req[i]==',') req[i]=' ';
                else if (req[i]==' ') counter++;
                if (counter>4){
                    sendd(client_socket,"Error","user not found.");
                    return 0;
                }
            }
            sscanf(req,"%s %s %s  %s",part1,part2,part3,part4);
            int i=find_token(part4);
            if (i==(-1)){
                    sendd(client_socket,"Error","token not found.");
                    return 0;
            }
            if (users[i].channel_name[0]==0){
                sendd(client_socket,"Error","you didn't join any channel");
                return 0;
            }
            find_user2(client_socket,i,part3);
        }
        else if (strncmp("find message", req, 12) == 0)
        {
            int counter=0;
            for(int i=0;i<strlen(req);i++){
                if (req[i]==',') req[i]=' ';
                else if (req[i]==' ') counter++;
                if (counter>4){
                    sendd(client_socket,"Error","user not found.");
                    return 0;
                }
            }
            sscanf(req,"%s %s %s  %s",part1,part2,part3,part4);
            int i=find_token(part4);
            if (i==(-1)){
                    sendd(client_socket,"Error","token not found.");
                    return 0;
            }
            if (users[i].channel_name[0]==0){
                sendd(client_socket,"Error","you didn't join any channel");
                return 0;
            }
            find_message(client_socket,i,part3);
        }
        else if(strncmp("exit",req,4)==0){
            global_flag=1;
            return 0;
        }
}
void find_message(int client_socket,int index,char* word){
    char req[1000],info[1000];
    char* finall;
    char* finall2;
    char* payam1= (char*) malloc(1000*sizeof(char));
    char address[100]="resources\\channels\\",address2[100];
    cJSON *reqq , *type,*aa;
    int n;
    FILE* fptr;
    strcat(address,users[index].channel_name);
    strcat(address,".channel");
    strcat(address,".json");
    fptr=fopen(address,"r");
    fgets(info,1000,fptr);
    cJSON* tarjome= cJSON_Parse(info);
    cJSON* payam=cJSON_CreateArray();
    cJSON* content=cJSON_GetObjectItem(tarjome,"messages");
    int t=0,d=0;
    char* nahaiy;
    //printf("aaaa %d\n",cJSON_GetArraySize(content));
    //printf("aaaaaaaaaaaaa %d\n",users[index].refreshh);
    for (int i = 0 ; i < cJSON_GetArraySize(content) ; i++)
    {
        cJSON * subitem = cJSON_GetArrayItem(content, i);
        cJSON* sender = cJSON_GetObjectItem(subitem, "sender");
       // strcpy(sender1,sender->valuestring);
        cJSON* payam = cJSON_GetObjectItem(subitem, "content");
        strcpy(payam1,payam->valuestring);
        /* char* p=strstr(payam1,word);
        if(p){
            d=1;
            strcpy(nahaiy,payam1);
        } */
        //strcat(word," ");
        //payam1 word nahaiy
        int counter=0;
        char a[1000];
        printf("trueeee\n");
         while (1)
        {
            sscanf(payam1+counter,"%s",a);
            printf("%s\n",a);
            counter+=strlen(a)+1;
            if (!strcmp(a,word))
            {
                strcpy(nahaiy,payam1);
                d=1;
                break;
            }
            if (counter>=strlen(payam1)) break;
        }
    }
    reqq=cJSON_CreateObject();
    if(d==1) {
            cJSON_AddItemToObject(reqq, "type", cJSON_CreateString("Successful"));
            cJSON_AddItemToObject(reqq, "content",cJSON_CreateString(nahaiy));
    }
    else{
            cJSON_AddItemToObject(reqq, "type", cJSON_CreateString("Error"));
            cJSON_AddItemToObject(reqq, "content",cJSON_CreateString(""));
    }
    finall=cJSON_PrintUnformatted(reqq);
    fclose(fptr);
    printf("%s\n",finall);
    send(client_socket,finall,strlen(finall)+1,0);
}
void find_user2(int client_socket,int index,char* name){
    int flag1=0;
    for(int i=0;i<counter_user;i++){
        if (strcmp(users[i].name,name)==0){
                if (strcmp(users[i].channel_name,users[index].channel_name)==0){
                        flag1=1;
                }
        }
    }
    if(flag1==1){
            sendd(client_socket,"Successful","");
    }
    else{
            sendd(client_socket,"Error","");
    }
}
int find_user_in_struct(char* name1){
    for(int i=0;i<counter_user;i++){
        if (strcmp(name1,users[i].name)==0){
            return i;
        }
    }
    return -1;
}
int find_channel(char* name){
    FILE* fptr;
    char address[500]="resources\\channels\\";
    strcat(address,name);
    strcat(address,".channel");
    strcat(address,".json");
    fptr=fopen(address,"r");
    if (fptr==NULL){
        return 1;
    }
    fclose(fptr);
    return 0;
}
int find_token(char* token2){
    for(int i=0;i<counter_user;i++){
            if (strcmp(token2,users[i].token)==0){
                return i;
            }
    }
    return -1;
}
int check_pass(char* user,char* pass){
    FILE* fptr;
    char info[500],user2[100],pass2[100];
    char address[500]="resources\\users\\";
    strcat(address,user);
    strcat(address,".user");
    strcat(address,".json");
    fptr=fopen(address,"r");
    fscanf(fptr,"%s",info);
    //printf("%s\n",info);
    cJSON* tarjome= cJSON_Parse(info);
    cJSON* type=cJSON_GetObjectItem(tarjome,"username");
    strcpy(user2,type->valuestring);
    cJSON* content=cJSON_GetObjectItem(tarjome,"password");
    strcpy(pass2,content->valuestring);
    fclose(fptr);
    if((strcasecmp(pass2,pass)==0) && (strcasecmp(user2,user)==0)){
        return 1;
    }
    return 0;
}
int find_user(char* name){
    FILE* fptr;
    char address[500]="resources\\users\\";
    strcat(address,name);
    strcat(address,".user");
    strcat(address,".json");
    fptr=fopen(address,"r");
    if (fptr==NULL){
        return 1;
    }
    fclose(fptr);
    return 0;
}
void create_token(char* token){
    srand(time(0));
    for (int i=0;i<32;){
        int a=((rand())% 74)+48;
        if(isalnum(a)){
                token[i]=a;
                i++;
        }
    }
    token[31]='\0';
}
void registerr(int client_socket,char* user,char* pass){
    char req[1000];
    char* finall;
    char address[100]="resources\\users\\",address2[100];
    cJSON *reqq , *type , *content,*info;
    int n;
    strcpy(users[counter_user].name,user);
    strcpy(users[counter_user].pass,pass);
    counter_user++;
    FILE* fptr;
    //strcat(address,"resources\\users\\");
    strcat(address,user);
    strcat(address,".user");
    strcat(address,".json");
    //printf("%s",address);
    fptr=fopen(address,"w");
    info=cJSON_CreateObject();
    printf("ok1\n");
    cJSON_AddItemToObject(info, "username", cJSON_CreateString(user));
    //printf("ok2\n");
    cJSON_AddItemToObject(info, "password", cJSON_CreateString(pass));
    //printf("ok3\n");
    finall=cJSON_PrintUnformatted(info);
    //printf("ok4\n");
   // printf("%s\n",finall);
    fprintf(fptr,"%s",finall);
    reqq=cJSON_CreateObject();
    cJSON_AddItemToObject(reqq, "type", cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(reqq, "content", cJSON_CreateString(""));
    finall=cJSON_PrintUnformatted(reqq);
    printf("%s\n",finall);
    fclose(fptr);
    send(client_socket,finall,strlen(finall)+1,0);
}
void create_channel(int index,int client_socket,char* channel_name){
    char req[1000];
    char* finall;
    char address[100]="resources\\channels\\",address2[100];
    cJSON *reqq , *type , *content,*info,*aa;
    int n;
    strcpy(users[index].channel_name,channel_name);
    FILE* fptr;
    strcat(address,channel_name);
    strcat(address,".channel");
    strcat(address,".json");
    //printf("%s",address);
    fptr=fopen(address,"w");
    info=cJSON_CreateObject();
    printf("ok11\n");
    cJSON* payam=cJSON_CreateArray();
    printf("ok22\n");
    aa = cJSON_CreateObject();
    printf("ok33\n");
    char* content2=users[index].name;
    strcat(content2," created channel.");
    cJSON_AddItemToObject(aa, "sender", cJSON_CreateString("server"));
    cJSON_AddItemToObject(aa, "content", cJSON_CreateString(content2));
    cJSON_AddItemToArray(payam,aa);
    cJSON_AddItemToObject(info, "messages",payam);
    cJSON_AddItemToObject(info, "name", cJSON_CreateString(channel_name));
    finall=cJSON_PrintUnformatted(info);
    //printf("%s\n",finall);
    fprintf(fptr,"%s",finall);
    //recv(client_socket,req, sizeof(req), 0);
    reqq=cJSON_CreateObject();
    cJSON_AddItemToObject(reqq, "type", cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(reqq, "content", cJSON_CreateString(""));
    finall=cJSON_PrintUnformatted(reqq);
    char st1[100],st2[100];
    sscanf(users[index].name,"%s",st1);
    strcpy(users[index].name,st1);
    fclose(fptr);
    printf("%s\n",finall);
    send(client_socket,finall,strlen(finall)+1,0);
}
void join_channel(int index,int client_socket,char* channel_name){
    char req[1000],info[1000];
    char* finall;
    char* finall2;
    char address[100]="resources\\channels\\",address2[100];
    cJSON *reqq , *type , *content,*aa;
    int n;
    strcpy(users[index].channel_name,channel_name);
    FILE* fptr;
    strcat(address,channel_name);
    strcat(address,".channel");
    strcat(address,".json");
    fptr=fopen(address,"r");
    fgets(info,1000,fptr);
   // printf("info: %s\n",info);
    fclose(fptr);
    fptr=fopen(address,"w");
    cJSON* tarjome= cJSON_Parse(info);
    cJSON* messages=cJSON_GetObjectItem(tarjome,"messages");
    aa=cJSON_CreateObject();
    printf("ok1\n");
    char* content2=users[index].name;
   // printf("---------------%s\n",users[index].name);
    strcat(content2," joined.");
    cJSON_AddItemToObject(aa, "sender", cJSON_CreateString("server"));
    printf("ok2\n");
    cJSON_AddItemToObject(aa, "content", cJSON_CreateString(content2));
    printf("ok3\n");
    cJSON_AddItemToArray(messages,aa);
    printf("ok4\n");
    finall2=cJSON_PrintUnformatted(tarjome);
    printf("ok5\n");
    printf("%s\n",finall2);
    fprintf(fptr,"%s",finall2);
    reqq=cJSON_CreateObject();
    printf("ok2\n");
    cJSON_AddItemToObject(reqq, "type", cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(reqq, "content", cJSON_CreateString(""));
    finall=cJSON_PrintUnformatted(reqq);
    char st1[100];
    sscanf(users[index].name,"%s",st1);
    strcpy(users[index].name,st1);
    fclose(fptr);
    printf("%s\n",finall);
    send(client_socket,finall,strlen(finall)+1,0);
}
void send_message(int index,int client_socket,char* messagee){
    char req[1000],info[1000];
    char* finall;
    char* finall2;
    char address[100]="resources\\channels\\",address2[100];
    cJSON *reqq , *type , *content,*aa;
    int n;
    FILE* fptr;
    strcat(address,users[index].channel_name);
    strcat(address,".channel");
    strcat(address,".json");
    fptr=fopen(address,"r");
    fgets(info,1000,fptr);
    //printf("info: %s\n",info);
    fclose(fptr);
    fptr=fopen(address,"w");
    cJSON* tarjome= cJSON_Parse(info);
    printf("ok1\n");
    cJSON* messages=cJSON_GetObjectItem(tarjome,"messages");
    printf("ok2\n");
    aa=cJSON_CreateObject();
    printf("ok3\n");
    char st1[100];
    sscanf(users[index].name,"%s",st1);
    strcpy(users[index].name,st1);
    //printf("user name:::::  %s\n",users[index].name);
    cJSON_AddItemToObject(aa, "sender", cJSON_CreateString(users[index].name));
    printf("ok4\n");
    cJSON_AddItemToObject(aa, "content", cJSON_CreateString(messagee));
    printf("ok5\n");
    cJSON_AddItemToArray(messages,aa);
    printf("ok6\n");
    finall2=cJSON_PrintUnformatted(tarjome);
    printf("ok7\n");
    //printf("%s\n",finall2);
    fprintf(fptr,"%s",finall2);
    reqq=cJSON_CreateObject();
    printf("ok8\n");
    cJSON_AddItemToObject(reqq, "type", cJSON_CreateString("Successful"));
    printf("ok9\n");
    cJSON_AddItemToObject(reqq, "content", cJSON_CreateString(""));
    printf("ok10\n");
    finall=cJSON_PrintUnformatted(reqq);
    printf("ok11\n");
    fclose(fptr);
    printf("%s\n",finall);
    send(client_socket,finall,strlen(finall)+1,0);
}
void refresh(int index,int client_socket){
    char req[1000],info[1000];
    char* finall;
    char* finall2;
    char address[100]="resources\\channels\\",address2[100];
    cJSON *reqq , *type,*aa;
    int n;
    FILE* fptr;
    strcat(address,users[index].channel_name);
    strcat(address,".channel");
    strcat(address,".json");
    fptr=fopen(address,"r");
    fgets(info,1000,fptr);
    //printf("----------\n");
    cJSON* tarjome= cJSON_Parse(info);
    cJSON* payam=cJSON_CreateArray();
    cJSON* content=cJSON_GetObjectItem(tarjome,"messages");
    int t=0;
    //printf("aaaa %d\n",cJSON_GetArraySize(content));
    //printf("aaaaaaaaaaaaa %d\n",users[index].refreshh);
    for (int i = users[index].refreshh ; i < 1+users[index].refreshh ; i++){
            cJSON * subitem = cJSON_GetArrayItem(content, i);
            cJSON_AddItemToArray(payam,subitem);
    }
    reqq=cJSON_CreateObject();
    users[index].refreshh= cJSON_GetArraySize(content);
    cJSON_AddItemToObject(reqq, "type", cJSON_CreateString("List"));
    cJSON_AddItemToObject(reqq, "content",(payam));
    finall=cJSON_PrintUnformatted(reqq);
    fclose(fptr);
    printf("%s\n",finall);
    send(client_socket,finall,strlen(finall)+1,0);
}
void channel_members(int index,int client_socket){
    char req[1000],info[1000];
    char* finall;
    char* finall2;
    char address[100]="resources\\channels\\",address2[100];
    cJSON *reqq , *type,*aa;
    int n;
    FILE* fptr;
    strcat(address,users[index].channel_name);
    strcat(address,".channel");
    strcat(address,".json");
    fptr=fopen(address,"r");
    fgets(info,1000,fptr);
    //printf("----------\n");
    cJSON* tarjome= cJSON_Parse(info);
    cJSON* payam=cJSON_CreateArray();
    for(int i=0;i<counter_user;i++){
        if(strcmp(users[i].channel_name,users[index].channel_name)==0){
            //printf("hiii\n");
            cJSON* inf = cJSON_CreateString(users[i].name);
            cJSON_AddItemToArray(payam,inf);
        }
    }
    reqq=cJSON_CreateObject();
    cJSON_AddItemToObject(reqq, "type", cJSON_CreateString("List"));
    cJSON_AddItemToObject(reqq, "content",(payam));
    finall=cJSON_PrintUnformatted(reqq);
    fclose(fptr);
    printf("%s\n",finall);
    send(client_socket,finall,strlen(finall)+1,0);
}
void leave_channel(int index,int client_socket){
    char req[1000],info[1000];
    char* finall;
    char* finall2;
    char address[100]="resources\\channels\\",address2[100];
    cJSON *reqq , *type , *content,*aa;
    char* channel_name=users[index].channel_name;
    users[index].channel_name[0]=0;
    users[index].refreshh=0;
    FILE* fptr;
    strcat(address,channel_name);
    strcat(address,".channel");
    strcat(address,".json");
    fptr=fopen(address,"r");
    fgets(info,1000,fptr);
   // printf("info: %s\n",info);
    fclose(fptr);
    fptr=fopen(address,"w");
    cJSON* tarjome= cJSON_Parse(info);
    cJSON* messages=cJSON_GetObjectItem(tarjome,"messages");
    aa=cJSON_CreateObject();
    char* content2=users[index].name;
   // printf("---------------%s\n",users[index].name);
    strcat(content2," leaved the channel.");
    cJSON_AddItemToObject(aa, "sender", cJSON_CreateString("server"));
    cJSON_AddItemToObject(aa, "content", cJSON_CreateString(content2));
    cJSON_AddItemToArray(messages,aa);
    finall2=cJSON_PrintUnformatted(tarjome);
    //printf("%s\n",finall2);
    fprintf(fptr,"%s",finall2);
    char st1[100];
    sscanf(users[index].name,"%s",st1);
    strcpy(users[index].name,st1);
    reqq=cJSON_CreateObject();
    cJSON_AddItemToObject(reqq, "type", cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(reqq, "content", cJSON_CreateString(""));
    finall=cJSON_PrintUnformatted(reqq);
    fclose(fptr);
    printf("%s\n",finall);
    send(client_socket,finall,strlen(finall)+1,0);
}
void logout(int index,int client_socket){
    users[index].token[0]=0;
    users[index].loginn=false;
    sendd(client_socket,"Successful","");
}
void sendd(int client_socket,char* type,char* content){
            cJSON *reqq;
            char* finall;
            reqq=cJSON_CreateObject();
            printf("ok1\n");
            memset(reqq,1000,0);
            memset(finall,1000,0);
            cJSON_AddItemToObject(reqq,"type", cJSON_CreateString(type));
            cJSON_AddItemToObject(reqq,"content", cJSON_CreateString(content));
            finall=cJSON_PrintUnformatted(reqq);
            printf("%s\n",finall);
            send(client_socket,finall,strlen(finall)+1,0);
}
int main(){
    int server_socket, client_socket;
    struct sockaddr_in server, client;
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Create and verify socket
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
	if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully bound..\n");

    // Now server is ready to listen and verify

 //-----------------------------------------------------------------------------------------
    DIR* dir = opendir("resources");
    if (dir) {
        closedir(dir);
        DIR* dir2 = opendir("resources\\users");
            if(!dir2){
            	int check;
                char* dirname = "resources\\users";
                check = mkdir(dirname);
            }
        DIR* dir3 = opendir("resources\\channels");
            if(!dir3){
            	int check;
                char* dirname = "resources\\channels";
                check = mkdir(dirname);
            }
        //  closedir(dir);
    }
    else if (ENOENT == errno) {
                int check;
                char* dirname = "resources";
                check = mkdir(dirname);
                DIR* dir2 = opendir("resources\\users");
                if(!dir2){
                    int check;
                    char* dirname = "resources\\users";
                    check = mkdir(dirname);
                }
            DIR* dir3 = opendir("resources\\channels");
                if(!dir3){
                    int check;
                    char* dirname = "resources\\channels";
                    check = mkdir(dirname);
                }
    }
   // ------------------------------------------------------------------------------------------
    for(int i=0;i<100;i++){
        users[i].token[0]=0;
        users[i].loginn=false;
        users[i].channel_name[0]=0;
        users[i].refreshh=0;
    }
    load_users();
    while(1){
        request(server_socket);
        if(global_flag==1) break;
        /* printf("--------\n");
        for(int i=0;i<counter_user;i++){
            printf("%s\n",users[i].name);
        } */
    }
    return 0;
}
