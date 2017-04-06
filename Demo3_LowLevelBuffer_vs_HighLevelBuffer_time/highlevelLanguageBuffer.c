#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
int main()
{
	char block[8192];
	FILE *open_f = fopen("./다운로드/naruto.avi","r");
	FILE *out_f = fopen("file_2.avi","w");
	int nread;
	while((nread=fread(block,1,8192,open_f))>0)
	{
		fwrite(block,1,nread,out_f);
	}
	fclose(open_f);
	fclose(out_f);
	exit(0);
}

