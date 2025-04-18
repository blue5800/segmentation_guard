#include "segmentation_guard.h"
#include <stdio.h>

int main(void){
	segmentation_guard_init();
	enable_segmentation_guard();
	*(volatile int *)0 = 0; //no problem for segmentation guard...
	printf("Segmentation guard caught a SIGSEGV with the following instruction:");
	print_last_offending_instruction();
	return 0;
}

