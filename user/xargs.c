//
// Created by lenovo on 2024/1/3.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define buf_size 512
int main(int argc, char *argv[]){
    //定义buf和xargs的大小
    char buf[buf_size + 1] = {0};
    uint  occupy = 0;//occupy相当于一个指针，记录read函数读到哪
    char *xargs[300] = {0};
    int stdin_end = 0;
    //把argv的值附到xargs上
    for(int i =1;i<argc;i++){
        xargs[ i - 1] = argv[i];
    }
    //循环读入文件序列号
    while (!(stdin_end && occupy == 0)) {
        // try read from left-hand program
        if (!stdin_end) {
            //剩下的字符
            int remain_size = buf_size - occupy;
            //剩下字符所占得bytes
            int read_bytes = read(0, buf + occupy, remain_size);
            //read_bytes<0,不合法
            if (read_bytes < 0) {
                fprintf(2, "xargs: read returns -1 error\n");
            }
            //raed_bytes == 0,说明argv[]没字符了
            if (read_bytes == 0) {
                close(0);
                stdin_end = 1;
            }
            //每一次把读完的字符加到occupy处
            occupy += read_bytes;
        }
        // process lines read
        char *line_end = strchr(buf, '\n');
        while (line_end) {
            char xbuf[buf_size + 1] = {0};
            memcpy(xbuf, buf, line_end - buf);
            xargs[argc - 1] = xbuf;
            int ret = fork();
            if (ret == 0) {
                // i am child
                if (!stdin_end) {
                    close(0);
                }
                if (exec(argv[1], xargs) < 0) {
                    fprintf(2, "xargs: exec fails with -1\n");
                    exit(1);
                }
            } else {
                // trim out line already processed
                memmove(buf, line_end + 1, occupy - (line_end - buf) - 1);
                occupy -= line_end - buf + 1;
                memset(buf + occupy, 0, buf_size - occupy);
                // harvest zombie
                int pid;
                wait(&pid);

                line_end = strchr(buf, '\n');
            }
        }
    }
    exit(0);
}
