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
	char txtname[100];  //文件名称
	itoa(username, txtname, 10);  //将数字转换成字符串
	strcat(txtname, ".txt");  //生成文件后缀
	char filename[100] = "user/";  //增加目录名
	strcat(filename, txtname);  //完整文件路径
	FILE* fp;
	fp = fopen(filename, "wb");  //打开文件
	fwrite(disk, sizeof(char), (DINODEBLK + FILEBLK + 2) * BLOCKSIZ, fp);  //写入磁盘
	fclose(fp);
	exit(0);
}





























