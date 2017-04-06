#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
int main()
{
	char block[8192];
	int in, out;
	int nread;
	in = open("./다운로드/naruto.avi",O_RDONLY);
	out = open("file_1.avi",O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
	while((nread=read(in,block,sizeof(block)))>0)
		write(out,block,nread);
	close(in);
	close(out);
	exit(0);
}
