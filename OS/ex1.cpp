#include<iostream>
#include<string>
#include<windows.h>
using namespace std;

#define PCBNUM 5

struct PCB
{
	string name;      //进程名
	int time;         //要求运行时间
	int priority;     //优先级
	char status;      //状态 R 就绪, E 结束
	struct PCB* next; //指向下一个进程PCB
};

//设置进程初始值
void SetPCB(PCB*& pcb, int i)
{
	pcb->name = "P" + to_string(i);

	cout << "请为进程P" << i << "依次输入进程优先数和要求运行时间：" << endl;
	cin >> pcb->priority >> pcb->time;
	if (pcb->priority < 0 || pcb->time < 0)
	{
		cout << "非法数据！" << endl;
		exit(0);
	}
	cout << endl;
	pcb->status = 'R';
}

//根据优先级排序，从大到小
void SortPriority(PCB *&head)
{
	PCB *p, * pre, * q;
	p = head->next->next;
	head->next->next = NULL;
	while (p != NULL)
	{
		q = p->next; 
		pre = head;
		while (pre->next != NULL && pre->next->priority > p->priority)
		//如果前一个大于后一个，顺序找下去
		    pre = pre->next;
								
		p->next = pre->next;    //交换节点
		pre->next = p;
		p = q;
	}
}

//运行
bool Run(PCB*& head)
{
	PCB* pcb = new PCB;
	pcb = head->next;
	pcb->priority = pcb->priority - 1;
	pcb->time = pcb->time - 1;
	return pcb->time;
}

//显示每一次的运行/就绪情况
void Show(PCB*& head)
{
	PCB* h = head;
	PCB* pcb = h->next;
	cout << "Run:" << endl;
	cout << "\tName\t" << "Time\t" << "Priority\t" << "Status\t" << endl;
	cout << '\t' << pcb->name << '\t' << pcb->time << '\t' << pcb->priority << "\t\t" << pcb->status << '\t' << endl;

	cout << "Ready:" << endl;
	h = h->next;
	pcb = h->next;

	while (pcb != NULL)
	{
		cout << '\t' << pcb->name << '\t' << pcb->time << '\t' << pcb->priority << "\t\t" << pcb->status << '\t' << endl;
		h = h->next;
		pcb = h->next;
	}
	cout << endl;
}

int main()
{
	PCB* head = new PCB;
	head->next = NULL;
	
	int count = 1;
	
	for (int i = 1; i <= PCBNUM; i++)
	{
		PCB* pcb = new PCB;
		SetPCB(pcb, i);
		pcb->next = head->next;
		head->next = pcb;
	}
	
	while( head->next != NULL)
	{
		SortPriority(head);
		cout << "* * * * * * * * * * * * * * * * * * * * * * * * *" << endl;
		cout << "第" << count << "次运行" << endl;
		Show(head);

		if (head->next == NULL)
			return -1;

		if (!Run(head))   //如果运行队首进程返回值为false，说明该进程要求运行时间为0，即已经结束了
		{
			head->next->status = 'E';
			cout << "End:" << endl;
			cout << "\tName\t" << "Status\t"<<endl;
			cout << '\t'<<head->next->name<< '\t'<<head->next->status<< endl;
			head->next = head->next->next;
		}
		count++;
	}
    system("pause");
	return 0;
}