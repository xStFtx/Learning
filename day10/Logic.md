# Logic
## This is for trying to understand some of the logic
### If I have this:
0 3 0 4 1 5 6

PUSH 3 PUSH 4 ADD PRINT HALT
### Steps:
[] - empty stack

[3] - PUSH 3 to the stack

[3,4] - PUSH 4 to the stack

[7] - 3+4 so stack becomes 7; only exists at runtime

Then PRINT; Looks at top of the stack and prints it.

Finally HALT. Execution stops
