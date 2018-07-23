# libadm - ITU-R BS.2076 Library

## Introduction

The `libadm` library is a modern C++11 library to parse, modify, create and
write ITU-R BS.2076-1 conform XML document. It works well with the header-only
library [`libbw64`](https://github.com/irt-open-source/libbw64) to write ADM
related applications with minimal dependencies.

## Features

- minimal dependencies
- expressive syntax
- easy access to referenced ADM elements
- common definitions support

## Dependencies

- compiler with C++11 support
- Boost header libraries (version 1.59 or later)
  - Boost.Optional
  - Boost.Variant
  - Boost.Range
  - Boost.Iterator
  - Boost.Functional
  - Boost.Format
  - Boost.Test (unit tests only, header-only version)
  - Boost.MPL (unit tests only)
  - Boost.TypeTraits (unit tests only)
- CMake build system (version 3.5 or later)

## Installation

```
git clone git@github.com:irt-open-source/libadm.git
cd libadm
mkdir build && cd build
cmake ..
make
make install
```

## Example

The following minimal example shows how easy a valid ADM file can be created
from scratch using the `libadm` library. For more examples have a look at the
[examples folder](examples) in the repository.

```cpp
#include <iostream>
#include <sstream>
#include <adm/adm.hpp>
#include <adm/utilities/object_creation.hpp>
#include <adm/xml_writer.hpp>

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
  std::stringstream xmlStream;
  writeXml(xmlStream, admDocument);
  std::cout << xmlStream.str();
  return 0;
}
```

## Current Limitations

1. some ADM sub-elements are missing
2. no SADM support (ITU-R BS.[SERIAL])

## Acknowledgement

This project has received funding from the European Union’s Horizon 2020
research and innovation programme under grant agreement No 687645.

## License

```
Copyright 2018 Institut fuer Rundfunktechnik GmbH (IRT)

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
