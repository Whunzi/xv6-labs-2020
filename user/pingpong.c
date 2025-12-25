#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv)
{   
    int p_to_c[2];
    int c_to_p[2];

    pipe(p_to_c);//父到子
    pipe(c_to_p);//子到父

    int pid = fork();
    if(pid == 0) //// 子进程要：从父那里读"ping"，向父那里写"pong"  谁写从谁出发 p1   谁读流向谁 p0  写是1  读是0
    {   
        //保留了 p_to_c[0]  父到子 用来读
        close(p_to_c[1]);//关闭父→子水管的写入口（子进程不需要写这个水管）
         //保留了 p_to_c[1]  子到父 用来写
        close(c_to_p[0]);//关闭子→父水管的读取口（子进程不需要读这个水管）  

        char buf;
        read(p_to_c[0],&buf,1);
        printf("%d: received ping\n", getpid());

        write(c_to_p[1],"p",1);

        close(p_to_c[0]);
        close(c_to_p[1]);
        exit(0);

    }
    
    //父进程
    else{
        close(p_to_c[0]);
        close(c_to_p[1]);

        write(p_to_c[1],"p",1);
        wait(0);  // 等待子进程完成

        char buf;
        read(c_to_p[0],&buf,1);
        printf("%d: received pong\n",getpid());
        
        close(p_to_c[1]);
        close(c_to_p[0]);
        exit(0);
    }

}