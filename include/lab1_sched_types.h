/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32143199
*	    Student name : 이민승
*       Group name : 한조
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H

#define RN	'@'
#define MAX_TIME	1000
    //구조체 Job
    struct Job{
        char name;
        int arrival_time;
        int service_time;
        struct Job* next;
    };
    typedef struct Job Job;

    //Jobs을 Node로 하는 Queue
   typedef struct Queue{ 
        Job *first;
        Job *last;
        int count;
    }Queue;

    //함수들의 전방선언
    //Queue
    int IsEmpty(Queue *queue);
    void PushQueue(Queue *queue, Job *job);
    Job *PopQueue(Queue *queue);
    int IsJobDone(Job *job);
    //Get Length of total Scheduling time_slice
    int Get_Total_Length(Job job[], int number_of_jobs);
    //Length of MLFQ's Ready Queue
    int Get_Ready_Queue_Length(Job job[], int number_of_jobs, int time_slice);
    //스케줄링 알고리즘
    void FIFO(int number_of_jobs, int total_length, Job job[], char Result[][7]);
    void Round_Robin(int number_of_jobs,int total_length, Job job[], char Result[][7], int time_slice, int version);
    void Shortest_Job_First(Job job[], char Result[][7], int total_length, int number_of_jobs);
    void Shortest_To_Completion_First(Job job[], char Result[][7], int total_length, int number_of_jobs);
    void MLFQ(Job job[], char Result[][7], int total_length, int number_of_jobs, int time_slice);
    //제곱
    int Power(int a, int b);
    //출력
    void Print_Result(char Result[][7], int number_of_jobs, int total_length); 

    
/*
 * You need to Declare functions in  here
 */


#endif /* LAB1_HEADER_H*/
