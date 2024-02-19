//
// Created by lenovo on 2024/1/2.
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if(argc == 1){
        printf("you should offer one parameter\n");
        exit(1);
    }
    int n = atoi(argv[1]);
    sleep(n);
    exit(0);
}

