# MacStd

Provides templates and function helpers for OS X API using Standard C++ in XCode 13+

## Features

### Portable

This project does not require building. Just `#include` individual files from this repository into your source code and get started.

### Functions and Templates

Different OS X API functions have different ways of returning variable-sized data. Getting tired of carefully studying manpages for each particular API function how to preallocate the output memory correctly? We too...

MacStd provides a subset of OS X API identically named functions (C++ polymorphism to the rescue), where one can use `std::string`, `std::wstring`, `std::vector<>` etc. as an output parameter. MacStd handles all the dirty work with memory allocation for you, so you can focus on your code.

#### Example

```C++
std::string path;
_NSGetExecutablePath(path);
std::cout << path.c_str() << std::endl;
```

## What MacStd Is Not

MacStd is not trying to be a full-fledged object-oriented framework on top of OS X API. MacStd aims at augmenting OS X API with a little bit of help from C++.

## Usage

1. Clone the repository into your solution folder.
2. Add MacStd's `include` folder to _Header Search Paths_ in your project's C/C++ settings.
3. Include `.h` files from MacStd as needed:
```C++
#include <MacStd/dyld.hpp>
#include <string>
#include <iostream>

using namespace std;

void main()
{
    wstring path;
    _NSGetExecutablePath(path);
    wcout << path.c_str() << endl;
}
```

An auto-generated documentation is [here](https://amebis.github.io/MacStd/).

More examples can also be found in the `Tests` project.

This is a one-man project for the time being, so the OS X API support is far from complete. It is added as needed. Contributions are welcome.
