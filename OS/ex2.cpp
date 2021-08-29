#include<iostream>
#include<string>
using namespace std;

//已分配表
struct Allocation_Table {
	int start_address;
	int size;
	int allocation_number = 0;//设置为空，表示未被分配(进程号)
	Allocation_Table* next;
};

//空闲区
struct Free_Node {
	string process_name;//进程名
	int number;//进程号
	bool status;//true表示空闲，false表示已分配
	int size;//分区大小
	int address;//首地址
	Free_Node* prev;
	Free_Node* next;

	//构造函数
	Free_Node() { }

	Free_Node(string process_name, int number, int size)
	{
		this->process_name = process_name;
		this->number = number;
		this->size = size;
	}
};

class MemorySpace
{
	Free_Node* head_free;
	Allocation_Table* head_allocation;
public:
	MemorySpace(int n);
	~MemorySpace();
	void ApplicationSpace();//内存申请 
	void Add_AllocationTable(int start_address, int size, int allocation_number);
	//登记已分配区表
	void Remove_AllocationTable(int allocation_number);//删除已分配空间
	void FreeSpace();//内存回收
	void MergeFreeSpace(int number);//合并空闲分区 
	void PrintALL_Table();//打印空闲区所有结点（包括已分配的空闲区）
	void Print_AllocationTable();//打印已分配表
};

MemorySpace::MemorySpace(int n)//参数主存的容量
{
	head_free = new Free_Node();
	head_free->status = false;
	head_free->next = NULL;
	head_free->size = n;//主存剩余容量

	Free_Node* firstnode = new Free_Node("空闲分区", 0, n);//链表的第一个结点，未分配，空间为n
	firstnode->address = 0;//设置起始地址为0
	firstnode->status = true;//未分配
	firstnode->next = NULL;

	firstnode->prev = head_free;//指向头结点，构成双向链表
	head_free->next = firstnode;//指向链表的第一个结点

	head_allocation = new Allocation_Table();
	head_allocation->next = NULL;
}

MemorySpace::~MemorySpace()//析构函数
{
	Free_Node* q = head_free;
	while (head_free)
	{
		q = head_free;
		head_free = head_free->next;
		delete q;
	}

	Allocation_Table* p = head_allocation;
	while (head_allocation)
	{
		p = head_allocation;
		head_allocation = head_allocation->next;
		delete p;
	}
}

void MemorySpace::ApplicationSpace()
{
	bool flag = false;//用于判断是否分配空间成功
	//创建新的空间
	string name;
	int number;
	int size;
	cout << "输入申请主存的进程名，进程号，申请大小：";
	cin >> name >> number >> size;
	if (size <= head_free->size)//判断内存空间剩余所有容量是否大于等于申请需求
	{
		Free_Node* p_work = head_free->next;
		while (p_work)
		{
			if (p_work->status == true && p_work->size > size)//如果是空闲分区 && 空间 > 需求量
			{
				Free_Node* p_new = new Free_Node(name, number, size);//创建新的空间，并调用构造函数
				p_new->status = false;//结点状态为已分配
				//双向链表中插入一个结点
				p_new->next = p_work;
				p_new->prev = p_work->prev;
				p_work->prev->next = p_new;
				p_work->prev = p_new;

				p_work->size = p_work->size - p_new->size;
				//p_work属于空闲区，p_new分配的主存空间是从p_work上分割出来的

				p_new->address = p_work->address;//起始地址的设置
				p_work->address = p_work->address + size;
				Add_AllocationTable(p_new->address, size, p_new->number);//登记已分配区说明表
				flag = true;//分配标志设置为成功
				break;
			}
			else if (p_work->status == true && p_work->size == size)//如果是空闲分区 && 空间 = 需求量
			{
				//如果该分区的空闲大小刚好相等，就直接在上面设置相应的信息
				p_work->process_name = name;
				p_work->number = number;
				p_work->size = size;
				p_work->status = false;//结点状态为已分配
				Add_AllocationTable(p_work->address, size, p_work->number);//登记已分配区说明表
				flag = true;//分配标志设置为成功
				break;
			}
			p_work = p_work->next;  //未找到合适的空闲块就继续往下找
		}
		if (flag == true) //分配成功
		{
			cout << "进程号为：" << number << " 的进程，分配主存成功！" << endl;

		}
		else//分配失败：要求分配空间大于每一个空闲区
			cout << "主存容量不足，申请失败！" << endl;
	}
	else   //失败原因是要求分配空间大于总空闲空间
		cout << "主存容量不足，申请失败！" << endl;
}

void MemorySpace::Add_AllocationTable(int start_address, int size, int allocation_number)//建立已分配说明表
{

	Allocation_Table* q_temp = new Allocation_Table();
	q_temp->allocation_number = allocation_number;//进程号
	q_temp->size = size;//分区大小
	q_temp->start_address = start_address;//起始地址
	q_temp->next = head_allocation->next;//单链表头插法
	head_allocation->next = q_temp;
}

void MemorySpace::Remove_AllocationTable(int allocation_number)
{
	//单链表删除结点操作
	Allocation_Table* p_prev = head_allocation;//前驱结点
	Allocation_Table* p_delete = head_allocation->next;//删除结点
	while (p_delete)
	{
		if (p_delete->allocation_number == allocation_number)//找到需要删除的结点，删除
		{
			p_prev->next = p_delete->next;
			delete p_delete;
			break;
		}
		p_prev = p_prev->next;
		p_delete = p_delete->next;
	}
}

