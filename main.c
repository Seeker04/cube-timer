#include <stdio.h>     /* FILE           */
#include <stdlib.h>    /* exit()         */
#include <sys/time.h>  /* gettimeofday() */
#include <unistd.h>    /* sleep()        */

#define BUFF_SIZE 255
#define FILE_NAME "placeholder.txt"
#define INSP_TIME 15

typedef unsigned long long MILLSEC;

void    add      ();
void    clear    ();
void    format   (MILLSEC, char *const);
MILLSEC get_ms   (char *const);
MILLSEC get_time ();
void    list     ();
void	stat     ();

FILE *f;

void
add()
{
	MILLSEC st;  /* start time  */
	MILLSEC ft;  /* finish time */
	char    res[BUFF_SIZE];

	printf("Inspection time (%d secs)\n",INSP_TIME);
	sleep(INSP_TIME);
	st = get_time();
	printf("Started\n");
	getchar();
	ft = get_time();
	format(ft-st,res);
	printf("Result: %s\n",res);
	f = fopen(FILE_NAME,"a");
	fprintf(f,"%s\n",res);
	fclose(f);
}

void
clear()
{
	f = fopen(FILE_NAME,"w");
	fclose(f);
}

void
format(MILLSEC t, char *const str)
{
	int min;
	int sec;
	int frac;

	sec = t/1000;
	min = sec/60;
	sec %= 60;
	frac = t%1000;

	sprintf(str,"%d:%d.%d",min,sec,frac);
}

MILLSEC
get_ms(char *const str)
{

}


MILLSEC
get_time()
{
	struct timeval tp;

	gettimeofday(&tp, NULL);
	return (MILLSEC) tp.tv_sec * 1000L + tp.tv_usec / 1000; 
}

void
list()
{
	char line[BUFF_SIZE];
	int  i = 1;

	f = fopen(FILE_NAME,"r");
	while (fgets(line,BUFF_SIZE,f))
		printf("%d. %s",i++,line);
	fclose(f);
}

void
stat()
{
	char    line[BUFF_SIZE];
	MILLSEC time;
	MILLSEC best;
	MILLSEC sum = 0;
	int     count = 1;

	f = fopen(FILE_NAME,"r");
	while (fgets(line,BUFF_SIZE,f)) {
		time = get_ms(line);
		best = time<best ? time : best;
		sum += time;
		++count;
	}
	fclose(f);

	format(best,line);
	printf("Best time: %s",line);
	format(sum/count,line);
	printf("Average:   %s",line);
}

int
main(int argc, char const *argv[])
{
	if (argc<2) {
		printf("Error: No argument given!\n");
		exit(1);
	}
	switch (argv[1][1]) {
	case 'a' : add();   break;
	case 'c' : clear(); break;
	case 'l' : list();  break;
	case 's' : stat();  break;
	default  : printf("Error: Illegal argument!\n");
		   exit(2);
	}

	return 0;
}

