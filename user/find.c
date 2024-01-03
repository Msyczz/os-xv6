//
// Created by lenovo on 2024/1/3.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//从整个路径检索出文件名
char *basename(char *pathname){
    //static char buf[DIRSIZ+1];
    char *prev = 0;
    char *curr = strchr(pathname,'/');
    while(curr != 0){
        prev = curr;
        curr = strchr(curr+1,'/');
    }
    return prev;
}



//递归调用
void find(char *curr_path,char *target){
    char buf[512], *p;
    //创建一个文件描述符用来描述文件
    int fd;
    //创建一个，目录项，存储文件的inode号和文件名
    struct dirent de;
    //存储文件的基本信息比如说文件的磁盘号、inode号、文件类型等
    struct stat st;
    //open（）函数打开path的指定文件，返回一个文件描述符
    if((fd = open(curr_path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", curr_path);
        return;
    }
    //将文件基本信息传入st
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", curr_path);
        close(fd);
        return;
    }

    //根据文件的不同类型进行处理
    char *f_name ;
    switch(st.type){

        case T_FILE:
            //接收返回回来的最后一个目录
             f_name = basename(curr_path);
            int match = 1;
            //如果文件名为空，则查找失败
            if(f_name == 0|| strcmp(f_name + 1, target) != 0){
                match = 0;
            }
            if(match){
                //输出完整路径
                printf("%s\n",curr_path);
            }
            close(fd);
            break;

        case T_DIR:
            //初始化buf
            memset(buf,0,sizeof(buf));
            //if(strlen(curr_path) + 1 + DIRSIZ + 1 > sizeof buf){
               // printf("find: path too long\n");
              //  break;
            //}
            uint curr_path_len = strlen(curr_path);//用无符号整数存curr_path长度
            //将完整的路径存入buf中
            memcpy(buf,curr_path,curr_path_len);//复制curr_path的长度到buf
            buf[curr_path_len] = '/';
            p = buf+curr_path_len + 1;
            //循环读取当前文件，如果读到了非.和..的路径，则进入该路径再次find
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0|| strcmp(de.name,".") == 0|| strcmp(de.name,".." )== 0)
                    continue;
                memcpy(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf,target);
                //printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
            }
            close(fd);
            break;
    }

}




int main(int argc, char *argv[]){

    if (argc != 3) {
        fprintf(2, "usage: find [directory] [target filename]\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
    
}