#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "tpool.h"

//Test Function Prototype
void print_job();

int main(){
		
	//Initalize Thread Pool with Function
	tpool_init(print_job);
	
	//Test Full Queue Case by Adding 300 jobs
	for(int index = 0; index < 300; index++){
		tpool_add_task(index);
	}

	sleep(1);
	
	//Print Queue
	printf("Current State of Queue...\n");
	print_queue();
	
	//Ensure Queue is Empty and Fully Processed
	sleep(10);
	
	//Add Small Number of Jobs
	tpool_add_task(1);
	tpool_add_task(1);
	tpool_add_task(1);
	tpool_add_task(255);
	tpool_add_task(255);
	tpool_add_task(50);
	
	sleep(1);
	
	//Print Queue
	printf("Current State of Queue...\n");
	print_queue();

	return 0;
}


void print_job(int job){
	
	printf("Job %d was completed...\n", job);
}