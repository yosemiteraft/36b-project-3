# Project 2 README

Student 1: Michelle Ho (924442197)

Student 2: Mitee Su

## Project Status
This the current status of the project ...

## Known Issues
The project fails to function properly in ...

## Code References
* [GoogleTest Primer](https://google.github.io/googletest/primer.html)
* [std::vector::push_back](https://cplusplus.com/reference/vector/vector/push_back/)
* [C++ Vectors](https://www.w3schools.com/cpp/cpp_vectors.asp)
* [std::unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map.html)
* [std::unordered_map<Key,T,Hash,KeyEqual,Allocator>::find](https://en.cppreference.com/w/cpp/container/unordered_map/find.html)
* [std::pair::pair](https://cplusplus.com/reference/utility/pair/pair/)
* [std::make_pair](https://cplusplus.com/reference/utility/make_pair/)
* [call to implicitly-deleted default constructor of ‘unordered_map<pair<int, int>, int>](https://medium.com/@gulshansharma014/call-to-implicitly-deleted-default-constructor-of-unordered-map-pair-int-int-int-d3b2a6da0b41)
* [std::stod](https://cplusplus.com/reference/string/stod/)
* [How to Access Elements of a Pair in C++?](https://medium.com/@ayushmishra8299/how-to-access-elements-of-a-pair-in-c-9e4cace740a4#:~:text=Accessing%20Elements%20of%20a%20Pair,element%20we%20want%20to%20access.)
* [Returning an empty string : efficient way in c++](https://stackoverflow.com/questions/26587110/returning-an-empty-string-efficient-way-in-c)
* [Basic Syntax](https://www.markdownguide.org/basic-syntax/)

## Generative AI Use
I used ChatGPT for Prompt 1 and Prompt 2.

### Prompt 1
What type does the .find() function return when used on an unordered map? How do I access the actual value from what .find() returns?

### Response 1
In C++ (STL), when you call .find() on an std::unordered_map, it returns an iterator to the element — not the value itself.

✅ Return Type of .find()

For a map declared as:

std::unordered_map<Key, Value> myMap;


The call:

myMap.find(key);


returns:

std::unordered_map<Key, Value>::iterator


(or const_iterator if the map is const)

✅ What the Iterator Points To

The iterator points to a std::pair<const Key, Value>:

(key, value)


So you must access either .first (key) or .second (value).

✅ How to Access the Value
Method 1 — Using ->second (most common)
auto it = myMap.find(key);

if (it != myMap.end()) {
    Value val = it->second;
}


it->first → key

it->second → value

Method 2 — Using Dereference (*it)
auto it = myMap.find(key);

if (it != myMap.end()) {
    Value val = (*it).second;
}


Equivalent to it->second, just less convenient.

⚠️ Important: Check for .end()

If the key is not found, .find() returns:

myMap.end()


Dereferencing that causes undefined behavior.

Always check first:

if (it != myMap.end()) {
    // safe to use it
}

✅ Example
#include <unordered_map>
#include <iostream>

int main() {
    std::unordered_map<int, std::string> m;
    m[1] = "apple";
    m[2] = "banana";

    auto it = m.find(2);

    if (it != m.end()) {
        std::cout << it->second << std::endl;  // prints "banana"
    }
}


If you want, I can explain:

Difference between .find() and operator[]

How to insert if not found

How this compares to std::map

Common mistakes students make with iterators

Just ask 👍

### Changes 1
I was unsure of how looking for a particular stop ID in the DStopsByID map would work, and what it would return. I realized that .find() returns an iterator, not the direct value. So, using the auto keyword will allow me to access the iterator without having to specify the exact type, which would be much more complex. After that, I can use target->second to access the value of the key-value pair. The response also made me realize that I have to check whether the key was actually found. I added a check to see if the target key is not equal to DStopByID.end(), since .find() returns .end() if the key was not found in the map. This check makes sure that we can proceed to use the specified key-value pair.

### Prompt 2
In C++, how can I use .find() on an unordered_map with two keys?

### Response 2
In C++, std::unordered_map only supports one key per entry. If you conceptually have “two keys,” you need to combine them into a single key type.
Here are the common ways to do it:

✅ Option 1: Use std::pair as the key (most common)
You combine the two keys into a single std::pair<K1, K2> — but you must provide a hash function because unordered_map doesn’t hash pairs by default (pre-C++20).
Example
#include <unordered_map>
#include <utility>

struct PairHash {
    size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

int main() {
    std::unordered_map<std::pair<int,int>, std::string, PairHash> m;

    m[{1, 2}] = "hello";

    auto it = m.find({1, 2});
    if (it != m.end()) {
        // Found
    }
}

✔️ You call .find({key1, key2})

✅ Option 2: Use a custom struct as the key (cleaner for complex cases)
struct Key {
    int a;
    int b;

    bool operator==(const Key& other) const {
        return a == other.a && b == other.b;
    }
};

struct KeyHash {
    size_t operator()(const Key& k) const {
        return std::hash<int>()(k.a) ^ (std::hash<int>()(k.b) << 1);
    }
};

std::unordered_map<Key, int, KeyHash> m;

m[{1, 2}] = 42;

auto it = m.find({1, 2});

✔️ Best when keys have meaning (e.g., coordinates, IDs)

✅ Option 3: Use nested maps (sometimes useful)
std::unordered_map<int, std::unordered_map<int, std::string>> m;

m[1][2] = "hello";

auto it1 = m.find(1);
if (it1 != m.end()) {
    auto it2 = it1->second.find(2);
    if (it2 != it1->second.end()) {
        // Found
    }
}

✔️ Good if the first key partitions data logically
❌ Slightly slower / more verbose

🚀 Recommendation
Use Option 1 (pair) unless you have a strong reason not to.
It’s:


Simple


Efficient


Idiomatic C++



If you want, I can also show:


A reusable generic hash for pairs


How to support more than two keys


How this differs from std::map


C++20/23 improvements


Just ask 👍

### Changes 2
PathByStopIDs() takes in two parameters, the start and end stop IDs. The .find() function only allows us to find an entry using one key, but we have to find a path that takes into account both the start and end stop IDs. I realized that I can utilize a std::pair to combine the start and stop IDs and be able to find a map entry using a singular pair key. However, to work with a more complex key, I changed DPathsByNodes from an unordered_map to a map so that the pair key type will be compatible.

