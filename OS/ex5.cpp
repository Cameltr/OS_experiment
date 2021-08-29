#include<iostream>
#include<iomanip>
 
using namespace std;
 
//定义变量并初始化
//进程的最大数
int N=10;
//资源的最大数
int M=50;
//可利用资源向量
int Available[50] = {0};
int Available_A[50] = {0};
//最大需求量
int Max[10][50] = {0};
//已分配资源量
int Allocation[10][50] = {0};
int Allocation_A[10][50] = {0};
//尚需资源数目
int Need[10][50] = {0};
int Need_N[10][50] = {0};
//请求资源向量
int Request[100]={0};
//工作向量
int Work[50] = {0};
//分配向量
int Finish[50] = {0};
//安全序列
int Security[10] = {0};	
//判断值
int Bool = 0;
//暂存矩阵	
int Temp[50] = {0};	
//资源名称
char Name[20] = {0};
 
//初始化资源数目M、输入每一种资源的名称和数目以及进程数目N	
void input_1(){	
	int m,n;
	cout<<"请输入资源的种类数目：";
	cin>>m;
	if(m<=M){
		M=m;
		//输入每一种资源的名称和数目Available
		for(int i=1;i<=M;i++){
			cout<<"请输入第"<<i<<"种资源的名称：";
			cin>>Name[i-1];
			cout<<"请输入第"<<i<<"种资源的数目：";
			cin>>Available[i-1];
		}
		cout<<endl;
	}
	else
		cout<<"资源数目太多，请重新输入···"<<endl;
	
	cout<<"请输入进程的数目：";
	cin>>n;
	if(n<=N){
		N=n;
		cout<<endl;
	}
	else
		cout<<"进程太多，请重新输入···"<<endl;
	
}
 
//输入最大需求矩阵和分配矩阵	
void input_2(){	
	int i,j; 
	//最大需求矩阵
	cout<<"请输入各进程的最大需求矩阵的值("<<N<<"*"<<M<<"矩阵)[Max]:"<<endl;
	for(i=0;i<N;i++){
		for(j=0;j<M;j++){
			cin>>Max[i][j];
		}
	}
	
	//分配矩阵
	cout<<"请输入各进程的分配矩阵的值("<<N<<"*"<<M<<"矩阵)[Allocation]:"<<endl;
	for(i=0;i<N;i++){
		for(j=0;j<M;j++){
			cin>>Allocation[i][j];
		}
	}
	
}
 
//计算系统目前可用资源和需求矩阵
void input_4();
void input_3(){	
	int i,j;
	Bool=1;
	//判断分配是否合理（看是否有分配大于最大需求）
	for(i=0;i<N;i++){
		for(j=0;j<M;j++){
			if(Allocation[i][j]>Max[i][j]){
				cout<<"不合理分配"<<endl;
				Bool=0;
				break;
			}
		}
	}	
	//如果判断合理 
	if(Bool==1){
		//计算系统目前可用资源
		for(j=0;j<M;j++){
			for(i=0;i<N;i++){
				Temp[j] += Allocation[i][j];
			}
			Available[j] -= Temp[j];
		}
	
		//得到需求矩阵
		for(i=0;i<N;i++){
			for(j=0;j<M;j++){
				Need[i][j] = Max[i][j]-Allocation[i][j];
			}
		}
	}
	//如果判断分配不合理 
	else
		input_4();
}
 
//重新输入
void input_4(){	
	int i,j;	
	cout<<"请重新输入各进程的分配矩阵的值("<<N<<"*"<<M<<"矩阵)[Allocation]:"<<endl;
	for(i=0;i<N;i++){
		for(j=0;j<M;j++){
			cin>>Allocation[i][j];
		}
	}
	//继续判断 
	input_3();
}
 
//显示信息
void show(){	
	cout<<"*************************************************************"<<endl;
	cout<<"系统目前可分配资源数目[Available]:"<<endl;	
	for(int i=0;i<M;i++){
		cout<<setw(3)<<Name[i];
	}
	cout<<endl;
	for(int i=0;i<M;i++){
		cout<<setw(3)<<Available[i];
	}
	cout<<endl;
	
	cout<<"系统当前的资源分配情况如下："<<endl;
	cout<<"           Max          Allocation         Need"<<endl;
	cout<<"进程名 ";
	for(int j=0;j<3;j++){
		for(int i=0;i<M;i++)
			cout<<setw(3)<<Name[i];
		cout<<"      ";
	}
	cout<<endl;
	//打印矩阵
	for(int i=0;i<N;i++){
		cout<<"  P"<<i<<"   ";
		for(int j=0;j<M;j++)
			cout<<setw(3)<<Max[i][j];
		cout<<"      ";
		for(int j=0;j<M;j++)
			cout<<setw(3)<<Allocation[i][j];
		cout<<"      ";
		for(int j=0;j<M;j++)
			cout<<setw(3)<<Need[i][j];
		cout<<endl;
	}
}
 
