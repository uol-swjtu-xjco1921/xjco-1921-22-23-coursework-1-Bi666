# XJCO 1921 Coursework 1

## Quick Links
- [Key Information](#key-information)
- [The Task](#the-task)
- [Running the Code](#using-the-files)
- [Hints and Tips](#hints-and-tips)
    - [Where to Start](#where-do-i-begin)
    - [Branching in Git](#branching-in-git)
- [Common Errors](#common-issues)
    - [Compilation Errors](#compilation-errors)
    - [Runtime Errors](#runtime-errors)

## Key Information
### Basic Submission Rules

This repository is your base code for XJCO 1921 Assignment 1.
You can edit and add any files that you want, but there are some basic submission requirements:

- You c, h and makefile should be in this directory, not inside any subfolders.
- Your executables should be created in this directory
- Your makefile and code must compile on Linux. 

**Submission is via GradeScope**.

### Academic Integrity

You should already have completed the University's Academic Integrity course - but to clarify:
- Any code you submit, other than code provided by the module leader, must be **your own work** - not that of your peers, any external person, or **any code generation tool**.
- You should not share any code you write with people other than module staff until final module marks are returned at the end of the academic year.
- You can discuss ideas with your peers, but do not write any code together
- Your work **will** be passed through an academic integrity tool

### Marking

This coursework is worth 40% of the final grade, divided between the sub-tasks as defined in the coursework spec. However, **code quality** is also a key aspect of this coursework and these marks are allocated separately.

**Please note that the testing script gives you a count of tests passed which is *not equivalent* to your expected mark.**

# The Tasks

See the coursework spec on Minerva. 


## Test data

We have provide some test data for you to start with. These PGM files are in the test_data/pgms directory, which has the following structure:
```
test_data/pgms/ascii: containing some input PGM in the ASCII form. These include some corrupted PGM files to test error handling.
test_data/pgms/binary: containing some binary input PGM
test_data/pgms/output: containing the expected outputs for tasks I and III when applying your developed program to the given input images.

```
However, you should test your program on other data including test cases for error handling. 

## Testing script

You are provided with a basic shell script to test Task I. Your should extend this script to test all other tasks and add any additional tests that you consider appropriate. Please keep in mind that when you submit your code, it will be evaluated using our comprehensive test script, which may test things you haven't considered.

All programs should return 0 if the program completes successfully or a non-0 value if it fails, along with an appropriate string to describe the result. This is a Unix convention, where 0 signifies success because there is only one way for a program to succeed, but there are numerous ways for it to fail. Please ensure that your programs follow this convention.

The messages and return codes which you should use are:


| Value  | String | Condition |
| ------------- | ------------- | ------------- |
| 1  | ERROR: Bad Argument Count | Program given wrong # of arguments |
| 2 | ERROR: Bad File Name (fname) | Program fails to open file |
| 3 | ERROR: Bad Magic Number (fname) | File has an illegal magic number |
| 4 | ERROR: Bad Comment Line (fname) | Program failed on comment line |
| 5 | ERROR: Bad Dimensions (fname) | File has illegal dimensions |
| 6 | ERROR: Bad Max Gray Value (fname) | Program failed on max gray value |
| 7 | ERROR: Image Malloc Failed | Malloc failed to allocate memory |
| 8 | ERROR: Bad Data (fname) | Reading in data failed |
| 9 | ERROR: Output Failed (fname) | Writing out data failed |
| 100 | ERROR: Miscellaneous (text description) | Any other error which is detected |

In all cases, fname should be replaced by the name of the file which caused the error.
In the case of Miscellaneous errors, in 100 chars or less please describe what the error was.

All other print statements will be ignored by the final test script.

You should also use these standard success messages, for which the value is always 0.

| Value | String | Program | Meaning |
| ------------- | ------------- | ------------- | ------------- |
| 0 | ECHOED | pgmEcho | Program echoed the input |
| 0 | IDENTICAL | pgmComp | The two files were logically identical |
| 0 | DIFFERENT | pgmComp | The two files were not logically identical |
| 0 | CONVERTED | pgma2b/pgmb2a | The file was converted |
| 0 | REDUCED | pgmReduce| The file was successfully reduced in size |

Usage messages are also a Unix convention - when an executable is run with no arguments, it is assumed that the user does not know the correct syntax and wishes to see a prompt. As this is not an error state, the return value is 0.

# Using The Files

You can compile the provided files by using:
```
make all
```
Executables can then be run the standard way.

You run the test suite using:
```
bash ./test.sh
```

# Hints and Tips
## Linux and Development Environment
Your code must compile and run on a standard Linux Distribution (e.g., Ubuntu)

If you're using a Windows operating system, you can still achieve this by installing the Windows Subsystem for Linux (WSL) and selecting Ubuntu as your Linux distribution.

To install WSL on Windows 10/11, follow the instructions provided at: https://learn.microsoft.com/zh-cn/windows/wsl/install. Once you have WSL installed, you can install the necessary development tools, such as gcc, gdb, and make, on Ubuntu by running the following commands in your Ubuntu terminal:

```
sudo apt update
sudo apt upgrade

sudo apt-get install -y build-essential
```

These commands will update the Ubuntu package list and install the build-essential package, which includes the development tools you'll need to compile and run your code on the Ubuntu Linux distribution.


## Where do I begin?

Start by reading through pgmEcho.c. This file is extensively commented, and you should be able to follow the logic quite easily through the file. Look for places with bad code quality- some key things to look out for are:
- blocks of repeated code
- complicated/hard to follow code
- unclear variable names
- very long functions

You can also run the testing suite as you will find a number of tests are failing - some of these are easy to fix.

Make yourself a list (to_do.md for example - this allows you to use [readme-style markup](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax) which is a useful skill) and write down all the issues you see - add line numbers so you can find them again easily later.

Once you have a list, read through it and put the changes into a sensible order - you should usually start at the top of the code and work down. If you find yourself making exactly the same change in 2 different places, consider if this might be moved into a function somewhere else!

## What is binary data?

A common issue which students encounter in these tasks is working with binary data.

When we talk about binary vs ASCII data in programming, we are essentially talking about how data is encoded. ASCII data is encoded in a human-readable format, and each character takes up 8 bits (technically, 7 bits and 1 for parity). Binary data is encoded in a smaller but non-human-readable format - while the number '255' would take 3 bytes of ASCII, it is 1 byte of binary. 

Let us start by looking at slice0.pgm, using the command-line hexdump utility:
```
$ hexdump -C test_data/pgms/ascii/slice0a.pgm
00000000 50 32 0a 32 20 32 0a 32 35 35 0a 30 20 31 30 0a |P2.2 2.255.0 10.|
00000010 31 30 20 32 35 35 0a |10 255.|
00000017
```
The left-hand column shows the address in the file (i.e. the index of the bytes). The next 16 columns show individual bytes in hexadecimal notation, and the final column shows the ASCII equivalent (using a . for anything non-printable). We see that the first byte is 0x50 (five-zero, not fifty). Consulting the ASCII chart, we
confirm that that is treated in ASCII representation as the letter 'P', as shown in the right-hand column.
The next byte is 0x32, which is treated as '2' The third byte is 0x0a, which is treated as a new line character, and so on. Note how the last 
4 bytes are 0x32 35 35 0a, as described above: 4 bytes to store one number. 

When we use fscanf() to read in data, it interprets the ASCII characters and converts them to numbers, so we store the single bytes 0x00 (decimal 0), 0x0a (decimal 10), 0x0a (decimal 10) and 0xff (decimal 255) in memory

However, when a human tries to open a binary-encoded file, you will not be able to read it. For example, 
```
$cat test_data/pgms/binary/slice0a.pgm
P5
2 2
255




```
This is the binary form of slice0a.pgm - it's smaller, but the bitmap information is no longer readable! However, the exact same numbers are there - just in a more compressed format.

You will find that there are  read and write functions built into C to read binary data (e.g., fread and fwrite), which you may find to be useful for completing task III. 

## Branching in git

Your use of git is assessed in this module, but more importantly it is a very useful tool when you're working on a large, complex set of files. Many of you will never have used git before, so there are a few useful things you should know:
 
- the 'main' branch should only ever contain code which compiles and passes the tests (we call this 'clean code')
- in industry, this is usually protected so that you cannot push to main directly
- developers usually work on branches and only merge in their working, tested code

These conventions are because the majority of software development happens on code which is *already deployed* - therefore if you were to push code which doesn't work onto main, you run the risk of this code going live and affecting your **paying customers**. There is even a widely used git command to find out who last edited a line of code (`git blame`) in order to find and discipline developers who break the deployed code.

As the main branch should always be 'clean', you should get used to making and working on new branches. A branch is a separate copy of the code- it is in the same repository, but is stored separately on the git server so changes to one branch do not affect other branches. You merge your branch into main once you are happy that the code works and does not break the tests. You can also abandon a branch if you break something!

This means that you will **always have a version of the code which runs** - if you use branching properly, you will never get into a situation where you need to start again from scratch.

To create and move to a new branch, make sure all your changes are committed and pushed, and use:
```
git branch new_branch_name
git checkout new_branch_name
```
or
```
git checkout -b new_branch_name
```
which is equivalent to the 2 line version.

`git branch` creates a new branch, and `git checkout` moves you onto it. The `-b` option means 'create this branch'.

From this point on, when you want to push your changes you will need to make sure you use:
```
git push origin new_branch_name
```
Once you are happy with your code, you will merge it into your 'main' branch - make sure you have commited and pushed everything.
```
git checkout main
git merge new_branch_name
```
If you have changed any files in main since your last push to main, you may have merge conflicts - you just need to select the version of your code which you want to keep. You can refer to this [very useful guide](https://git-scm.com/book/en/v2/Git-Branching-Basic-Branching-and-Merging) which explains merging and merge conflicts in more detail.

# Common Issues

## Compilation Errors

Usually GCC will give you a relatively helpful error when you compile your code, telling you what and where your error is. **However** sometimes you will get an error produced by your makefile itself. The most common issue is:

```
make: *** No rule to make target 'file.o', needed by 'file'.  Stop.
```

This is almost always caused by 'file.c' not existing in your folder - you should double check spelling and capital letters and ensure that your files are in the right place.

Another common issue is:
```
multiple definition of 'main'
```
You can only ever have one 'main' function per executable - if you have tried to compile two .c files into one and they both have a main() function, you will get this error. If you want two files which both have their own main function to share some other utility function, the shared functions should be put in a different .c file.

## Runtime Errors

These errors don't prevent your code from compiling, but make your code crash while you are running it.

The one which most students will get at some point is:

```
Segmentation Fault (Core Dumped)
```

This means that you are trying to access some memory which you do not have permissions to access. There are a wide variety of things which can cause it, but most often it is incorrect use of malloc/calloc, trying to access NULL, or going out of bounds in an array.

To find the cause of your segfault, GDB is the best tool.

```
gdb executableName
set args arg1 arg2
run
```
Replace executableName, arg1 and arg2 with the executable name and arguments which gave you the segfault when you ran your code manually.

This should cause another segfault, but this time *inside GDB* where we can find out what happened.
```
backtrace
```

Backtrace lets you see what steps led to the segmentation fault - usually, this will point you to the file and line where the segfault happened. 

