.. getting_started:

Getting started
###############

Requirements and dependencies
-----------------------------

The library aims to minimize dependencies to limit the integration work
necessary to use it.

* compiler with C++11 support
* Boost header libraries (version 1.57 or later)

  * Boost.Optional
  * Boost.Variant
  * Boost.Range
  * Boost.Iterator
  * Boost.Functional
  * Boost.Format

* CMake build system (version 3.5 or later)

Installation
------------

macOS
*****

On macOS you can use homebrew to install the library. You just have to
add the IRT's NGA homebrew tap and can then use the usual install command.

.. code:: console

    brew tap irt-open-source/homebrew-nga
    brew install libadm

Manual installation
*******************

To manually install the library you have to clone the git repository and then
use the CMake build system to build and install it.

.. code:: console

    git clone git@github.com:irt-open-source/libadm.git
    cd libadm
    mkdir build && cd build
    cmake ..
    make
    make install

CMake
-----

As the library uses CMake as a build system it is really easy to set up and
use if your project does too. Assuming you have installed the library, the
following code shows a complete CMake example to compile a program which
uses the libadm.

.. code:: console

  cmake_minimum_required(VERSION 3.5)
  project(libadm_example VERSION 1.0.0 LANGUAGES CXX)

  find_package(adm REQUIRED)

  add_executable(examples example.cpp)
  target_link_libraries(example PRIVATE adm)

If you prefer not to install the library on your system you can also use the
library as a subproject. You can just add the library as a CMake subproject.
Just add the folder containing the repository to your project and you can use
the ``adm`` target.

.. code:: console

  cmake_minimum_required(VERSION 3.5)
  project(libadm_example VERSION 1.0.0 LANGUAGES CXX)

  add_subdirectory(submodules/libadm)

  add_executable(example example.cpp)
  target_link_libraries(example PRIVATE adm)

.. note::
    If ``libadm`` is used as a CMake subproject the default values of the options

      * ``ADM_UNIT_TESTS``
      * ``ADM_EXAMPLES``
      * ``ADM_PACKAGE_AND_INSTALL``

    are automatically set to ``FALSE``.
