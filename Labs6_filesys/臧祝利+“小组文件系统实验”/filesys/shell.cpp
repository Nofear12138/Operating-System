#include "filesys.h"
#define CLEN 20
#define CNUM 20
//enum ctype  
char commands[CNUM][CLEN]={
"exit",
"dir",
"mkdir",
"cd",
"mkfile",
"del",
"write",
"read",
"adduser",
"pwd",
"who",
"password",
"help"
};
int getcid(char *command){
	int i;
	if(command == NULL)
		return -1;
	for(i=0;i<CNUM;i++){
		if(strcmp(command,commands[i]) == 0)
			return i;
	}
	return -1;
}
int shell(int user_id,char *str){
	char seps[] =" \t\n\0";
	char* token,*tstr,*buf;
	unsigned short mode;
	int fd;
	int cid,size;
	token = strtok(str,seps);
	if(token == NULL)
		return 1;
	cid = getcid(token);
	switch(cid){
	case 1:
		_dir();
		break;
	case 2:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("mkdir命令的正确格式为mkdir dirname，请检查命令!\n");
			break;
		}
		mkdir(token);
		break;
	case 3:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("cd命令的正确格式为cd dirname，请检查命令!\n");
			break;
		}
		if (strcmp(token, "..") && chdir(token)!=-1) {  //如果不是上级目录，且目录存在
			strcpy(address_name[point_address], token);  //加入栈中
			point_address++;
		}
		else if (strcmp(token, "..") == 0) {  //否则返回上一级目录
			point_address--;
		}
		break;
	case 4:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("mkfile 命令的正确格式为mkfile filename [mode]，请检查命令!\n");
			break;
		}
		tstr =token;
		mode = DEFAULTMODE;
		token = strtok(NULL,seps);
		if(token != NULL){
			sscanf(token,"%o",&mode);
		}
		mode = mode|DIFILE|0700;
		fd = creat(user_id,tstr,mode);
		if(fd == -1){
			printf("创建文件失败！\n");
			break;
		}
		close(user_id,fd);
		break;
	case 5:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("del 命令的正确格式为del filename，请检查命令!\n");
			break;
		}
		delete(token);
		break;
	case 6:  //wirte
		mode = WRITE;
		token = strtok(NULL,seps);
		tstr = token;
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("write 命令的正确格式为write filename bytes，请检查命令!\n");
			break;
		}
		if(token[0] == '-'){
			if(token[1] == 'a')
				mode = FAPPEND;
		}else{
			sscanf(token,"%d",&size);
		}
		fd = open(user_id,tstr,char(mode));
		if (fd == -1) {
			printf("unable to write!\n");
			break;
		}

		buf = (char*)malloc(size);
		size = write(fd,buf,size);
		printf("%d bytes have been writed in file %s.\n",size,tstr);
		free(buf);
		close(user_id,fd);
		break;
	case 7:
		token = strtok(NULL,seps);
		tstr = token;
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("read 命令的正确格式为read filename bytes，请检查命令!\n");
			break;
		}
		sscanf(token,"%d",&size);
		fd = open(user_id,tstr,READ);
		if (fd == -1) {
			printf("unable to read\n");
			break;
		}
		buf = (char*)malloc(size+1);
		size = read(fd,buf,size);
		printf("%d bytes have been read in buf from file %s.\n",size,tstr);
		free(buf);
		close(user_id,fd);
		break;

	case 8:  //adduser
		int id, group;
		char* ids, * groups;
		char* pswd;
		token = strtok(NULL, seps);
		ids = token;
		token = strtok(NULL, seps);
		pswd = token;
		token = strtok(NULL, seps);
		if (token == NULL) {
			printf("adduser 命令的正确格式为adduser username password group\n");
			break;
		}
		groups = token;
		id = atoi(ids);
		group = atoi(groups);
		//printf("%d %d\n", id, group);
		adduser(id, pswd, group);
		break;

	case 9:  //pwd命令
		for (int i = 0; i < point_address - 1;i++) {
			printf("%s/", address_name[i]);
		}
		printf("%s\n", address_name[point_address - 1]);
		break;

	case 10:
		printf("id\t\tpswd\t\tgroup\n");
		printf("%d\t\t", pwd[user_id].p_uid);
		printf("%s\t\t", pwd[user_id].password);
		printf("%d\n", pwd[user_id].p_gid);
		//printf("暂时输出一下全部成员的username :\n");
		//usernumber = getusernumber();
		//for (int i = 0; i < usernumber;i++) {
			//printf("%d\t\t %s\n", pwd[i].p_uid,pwd[i].password);
		//}
		break;

	case 11:  //password，用来修改密码
		char* password, * idss;
		int usid;
		token = strtok(NULL, seps);
		idss = token;
		token = strtok(NULL, seps);
		if (token == NULL) {
			printf("password 命令的正确格式为password id new_password，请检查命令!\n");
			break;
		}
		password = token;
		usid = atoi(idss);
		modify_pswd(usid, password);

		break;

	case 12:
		token = strtok(NULL, seps);
		if (token == NULL) {
			printf("有关某个命令的详细信息，请键入 help 命令名\n");
			printf("exit\t\t退出程序并且将当前用户的磁盘存储到文件中\n");
			printf("dir\t\t显示当前目录\n");
			printf("mkdir\t\t创建新目录\n");
			printf("cd\t\t切换目录\n");
			printf("mkfile\t\t创建新文件\n");
			printf("del\t\t删除文件\n");
			printf("write\t\t写入文件\n");
			printf("read\t\t读取文件\n");
			printf("adduser\t\t加入新用户\n");
			printf("pwd\t\t显示当前路径\n");
			printf("who\t\t显示当前用户信息\n");
			printf("password\t\t修改用户密码\n");
			break;
		}
		helporder(token);
		break;

	case 0:
		return 0;
	default:
		printf("错误:没有命令%s！\n",token);
		break;
	}
	return 1;
}
