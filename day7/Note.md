# Day 7

## Memory Model

- Stack vs Heap
- Storage duration - lives as long as the object exists
- new/delete - new allocates an array on the heap. delete frees it. forgetting delete causes memory leaks and UB
- Memory layout
- Undefined behavior (UB) 

Capacity doubling -> amortized O(1) push_back
size_ vs cap_ -> logical size vs allocated memory

Exercise:
- Implement dynamic array using raw pointers.

# Personal Notes
This took a 2 days. Because I had to research exactly how stack vs heap works and the logic behind STL vector. 
I also got a new keyboard(keychron v6 max) and this thing is butter smooth compared to my old one so it is taking some time to get used to it.

I feel more comfortable with pointers in c++. I still have a long way to go though. This dyn_arr is a very simple implementation of the STL vector.
