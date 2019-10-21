/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32121826
*	    Student name : YeongJun Seo
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"


int main(){
    
	int number_of_jobs = 0;
	int time_slice_1 = 1;
	int time_slice_2 = 1;
    int time_slice_3 = 1;
	int i,j;
	char choice;	

	printf("How many jobs would you like to simultae?: ");
	scanf("%d", &number_of_jobs);
    

	//구조체 Job의 배열을 선언한다.(배열의 길이는 입력받은 값이다.)
	Job job[number_of_jobs];


	//반복문을 통해 Job과 time slice의 정보를 입력받는다.
    printf("Please insert the jobs in order of their arrivals\n");
	for(i=0 ; i< number_of_jobs ; i++){
	printf("Insert Arrival Time & Service Time of job[%c]:", 65 + i);
	job[i].name = (char)(65+i);
	scanf("%d %d", &job[i].arrival_time, &job[i].service_time);
	}
	printf("Insert the value of time slice for Round Robin 1: ");
	scanf("%d", &time_slice_1);
	printf("Insert the value of time slice for Round Robin 2: ");
	scanf("%d", &time_slice_2);
    printf("Insert the value of time slice for MLFQ        2: ");
    scanf("%d", &time_slice_3);
	printf("\n");


	//실행전에 입력받은 값을 확인한다.
	printf("Job\tArrival Time\tService Time\n");
	for(i=0 ; i<number_of_jobs ; i++){
	printf("%c\t%d\t\t%d \n", job[i].name, job[i].arrival_time, job[i].service_time);
	}
	printf("Round Robin 1 Time Slice: %d\n", time_slice_1);
	printf("Round Robin 2 Time Slice: %d\n", time_slice_2);
    printf("MLFQ        2 Time Slice: %d\n", time_slice_3);
	printf("\n");
	fflush(stdin);
	printf("Are your insertions accurate? (y/n): ");
	while(1){
	scanf("%c", &choice);
	if (choice =='y'|| choice =='n') break;
	}
	if (choice == 'n'){printf("Ok, Bye\n"); exit(1);}
	else {printf("Initiating Scheduler...\n\n");}


	//스케줄링 결과를 저장하기 위한 2차원 문자배열 Result를 생성하고 초기화한다.
    int total_length = Get_Total_Length(job, number_of_jobs);
	char Result[total_length][7];	
	for(i=0 ; i<7 ; i++){
		for(j=0 ; j<total_length ; j++)
		Result[j][i] = '0';
	}	


	//FIFO
	FIFO(number_of_jobs,total_length, job, Result);
	//Round Robin 1
	Round_Robin(number_of_jobs, total_length, job, Result, time_slice_1, 1);
	//Round Robin 2
   	Round_Robin(number_of_jobs, total_length, job, Result, time_slice_2, 2);
	//SJF
	Shortest_Job_First(job, Result, total_length, number_of_jobs);
	//STCF
   	Shortest_To_Completion_First(job, Result, total_length, number_of_jobs);
	//MLFQ 1
   	MLFQ(job, Result, total_length, number_of_jobs, 1);
	//MLFQ 2
	MLFQ(job, Result, total_length, number_of_jobs, time_slice_3);
	//결과를 출력한다.(리다이렉션)
	Print_Result(Result, number_of_jobs, total_length);
     
    
	return 0;
}

