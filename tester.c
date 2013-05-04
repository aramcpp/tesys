#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <windows.h>

#define MAX_STR_LEN 2048

char *tres[]={
    "accepted",
    "wrong answer"
};

int run(char *fn,char *cmd,int tm);
void help_msg();
int exist(char *fn);
int is_num(char *s);

int main(int argc,char *argv[])
{
    if (argc<5)
        help_msg();
    char s[MAX_STR_LEN],t[MAX_STR_LEN],fn[MAX_STR_LEN],ck[MAX_STR_LEN],lfn[MAX_STR_LEN];
    int i,r,n,tm,ul;
    if (!is_num(argv[1]))
    {
        fprintf(stderr,"error: first argument must be a number");
        return 1;
    }
    n=atoi(argv[1]);
    strcpy(fn,argv[2]);
    strcpy(ck,argv[3]);
    tm=atoi(argv[4]);
    if (argc==6)
    {
        strcpy(lfn,argv[5]);
        ul=1;
    }
    else
        ul=0;
    FILE *log;
    if (ul)
        log=fopen(lfn,"w");
    if (!exist(fn))
    {
        fprintf(stderr,"error: testing file doesn\'t exist");
        return 1;
    }
    if (!exist(ck))
    {
        fprintf(stderr,"error: checker doesn\'t exist");
        return 1;
    }
    for (i=1;i<=n;i++)
    {
        sprintf(s,"input%d.txt",i);
        if (!exist(s))
        {
            fprintf(stderr,"error: %s doesn\'t exist",s,i);
            return 1;
        }
        CopyFile(s,"input.txt",0);
        sprintf(t,"output%d.txt",i);
        if (!exist(t))
        {
            fprintf(stderr,"error: %s %d doesn\'t exist",t,i);
            return 1;
        }
        CopyFile(t,"answer.txt",0);
        r=run(fn,"",tm);
        if (!r)
        {
            r=run(ck," input.txt output.txt answer.txt",-1);
            printf("test %2d: %s\n",i,tres[r]);
            if (ul)
                fprintf(log,"%s\n",tres[r]);
        }
        else
        {
            printf("test %2d: run-time error\n",i);
            if (ul)
                fprintf(log,"run-time error\n");
        }
    }
    DeleteFile("output.txt");
    DeleteFile("answer.txt");
    if (ul)
        fclose(log);
	return 0;
}
int run(char *fn,char *cmd,int tm)
{
    STARTUPINFO si;
	memset(&si,0,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	memset(&pi,0,sizeof(PROCESS_INFORMATION));
	CreateProcess(fn,cmd,NULL,NULL,0,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);
	DWORD st=WaitForSingleObject(pi.hProcess,tm);
	if (st==WAIT_TIMEOUT)
    {
        TerminateProcess(pi.hProcess,0);
        return -1;
    }
	DWORD code;
	GetExitCodeProcess(pi.hProcess,&code);
	return code;
}
void help_msg()
{
    puts("Usage: tester.exe <number of tests> <testing file name> <checker file name> <time limit> [log file name]");
    exit(0);
}
int exist(char *fn)
{
    struct stat p;
    return !stat(fn,&p);
}
int is_num(char *s)
{
    int i;
    for (i=0;s[i];i++)
        if (!isdigit(s[i]))
            return 0;
    return 1;
}
