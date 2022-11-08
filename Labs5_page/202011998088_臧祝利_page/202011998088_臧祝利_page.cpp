#include <iostream>
#include <cstdio>
using namespace std;

#define N 64
#define LENGTH 10

struct
{
    int lNumber; //页号
    int pNumber; //物理块号
    int dNumber; //在磁盘上的位置
    int write;   //修改位标志
    int flag;    //存在位标志
} page[N];

int p[N]; //存放页号的队列

int lnumber, dnumber, pnumber;       //输入的页号,辅存地址,主存块
int cnt = 0;                         //页数
int m = 0;                           //给进程的内存块数
unsigned logicaddress, pysicaddress; //逻辑地址，物理地址
int option;                          //指令
int cursor;                          //指针
int pageout;                         //出队页面
unsigned pagenum;                    //计算得到的页号

void Judge(unsigned logicaddress)
{
    if (pagenum > cnt - 1) //发生越界
    {
        printf("不存在此页!\n");
    }
    else
    {
        if (page[pagenum].flag == 0) //发生缺页中断
        {
            printf("发生缺页中断: %d\n", pagenum);
            pageout = p[cursor];          //取得最先进来的页号
            p[cursor] = pagenum;          //替换页号
            if (page[pagenum].write == 1) //被修改过，要写回页面
            {
                printf("将%d页写回磁盘第%d块\n", pageout, page[pageout].dNumber);
            }
            printf("淘汰主存块%d中的页%d,从磁盘第%d块中调入页%d\n", page[pageout].pNumber, pageout, page[pagenum].dNumber, pagenum);
            page[pagenum].pNumber = page[pageout].pNumber; //装入pageout页的主存块中
            page[pagenum].flag = 1;                        //存在，置为1
            page[pagenum].write = 0;                       //没被修改，置为0
            page[pageout].flag = 0;                        //不存在，置为0
            cursor = (cursor + 1) % cnt;
        }
    }
}

void print()
{
    printf("-----------页表-------------\n");
    for (int i = 0; i < cnt; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\n", page[i].lNumber, page[i].pNumber, page[i].dNumber, page[i].flag, page[i].write);
    }

    printf("-----------页表-------------\n");
}
int main()
{
    printf("输入页表的信息,创建页表(若页号为-1,则结束输入)\n");
    for (int i = 0; i < N; i++)
    {
        printf("输入页号和辅存地址:");
        scanf("%d %d", &lnumber, &dnumber);
        if (lnumber == -1) //结束输入
        {
            break;
        }
        page[i].lNumber = lnumber; //存入页号和磁盘位置
        page[i].dNumber = dnumber;
        page[i].write = 0; //没有被修改过
        cnt++;             //输入的总个数
    }

    printf("输入主存块号,主存块数要小于%d,(以-1结束):", cnt);
    for (int i = 0; i < cnt; i++)
    {
        scanf("%d", &pnumber); //输入块号
        if (pnumber == -1)     //结束输入
        {
            break;
        }
        page[i].pNumber = pnumber; //存储块号
        page[i].flag = 1;          //存在置为1；
        p[i] = i;                  //将页调入队列中
    }

    // print();
    while (1)
    {
        printf("输入指令性质(1-修改, 0-不修改，其他-结束程序运行)和逻辑地址:");
        scanf("%d %d", &option, &logicaddress);
        if (option != 1 && option != 0)
        {
            printf("程序退出!\n");
            return 0;
        }
        pagenum = logicaddress >> LENGTH; //求得页号
        Judge(logicaddress);              //判断是否越界或缺页中断
        if (option == 1)                  //该页修改，修改位置1·
        {
            page[pagenum].write = 1;
        }
        else if (option == 0) //不做操作
        {
        }
        pysicaddress = page[pagenum].pNumber * (1 << 10) + logicaddress % (1 << 10); //计算物理地址
        printf("逻辑地址是%d, 对应的物理地址是%d\n", logicaddress, pysicaddress);    //输出地址
        // print();
    }
    return 0;
}