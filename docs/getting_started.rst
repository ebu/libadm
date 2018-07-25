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

.. code:: console

    git clone git@github.com:irt-open-source/libadm.git
    cd libadm
    mkdir build && cd build
    cmake ..
    make
    make install
