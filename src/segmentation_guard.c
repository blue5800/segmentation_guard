#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "xed/xed-interface.h"
#include "xed/xed-decoded-inst-api.h"

void init_instruction_decoder();
xed_decoded_inst_t current_instruction;
unsigned int num_segfaults = 0;
char buf[1024] = {0};

long long instruction_length(uint8_t* instruction){
    xed_bool_t ok = xed_decode(&current_instruction, instruction, 15);
    long long len = 1;
    if (ok == XED_ERROR_NONE){
	len = xed_decoded_inst_get_length(&current_instruction);
	memset(buf, 0, sizeof(buf));
	ok = xed_format_context(XED_SYNTAX_INTEL,&current_instruction,buf,sizeof(buf),(long long)(instruction),0,0);
    }
    init_instruction_decoder();
    return len;
}


void signal_problem(int signum){
    static int segfault_count = 0;
    long long buf[1] = {};
    buf[0] = open("/tmp", O_TMPFILE | O_RDWR);
    buf[0] |= ((uint64_t)write(buf[0], (void *)buf[25], 1) << 32);
    close((int)(buf[0] & 0xFFFFFFFF));
    if((int)(buf[0] >> 32) < 0){
	exit(0);
    }
    buf[25] += instruction_length((uint8_t*)buf[25]);

    if (++segfault_count >= 1<<7){
      exit(0);
    }
    ++num_segfaults;
    segfault_count = num_segfaults;
}

void init_instruction_decoder(){
    static int initialized = 0;
    if (!initialized) xed_tables_init();
    initialized = 1;
    xed_decoded_inst_zero(&current_instruction);
    xed_decoded_inst_set_mode(&current_instruction, XED_MACHINE_MODE_LONG_64, XED_ADDRESS_WIDTH_64b);
}

void enable_segmentation_guard(){
    signal(SIGSEGV, signal_problem);
}
void disable_segmentation_guard(){
    signal(SIGSEGV, SIG_DFL);
}
int number_of_segfaults(){
    return num_segfaults;
}
void reset_segfault_counter(){
    num_segfaults = 0;
}
void get_last_offending_instruction(char* dest,unsigned int len){
	if (len > 1024) len = 1024;
	memcpy(dest, buf, len);
}
void print_last_offending_instruction(){
	printf("%s\n", buf);
}

void segmentation_guard_init(){
	init_instruction_decoder();
}
