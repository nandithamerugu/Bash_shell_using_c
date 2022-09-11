# How to execute?
``` gcc main.c 
    ./a.out
```


## cd command

- The input is the command, home and current working directory.
- If we enter nothing 


<nandu@nandu-Inspiron-5590:~> 
> then it has to stay on the same directory
<nandu@nandu-Inspiron-5590:~> 


- If we enter say if we have Try folder and dummy folder in Try folder.

- <nandu@nandu-Inspiron-5590:~> cd Try
- <nandu@nandu-Inspiron-5590:~/Try> cd dummy
- <nandu@nandu-Inspiron-5590:~/Try> cd dummy
- <nandu@nandu-Inspiron-5590:~>

## echo command

- This command has to print whatever is written to it.
- <nandu@nandu-Inspiron-5590:~> echo HI  (will return HI)
- <nandu@nandu-Inspiron-5590:~> echo "HI" (will return HI)

- I additionally added the other feature where we keep i.e., if we write the word in quotes then the word is only returned not the quotes(which is similar to that of the normal terminal).

## pwd

- This command should return the present working directory.
- I implemented this directory by using getcwd().
- <nandu@nandu-Inspiron-5590:~> pwd
- /home/nandu/Desktop/OS/2020101106_Assignment2/Nanditha_shell

## history

- In this i kept track on all the commands we are entering using fgets. 
- And appending them. And if it crosses a limit of 20 then it will be overwitten. 

## exit 
- This will exit from the shell

## ls
- I wrote the ls ls_l ls_a ls_al. 
- ls will print all the files, directories and executable files.
- ls_l will print discription of each file.
- ls_a will print all hidden files.
- ls_al will print all the hidden files discription.

## foreground and background
- Foreground process is like implementing on board. If sleep 2 is printed then it waits for 2 second and then only new command can be written. 
- <nandu@nandu-Inspiron-5590:~> sleep 2
# sleeps for 2 seconds
- <nandu@nandu-Inspiron-5590:~> 
- But in case of background process. If sleep 2& then new command works and waits for 2 second and prints the pid of the terminated process.
- <nandu@nandu-Inspiron-5590:~> sleep 2&
- <nandu@nandu-Inspiron-5590:~> 
- <nandu@nandu-Inspiron-5590:~> 
- exited pid

## discover
- We will first consider a paramter path which is with '.' .
- We recurrently check the file/directories and which or not hidden.
- If the path size is greater then the given path size.Then we directly print the path.
- if not we add the additional name of the directory/path. 
- Based on what we use like say if we want discover then it should print all the files, directories etc. So, we use no if condition to check the directory.
- If we use directory -d. Then we use isDir function (i implemented), that will check each time and print the directories only.
- If we use directory -d -f. Then all files and directories are printed.
- <nandu@nandu-Inspiron-5590:~> discover -d
.
./Try
./Try/dummy
./2020102061_Assignment2














