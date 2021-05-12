.. library_design:

Library Design
##############

The API of the libadm library is probably not self-explanatory if some of the
concepts are not known. This sections aims at filling the gaps.

Named types
-----------

The libadm library makes an extensive use of so called named types. Thus, it is
essential to understand named types to understand the design of the library. So
we begin with a short introduction into named types. For more information on the
implementation details of named types please refer to the blog post series on
`FluentCpp <https://www.fluentcpp.com/2016/12/05/named-constructors/>`_.

The idea of named types is to not use standard types directly but instead wrap
them into a class. This approach has several advantages. Most importantly it
makes the code safer and more expressive. To illustrate this have a look at the
following snippet, which creates an :cpp:class:`adm::AudioContent` object.

.. code:: cpp

    auto speechContentDe = AudioContent::create(AudioContentName("Speech"),
                                                AudioContentLanguage("de"));

It is obvious, that ``Speech`` is the name of the :cpp:class:`adm::AudioContent`
and  ``de`` is the language. Yet still you can compare these types to a
``std::string`` like this:

.. code-block:: cpp

    auto myContentName = AudioContentName("Speech");
    if (myContentName == "Speech") {
      std::cout << "Name of Content is Speech" << std::endl;
    }

If for whatever reason it is really necessary to get the underlying type we can
get it using the get method of the NamedTypes. But this should usually be
avoided.

.. code-block:: cpp

    std::string myContentName = AudioContentName("Speech").get();

As we don't want to manually write a class for each type, named types are
declared using the templated class :cpp:class:`adm::detail::NamedType`.
This makes declaring a new named type quite simple. The declaration for the
:cpp:type:`adm::AudioContentName` for example looks like this.

.. code-block:: cpp

    struct AudioContentNameTag {};
    using AudioContentName = detail::NamedType<std::string, AudioContentNameTag>;

But the named types offer even more functionality. We can add a validator to it.
Some basic validators are already implemented. E. g. the
:cpp:type:`adm::Importance` within the ADM can only have values between 0 and
10. To achieve this we add a :cpp:class:`adm::detail::RangeValidator`
to the type declaration.

.. code-block:: cpp

    struct ImportanceTag {};
    using Importance = detail::NamedType<int, ImportanceTag, detail::RangeValidator<0, 10>>;

Basic structure
---------------

The libadm library is (for now) a quite low level library. Every ADM element has
a class representation (either an ordinary class or a named type). Every class
or named type is named exactly the same as in the ADM. The main ADM elements
(see following list) are then managed by an :cpp:class:`adm::Document`.

 - :cpp:class:`adm::AudioProgramme`
 - :cpp:class:`adm::AudioContent`
 - :cpp:class:`adm::AudioObject`
 - :cpp:class:`adm::AudioTrackUid`
 - :cpp:class:`adm::AudioPackFormat`
 - :cpp:class:`adm::AudioChannelFormat`
 - :cpp:class:`adm::AudioStreamFormat`
 - :cpp:class:`adm::AudioTrackFormat`

.. note:: At the moment there are still some sub-elements missing. Please
  refer to the documentation of the main ADM elements for a list of
  supported/unsupported sub-elements.

The :cpp:class:`adm::Document` and the main ADM elements always have to be
``std::shared_ptr<>``. This is enforced by making the constructors private and
adding static ``create`` functions to each class, which return a
``std::shared_ptr<>``.

.. note:: An ADM element can only belong to one :cpp:class:`adm::Document`!

Once added to an :cpp:class:`adm::Document` they cannot be added to another one.
Trying to do so will result in a ``std::runtime_error``. If you really want to
move or copy an ADM element to another :cpp:class:`adm::Document` the
:cpp:class:`adm::Document::move()` and :cpp:class:`adm::Document::copy()`
functions of the :cpp:class:`adm::Document` have to be used.

