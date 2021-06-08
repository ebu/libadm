.. libadm documentation master file

.. image:: irt_logo.png
   :alt: IRT Logo

------------------------------

libadm – ITU-R BS.2076 Library
==============================

.. toctree::
  :hidden:
  :maxdepth: 1

  getting_started
  tutorial
  library_design
  changelog

.. toctree::
  :hidden:
  :maxdepth: 2
  :caption: Reference

  reference_document
  element_api
  reference_elements
  reference_helpers
  reference_utilities
  reference_read_write

The `libadm <https://github.com/ebu/libadm>`_ library is a modern
C++11 library to parse, modify, create and write `Recommendation ITU-R BS.2076-1
<https://www.itu.int/rec/R-REC-BS.2076/en>`_ conform XML document. It works well
with the header-only library `libbw64
<https://github.com/ebu/libbw64>`_ to write ADM related applications
with minimal dependencies.

Features
--------

- minimal dependencies
- expressive syntax
- easy access to referenced ADM elements
- common definitions support

Acknowledgement
---------------

This project has received funding from the European Union’s Horizon 2020
research and innovation programme under grant agreement No 687645.
