# Static-Vector

A statically allocated Vector-like type. Great for embedded applications

## Example 

```c++

// Create a vector that has max of 16 values
StaticVector<int, 16> vec;

// Same(ish) interface as std::vector
vec.push_back(4);
vec.push_back(2);

```
