#include <stdio.h>
#include <string.h>
#include "filesys.h" 

void format(){
	struct inode *inode;
	struct direct dir_buf[BLOCKSIZ/(DIRSIZ+4)];
	struct pwd passwd[32];
	unsigned int block_buf[BLOCKSIZ/sizeof(int)];
	int i,j;

	//��ʼ��Ӳ�� 
	memset(disk, 0x00, ((DINODEBLK+FILEBLK+2)*BLOCKSIZ));

	/* 0.initialize the passwd */
	

	/* 1.creat the main directory and its sub dir etc and the file password */

	inode = iget(0);   /* 0 empty dinode id*/
	inode->di_number = 1;			//??��i�ڵ��к���????
	inode->di_mode = DIEMPTY;
	iput(inode);

	inode = iget(1);   /* 1 main dir id*/
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_size = 3*(DIRSIZ + 4);
	inode->di_addr[0] = 0; /*block 0# is used by the main directory*/
	
	strcpy(dir_buf[0].d_name,"..");
	dir_buf[0].d_ino = 1;
	strcpy(dir_buf[1].d_name,".");
	dir_buf[1].d_ino = 1;
	strcpy(dir_buf[2].d_name,"etc");
	dir_buf[2].d_ino = 2;

	memcpy(disk+DATASTART, &dir_buf, 3*(DIRSIZ+4));
	iput(inode);

	inode = iget(2);  /* 2  etc dir id */ 
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_size = 3*(DIRSIZ + 4);
	inode->di_addr[0] = 1; /*block 1# is used by the etc directory*/
	
	strcpy(dir_buf[0].d_name,"..");
	dir_buf[0].d_ino = 1;
	strcpy(dir_buf[1].d_name,".");
	dir_buf[1].d_ino = 2;
	strcpy(dir_buf[2].d_name,"password");
	dir_buf[2].d_ino = 3;

	memcpy(disk+DATASTART+BLOCKSIZ*1, dir_buf, 3*(DIRSIZ+4));
	iput(inode);

	inode = iget(3);  /* 3  password id */
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIFILE;
	inode->di_size = BLOCKSIZ;
	inode->di_addr[0] = 2; /*block 2# is used by the password file*/

	usernumber = getusernumber();
	for (i = 0;i < usernumber;i++) {
		int p_uid, p_gid;
		char pswd[100];

		char txtname[100];  //�ļ�����
		itoa(i, txtname, 10);  //������ת�����ַ���
		strcat(txtname, ".txt");  //�����ļ���׺,����5.txt,����֮��ֵpwd
		char filename[100] = "user_info/";  //����Ŀ¼��
		strcat(filename, txtname);  //�����ļ�·��
		FILE* fp;
		fp = fopen(filename, "r");  //���ļ�

		fscanf(fp, "%d", &p_uid);
		fscanf(fp, "%s", pswd);
		fscanf(fp, "%d", &p_gid);
		passwd[i].p_uid = p_uid;
		passwd[i].p_gid = p_gid;
		strcpy(passwd[i].password,pswd);  

	}
	for (i=usernumber; i<PWDNUM; i++){
		passwd[i].p_uid = 0;
		passwd[i].p_gid = 0;
		strcpy(passwd[i].password, "           ");  // PWDSIZ " "
	}
	
	memcpy(pwd, passwd, 32*sizeof(struct pwd));
	memcpy(disk+DATASTART+BLOCKSIZ*2, passwd, BLOCKSIZ);
	iput(inode);

	/*2. initialize the superblock */

	filsys.s_isize = DINODEBLK;
	filsys.s_fsize = FILEBLK;

	filsys.s_ninode = DINODEBLK * BLOCKSIZ/DINODESIZ - 4;
	filsys.s_nfree = FILEBLK - 3;

	for (i=0; i < NICINOD; i++){
		/* begin with 4,    0,1,2,3, is used by main,etc,password */
		filsys.s_inode[i] = 4+i;
	}

	filsys.s_pinode = 0;
	filsys.s_rinode = NICINOD + 4; 

	block_buf[NICFREE-1] = FILEBLK+1;  /*FILEBLK+1 is a flag of end*/
	for (i=0; i<NICFREE-1; i++)
		block_buf[NICFREE-2-i] = FILEBLK-i-1;			//�����һ�����ݿ鿪ʼ����??????

	memcpy(disk+DATASTART+BLOCKSIZ*(FILEBLK-NICFREE), block_buf, BLOCKSIZ);
	for (i=FILEBLK-2*NICFREE+1; i>2; i-=NICFREE){
		for (j=0; j<NICFREE;j++){
			block_buf[j] = i+j;
		}
		memcpy(disk+DATASTART+BLOCKSIZ*(i-1), block_buf, BLOCKSIZ);
	}
	i += NICFREE;
	j = 1;
	for (; i>3; i--)
	{
		filsys.s_free[NICFREE-j] = i-1; 
		j ++;
	}
	
	filsys.s_pfree = NICFREE - j+1; 
	memcpy(disk+BLOCKSIZ, &filsys, sizeof(struct filsys));
	return;
	
}
