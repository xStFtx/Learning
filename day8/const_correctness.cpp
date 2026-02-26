//https://isocpp.org/wiki/faq/const-correctness

#include <iostream>
#include <string>
//Passing by value to a function creates a local copy in the scope of the function that can modified but it is destroyed when you exit that scope.

void f1(const std::string& s);      // Pass by reference-to-const
void f2(const std::string* sptr);   // Pass by pointer-to-const
void f3(std::string s);             // Pass by value

/* void g1(std::string& s);
void f1(const std::string& s)
{
  g1(s);          // Compile-time Error since s is const
  std::string localCopy = s;
  g1(localCopy);  // Okay since localCopy is not const
}
 */
int main() {
     
    return 0;
}