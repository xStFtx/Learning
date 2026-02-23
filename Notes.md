# Day 1 and 2 
I was following along the code crafters build your own redis. But I dont have money to pay for the premium. So I can't continue further on the site so I started going manually and I am hitting a few bumps in the road. This is better because I am not going though a set path. I am trying to understand and research Redis and similar things through exposure instead of a course.

# Day 3 
File descriptors. Change if(clientFd > 0) to if(clientFd >= 0).

Valid file descriptors:
```
0  -> stdin
1  -> stdout
2  -> stderr
3+ -> sockets/files
```

## Working on command parsing. 
First tried an if else primitive setup. But that is not going to cut it. So I want to setup a command table.

# Day 4
I did a quiz to understand C++ better on the lower level. This includes:
- Compilation Model
- Core Language
- Memory Model
- Pointer Deep Dive

I asked Claude to generate a quiz and coding problems for this. I understand most of it. Had a point deduction here and there. Got 37/40.

From here on it is repetition and understanding the compiler better.

Later the day I saw a youtube video and decided to follow along and build a compiler in c++. [Here is the link to what I did. ](https://github.com/xStFtx/Hydrogen).

# Day 5
Exercises:
- Create multi-file project.
- Inspect generated object files.
- Use nm or objdump.

# Day 6
Building my own math library.

I used templates.

So `.hpp` and `.tpp` files.

This is just for learning so there are no fancy buffers or optimizations.