//
// Created by lenovo on 2024/1/2.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void readin(int readfd){
    int start;
    int buff;
    int flag = 0;
    read(readfd,&start,4);
    printf("prime %d\n",start);
    int pipes[2];
    pipe(pipes);
    while(read(readfd,&buff,4)>0){
        if(buff%start==0){
            continue;
        }else{
            write(pipes[1], &buff, 4);
            if(flag == 0){
                flag = 1;
                int pid = fork();
                if(pid > 0){//父进程
                    close(pipes[0]);

                }else if(pid == 0){//子进程
                    close(pipes[1]);
                    readin(pipes[0]);
                    exit(0);
                }else{
                    exit(1);
                }
            }
        }
    }
    close(pipes[1]);
    int state;
    wait(&state);
    exit(0);

}


int main(int argc, char *argv[]){
   int writein[2];
   pipe(writein);
   for(int i = 2;i<=35 ;i++){
       write(writein[1],&i,4);
   }
   close(writein[1]);
   readin(writein[0]);
   exit(0);

}
