#include <iostream>
#include <cstdio>
using namespace std;

#define N 64
#define LENGTH 10

struct
{
    int lNumber; //ҳ��
    int pNumber; //������
    int dNumber; //�ڴ����ϵ�λ��
    int write;   //�޸�λ��־
    int flag;    //����λ��־
} page[N];

int p[N]; //���ҳ�ŵĶ���

int lnumber, dnumber, pnumber;       //�����ҳ��,�����ַ,�����
int cnt = 0;                         //ҳ��
int m = 0;                           //�����̵��ڴ����
unsigned logicaddress, pysicaddress; //�߼���ַ�������ַ
int option;                          //ָ��
int cursor;                          //ָ��
int pageout;                         //����ҳ��
unsigned pagenum;                    //����õ���ҳ��

void Judge(unsigned logicaddress)
{
    if (pagenum > cnt - 1) //����Խ��
    {
        printf("�����ڴ�ҳ!\n");
    }
    else
    {
        if (page[pagenum].flag == 0) //����ȱҳ�ж�
        {
            printf("����ȱҳ�ж�: %d\n", pagenum);
            pageout = p[cursor];          //ȡ�����Ƚ�����ҳ��
            p[cursor] = pagenum;          //�滻ҳ��
            if (page[pagenum].write == 1) //���޸Ĺ���Ҫд��ҳ��
            {
                printf("��%dҳд�ش��̵�%d��\n", pageout, page[pageout].dNumber);
            }
            printf("��̭�����%d�е�ҳ%d,�Ӵ��̵�%d���е���ҳ%d\n", page[pageout].pNumber, pageout, page[pagenum].dNumber, pagenum);
            page[pagenum].pNumber = page[pageout].pNumber; //װ��pageoutҳ���������
            page[pagenum].flag = 1;                        //���ڣ���Ϊ1
            page[pagenum].write = 0;                       //û���޸ģ���Ϊ0
            page[pageout].flag = 0;                        //�����ڣ���Ϊ0
            cursor = (cursor + 1) % cnt;
        }
    }
}

void print()
{
    printf("-----------ҳ��-------------\n");
    for (int i = 0; i < cnt; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\n", page[i].lNumber, page[i].pNumber, page[i].dNumber, page[i].flag, page[i].write);
    }

    printf("-----------ҳ��-------------\n");
}
int main()
{
    printf("����ҳ�����Ϣ,����ҳ��(��ҳ��Ϊ-1,���������)\n");
    for (int i = 0; i < N; i++)
    {
        printf("����ҳ�ź͸����ַ:");
        scanf("%d %d", &lnumber, &dnumber);
        if (lnumber == -1) //��������
        {
            break;
        }
        page[i].lNumber = lnumber; //����ҳ�źʹ���λ��
        page[i].dNumber = dnumber;
        page[i].write = 0; //û�б��޸Ĺ�
        cnt++;             //������ܸ���
    }

    printf("����������,�������ҪС��%d,(��-1����):", cnt);
    for (int i = 0; i < cnt; i++)
    {
        scanf("%d", &pnumber); //������
        if (pnumber == -1)     //��������
        {
            break;
        }
        page[i].pNumber = pnumber; //�洢���
        page[i].flag = 1;          //������Ϊ1��
        p[i] = i;                  //��ҳ���������
    }

    // print();
    while (1)
    {
        printf("����ָ������(1-�޸�, 0-���޸ģ�����-������������)���߼���ַ:");
        scanf("%d %d", &option, &logicaddress);
        if (option != 1 && option != 0)
        {
            printf("�����˳�!\n");
            return 0;
        }
        pagenum = logicaddress >> LENGTH; //���ҳ��
        Judge(logicaddress);              //�ж��Ƿ�Խ���ȱҳ�ж�
        if (option == 1)                  //��ҳ�޸ģ��޸�λ��1��
        {
            page[pagenum].write = 1;
        }
        else if (option == 0) //��������
        {
        }
        pysicaddress = page[pagenum].pNumber * (1 << 10) + logicaddress % (1 << 10); //���������ַ
        printf("�߼���ַ��%d, ��Ӧ�������ַ��%d\n", logicaddress, pysicaddress);    //�����ַ
        // print();
    }
    return 0;
}