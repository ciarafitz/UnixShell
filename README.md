# Welcome to my Unix Shell Project!
 ## LSH
* LSH stands for limited shell in C.
* It does the following:  read, parse, fork, exec, and wait
    * All commands must be on a *single* line.
    * Arguments separated by *whitespace*.
    * No redirection or piping.

* Only built-ins: **cd**, **help**, **exit**.

Link to shell commands:
https://docs.cs.cf.ac.uk/notes/linux-shell-commands/

## How to Compile:

* **main.c** file is the source code where code is written into.
* **lsh** is the binary file containing machine code (0's and 1's).
    * This file should **not** be committed to GitHub
        * 1) Binary files are too big & bloat repository.
        * 2) Compiled binary file on my machine won't be the same work on others' machines when they pull the project files.
        * 3) Good to build binaries yourself when cloning a project so all dependies and correct configuration are set up.


To compile run this command in the projects directory:

     gcc -o lsh main.c 

 **Note:** When you change the source file **main.c** you need to *re-compile* the *lsh* using the same command above.


## How to Run: 

 Run the following command:

    ./lsh

##  Bugs:

* Currently none (2/3/22)

## Additions for the Future:

* Add more built-ins.