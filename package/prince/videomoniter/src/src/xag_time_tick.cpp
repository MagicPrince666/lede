#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "xag_time_tick.h"

struct timer_wheel timer = {{0}, 0};

void tick(int signo) {
	// 使用二级指针删进行单链表的删除 
	struct timer_node **cur = &timer.slot[timer.current];
	while (*cur) {
		struct timer_node *curr = *cur;
		if (curr->rotation > 0) {
			curr->rotation--;
			cur = &curr->next;
			} else {
				curr->proc(curr->data);
				*cur = curr->next;
                free(curr);
			}
	}
	timer.current = (timer.current + 1) % TIME_WHEEL_SIZE;
	alarm(1);
}

void add_timer(int len, func action) {
	int pos = (len + timer.current) % TIME_WHEEL_SIZE;
	struct timer_node *node = (timer_node*)malloc(sizeof(struct timer_node));
	// 插入到对应格子的链表头部即可, O(1)复杂度
	node->next = timer.slot[pos];
	timer.slot[pos] = node;
	node->rotation = len / TIME_WHEEL_SIZE;
	node->data = 0;
    node->proc = action;
}
/*
int test_func()
{
    static int count = 0;

    printf("count is %d\n", count++);
}

void init_sigaction()
{
    struct sigaction act;
          
    act.sa_handler = test_func; //设置处理信号的函数
    act.sa_flags  = 0;

    sigemptyset(&act.sa_mask);
    sigaction(SIGPROF, &act, NULL);//时间到发送SIGROF信号
}

void init_time()
{
    struct itimerval val;
         
    val.it_value.tv_sec = 1; //1秒后启用定时器
    val.it_value.tv_usec = 0;

    val.it_interval = val.it_value; //定时器间隔为1s

    setitimer(ITIMER_PROF, &val, NULL);
}

int main(int argc, char **argv)
{

    init_sigaction();
    init_time();

    while(1);

    return 0;
}
*/
