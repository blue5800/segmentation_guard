#include "segmentation_guard.h"
#include <stdio.h>


int main() {
	segmentation_guard_init();
	enable_segmentation_guard();

	void (*null_function)();
	null_function(); // This should trigger a segmentation fault which cannot be caught by the program.
	printf("something wrong\n");
	return 0;
}
