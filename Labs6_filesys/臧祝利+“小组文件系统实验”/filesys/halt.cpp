#include <stdio.h>
#include <string.h>
#include "filesys.h"

void halt(int username){
	int i,j;

	/*1. write back the current dir */ 
	char s[] = "..";
	char* ss = s;
	chdir(s);
	iput(cur_path_inode);

	/*2. free the u_ofile and sys_ofile and inode*/
	for(i=0; i<USERNUM; i++){
		if(user[i].u_uid != 0){
			for(j=0; j<NOFILE;j++){
				if(user[i].u_ofile[j] != SYSOPENFILE + 1){
					close(i,j);
					user[i].u_ofile[j] = SYSOPENFILE + 1;
				}//if
			}//for
		}//if
	}//for

	/*3. write back the filesys to the disk*/
	memcpy(disk+BLOCKSIZ, &filsys, sizeof(struct filsys));

	/*4. close the file system column*/

	/*5. say GOOD BYE to all the user*/
	printf("\nGood Bye. See You Next Time. Please turn off the switch\n");
	char txtname[100];  //�ļ�����
	itoa(username, txtname, 10);  //������ת�����ַ���
	strcat(txtname, ".txt");  //�����ļ���׺
	char filename[100] = "user/";  //����Ŀ¼��
	strcat(filename, txtname);  //�����ļ�·��
	FILE* fp;
	fp = fopen(filename, "wb");  //���ļ�
	fwrite(disk, sizeof(char), (DINODEBLK + FILEBLK + 2) * BLOCKSIZ, fp);  //д�����
	fclose(fp);
	exit(0);
}





























