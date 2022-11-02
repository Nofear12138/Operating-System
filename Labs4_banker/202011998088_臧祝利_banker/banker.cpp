#include <stdio.h>
#include <stdlib.h>

#define RESOURCE_NUM 10 // 最大资源数
#define PROCESS_NUM 10  // 最大进程数
#define Length 10       // 资源名称的最大长度

int Available[RESOURCE_NUM];               //可用资源矩阵
int MaxRequest[PROCESS_NUM][RESOURCE_NUM]; //最大需求矩阵
int Allocation[PROCESS_NUM][RESOURCE_NUM]; //分配矩阵
int Need[PROCESS_NUM][RESOURCE_NUM];       //需求矩阵
bool Finish[PROCESS_NUM];                  //判断系统是否有足够资源分配给各个进程
int safeSeries[PROCESS_NUM];               //安全序列
int Request[RESOURCE_NUM];                 //请求资源向量

int resource;                            //输入的资源数量
int process;                             //输入的进程数量
char ResourceName[RESOURCE_NUM][Length]; //各资源名称

void showInfo()
{
    printf("系统目前可用的资源[Available]:\n");
    for (int i = 0; i < resource; i++)
    {
        printf("%s\t", ResourceName[i]);
    }
    printf("\n");
    for (int i = 0; i < resource; i++)
    {
        printf("%d\t", Available[i]);
    }
    printf("\n");

    printf("\tMax  Allocation  Need\n");
    printf("进程");
    for (int i = 0; i < 3; i++)
    {
        printf("\t");
        for (int j = 0; j < resource; j++)
        {
            printf("%s ", ResourceName[j]);
        }
    }
    printf("\n");

    for (int i = 0; i < process; i++)
    {
        printf("%d\t", i);
        for (int j = 0; j < resource; j++)
        {
            printf("%d ", MaxRequest[i][j]);
        }
        printf("\t");
        for (int j = 0; j < resource; j++)
        {
            printf("%d ", Allocation[i][j]);
        }
        printf("\t");
        for (int j = 0; j < resource; j++)
        {
            printf("%d ", Need[i][j]);
        }
        printf("\n");
    }
}

bool issafe()
{
    int work[RESOURCE_NUM]; //存放系统可提供资源量
    int safe_index = 0;     //序列下标
    int num = 0;
    for (int i = 0; i < resource; i++)
    {
        work[i] = Available[i]; //初始化work
    }
    for (int i = 0; i < process; i++)
    {
        Finish[i] = false; //初始化Finish
    }

    for (int i = 0; i < process; i++) //求安全序列
    {
        num = 0; //需要的数目小于可利用资源数的资源个数
        for (int j = 0; j < resource; j++)
        {
            if (Finish[i] == false && Need[i][j] <= work[j])
            {
                num++;
                //每类资源都少于才可分配
                if (num == resource)
                {
                    for (int k = 0; k < resource; k++)
                    {
                        work[k] += Allocation[i][k]; //更改当前可分配的资源
                    }
                    Finish[i] = true;
                    safeSeries[safe_index++] = i;
                    i = -1; //使得每次查询都从第一个进程开始；
                }
            }
        }
    }

    for (int i = 0; i < process; i++)
    {
        if (Finish[i] == false)
        {
            printf("系统不存在安全序列!\n");
            return false;
        }
    }
    showInfo();
    printf("系统安全!\n");
    printf("存在一个安全序列:");
    for (int i = 0; i < process; i++)
    {
        printf("%d", safeSeries[i]);
        if (i != process - 1)
        {
            printf("->");
        }
    }
    printf("\n");
    return true;
}

void set(int processID)
{
    for (int i = 0; i < resource; i++)
    {
        Available[i] = Available[i] - Request[i];
        Allocation[processID][i] = Allocation[processID][i] + Request[i];
        Need[processID][i] = Need[processID][i] - Request[i];
    }
    return;
}

void reset(int processID)
{
    for (int i = 0; i < resource; i++)
    {
        Available[i] = Available[i] + Request[i];
        Allocation[processID][i] = Allocation[processID][i] - Request[i];
        Need[processID][i] = Need[processID][i] + Request[i];
    }
    return;
}

void banker()
{
    bool flag = true; //判断能否进入算法的下一步
    int processID;    //所选择的进程号
    printf("请输入请求分配的资源进程号(0-%d):", process - 1);
    scanf("%d", &processID);

    printf("请输入进程%d要申请的资源个数:\n", processID);
    for (int i = 0; i < resource; i++)
    {
        printf("%s:", ResourceName[i]);
        scanf("%d", &Request[i]);
    }

    for (int i = 0; i < resource; i++)
    {
        if (Request[i] > Need[processID][i]) //判断申请是否大于需求
        {
            printf("进程%d申请的资源大于它需要的资源", processID);
            printf("分配不合理，不予分配!\n");
            flag = false;
            break;
        }
        else
        {
            if (Request[i] > Available[i]) //判断申请是否大于当前可分配资源
            {
                printf("进程%d申请的资源大于系统现在可利用的资源", processID);
                printf("\n");
                printf("系统无足够资源，不予分配!\n");
                flag = false;
                break;
            }
        }
    }
    //尝试分配资源，寻找安全序列；
    if (flag)
    {
        set(processID); //尝试分配
        if (!issafe())  //寻找安全序列；如果不安全，恢复状态
        {
            reset(processID);
            printf("不予分配!\n");
        }
    }
}

int main()
{
    printf("*****************单处理机系统进程调度实现*****************\n");
    printf("请首先输入系统可供资源种类的数量:");
    scanf("%d", &resource);
    for (int i = 0; i < resource; i++)
    {
        printf("输入资源%d的名称:", i + 1);
        scanf("%s", ResourceName[i]);
        printf("输入资源%d的数量:", i + 1);
        scanf("%d", &Available[i]);
    }
    printf("输入进程的数量:");
    scanf("%d", &process);
    printf("输入各进程的最大需求量(%d*%d矩阵)[MaxRequest]:\n", process, resource);
    for (int i = 0; i < process; i++)
    {
        for (int j = 0; j < resource; j++)
        {
            scanf("%d", &MaxRequest[i][j]);
        }
    }
    printf("输入各进程已经申请的资源量(%d*%d矩阵)[Allocation]:\n", process, resource);
    for (int i = 0; i < process; i++)
    {
        for (int j = 0; j < resource; j++)
        {
            scanf("%d", &Allocation[i][j]);
        }
    }
    //计算Need矩阵
    for (int i = 0; i < process; i++)
    {
        for (int j = 0; j < resource; j++)
        {
            Need[i][j] = MaxRequest[i][j] - Allocation[i][j];
        }
    }

    if (!issafe())
        exit(0);

    while (1)
    {
        printf("*****************银行家算法演示*****************\n");
        printf("\t\t1:分配资源\n\t\t2:显示分配情况\n\t\t0:离开\n");
        printf("***********************************************\n");
        printf("请选择功能号:");
        int option;
        scanf("%d", &option);
        if (option == 1)
        {
            banker();
        }
        else if (option == 2)
        {
            showInfo();
            continue;
        }
        else
        {
            printf("已离开!\n");
            break;
        }
    }
    return 0;
}