//判断此时系统是否是安全的
int safetycheck(){	
	int i,j;
	for(j=0;j<M;j++){
		Work[j]=Available[j];
	}	
	int s=0;
	//本层循环保证进行N次排查，使得前几次排查过程中不能满足的进程能继续判断 
	for(int a=0;a<N;a++){
		//i=0;
		//本层循环控制从0~N-1个进程的判断 
		for(i=0;i<N;i++){
			//判断进程资源请求状态是否为未完成	
			if(Finish[i]==0){
				//判断每一种资源的需求是否小于可用 
				j=0;
				while(j<M){
					if(Need[i][j]<=Work[j]){
						//该条件满足时，说明符合分配规则，进行试分配 
						if(j==M-1){
							//分配资源
							for(int b=0;b<M;b++){
								Work[b] += Allocation[i][b];
							}
							//修改状态 
							Finish[i]=1;
							//记录安全顺序 
							Security[s]=i;
							s++;
						}
						j++;
					}
					//只要有一种需求不能被满足，则跳过 
					else
						break;
				}
			}	
		}
	}
	
	Bool=1;
	//判断是否有没有得到分配的进程
	//如果有，使Bool的值改为 0 
	for(j=0;j<N;j++){
		if(Finish[j]==0){
			Bool=0;
			break;
		}
	}
	//Bool=1表示所有进程的Finish都等于 1 ，既系统是安全的 
	if(Bool==1){
		cout<<"系统是安全的！"<<endl;
			cout<<"存在一个安全序列是：";
			//输出安全序列
			for(int j=0;j<N;j++){
				cout<<"P"<<Security[j];
				if(j<N-1)
					cout<<"-->";
			}
			cout<<endl;
			return Bool;
	}
	//Bool！=1；说明有没有得到资源的进程，系统不安全 
	else{
		cout<<"系统不是安全的！"<<endl;
		return Bool;
	} 
}
 
//分配资源
int allocate(int p){	
	int j=0;
	Bool=1;
	int flag=1;
	//判断申请的合法性 
	for(j=0;j<M;j++){
		//判断请求是否大于需求 
		if(Request[j]>Need[p][j]){
			Bool=0;
			//大于时将 0 值返回给主调函数 
			return Bool;
			break;
		}
		//判断请求是否大于可用资源 
		else if(Available[j]<Request[j]){
			Bool=-1;
			//大于时将 -1 值返回给主调函数 
			return Bool;
			break;
		}
	}
	//当Bool值还是 1 ，说明请求合理，给予分配 
	if(Bool==1){
		for(j=0;j<M;j++){
			Allocation[p][j] += Request[j];
			Need[p][j] -= Request[j];
			Available[j] -= Request[j];
		}
		//判断进程是否完成，如果完成，回收进程，更新可用资源 
		for(j=0;j<M;j++){
			if(Need[p][j]!=0)
				flag=0;
				break;
		} 
		if(flag==1){
			for(j=0;j<M;j++){
				Available[j]+=Allocation[p][j];	
			}
		}
		return Bool; 
	}
}
 
//发出资源申请请求并检验请求的合理性
void bank(){	
	int i,j,p;	
	//数据备份
	for(i=0;i<N;i++){
		Available_A[i]=Available[i];
		for(j=0;j<M;j++){
			Allocation_A[i][j]=Allocation[i][j];
			Need_N[i][j]=Need[i][j];
		}
	}
	
	//接收申请
	cout<<"请输入申请资源的进程数:（0--"<<N-1<<"）"<<endl;
	cin>>p;
	cout<<"请输入申请资源的组合数:（1*"<<M<<"的矩阵）"<<endl;
	for(j=0;j<M;j++){
		cin>>Request[j];
	}
	
	//调用分配算法
	int value_1=allocate(p);
	//如果请求不合法 
	if(value_1==0){
		cout<<"非法请求"<<endl;
		//退出该函数 
		return; 
	}
	if(value_1==-1){
		cout<<"系统资源不足"<<endl;
		//退出该函数 
		return;
	}
	//如果请求符合规范 
	if(value_1==1){		
		//执行安全性算法
		int value_2=safetycheck();
		//符合安全性算法 
		if(value_2==1){
			cout<<"资源已分配"<<endl;
			show();
		}
		//不符合安全性算法 
		else{
			cout<<"该请求导致系统不安全，不进行分配。"<<endl;
			//数据还原  
			for(i=0;i<N;i++){
				Available[i]=Available_A[i];
				for(j=0;j<M;j++){
					Allocation[i][j]=Allocation_A[i][j];
					Need[i][j]=Need_N[i][j];
				}
			}
			show(); 
		}	
	}
}
 
//主函数
int main(){	
	char choice;
	int a=1; 	
	//输入 
	input_1();
	input_2();
	input_3();
	//显示 
	show();
	//判断输入的状态是否安全 
	safetycheck();
	//功能选项 
	while(a==1){
		cout<<"\t-------------------银行家算法演示------------------"<<endl;
		cout<<"                     R:请求分配   "<<endl;	
		cout<<"                     E:退出       "<<endl;
		cout<<"\t---------------------------------------------------"<<endl;
		cout<<"请选择：";
		cin>>choice;
		switch(choice){
			case 'R':
				bank();
				break;
			case 'E':
				a=0;
				break;
			default: cout<<"请正确选择!"<<endl;
			break;
		}
	}		
	return 0;
}
/*示例：
4
A
3
B
12
C
14
D
14
5
max：
0 0 4 4
2 7 5 0
0 9 8 4
0 6 6 10
3 6 10 10
allocate:
0 0 3 2
1 0 0 0
0 0 3 2
0 0 1 4
1 3 5 4
*/ 