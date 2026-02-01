# Project 2 README

Student 1: Michelle Ho (924442197)

Student 2: Mitee Su

## Project Status
This the current status of the project ...

## Known Issues
The project fails to function properly in ...

## Code References
* [GoogleTest Primer](https://google.github.io/googletest/primer.html)
* [Using Expat](https://www.xml.com/pub/1999/09/expat/index.html)
* [Getting Started With Expat](https://libexpat.github.io/doc/getting-started/)
* [Expat Internals: A Simple Parse](https://libexpat.github.io/doc/expat-internals-a-simple-parse/)
* [The Expat XML Parser](https://ftp.ccp4.ac.uk/ccp4/7.0/unpacked/checkout/wxPython-src-3.0.2.0/src/expat/doc/reference.html#XML_ParserCreate)
* [C++ Vector](https://www.w3schools.com/cpp/cpp_vectors.asp)
* [std::make_unique, std::make_unique_for_overwrite](https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique)
* [Differences between unique_ptr and shared_ptr](https://stackoverflow.com/questions/6876751/differences-between-unique-ptr-and-shared-ptr)
* [How Do I Add Characters to Strings in C++ The Right Way?](https://learncplusplus.org/how-do-i-add-characters-to-strings-in-c-the-right-way/)
* [Destructors in C++](https://www.geeksforgeeks.org/cpp/destructors-c/)
* [Checking for empty string in C++ alternatives](https://stackoverflow.com/questions/41425569/checking-for-empty-string-in-c-alternatives)
* [How to convert a std::string to const char* or char*](https://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char)
* [C++ vector size() function](https://www.w3schools.com/cpp/ref_vector_size.asp)
* [Assertions Reference](https://google.github.io/googletest/reference/assertions.html)
* [std::string::find](https://cplusplus.com/reference/string/String/Find/)
* [<g>](https://developer.mozilla.org/en-US/docs/Web/SVG/Reference/Element/g)
* [std::string::string](// https://cplusplus.com/reference/string/string/string/)
*[std::queue] (https://en.cppreference.com/w/cpp/container/queue.html)
* [std::deque<T,Allocator>::push_back](https://en.cppreference.com/w/cpp/container/deque/push_back.html)
* [Vector back() in C++ STL](https://www.geeksforgeeks.org/cpp/vector-back-in-cpp-stl/)
* [XML Syntax Rules](https://www.w3schools.com/xml/xml_syntax.asp)
* [Expat XML Parser](https://svn.apache.org/repos/asf/apr/apr-util/vendor/expat/1.95.2/doc/reference.html)
* [queue::empty() and queue::size() in C++ STL](https://www.geeksforgeeks.org/cpp/queueempty-queuesize-c-stl/)
* [Help understanding svg vs stl](https://www.reddit.com/r/impcat/comments/pk51dk/help_understanding_svg_vs_stl/)
* [How to initialize an std::string with a length?](https://stackoverflow.com/questions/27370480/how-to-initialize-an-stdstring-with-a-length)
* [C++ String Concatenation](https://www.w3schools.com/cpp/cpp_strings_concat.asp)
* [Encoding Special Characters in XML](https://www.baeldung.com/xml-encode-special-characters#bd-standard-xml-entities)
* [Why does std::string have a length() member function in addition to size()?](https://stackoverflow.com/questions/905479/why-does-stdstring-have-a-length-member-function-in-addition-to-size)

## Generative AI Use
I used ChatGPT for Prompt 1 and Prompt 2.

### Prompt 1
How can I initialize a shared_ptr?

### Response 1
In C++, there are several common (and safe) ways to initialize a `std::shared_ptr`, depending on what you’re trying to do. Here’s a clear breakdown with examples and guidance on when to use each.

1. Using `std::make_shared` (⭐ **recommended**)

```cpp
#include <memory>

auto ptr = std::make_shared<int>(42);
```

**Why this is preferred:**

* Exception-safe (single allocation for object + control block)
* More efficient than separate allocations
* Cleaner and harder to misuse

Use this whenever you are creating a new object.

2. From `new` (allowed, but usually discouraged)

```cpp
std::shared_ptr<int> ptr(new int(42));
```

⚠️ **Downsides:**

* Two allocations (object + control block)
* Risk of leaks if exceptions occur between allocations
* Easier to accidentally mix with raw pointers

Only use this if you **cannot** use `make_shared` (e.g., custom deleter requirements).

3. With a custom deleter

```cpp
auto deleter = [](FILE* f) {
    if (f) fclose(f);
};

std::shared_ptr<FILE> ptr(fopen("file.txt", "r"), deleter);
```

Use this when managing non-`new` resources (FILE*, sockets, handles, etc.).

4. From an existing `std::unique_ptr`

```cpp
std::unique_ptr<int> uptr = std::make_unique<int>(42);
std::shared_ptr<int> sptr = std::move(uptr);
```

Ownership is transferred from `unique_ptr` to `shared_ptr`.

5. Aliasing constructor (advanced)

```cpp
auto sptr = std::make_shared<std::pair<int, int>>(1, 2);
std::shared_ptr<int> alias(sptr, &sptr->first);
```

* `alias` shares ownership with `sptr`
* Points to a subobject

Useful for advanced lifetime management.

6. Copying or assigning another `shared_ptr`

```cpp
std::shared_ptr<int> p1 = std::make_shared<int>(10);
std::shared_ptr<int> p2 = p1;   // reference count increases
```

What **not** to do 🚫

```cpp
int* raw = new int(5);
std::shared_ptr<int> p1(raw);
std::shared_ptr<int> p2(raw);  // ❌ double delete!
```

Each `shared_ptr` must be created from the **same control block**, not the same raw pointer.

---

## Summary

| Situation                  | Best approach                 |
| -------------------------- | ----------------------------- |
| Creating a new object      | `std::make_shared<T>(...)`    |
| Custom cleanup needed      | `shared_ptr` + custom deleter |
| Converting ownership       | Move from `unique_ptr`        |
| Sharing existing ownership | Copy another `shared_ptr`     |

If you want, tell me your **specific use case** (object type, ownership model, custom cleanup, etc.), and I can recommend the safest initialization pattern.

### Changes 1
At first, I initialized std::shared_ptr with std::make_shared and the auto keyword:
    auto sink = std::make_shared<CDataSink>();  
But after Discussion section, I changed that to use a more straightforward method by using:
    std::shared_ptr<CDataSink> DSink;

### Prompt 2
Given this constructor, how can I give the parser callback functions? SImplementation(std::shared_ptr< CDataSource > src){ DSource = src; DParser = XML_ParserCreate(nullptr); }

### Response 2
That constructor looks like you’re using Expat (XML_ParserCreate), so the way you give the parser callback functions is by registering handlers on the XML_Parser object after you create it.

The key ideas are:

Call the appropriate XML_Set*Handler functions

Use XML_SetUserData so your callbacks can access the SImplementation instance

Make the callbacks static (or free functions), then forward to the object

Basic pattern (Expat-style)
1. Constructor: register handlers
SImplementation::SImplementation(std::shared_ptr<CDataSource> src) {
    DSource = src;
    DParser = XML_ParserCreate(nullptr);

    // Let callbacks know which object they belong to
    XML_SetUserData(DParser, this);

    // Register callbacks
    XML_SetElementHandler(
        DParser,
        &SImplementation::StartElement,
        &SImplementation::EndElement
    );

    XML_SetCharacterDataHandler(
        DParser,
        &SImplementation::CharacterData
    );
}

2. Callback function declarations

Expat requires C-style function signatures, so these must be static:

class SImplementation {
public:
    SImplementation(std::shared_ptr<CDataSource> src);

private:
    std::shared_ptr<CDataSource> DSource;
    XML_Parser DParser;

    // Static callbacks
    static void StartElement(
        void* userData,
        const char* name,
        const char** attrs
    );

    static void EndElement(
        void* userData,
        const char* name
    );

    static void CharacterData(
        void* userData,
        const char* data,
        int length
    );

    // Non-static helpers (optional)
    void OnStartElement(const char* name, const char** attrs);
};

3. Forward callbacks to the instance
void SImplementation::StartElement(
    void* userData,
    const char* name,
    const char** attrs
) {
    auto* self = static_cast<SImplementation*>(userData);
    self->OnStartElement(name, attrs);
}

void SImplementation::EndElement(void* userData, const char* name) {
    auto* self = static_cast<SImplementation*>(userData);
    // handle end element
}

void SImplementation::CharacterData(
    void* userData,
    const char* data,
    int length
) {
    auto* self = static_cast<SImplementation*>(userData);
    // handle character data
}

Why this works (important concept)

Expat is a C library, so it can’t call C++ member functions directly

XML_SetUserData gives you a void* back in every callback

You store this there and cast it back inside the callback

This lets callbacks behave like real member functions

Common mistakes to avoid 🚫

❌ Trying to use non-static member functions as callbacks

❌ Forgetting XML_SetUserData

❌ Capturing lambdas (Expat can’t use them)

If you want, I can:

Show a minimal working XML parse loop

Explain how to handle attributes safely

Help design a clean RAII wrapper for XML_Parser

Just tell me 👍

### Changes 2
I added code to the SImplementation constructor to make sure that the constructed object has callback functions that the parser can use to process elements that it is picking up from the data source. The line XML_SetUserData(DParser, this); makes sure that when the parser finds an element and callback functions are called, a pointer to the current SImplementation object is passed in as the data parameter. With this, the callback functions have access to the parser, source, and queue. The line XML_SetElementHandler(DParser, ExpatStartElement, ExpatEndElement); saves the two callback functions to the parser so that the parser can call the functions when it reads in a start tag or closing tag. The line XML_SetCharacterDataHandler(DParser, ExpatCharacterData); 
saves the ExpatCharacterData function to the parser so that the parser can call the function if it reads text between tags.

### Prompt 3
What could be causing a bug where XMLReader is only returning partial character data in my code? static void ExpatCharacterData(void *data, const XML_Char *s, int len){ SImplementation *Implementation = (SImplementation*) data; std::string datastr(s, len); SXMLEntity entity; entity.DType = SXMLEntity::EType::CharData; // https://cplusplus.com/reference/string/string/string/ entity.DNameData = std::string(s, len); Implementation->DQueue.push(entity); }

### Response 3
This is a *classic* SAX/XMLReader gotcha — your code is actually fine, but the **assumption behind it isn’t** 🙂

### What’s going on

In SAX-style parsers (including Expat / XMLReader), **character data is not guaranteed to be delivered in one piece**.

That means this callback:

```cpp
static void ExpatCharacterData(void *data, const XML_Char *s, int len)
```

may be called **multiple times for what looks like a single block of text in the XML**.

So instead of:

```xml
<name>Hello world</name>
```

producing one call with `"Hello world"`,

you might get:

```
"Hello "
"world"
```

or even smaller chunks.

### Why this happens

SAX parsers are:

* **Streaming**
* **Buffer-based**
* **Allowed to split text arbitrarily**

Character data may be split because of:

* Internal buffer boundaries
* Entity expansion
* Encoding conversions
* Mixed content
* Parser optimizations

The XML spec explicitly allows this behavior.

### Why it looks like a bug

Your callback correctly constructs:

```cpp
std::string(s, len);
```

…but you treat each callback as a *complete* text node and push it immediately:

```cpp
Implementation->DQueue.push(entity);
```

So downstream code sees “partial” text.

### The correct way to handle character data

You must **accumulate character data until the enclosing element ends**.

Typical pattern:

1. On `startElement` → clear a buffer
2. On `characterData` → append to buffer
3. On `endElement` → consume the full buffer

Example fix:

```cpp
static void ExpatCharacterData(void *data, const XML_Char *s, int len) {
    SImplementation *Implementation = static_cast<SImplementation*>(data);
    Implementation->currentText.append(s, len);
}
```

Then on end element:

```cpp
if (!Implementation->currentText.empty()) {
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::CharData;
    entity.DNameData = Implementation->currentText;
    Implementation->DQueue.push(entity);
    Implementation->currentText.clear();
}
```

### Key takeaway (the important rule)

> **Never assume a single `characterData` callback corresponds to a single text node.**

If you don’t accumulate, you *will* see partial data — and that’s 100% expected behavior, not a parser bug.

If you want, I can also:

* Show how to ignore pure-whitespace callbacks
* Explain how this interacts with mixed content
* Help redesign the queue model to be SAX-safe

Just say the word 👍

### Changes 3
ExpatCharacterData was being called multiple times when parsing long character data. This caused separate entities to be created for each call to ExpatCharacterData, so this was causing my tests to fail. To fix this, I had to add this if statement: if(!Implementation->DQueue.empty() && Implementation->DQueue.back().DType == SXMLEntity::EType::CharData,
to check whether the last entity in queue is CharData. If it is CharData, I appended the data to the existing entity to avoid created a new one. However, if the last entity is not CharData, or if the queue is now empty, it should create a new entity. This fixes the problem where one entity is incorrectly broken up into multiple parts.