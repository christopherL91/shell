
![unix](https://static.flickr.com/87/240803829_9212773615_o.png "here's a nickel kid")

##About

This is a simple *NIX shell written in ANSI C for the
course ID2200 Operating systems. Globbing is supported through
the posix function ```glob(3)```

##Task

* a built-in command "exit" which terminates all remaining processes started
from the shell in an orderly manner before exiting the shell itself
* a built-in command "cd" which should behave exactly as a normal built-in
command "cd" with the exception that you do not need explicitly to update
environment variables.
* a built-in command "checkEnv" which should execute "printenv | sort | pager"
if no arguments are given to the command. If arguments are passed to the
command then "printenv | grep <arguments> | sort | pager" should be executed.
The pager executed should be selected primarily based on the value of the users
"PAGER" environment variable. If no such variable is set then one should first
try to execute "less" and if that fails "more". Any errors in the execution of
the pipeline should be handled in a nice manner (i.e. similar to how your
normal shell handles errors in the execution of a manually set up
pipeline).
* It should handle the execution of processes in foreground and background. For
foreground processes the total execution time for the process should be printed
when it terminates. For background process information on that they have
terminated should be printed no later than when the shell prompts next time
after the background process/es have terminated.
* Detection of terminated background process should be implemented by two
mechanisms where one selects which should be compiled at compilation time. The
mechanisms are ordinary polling and detection by signals sent from the child
processes. The user should be able to select which mechanism to use by defining
a macro SIGDET=1 at compilation time to have termination detected by signals.
If SIGDET is undefined or equals zero then termination should be detected by
polling.

##Constraints

* The lab should be written in ANSI standard C. (the reason for this is that
one can always assume that even old systems adhere to this standard and
the differences to more recent standards as C99 is not that huge)
* The return value from all system calls have to be checked for errors.
* All termination of processes started from the shell must be handled correctly
as not to leave a possibility for creating zombie-processes
* The code should compile without warnings on shell.ict.kth.se/shell.it.kth.se
when compiled with the command "gcc -pedantic -Wall -ansi -O4"
* The shell should behave controlled, i.e. as your normal shell (bash, tcsh)
for all reasonable input to the shell and to processes started from the
shell (for the latter check what happens if you terminate a child process
with Ctrl-C"
* You are not allowed to keep lists of your own for background processes or
communicate with a signal handler by global variables. This simple shell does
not require that - this restriction forces you to better understand what the OS
is doing (i.e.do not do anything unneccessary)
* You may use the POSIX library functions sighold() and sigrelse() if
available.
* You need not be able to handle longer command-line input than 80 characters.
* You are not allowed to use excessive memory or have "memory leaks" (i.e. the
amount of memory allocated through malloc(3C) may not grow over time -
however you probably do not need to allocate any memory in this way)

##Useful resources

* Use the manual pages on shell.ict.kth.se (or the system where you use). You
read these pages by the "man" command". "man -k topic" searches all man-pages
for occurances of topic. man- pages are in general more reliable than other
Internet-sources. Be sure to read all of a man-page down to "return values" and
"errors".
* This small PM may also be of use
http://www.ict.kth.se/courses/2G1520/COURSELIB/labs/AnvandbaraSysanrop.pdf

##Install instructions

SIGDET is off by default.
```
make
./shell
```

In order to override SIGDET please type the following.
```
make CFLAGS='-D SIGDET=1'
./shell
```

In order to obtain the *NIX manual for this program type ```make manual```.
You'll need ```pandoc``` in order to make this work.

##See it in action

[![asciicast](https://asciinema.org/a/32728.png)](https://asciinema.org/a/32728)

##LICENSE
[MIT License](https://en.wikipedia.org/wiki/MIT_License)
