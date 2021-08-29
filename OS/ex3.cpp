#include <iostream>
using namespace std;

#define Maxsize 64       //64个块
#define Free 0
#define Occupied 1

int bitMap[8][8];

struct Task
{
	int name ;	
	int need ;	
	int x[64];	//存储柱面号（byte）
	int y[64];	//存储磁道号和物理地址（bit）
	struct Task* next;
};

//获取剩余磁盘空间数量
int GetRest()
{
	int rest = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (bitMap[i][j] == Free)
			{
				rest++;
			}
		}
	}
	return rest;
}

//分配磁盘空间
bool Assign(Task*& head, int name, int need)
{
	if (need <= GetRest() )
	{
		Task* task = new Task;
		task->next = head->next;
		head->next = task;

		task->name = name;
		task->need = need;
		int temp = 0;//磁盘块
		cout << "柱面号\t磁道号\t物理记录号" << endl;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (bitMap[i][j] == Free)
				{
					if (need <= 0)
					{
						return true;
					}
					bitMap[i][j] = Occupied;
					task->x[temp] = i;
					task->y[temp] = j;
					cout << i << "\t" << j / 4 << "\t" << j % 4 << endl;//每个磁道4有个物理块
					temp++;
					need--;  
				}
			}
		}
	}
	return false;
}

//回收磁盘空间
bool Recycle(Task*& head, int name)
{
	Task* task = new Task;
	task = head->next;
	bool find = false;
	int byte, bit;

	while (task != NULL)
	{
		if (name == task->name)
		{
			find = true;
			break;
		}
		head = head->next;
		task = head->next;
	}
		if (find == true) 
		{
			cout << "字节号\t" << "位数\t" << endl;
			for (int i = 0; i < task->need; i++)
			{   //将其占用的空间一一释放
				byte = task->x[i];
				bit = task->y[i];
				bitMap[byte][bit] = Free;
				cout << byte << "\t" << bit << endl;
			}
			head->next = task->next;
			return true;
		}
		else
		{
			cout << "该任务不存在" << endl;
			return false;
		}
}

//显示位视图
void Show()
{
	cout << "* * * * * * * * * * * *     位视图     * * * * * * * * * * ** * * *" << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << "\t" << i;
	}
	cout << "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl;

	for (int i = 0; i < 8; i++)
	{
		cout << i << "  *" << "\t";
		for (int j = 0; j < 8; j++)
		{
			cout << bitMap[i][j] << "\t";
		}
		cout << endl;
	}
	cout << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * " << endl;
}

int main()
{
	Task* head = new Task;
	head->next = NULL;

	int choice;
	int assignName = 0, need = 0;
	int returnName = 0;

	while (true)
	{
		Show();
		cout << "输入：\n\t" << "1\t分配" << "\n\t2\t回收" << "\n\t其他\t退出" << endl;
		cin >> choice;
		switch (choice)
		{
		case 1:

			cout << "请依次输入需要分配的 任务名、空间大小：" << endl;
			cin >> assignName >> need;
			if (assignName < 0 || need <= 0 || need > Maxsize)
			{
				cout << "请按要求输入数据！" << endl;
				exit(0);
			}
			if (Assign(head, assignName, need))
			{
				cout << "分配成功！" << endl;
			}
			else
			{
				cout << "分配失败！" << endl;
			}
			break;
		case 2:

			cout << "请输入要释放磁盘空间的任务名：" << endl;
			cin >> returnName;
			if (Recycle(head, returnName))
			{
				cout << "归还成功！" << endl;
			}
			else
			{
				cout << "归还失败！" << endl;
			}
			break;
		default:
			exit(0);
			break;
		}
	}

	return 0;
}