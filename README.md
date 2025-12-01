# segmentation_guard
- SIGSEGV handler which skips the offending instruction

# Disclaimer:
- DO NOT use this in prod. it is fragile, exploits undefined behaviour, isn't signal, or anything safe.
- i wrote this to learn what would happen if a C program had essentially the "on error resume next" mechanism
- the code is so poorly written because i thought it was funnier this way.

# credits:
intelxed, https://github.com/intelxed/xed, used for dynamically decoding the current instruction in order to find length and capture the disassembled instruction

# how it works:
- make use of a signal handler in order to catch SIGSEGV signals.
- check if the return address is accessible. if it's not then exit
- pass this address to xed_decode which decodes the instruction
- decode the instruction and calculate length
- add the length of the instruction to the saved return address
- program continues execution from the next instruction.

# limitations:
- fragility: this is extremely platform and compiler dependent. abuses global state and is not thread safe.
- in case of severe $RIP corruption, the program simply exits. recovery from this is beyond the scope of the program
- the program will segfault in cases where the faulting $RIP is split across 2 page boundaries, and one page is inaccessible (this could be fixed but i thought it was a cool bug so i won't fix it)
- stack pointer corruption will also cause a segfault

