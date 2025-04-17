# segmentation_guard
SIGSEGV handler which skips the offending instruction

# Disclaimer:
despite the funny name and description, this is pretty much just experimental code. it exploits undefined behaviour, making it incredibly fragile. furthermore, even if it were implemented correctly, the entire premise of this library is idiotic. simply skipping over an instruction that caused a memory access violation would make your code far more difficult to debug and has pretty much no practical application. this is NOT a get out of jail free card; you should learn to code and fix your own bugs. I wrote this entirely to satiate my curiosity. this is a learning experiment that should never see the light of prod.

# credits:
intelxed, https://github.com/intelxed/xed, used for dynamically decoding the current instruction in order to find length and capture the disassembled instruction

# how it works:
- we make use of a signal handler in order to catch SIGSEGV signals.
- through some trial and error, we do an out of bounds buffer read, dereferencing the return address of the signal handler.
- this brings us to the instruction which caused the memory access violation. 
- we pass this address to xed_decode which decodes the instruction
- we get data about said instruction such as the length. we use the length and add that to the return address to essentially move to the next instruction

# limitations:
- fragility: this is extremely platform and compiler dependent due to its reliance on undefined behaviour. while it works in my environment, your build may fail and i won't be able to help you. you would have to calculate the offset yourself or switch to using ucontext_t instead of this approach. i would've used that too if i knew that it existed before writing this...
- invalid %RIP: this library cannot save you if the reason for the SIGSEGV is that you try to execute instructions at invalid memory. consider the following example:

```c
    void (*n) (void);
    n();
```
this code would jump to invalid memory and so the library's attempt to read the return address would result in a segmentation fault.




