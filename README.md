# libadm - ITU-R BS.2076 Library

[![](https://github.com/ebu/libadm/workflows/Linux/badge.svg)](https://github.com/ebu/libadm/actions?workflow=Linux)
[![](https://github.com/ebu/libadm/workflows/macOS/badge.svg)](https://github.com/ebu/libadm/actions?workflow=macOS)
[![](https://github.com/ebu/libadm/workflows/Windows/badge.svg)](https://github.com/ebu/libadm/actions?workflow=Windows)
[![Documentation Status](https://readthedocs.org/projects/libadm/badge/?version=latest)](https://libadm.readthedocs.io/en/latest/?badge=latest)
[![codecov](https://codecov.io/gh/ebu/libadm/branch/master/graph/badge.svg)](https://codecov.io/gh/ebu/libadm)

## Introduction

The `libadm` library is a modern C++11 library to parse, modify, create and
write [`ITU-R BS.2076`](https://www.itu.int/rec/R-REC-BS.2076) conformant XML. It works well with the header-only
library [`libbw64`](https://github.com/ebu/libbw64) to write ADM
related applications with minimal dependencies.

[Read the documentation](https://libadm.readthedocs.io/en/latest/) to get
started.

## Features

- minimal dependencies
- expressive syntax
- easy access to referenced ADM elements
- common definitions support

## Dependencies

- compiler with C++11 support
- Boost header libraries (version 1.57 or later)
  - Boost.Optional
  - Boost.Variant
  - Boost.Range
  - Boost.Iterator
  - Boost.Functional
  - Boost.Format
- CMake build system (version 3.5 or later)

## Installation

### macOS
On macOS you can use homebrew to install the library. You just have to add the NGA homebrew tap and can then use the usual install command.

```
brew tap ebu/homebrew-nga
brew install libadm
```

### Manual installation
To manually install the library you have to clone the git repository and then use the CMake build system to build and install it.

```
git clone git@github.com:ebu/libadm.git
cd libadm
mkdir build && cd build
cmake ..
make
make install
```

## CMake
As the library uses CMake as a build system it is really easy to set up and use if your project does too. Assuming you have installed the library, the following code shows a complete CMake example to compile a program which uses the libadm.

```
cmake_minimum_required(VERSION 3.5)
project(libadm_example VERSION 1.0.0 LANGUAGES CXX)

find_package(adm REQUIRED)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE adm)
```

If you prefer not to install the library on your system you can also use the library as a subproject. You can just add the library as a CMake subproject. Just add the folder containing the repository to your project and you can use the adm target.

```
cmake_minimum_required(VERSION 3.5)
project(libadm_example VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(submodules/libadm)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE adm)
```

#### Note

If `libadm` is used as a CMake subproject the default values of the options

- `ADM_UNIT_TESTS`
- `ADM_EXAMPLES`
- `ADM_PACKAGE_AND_INSTALL`

are automatically set to `FALSE`.

## Example

The following minimal example shows how easy a valid ADM file can be created
from scratch using the `libadm` library. For more examples have a look at the
[examples folder](examples) in the repository.

```cpp
#include <iostream>
#include <sstream>
#include <adm/adm.hpp>
#include <adm/utilities/object_creation.hpp>
#include <adm/write.hpp>

int main() {
  using namespace adm;

  // create ADM elements
  auto admProgramme = AudioProgramme::create(AudioProgrammeName("Alice and Bob talking"));
  auto speechContent = AudioContent::create(AudioContentName("Speech"));
  auto aliceHolder = createSimpleObject("Alice");
  auto bobHolder = createSimpleObject("Bob");

  // add references
  admProgramme->addReference(speechContent);
  speechContent->addReference(aliceHolder.audioObject);
  speechContent->addReference(bobHolder.audioObject);

  auto admDocument = Document::create();
  admDocument->add(admProgramme);

  // write XML data to stdout
  writeXml(std::cout, admDocument);
  return 0;
}
```

## Current Limitations
It can take time for revisions to Rec. ITU-R BS.2076 to be incorporated into this library. The implementation of the library might not include all possible uses of all the Recommendation.

The areas that are currently unsupported include:
1. Some ADM sub-elements are missing
2. There is no SADM support (ITU-R BS.2125)

## Credits

*libadm* is originally a development of the [IRT](https://www.irt.de).

## Acknowledgement

This project has received funding from the European Union’s Horizon 2020
research and innovation programme under grant agreement No 687645.

## License

```
Copyright 2018-2020 The libadm Authors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```
