#include <stdio.h>
#include <stdlib.h>

#define RESOURCE_NUM 10 // �����Դ��
#define PROCESS_NUM 10  // ��������
#define Length 10       // ��Դ���Ƶ���󳤶�

int Available[RESOURCE_NUM];               //������Դ����
int MaxRequest[PROCESS_NUM][RESOURCE_NUM]; //����������
int Allocation[PROCESS_NUM][RESOURCE_NUM]; //�������
int Need[PROCESS_NUM][RESOURCE_NUM];       //�������
bool Finish[PROCESS_NUM];                  //�ж�ϵͳ�Ƿ����㹻��Դ�������������
int safeSeries[PROCESS_NUM];               //��ȫ����
int Request[RESOURCE_NUM];                 //������Դ����

int resource;                            //�������Դ����
int process;                             //����Ľ�������
char ResourceName[RESOURCE_NUM][Length]; //����Դ����

void showInfo()
{
    printf("ϵͳĿǰ���õ���Դ[Available]:\n");
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
    printf("����");
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
    int work[RESOURCE_NUM]; //���ϵͳ���ṩ��Դ��
    int safe_index = 0;     //�����±�
    int num = 0;
    for (int i = 0; i < resource; i++)
    {
        work[i] = Available[i]; //��ʼ��work
    }
    for (int i = 0; i < process; i++)
    {
        Finish[i] = false; //��ʼ��Finish
    }

    for (int i = 0; i < process; i++) //��ȫ����
    {
        num = 0; //��Ҫ����ĿС�ڿ�������Դ������Դ����
        for (int j = 0; j < resource; j++)
        {
            if (Finish[i] == false && Need[i][j] <= work[j])
            {
                num++;
                //ÿ����Դ�����ڲſɷ���
                if (num == resource)
                {
                    for (int k = 0; k < resource; k++)
                    {
                        work[k] += Allocation[i][k]; //���ĵ�ǰ�ɷ������Դ
                    }
                    Finish[i] = true;
                    safeSeries[safe_index++] = i;
                    i = -1; //ʹ��ÿ�β�ѯ���ӵ�һ�����̿�ʼ��
                }
            }
        }
    }

    for (int i = 0; i < process; i++)
    {
        if (Finish[i] == false)
        {
            printf("ϵͳ�����ڰ�ȫ����!\n");
            return false;
        }
    }
    showInfo();
    printf("ϵͳ��ȫ!\n");
    printf("����һ����ȫ����:");
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
    bool flag = true; //�ж��ܷ�����㷨����һ��
    int processID;    //��ѡ��Ľ��̺�
    printf("����������������Դ���̺�(0-%d):", process - 1);
    scanf("%d", &processID);

    printf("���������%dҪ�������Դ����:\n", processID);
    for (int i = 0; i < resource; i++)
    {
        printf("%s:", ResourceName[i]);
        scanf("%d", &Request[i]);
    }

    for (int i = 0; i < resource; i++)
    {
        if (Request[i] > Need[processID][i]) //�ж������Ƿ��������
        {
            printf("����%d�������Դ��������Ҫ����Դ", processID);
            printf("���䲻�����������!\n");
            flag = false;
            break;
        }
        else
        {
            if (Request[i] > Available[i]) //�ж������Ƿ���ڵ�ǰ�ɷ�����Դ
            {
                printf("����%d�������Դ����ϵͳ���ڿ����õ���Դ", processID);
                printf("\n");
                printf("ϵͳ���㹻��Դ���������!\n");
                flag = false;
                break;
            }
        }
    }
    //���Է�����Դ��Ѱ�Ұ�ȫ���У�
    if (flag)
    {
        set(processID); //���Է���
        if (!issafe())  //Ѱ�Ұ�ȫ���У��������ȫ���ָ�״̬
        {
            reset(processID);
            printf("�������!\n");
        }
    }
}

int main()
{
    printf("*****************�������ϵͳ���̵���ʵ��*****************\n");
    printf("����������ϵͳ�ɹ���Դ���������:");
    scanf("%d", &resource);
    for (int i = 0; i < resource; i++)
    {
        printf("������Դ%d������:", i + 1);
        scanf("%s", ResourceName[i]);
        printf("������Դ%d������:", i + 1);
        scanf("%d", &Available[i]);
    }
    printf("������̵�����:");
    scanf("%d", &process);
    printf("��������̵����������(%d*%d����)[MaxRequest]:\n", process, resource);
    for (int i = 0; i < process; i++)
    {
        for (int j = 0; j < resource; j++)
        {
            scanf("%d", &MaxRequest[i][j]);
        }
    }
    printf("����������Ѿ��������Դ��(%d*%d����)[Allocation]:\n", process, resource);
    for (int i = 0; i < process; i++)
    {
        for (int j = 0; j < resource; j++)
        {
            scanf("%d", &Allocation[i][j]);
        }
    }
    //����Need����
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
        printf("*****************���м��㷨��ʾ*****************\n");
        printf("\t\t1:������Դ\n\t\t2:��ʾ�������\n\t\t0:�뿪\n");
        printf("***********************************************\n");
        printf("��ѡ���ܺ�:");
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
            printf("���뿪!\n");
            break;
        }
    }
    return 0;
}