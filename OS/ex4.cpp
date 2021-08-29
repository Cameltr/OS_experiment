#include<stdio.h>
#include<unistd.h>
#include"wait.h"


int main()
{
	pid_t pid1, pid2;
	if ((pid1 = fork()) < 0)
	{
		printf("fork error\n");
		return -1;
	}
	else if (pid1 == 0)     //子进程1
	{
		printf("子进程1, PID=%d, 父进程的PID=%d, B\n",getpid(),getpid());
		return 0;
	}
	if ((pid2 = fork()) < 0)
	{
		printf("fork error\n");
		return -1;
	}
	else if (pid2 == 0)
	{
		printf("子进程2, PID=%d, 父进程的PID=%d, C\n",getpid(),getpid());
		return 0;
	}

	waitpid(pid1, NULL, 0);//等待子进程结束
	waitpid(pid2, NULL, 0);
	printf("父进程，PID=%d, 子进程1的PID=%d，子进程2的PID=%d, A\n",getpid(),pid1,pid2);
	return 0;
}