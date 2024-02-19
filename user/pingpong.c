//
// Created by lenovo on 2024/1/2.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int ptoc[2] ;
    int ctop[2] ;
    char w1 = 'x';
    char w2 = 'm';

    pipe(ptoc);
    pipe(ctop);

    int process_id = fork();
    if(process_id > 0){

        write(ptoc[1],&w1,1);
        read(ctop[0],&w2,1);
        int pid = getpid();
        printf("%d: received pong\n",pid);
        exit(1);

    }if(process_id == 0){
        read(ptoc[0],&w1,1);
        int pid = getpid();
        printf("%d: received ping\n",pid);
        write(ctop[1],&w2,1);

        exit(1);
    }else{
        exit(0);
    }


}
