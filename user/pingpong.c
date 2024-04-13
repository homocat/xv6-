#include "kernel/types.h"
#include "user/user.h"

#define BUFFER_SIZE 16
#define INDEX_READ 0
#define INDEX_WRITE 1

int main(int argc, char *argv[])
{
    int fds_p2c[2]; //父进程到子进程
    int fds_c2p[2]; //子进程到父进程

    //创建两个管道
    pipe(fds_c2p);
    pipe(fds_p2c);

    int pid;
    pid = fork(); //fork出子进程
    //子进程
    if (pid == 0)
    {
        //通信方向：子进程到父进程，子进程不需要读
        close(fds_c2p[INDEX_READ]);
        //通信方向：父进程到子进程，子进程不需要写
        close(fds_p2c[INDEX_WRITE]);

        //step2：子进程收到ping，打印
        char buf[BUFFER_SIZE];
        if (read(fds_p2c[INDEX_READ], buf, 1) == 1)
        {
            printf("%d: received ping\n", getpid());
        }

        //step3: 子进程继续向父进程发出信号
        write(fds_c2p[INDEX_WRITE], "f", 1);

        exit(0);
    }
    //父进程
    else
    {
        //通信方向：子进程到父进程，父进程不需要写
        close(fds_c2p[INDEX_WRITE]);
        //通信方向：父进程到子进程，父进程不需要读
        close(fds_p2c[INDEX_READ]);

        //step1: 父进程给子进程发ping
        write(fds_p2c[INDEX_WRITE], "f", 1);

        //step4：父进程读取子进程发出的ping
        char buf[BUFFER_SIZE];
        if (read(fds_c2p[INDEX_READ], buf, 1) == 1)
        {
            printf("%d: received pong\n", getpid());
        }
    }

    //父进程结束
    exit(0);
}
