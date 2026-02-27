# Day 9
Classes and RAII

- Constructors
- Destructors
- Rule of 3
- Rule of 5
- Copy vs Move
- RAII


Now what is RAII? 
-
RAII stand for Resource Acquisition Is Initialization. 

It is a technique that binds the life cycle of a resource to the lifetime of an object.

Now that is just copied from https://en.cppreference.com/w/cpp/language/raii.html. 

So this Day 9 learning is for me to fully explore and test what it really is and how it works.

Full ownership of an object's resources.

After further analyses I saw that I already did RAII in my day7 dyn_arr. It is when you have allocate in the constructor and free in the destructor. I also ahve a deep copy in my copy constructor. I know my dyn_arr isn't perfect but it I am still learning the memory models.

## Copy assignment and making deep copies
Allocate first to preserve the original object in case allocation throws, ensuring strong exception safety.

## Notes on my str implementation

It is basically the same as my dyn_arr. Except that I didnt use templates because stings are dynamic arrays of chars. So we already know the data type of the elements.

Also I didnt know about cap_+1 and '\0'.