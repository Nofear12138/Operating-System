#include <stdio.h>
#define n 64 /*模拟实验中假定的页表长度，即最多允许程序含有多少页；假定页面大小为1K*/
#define length 10
struct {
	int lnumber;/*页号*/
	int flag; /*表示该页是否在主存，"1"表示在主存中，"0"表示不在*/
	int pnumber;/*该页所在主存块的块号*/
	int write; /*该页是否被修改过，"1"表示修改过，"0"表示没有修改过*/
	int dnumber;/*该页存放在磁盘上的位置，即磁盘块号*/
}page[n]; /*页表定义*/ 
int m; /*m为该作业在主存中的主存块块数*/
int page_length;/*页表实际长度*/
int p[length]; /*存放在主存中的页的页号*/
int head; /*主存中页号队列，指向下次要被替换的页号*/

/*缺页中断处理函数，采用FIFO页面置换算法*/
void page_interrupt(int lnumber) {
	int j;
	printf("发生缺页中断* %d\n",lnumber);
	/*淘汰页*/
	j = p[head];
	p[head] = lnumber;/* 装入替换后的新页号*/
	head = (head+1)%m;
	if (page[j].write == 1)
		printf("将页%d写回磁盘第%d块\n", j, page[j].dnumber);
	/*修改页表*/
	page[j].flag = 0;/* 第j页被换出，第j页存在标志改为"0"*/
	page[lnumber].pnumber = page[j].pnumber;/* 第lnumber页装入原来第j页所在的物理块*/
	page[lnumber].flag = 1;/* 第lnumber页存在标志改为"1"*/
	page[lnumber].write = 0;/* 第lunmber页修改标志改为"0"*/
	printf("淘汰主存块%2d中的页%2d, 从磁盘第%d块中调入页%2d\n", page[j].pnumber, j,page[lnumber].dnumber, lnumber);
}/*缺页中断处理函数结束*/

/*命令处理函数*/
void command(unsigned laddress,int write) {
	unsigned paddress, ad, pnumber;//分别存放物理地址、页内地址和外存地址（盘块号）；
	int lnumber;//存放页号；
	lnumber = laddress>>10;//页号=逻辑地址/2^10                 /*先计算出页号（高6位）和页内地址（低10位）；*/
	ad = laddress%1024;//页内地址利用位运算来取出后10位           
	
	kk:
	/*判断页号是否越界，若越界，显示越界不再进行地址变换；*/
	if(lnumber >= page_length) {
		printf("不存在该页\n");
		return;
	}
	/*若访问的页在内存，查找对应的物理块，计算物理地址；若不在内存，先调用page_interrupt(int lnumber)进行页面置换，再进行地址变换。*/
	if(page[lnumber].flag == 1) {
		pnumber = page[lnumber].pnumber;
		paddress = pnumber<<10|ad;
		printf("逻辑地址是:%d,对应物理地址是:%d\n", laddress, paddress);
		if(write == 1)
			page[lnumber].write = 1;
	}
	else {
		page_interrupt(lnumber);
		goto kk;
	}
}
  
  
/*命令处理函数结束*/
int main( ) {
	int lnumber, flag, pnumber, write, dnumber;
	unsigned laddress;
	int i;
	/*输入页表的信息，页号从0开始，依次编号，创建页表page*/
	printf("输入页表的信息, 创建页表 (若页号为-1, 则结束输入)\n");
	//printf("输入页号和辅存地址:");
	//scanf("%d%d",&lnumber,&dnumber);
	i = 0;
	do {
		printf("输入页号和辅存地址:");
		scanf("%d%d", &lnumber, &dnumber);
		page[i].lnumber = lnumber; 
		page[i].flag = 0;
		page[i].write = 0;
		page[i].dnumber = dnumber;
		i++;
	}while(lnumber != -1);
	page_length = i-1;//因为多输入了一个-1；
	printf("输入主存块号, 主存块数要小于%d (以-1结束):", i-1);
	scanf("%d", &pnumber);
	m = 0;
	head = 0;
	while(pnumber != -1) {
		if(m <= i) { 
			page[m].pnumber = pnumber;
			page[m].flag = 1;
			p[m] = m;//对数组P进行初始化，一开始程序的前m+1页调入内存
			m++;
		}
	    scanf("%d", &pnumber);
	}
	printf("输入指令性质 (1-修改, 0-不需要, 其他-结束程序运行) 和逻辑地址:");
	scanf("%d%u", &write, &laddress);
	while(write == 0 || write == 1) {
		command(laddress, write);/* 执行指令*/
		printf("输入指令性质 (1-修改, 0-不需要, 其他-结束程序运行) 和逻辑地址:");
		scanf("%d%u", &write, &laddress);
	}
	return 0;
}

