void enable_segmentation_guard();
void disable_segmentation_guard();
int number_of_segfaults();
void reset_segfault_counter();
void get_last_offending_instruction(char* dest,unsigned int len);
void print_last_offending_instruction();
void segmentation_guard_init();