As you have maybe noticed the ``AudioBlockFormats`` are not part of the previous
list of main ADM Elements. That's because they are more like a special attribute
of the :cpp:class:`adm::AudioChannelFormat`. As the main ADM elements they
also can only be created as ``std::shared_ptr<>`` s, but instead of the
:cpp:class:`adm::Document` they are managed by the
:cpp:class:`adm::AudioChannelFormat` they belong to. The same principles as for
the main ADM elements and the :cpp:class:`adm::Document` apply here. An
``AudioBlockFormat`` can only belong to one :cpp:class:`adm::AudioChannelFormat`
and if you want to move or copy it you have to use the corresponding functions
of the :cpp:class:`adm::AudioChannelFormat`.

References
----------

References between the basic ADM elements can be established using the
``addReference`` or ``addReferences`` methods. Trying to establish a reference
between two ADM elements which belong to different :cpp:class:`adm::Document`
results in a ``std::runtime_error``. Adding an ADM element to an
:cpp:class:`adm::Document` will automatically add the referenced ADM elements
too.

Overloaded/templated methods whenever possible
----------------------------------------------

As we use classes or named types everywhere, it is quite straight forward to
overload or use templated functions. Sub-elements or attributes of an ADM
element can all be accessed using the following set of functions:

================================== ===============================================================================================
Function                           Explanation
================================== ===============================================================================================
``Parameter get<Parameter>();``    Templated getter method
``void set(Parameter parameter);`` Overloaded setter method
``bool has<Parameter>();``         Returns true if the ADM parameter is set or has a default value.
``void unset<Parameter>();``       Removes the ADM parameter if it is optional or resets it to the default value if there is one.
``bool isDefault<Parameter>();``   Returns true if the current ADM parameter is the default value.
================================== ===============================================================================================

To illustrate the usage here is a simple example which uses them.

.. code-block:: cpp

    JumpPosition jumpPosition;

    if(jumpPosition.has<InterpolationLength>() == true &&
       jumpPosition.isDefault<InterpolationLength>() == true) {
      std::cout << "JumpPositon has a default value for InterpolationLength: "
                << jumpPosition.get<InterpolationLength>() << std::endl;
    }

    jumpPosition.set(InterpolationLength(1.5f));
    if(jumpPosition.has<InterpolationLength>() == true &&
       jumpPosition.isDefault<InterpolationLength>() == false) {
      std::cout << "InterpolationLength is now set to a custom value: "
                << jumpPosition.get<InterpolationLength>() << std::endl;
    }


Constructors with optional arguments in arbitrary order
-------------------------------------------------------

The constructors (or the ``create`` functions in case of the main ADM elements)
also make use of the named types. Using some black template magic they support
optional arguments in arbitrary order. So let us revisit our first named type
example.

.. code-block:: cpp

    auto speechContentDe = AudioContent::create(AudioContentName("Speech"),
                                                AudioContentLanguage("de"));

We can simply reorder :cpp:type:`adm::AudioContentName` and
:cpp:type:`adm::AudioContentLanguage` and have the same result.

.. code-block:: cpp

    auto speechContentDe = AudioContent::create(AudioContentLanguage("de"),
                                                AudioContentName("Speech"));

Reading/writing ADM data
------------------------

Parsing ADM data is as easy as it gets. You just have to include the file
``adm/parse.hpp`` and use one of the :cpp:func:`adm::parseXml()`
functions. You can either pass an ``std::istream``

.. code-block:: cpp

    std::istream myAdmDataStream;
    // Add XML data to stream ...
    auto admDocument = adm::parseXml(myAdmDataStream);

or use the convenience function and pass the name of the input file as a
``std::string``

.. code-block:: cpp

    std::string myFilename("./my_adm_file.xml");
    auto admDocument = adm::parseXml(myFilename);

The same applies for writing an :cpp:class:`adm::Document` to a file or stream.
You just have to include the file ``adm/write.hpp`` and use one of the
:cpp:func:`adm::writeXml()` functions. You can either pass an ``std::ostream``

.. code-block:: cpp

    auto admDocument = adm::Document::create();
    // Add ADM elements ...
    std::ostream xmlStream(...);
    adm::writeXml(xmlStream, admDocument);

or use the convenience function and pass the name of the output file as a `std::string`

.. code-block:: cpp

    auto admDocument = adm::Document::create();
    // Add ADM elements ...
    adm::writeXml("outFilename.xml", admDocument);