void MemorySpace::FreeSpace()//释放空间
{
	int number;
	bool find = false;//找到标志 true：找到，false：未找到
	cout << "输入要释放的进程号：";
	cin >> number;
	Free_Node* p_work = head_free->next;
	//找到进程号为number的结点
	while (p_work)
	{
		if (p_work->number == number)
		{
			find = true; //找到标志设置为true
			break;
		}
		p_work = p_work->next;
	}
	if (find == true)
	{
		cout << "进程号为：" << p_work->number << "  进程名：" << p_work->process_name << "已释放空间！" << endl;
		Remove_AllocationTable(p_work->number);//将已分配表中对应的表目删除
		//实现释放主存
		p_work->process_name = "空闲分区";//进程名修改为 空闲分区
		p_work->status = true;//未分配
		
		head_free->size = head_free->size + p_work->size;//系统剩余容量增加刚刚释放的容量
		MergeFreeSpace(p_work->number);//调用合并分区函数
	}
	else
		cout << "进程号为 " << number << " 的进程未找到！" << endl;
}

void MemorySpace::MergeFreeSpace(int number)//合并空闲分区
{
	Free_Node* p_work = head_free->next;
	//找到那个释放主存空间的结点
	while (p_work)
	{
		if (p_work->number == number)
			break;
		p_work = p_work->next;
	}

	if (p_work->next == NULL)//主存全部分配完，无剩余空间
	{
		p_work->number = 0;//指针不需要变动，将相应结点的进程号设为0
	}
	else if (p_work->prev->status == false && p_work->next->status == false)
	//释放分区的前、后邻接分区都是已分配区，没有合并的问题存在
	{
		p_work->number = 0;//指针不需要变动，将相应结点的进程号设为0
	}
	else if (p_work->prev->status == true && p_work->next->status == false)
	//释放分区的前邻接分区是空闲区，后邻接分区是已分配区。
	{
		p_work->prev->size = p_work->prev->size + p_work->size;
		p_work->prev->next = p_work->next;
		p_work->next->prev = p_work->prev;
		delete p_work;
	}
	else if (p_work->prev->status == false && p_work->next->status == true)
	//释放区的前邻接分区是已分配区，后邻接分区是空闲区。
	{

		p_work->size = p_work->size + p_work->next->size;
		Free_Node* p_temp = p_work->next;
		if (p_temp->next == NULL)//如果空闲区为双向链表的最后一个结点，则不需要设置前驱指针
		{
			p_work->next = NULL;
		}
		else {
			p_work->next = p_temp->next;//如果空闲区非双向链表的最后一个结点，需要设置前驱指针
			p_temp->next->prev = p_work;
		}
		delete p_temp;
		p_work->number = 0;

	}
	else if (p_work->prev->status == true && p_work->next->status == true)//释放区的前、后邻接分区都是空闲区。
	{

		Free_Node* p_tempprev = p_work->prev;
		Free_Node* p_tempnext = p_work->next;
		p_tempprev->size = (p_tempprev->size + p_work->size + p_tempnext->size);
		if (p_tempnext->next == NULL)//释放区的后邻接分区是空闲区的双向链表的最后一个结点，则不需要设置前驱指针
		{
			p_tempprev->next = NULL;
		}
		else {
			p_tempnext->next->prev = p_tempprev;//释放区的后邻接分区非空闲区双向链表的最后一个结点，则需要设置前驱指针
			p_tempprev->next = p_tempnext->next;
		}
		delete p_work;
		delete p_tempnext;
	}
}

void MemorySpace::PrintALL_Table()
{
	//输出完整链表
	
		cout << "主存中的所有分区：" << endl;
		Free_Node* p_temp = head_free->next;
		while (p_temp)
		{
			cout << "进程名：" << p_temp->process_name << " 进程号：" << p_temp->number << " 分区大小：" << p_temp->size << " 分配状态：" 
				<< p_temp->status << " 首地址：" << p_temp->address << endl;
			p_temp = p_temp->next;
		}
	
}

void MemorySpace::Print_AllocationTable()//输出已分配说明表
{
	cout << "已分配说明表：" << endl;
	Allocation_Table* p_work = head_allocation->next;
	while (p_work)
	{
		cout << "进程号：" << p_work->allocation_number << " 起始地址：" << p_work->start_address << " 分区大小：" << p_work->size << endl;
		p_work = p_work->next;
	}
}

int main()
{
	int space;
	cout << "请输入主存的空间大小：";
	cin >> space;
	MemorySpace Space(space);
	int choice;
	do
	{
		cout << endl;
		cout << "      1. 进程申请主存空间  " << endl;
		cout << "      2. 进程释放主存空间  " << endl;
		cout << "      3. 打印主存所有分区 " << endl;
		cout << "      4. 打印已分配区表" << endl;
		cout << "      0. 退 出" << endl << endl;
		cout << "输入命令：";
		cin >> choice;
		switch (choice)
		{
		case 1:
			Space.ApplicationSpace();
			break;
		case 2:
			Space.FreeSpace();
			break;
		case 3:
			Space.PrintALL_Table();
			break;
		case 4:
			Space.Print_AllocationTable();
			break;
		case 0:
			break;
		default:
			cout << "错误操作！" << endl;
			exit(1);
			break;
		}
	} while (choice != 0);
	return 0;
}