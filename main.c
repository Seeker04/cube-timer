/* 
 * CUBE-TIMER
 *
 * A simple and lightweight program to record, store, list and get statistics
 * about Rubik's Cube times.
 *
 */

#include <stdio.h>     /* FILE           */
#include <stdlib.h>    /* exit()         */
#include <sys/time.h>  /* gettimeofday() */
#include <unistd.h>    /* sleep()        */

#define BUFF_SIZE 255
#define FILE_NAME "results.txt"
#define INSP_TIME 15

typedef unsigned long long MILLSEC;

void    add      ();
void    clear    ();
MILLSEC deformat (char *const);
void    format   (MILLSEC, char*);
MILLSEC get_time ();
void    list     ();
void	stat     ();

FILE *f;

void
add()
{
	MILLSEC st;  /* start time  */
	MILLSEC ft;  /* finish time */
	char res[10];

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
format(MILLSEC t, char *str)
{
	int min;
	int sec;
	int frac;

	sec = t/1000;
	min = sec/60;
	sec %= 60;
	frac = t%1000;

	str[0] = min<10 ? '0' : min/10;
	str[1] = min<10 ? min+48 : min%10+48;
	str[2] = ':';
	str[3] = sec<10 ? '0' : sec/10+48;
	str[4] = sec<10 ? sec+48 : sec%10+48;
	str[5] = '.';
	str[6] = frac/100 ? frac/100+48 : '0';
	str[7] = frac%100/10 ? frac%100/10+48 : '0';
	str[8] = frac%10 ? frac%10+48 : '0';
	str[9] = '\0';
}

MILLSEC
deformat(char *const str)
{
	MILLSEC time = 0;

	time += (str[0]-48) * 600000;
	time += (str[1]-48) * 60000;
	time += (str[3]-48) * 10000;
	time += (str[4]-48) * 1000;
	time += (str[6]-48) * 100;
	time += (str[7]-48) * 10;
	time += (str[8]-48);

	return time;
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
	int  i = 0;

	f = fopen(FILE_NAME,"r");
	while (fgets(line,BUFF_SIZE,f))
		printf("%d. %s",++i,line);
	fclose(f);
}

void
stat()
{
	/* TODO: Add "Average of 5" and "Average of 12" */

	char    line[BUFF_SIZE];
	MILLSEC time;
	MILLSEC best  = 0;
	MILLSEC sum   = 0;
	int     count = 0;

	f = fopen(FILE_NAME,"r");
	while (fgets(line,BUFF_SIZE,f)) {
		time = deformat(line);
		best = time<best || !best ? time : best;
		sum += time;
		++count;
	}
	fclose(f);

	printf("Count:     %d\n",count);
	if (count) {
		format(best,line);
		printf("Best time: %s\n",line);
		format(sum/count,line);
		printf("Average:   %s\n",line);
	}
	else {
		printf("Best time: N/A\n");
		printf("Average:   N/A\n");
	}
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

