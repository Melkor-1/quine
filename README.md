* Quines in C

`quine1.c` is a sample impelemntation of a Quine program in C. `quine2.c`, and 
`quine3.c` are different variants of it.

`quine4.c` is a quine implementation that "clones" itself (makes a replica with
an altered filename). E.g. `"./quine"` will lead to both `quine` and `rvjof` in
the directory, and `rvkof` will lead to `swkpg` and so on. 

Caesar's Cipher is used for the filename alteration. When `./quine` is invoked
the second time, it forks a child process and executes all the cloned
programs, which in turn fork new processes and clone themselves further.

The program acts akin to a fork bomb, but is special in that it is a quine.

`quine5.c` is the same program, but is a shorter version of it.
