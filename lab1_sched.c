/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32121826
*	    Student name : YeongJun Seo
*       Group name : 한조
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

//제곱함수
    int Power(int a, int b){
        int i, c=1;
        for(i=1 ; i<=b ; i++)
            c *= a;
        return c;
    }

    //Queue 함수들 구현
    void initQueue(Queue *queue){
        queue->first = queue->last = NULL;
        queue->count = 0;
    }
    
    int IsEmpty(Queue *queue){
        return queue->count == 0;
    }

    void PushQueue(Queue *queue, Job *job){
        Job *new_job = (Job *)malloc(sizeof(Job));
        new_job->name = job->name;
        new_job->arrival_time = job->arrival_time;
        new_job->service_time = job->service_time;
        new_job->next = NULL;

        if(IsEmpty(queue)){
            queue->first = new_job;
        } 
        else{
            queue->last->next = new_job;
        }
        queue->last = new_job;
        queue->count++;
    }

    Job *PopQueue(Queue *queue){
        if(IsEmpty(queue)) return 0;
        Job *returning_job = queue->first;
        queue->first = queue->first->next;
    	queue->count--;
        return returning_job;
    }

    Job priority(Queue *queue)
    {
	char pri_name = RN;
	int time = MAX_TIME;

    	Job *pri_job_scan;
	Job *back = NULL;	
	Job return_job;

	pri_job_scan = queue->first;
	
	while(pri_job_scan != NULL)
	{
		if(time > (pri_job_scan->service_time))
		{
			time = pri_job_scan->service_time;
			pri_name = pri_job_scan->name;
		}
	
		if(pri_job_scan->next==NULL)
		{
			break;
		}

		pri_job_scan = pri_job_scan->next;
	}
    	
	pri_job_scan = queue->first;
	back = NULL;
	
	while( pri_job_scan != NULL)
	{
		if(pri_name == pri_job_scan->name)
		{
			if(back == NULL)
			{
				queue->first = queue->first->next;
			}

			else
			{
				back->next = pri_job_scan->next;
			}
	
			break;
		}

		back = pri_job_scan;
		pri_job_scan = pri_job_scan->next;
	}

	if( pri_job_scan->next == NULL )
		queue->last = back;
	
	queue->count--;
	return_job = *pri_job_scan;
	free(pri_job_scan);

	return return_job;
    }
    
    char queue_arrive_job(int sec, Job job[], int size)
    {
	int i;

	for(i=0; i<size; i++)
	{
		if(job[i].arrival_time==sec)
		{
			return job[i].name;
		}
	}

	return RN;
    }

    int IsJobDone(Job *job){
        return job->service_time == 0;
    }

    int Get_Total_Length(Job job[], int number_of_jobs){
        int length = job[0].arrival_time;
        int i;
        for(i=0 ; i<number_of_jobs ; i++){
            if(i+1<number_of_jobs){
                if(length+job[i].service_time < job[i+1].arrival_time)
                    length = job[i+1].arrival_time;
                else
                    length += job[i].service_time;
            }
            else
                length+=job[i].service_time;
        }
        
        return length;
    }

    int Get_Ready_Queue_Length(Job job[], int number_of_jobs, int time_slice){
        int longest_job = job[0].service_time;
        int i;
        int length=0;
        for(i=0 ; i<number_of_jobs ; i++){
            if(job[i].service_time>longest_job)
                longest_job = job[i].service_time;
        }
        for(i=0 ; longest_job>=0 ; longest_job-=Power(time_slice, i++) ){
            length++;
        }
        return length;
    }

    //FIFO
    void FIFO(int number_of_jobs, int total_length, Job job[], char Result[][7]){
        int i,j;
        int first_start = job[0].arrival_time;
        Job *temp;
        Queue queue;
        initQueue(&queue);

        for(i=0 ; i<number_of_jobs ; i++){
            if(job[i].arrival_time == first_start){
                PushQueue(&queue, &job[i]);
            }
        }
        for(i=first_start ; i<total_length ; ){
            if(IsEmpty(&queue)){
                i++;
                for(j=0 ; j<number_of_jobs ; j++){
                    if(job[j].arrival_time == i)
                        PushQueue(&queue, &job[j]);
                }

            }
            else{
                temp = PopQueue(&queue);
                for( ; !IsJobDone(temp) ;){
                    Result[i][0] = temp->name;
                    temp->service_time--;
                    i++;
                    for(j=0 ; j<number_of_jobs ; j++){
                        if(job[j].arrival_time == i){
                            PushQueue(&queue, &job[j]);
                        }
                    }
                }
            }
        }
    }

    //Round Robin
    void Round_Robin(int number_of_jobs,int total_length, Job job[], char Result[][7], int time_slice, int version){
        int i, j, k;
        int first_start = job[0].arrival_time;
        int real_time_slice;
        Job *temp;
        Queue queue; 
        initQueue(&queue);

        for(j=0 ; j<number_of_jobs ; j++){
            if(job[j].arrival_time == first_start)
                PushQueue(&queue, &job[j]);
        }

        for(i=first_start ; i<total_length ;){
            if(IsEmpty(&queue)){
                i++;
                for(k=0 ; k<number_of_jobs ; k++){
                    if(job[k].arrival_time==i)
                        PushQueue(&queue, &job[k]);
                }
            }
            else{
                temp = PopQueue(&queue);
                for(real_time_slice = time_slice ; real_time_slice && !IsJobDone(temp) ; real_time_slice--){
                    Result[i][version] = temp->name;
                    temp->service_time--;
                    i++;
                    for(k=0 ; k<number_of_jobs ; k++){
                        if(job[k].arrival_time==i)
                          PushQueue(&queue, &job[k]);
                    }
                }
                if(!IsJobDone(temp)){
                    PushQueue(&queue, temp);
                }
            }
        }
    }

    //SJF
    void Shortest_Job_First(Job job[], char Result[][7], int total_length, int number_of_jobs){
    int i, j, sec;
	Job executing;

	Queue ready_queue;	
	initQueue(&ready_queue);

	executing.name = RN;
	executing.service_time=-1;

	for(sec=0; sec<=total_length; sec++)
	{
		char tmp = queue_arrive_job(sec,job,number_of_jobs);

		if(tmp!=RN)
		{
			PushQueue(&ready_queue, &job[(int)tmp-65]);
		}

		if(executing.service_time==0) 
		{
			executing.name = RN;
			if(sec==total_length)
			{
				break;
			}
		}

		if(executing.name==RN&&ready_queue.count!=0)
		{	
			executing = priority(&ready_queue);
		}
		
		if(executing.name!=RN)
		{
			executing.service_time--;
		}
		
		Result[sec][3]=executing.name;
	}
    }

    //STCF
    void Shortest_To_Completion_First(Job job[], char Result[][7], int total_length, int number_of_jobs){
    int i, j, sec;
	Job executing, executing2;

	Queue ready_queue;	
	initQueue(&ready_queue);

	executing.name = RN;
	executing.service_time = -1;

	for(sec=0; sec<=total_length; sec++)
	{
		char tmp = queue_arrive_job(sec,job,number_of_jobs);

		if(tmp!=RN)
		{
			PushQueue(&ready_queue, &job[(int)tmp-65]);
		}
		
		if(executing.service_time==0) 
		{
			executing.name = RN;

			if(sec==total_length)
			{
				break;
			}
		}
		
		if(executing.name==RN && ready_queue.count!=0)
		{
			executing = priority(&ready_queue);
		}
		
		if(ready_queue.count!=0 && executing.name!=RN)
		{
			executing2 = priority(&ready_queue);
			if(executing.service_time > executing2.service_time)
			{
				PushQueue(&ready_queue, &executing);
				executing = executing2;
			}

			else if(executing.service_time==executing2.service_time)
			{
				if((int)executing.name<(int)executing2.name)
				{
					PushQueue(&ready_queue, &executing2);
				}

				else
				{
					PushQueue(&ready_queue, &executing);
					executing = executing2;
				}
			}

			else
			{
				PushQueue(&ready_queue, &executing2);
			}
		}
		
		if(executing.name!=RN)
		{
			executing.service_time--;
		}

		Result[sec][4]=executing.name;
	}
    }

    //MLFQ
    void MLFQ(Job job[], char Result[][7], int total_length, int number_of_jobs, int time_slice){
        int i, j, k, l;
        int first_start = job[0].arrival_time;
        int time;
        int version=5;
        int priority = 2 * Get_Ready_Queue_Length(job, number_of_jobs, time_slice);
        Queue ready_queue[priority];

        for(i=0; i<total_length ; i++){if(Result[i][5]!='0'){version=6; break;}}
        for(i=0 ; i<priority ; i++){initQueue(&ready_queue[i]);}
        for(i=0 ; i<number_of_jobs ; i++){
           if(first_start>job[i].arrival_time) first_start=job[i].arrival_time;
        }
        for(i=first_start ; i<total_length ; ){
            for(j=0 ; j<number_of_jobs && i==first_start ; j++){
                if(job[j].arrival_time == i) PushQueue(&ready_queue[0], &job[j]);
            }
            for(k=0 ; k<priority ; k++){if(!IsEmpty(&ready_queue[k])) break;}
            if(k<priority){
                Job *temp_job = PopQueue(&ready_queue[k]);
                time = Power(time_slice, k);
                for(l=0 ; l<time && !IsJobDone(temp_job) ; l++){
                    Result[i++][version] = temp_job->name;
                    temp_job->service_time--;
                    for(j=0 ; j<number_of_jobs ; j++){
                        if(i==job[j].arrival_time) PushQueue(&ready_queue[0], &job[j]);
                    }
                }
                if(!IsJobDone(temp_job)){
                    int a_empty=1;
                    for(l=0 ; l<priority ; l++){
                        if(!IsEmpty(&ready_queue[l])) a_empty=0;
                    }
                    if(k == priority-1 || a_empty) PushQueue(&ready_queue[k], temp_job);
                    else PushQueue(&ready_queue[k+1], temp_job);
                }
            }
            else{
                i++;
                for(j=0 ; j<number_of_jobs ; j++){
                    if(i==job[j].arrival_time) PushQueue(&ready_queue[0], &job[j]);
                }
            }  
        }
    }

    //Print Result (Redirection)
    void Print_Result(char Result[][7], int number_of_jobs, int total_length){ 
         int i, j, k, q=0;
         close(STDOUT_FILENO);
         open("./Result.txt", O_CREAT|O_WRONLY|O_TRUNC, 0777);
         printf("한조 Scheduler Simulation Result\n용의 눈으로 봐라...\n");
         while(q<7){
             switch(q){
                 case 0: printf("FIFO\n"); break;
                 case 1: printf("Round Robin 1\n");  break;
                 case 2: printf("Round Robin 2\n");  break;
                 case 3: printf("Shortest Job First\n");  break;
                 case 4: printf("Shortest To Completion First\n");  break;
                 case 5: printf("MLFQ 1\n");  break;
                 case 6: printf("MLFQ 2\n"); break;
             }
         printf("Job\\Time 00");
         for(i=0 ; i<total_length ; i++){
            if(i+1 < 10) printf("===0%d", i+1);
            else printf("===%d", i+1);                                                                        
         }   
         printf("\n");
         for(j=0 ; j<number_of_jobs ; j++){
            printf("%C  :       ",(char)(65+j));
            for(k=0 ; k<total_length ; k++){
                if(Result[k][q]==(char)(65+j)) printf("===  ");
                else printf("     ");                                                                        
            }
            printf("\n");                                         
         }   
         printf("\n\n");   
         q++;
         }
    } 

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 */


