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