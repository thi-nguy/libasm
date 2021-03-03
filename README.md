# libasm
## The idea
- The goal is to make a library (**libasm.a**) with functions written in assembly.
- Actually we need to write each function in assembly. Then we link all small functions into a library.
- Then the library is ready to use.
- We need to submit a **main.c** to check how our functions work.
- We write **64 bits ASM, x86, Intel syntax**.
## My approach
- Review how to use a static library with a main.
- Write Makefile first.
- Write main.c
- Write ft_strlen.a. Then create the library and check if it works well with the main.
- Then Write other functions.
- Handle error.
## Some Question?
- What is variable **errno**?
- When and why we need to call *extern ___error*?
