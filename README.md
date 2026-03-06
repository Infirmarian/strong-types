# Strong Types

### A single header library to provide simple C++ strong types

C++ is lacking in a built-in strong type system to easily create strong types. With this library, it is simple to create new types that cannot be easily swapped, creating a compile-time checked rich type system. Additionally with the use of traits, users of the library are able to add and remove functionality. See the examples below.

#### Installation

This is a single header library, so just copy `include/strong_type.hpp` into your project. The CMake setup here is purely for running tests.

#### Support

This library is built and tested against C++11, 14, 17 and 20. It builds with -Wall, -Wextra and -Wpedantic

#### Examples

For the most basic example, simply include the header, define the strong type and use in code. This example includes no traits. By default, strong types can only be constructed, compared for equality/inequality and hashed.

```cpp
// Include the single header
#include "strong_type.hpp"

// Create tags for specific types
struct UserIdTag{};

// Define the strong type, including optional traits
using UserId = rg::strong_type<uint64_t, UserIdTag>;

// Use the strong type in APIs
User load_user(const UserId id);
```

Note in this example, since we added no traits, it is a compiler error to do the following

```cpp
UserId a{1};
UserId b{2};

assert(a < b); // No Orderable trait
a++; // No Incrementable trait
a + b; // No Addable trait
User c; // No DefaultConstructible trait
```

For more examples and the process of building out this library, check out my blog post
[Strong Types](https://hotpath.substack.com/p/strong-types)
