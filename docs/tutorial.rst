.. tutorial:

Tutorial
########

In this tutorial we will create a simple object-based ADM document and write it
to ``std::cout``. We assume that ``libadm`` is installed and, that the
``include`` path is added to the ``PATH`` and you are linking with the library.
We also assume, that you have at least basic knowledge on how an ADM file is
structured.

First example
-------------

Let us have a look at the following first example.

.. code-block:: cpp

    #include <iostream>
    #include <sstream>
    #include <adm/adm.hpp>
    #include <adm/xml_writer.hpp>

    int main() {
      using namespace adm;

      auto admDocument = Document::create();

      std::stringstream xmlStream;
      writeXml(xmlStream, admDocument);
      std::cout << xmlStream.str();
      return 0;
    }

For most of the functionality of the library only the header ``adm/adm.hpp`` has
to be included. As we simultaneously want to see how our ADM document takes
shape we also included ``adm/xml_writer.hpp``. This header contains the
declaration of the :cpp:func:`adm::writeXml()` functions. These functions can be
used to write an ADM document to an ``std::ostream`` or a file. Apart from that
not much is happening yet. We just create a :cpp:class:`adm::document`, which is
the class representation of a whole ADM file.

.. code-block:: xml

    <?xml version="1.0" encoding="utf-8"?>
    <ebuCoreMain xmlns:dc="http://purl.org/dc/elements/1.1/"
                 xmlns="urn:ebu:metadata-schema:ebuCore_2014"
                 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                 schema="EBU_CORE_20140201.xsd" xml:lang="en">
      <coreMetadata>
        <format>
          <audioFormatExtended/>
        </format>
      </coreMetadata>
    </ebuCoreMain>

As our ADM document is still empty the output is no suprise: the
``audioFormatExtended`` node does not contain any ADM elements, but the basic
``ebuCore`` structure is already there.

.. note:: For all following XML code examples we are omitting the EBUcore
    structure for reasons of simplicity and only include the
    ``audioFormatExtended`` node.

Adding content
--------------

So let us fill our ADM document with some content.

.. code-block:: cpp

    #include <iostream>
    #include <sstream>
    #include <adm/adm.hpp>
    #include <adm/xml_writer.hpp>

    int main() {
      using namespace adm;

      auto admDocument = Document::create();

      auto admProgramme = AudioProgramme::create(
          AudioProgrammeName("Alice and Bob talking in the forrest"));
      auto speechContent = AudioContent::create(AudioContentName("Speech"));
      auto atmoContent = AudioContent::create(AudioContentName("Atmo"));

      admDocument->add(admProgramme);
      admDocument->add(speechContent);
      admDocument->add(atmoContent);

      std::stringstream xmlStream;
      writeXml(xmlStream, admDocument);
      std::cout << xmlStream.str();
      return 0;
    }

We have created an audioProgramme and two audioContent ADM elements and added
them to our document.

.. code-block:: xml

    <audioFormatExtended>
      <audioProgramme audioProgrammeID="APR_1001" audioProgrammeName="Alice and Bob talking in the forrest"/>
      <audioContent audioContentID="ACO_1001" audioContentName="Speech"/>
      <audioContent audioContentID="ACO_1002" audioContentName="Atmo"/>
    </audioFormatExtended>

Note that the IDs of the ADM elements are already properly set. This
automatically happens when an ADM element is added to an
:cpp:class:`adm::Document`. This is usually very convenient. But in some cases
one might want to manually set the ID. If an ADM element already has an ID, the
:cpp:class:`adm::IdAssigner` will use the ID if the ID is not already in use in
the document. If it is, the :cpp:class:`adm::IdAssigner` will increase the ID
value until it finds an ID which is not used yet.

.. code-block:: cpp

    #include <iostream>
    #include <sstream>
    #include <adm/adm.hpp>
    #include <adm/xml_writer.hpp>
    #include <adm/utilities/object_creation.hpp>

    int main() {
      using namespace adm;

      auto admDocument = Document::create();

      auto admProgramme = AudioProgramme::create(
          AudioProgrammeName("Alice and Bob talking in the forrest"));
      auto speechContent = AudioContent::create(AudioContentName("Speech"));
      auto atmoContent = AudioContent::create(AudioContentName("Atmo"));
      auto aliceHolder = createSimpleObject("Alice");
      auto bobHolder = createSimpleObject("Bob");

      admDocument->add(admProgramme);
      admDocument->add(speechContent);
      admDocument->add(atmoContent);
      admDocument->add(aliceHolder.audioObject);
      admDocument->add(bobHolder.audioObject);

      admProgramme->addReference(speechContent);
      admProgramme->addReference(atmoContent);
      speechContent->addReference(aliceHolder.audioObject);
      speechContent->addReference(bobHolder.audioObject);

      std::stringstream xmlStream;
      writeXml(xmlStream, admDocument);
      std::cout << xmlStream.str();
      return 0;
    }

As a next step we added two "objects". In an object-based situation we usually
always have the same composition of audioObject, audioTrackUID, audioPackFormat,
audioChannelFormat, audioStreamFormat, audioTrackFormat ADM elements. To
simplify the process of creating an "object", we use the utility function
:cpp:func:`adm::createSimpleObject()`. It creates all the necessary ADM elements
and adds the references.

The output of our programme is now as follows:

.. code-block:: xml

    <audioFormatExtended>
      <audioProgramme audioProgrammeID="APR_1001" audioProgrammeName="Alice and Bob talking in the forrest">
        <audioContentIDRef>ACO_1001</audioContentIDRef>
        <audioContentIDRef>ACO_1002</audioContentIDRef>
      </audioProgramme>
      <audioContent audioContentID="ACO_1001" audioContentName="Speech">
        <audioObjectIDRef>AO_1001</audioObjectIDRef>
        <audioObjectIDRef>AO_1002</audioObjectIDRef>
      </audioContent>
      <audioContent audioContentID="ACO_1002" audioContentName="Atmo"/>
      <audioObject audioObjectID="AO_1001" audioObjectName="Alice">
        <audioPackFormatIDRef>AP_00031001</audioPackFormatIDRef>
        <audioTrackUIDRef>ATU_00000001</audioTrackUIDRef>
      </audioObject>
      <audioObject audioObjectID="AO_1002" audioObjectName="Bob">
        <audioPackFormatIDRef>AP_00031002</audioPackFormatIDRef>
        <audioTrackUIDRef>ATU_00000002</audioTrackUIDRef>
      </audioObject>
      <audioPackFormat audioPackFormatID="AP_00031001" audioPackFormatName="Alice" typeLabel="0003" typeDefinition="Objects">
        <audioChannelFormatIDRef>AC_00031001</audioChannelFormatIDRef>
      </audioPackFormat>
      <audioPackFormat audioPackFormatID="AP_00031002" audioPackFormatName="Bob" typeLabel="0003" typeDefinition="Objects">
        <audioChannelFormatIDRef>AC_00031002</audioChannelFormatIDRef>
      </audioPackFormat>
      <audioChannelFormat audioChannelFormatID="AC_00031001" audioChannelFormatName="Alice" typeLabel="0003" typeDefinition="Objects"/>
      <audioChannelFormat audioChannelFormatID="AC_00031002" audioChannelFormatName="Bob" typeLabel="0003" typeDefinition="Objects"/>
      <audioStreamFormat audioStreamFormatID="AS_00031001" audioStreamFormatName="Alice" formatLabel="0001" formatDefinition="PCM">
        <audioChannelFormatIDRef>AC_00031001</audioChannelFormatIDRef>
        <audioTrackFormatIDRef>AT_00031001_01</audioTrackFormatIDRef>
      </audioStreamFormat>
      <audioStreamFormat audioStreamFormatID="AS_00031002" audioStreamFormatName="Bob" formatLabel="0001" formatDefinition="PCM">
        <audioChannelFormatIDRef>AC_00031002</audioChannelFormatIDRef>
        <audioTrackFormatIDRef>AT_00031002_01</audioTrackFormatIDRef>
      </audioStreamFormat>
      <audioTrackFormat audioTrackFormatID="AT_00031001_01" audioTrackFormatName="Alice" formatLabel="0001" formatDefinition="PCM">
        <audioStreamFormatIDRef>AS_00031001</audioStreamFormatIDRef>
      </audioTrackFormat>
      <audioTrackFormat audioTrackFormatID="AT_00031002_01" audioTrackFormatName="Bob" formatLabel="0001" formatDefinition="PCM">
        <audioStreamFormatIDRef>AS_00031002</audioStreamFormatIDRef>
      </audioTrackFormat>
      <audioTrackUID UID="ATU_00000001">
        <audioTrackFormatIDRef>AT_00031001_01</audioTrackFormatIDRef>
        <audioPackFormatIDRef>AP_00031001</audioPackFormatIDRef>
      </audioTrackUID>
      <audioTrackUID UID="ATU_00000002">
        <audioTrackFormatIDRef>AT_00031002_01</audioTrackFormatIDRef>
        <audioPackFormatIDRef>AP_00031002</audioPackFormatIDRef>
      </audioTrackUID>
    </audioFormatExtended>

But wait, we only added the audioObject to our document and all the elements
created by :cpp:func:`adm::createSimpleObject()` are now also part of the
document. This is because the :cpp:func:`adm::Document::add()` function
automatically adds all referenced ADM elements too. Knowning this we can
simplify our programme, while still getting the exact same output. We just add
all our references first and only add the audioProgramme to the document.

.. code-block:: cpp

    #include <iostream>
    #include <sstream>
    #include <adm/adm.hpp>
    #include <adm/xml_writer.hpp>
    #include <adm/utilities/object_creation.hpp>

    int main() {
      using namespace adm;

      auto admDocument = Document::create();

      auto admProgramme = AudioProgramme::create(
          AudioProgrammeName("Alice and Bob talking in the forrest"));
      auto speechContent = AudioContent::create(AudioContentName("Speech"));
      auto atmoContent = AudioContent::create(AudioContentName("Atmo"));
      auto aliceHolder = createSimpleObject("Alice");
      auto bobHolder = createSimpleObject("Bob");

      admProgramme->addReference(speechContent);
      admProgramme->addReference(atmoContent);
      speechContent->addReference(aliceHolder.audioObject);
      speechContent->addReference(bobHolder.audioObject);

      admDocument->add(admProgramme);

      std::stringstream xmlStream;
      writeXml(xmlStream, admDocument);
      std::cout << xmlStream.str();
      return 0;
    }

Using Common Definitions
------------------------

As a next step we will add a channel bed to our document. The channel bed we are
adding is a standard stereo signal. So we are going to use the common
definitions. The first thing we need to do is add them to our document.

.. code-block:: cpp

    #include <adm/common_definitions.hpp>
    #include <adm/utilities/copy.hpp>
    ...
    auto commonDefDoc = getCommonDefinitions(); // load common definitions
    deepCopyTo(commonDefDoc, admDocument);      // copy common definitions to our doc

Using the function :cpp:func:`adm::deepCopyTo()` ensures, that all the
references are preserved. Then we manually create our audioObject and the two
audioTrackUIDs for the left and right channel.

.. code-block:: cpp

    auto atmoObject = AudioObject::create(AudioObjectName("Forrest Atmo"));
    auto trackUidLeft = AudioTrackUid::create();
    auto trackUidRight = AudioTrackUid::create();

What is now missing is the connection between our object and the common
definition ADM elements. To simplify the identification of the necessary ADM
elements there are two lookup tables you can use. Those map the loudspeaker IDs
and speaker labels specified in ITU-R BS.2051 to the corresponding ADM element
IDs. To get the right ADM elements those IDs can then be used to look them up in
the ADM document.

.. code-block:: cpp

    auto packFormatLookup = audioPackFormatLookupTable();
    auto trackFormatLookup = audioTrackFormatLookupTable();

    auto packFormatStereo = admDocument->lookup(packFormatLookup.at("0+2+0"));
    auto trackFormatLeft = admDocument->lookup(trackFormatLookup.at("M+030"));
    auto trackFormatRight = admDocument->lookup(trackFormatLookup.at("M-030"));

    trackUidLeft->setReference(trackFormatLeft);
    trackUidRight->setReference(trackFormatRight);
    trackUidLeft->setReference(packFormatStereo);
    trackUidRight->setReference(packFormatStereo);

    atmoObject->addReference(trackUidLeft);
    atmoObject->addReference(trackUidRight);
    atmoObject->addReference(packFormatStereo);

That's it. We are done.

.. code-block:: cpp

    #include <iostream>
    #include <sstream>
    #include <adm/adm.hpp>
    #include <adm/xml_writer.hpp>
    #include <adm/utilities/object_creation.hpp>
    #include <adm/common_definitions.hpp>
    #include <adm/utilities/copy.hpp>

    int main() {
      using namespace adm;

      auto admDocument = Document::create();

      auto admProgramme = AudioProgramme::create(
          AudioProgrammeName("Alice and Bob talking in the forrest"));
      auto speechContent = AudioContent::create(AudioContentName("Speech"));
      auto atmoContent = AudioContent::create(AudioContentName("Atmo"));
      auto aliceHolder = createSimpleObject("Alice");
      auto bobHolder = createSimpleObject("Bob");

      auto commonDefDoc = getCommonDefinitions();
      deepCopyTo(commonDefDoc, admDocument);

      auto atmoObject = AudioObject::create(AudioObjectName("Forrest Atmo"));
      auto trackUidLeft = AudioTrackUid::create();
      auto trackUidRight = AudioTrackUid::create();

      auto packFormatLookup = audioPackFormatLookupTable();
      auto trackFormatLookup = audioTrackFormatLookupTable();

      auto packFormatStereo = admDocument->lookup(packFormatLookup.at("0+2+0"));
      auto trackFormatLeft = admDocument->lookup(trackFormatLookup.at("M+030"));
      auto trackFormatRight = admDocument->lookup(trackFormatLookup.at("M-030"));

      trackUidLeft->setReference(trackFormatLeft);
      trackUidRight->setReference(trackFormatRight);
      trackUidLeft->setReference(packFormatStereo);
      trackUidRight->setReference(packFormatStereo);

      atmoObject->addReference(trackUidLeft);
      atmoObject->addReference(trackUidRight);
      atmoObject->addReference(packFormatStereo);

      admProgramme->addReference(speechContent);
      admProgramme->addReference(atmoContent);
      atmoContent->addReference(atmoObject);
      speechContent->addReference(aliceHolder.audioObject);
      speechContent->addReference(bobHolder.audioObject);

      admDocument->add(admProgramme);

      std::stringstream xmlStream;
      writeXml(xmlStream, admDocument);  // write XML data to stdout
      std::cout << xmlStream.str();
      return 0;
    }

Now let us have a final look at the output.

.. code-block:: xml

    <audioFormatExtended>
      <audioProgramme audioProgrammeID="APR_1001" audioProgrammeName="Alice and Bob talking in the forrest">
        <audioContentIDRef>ACO_1002</audioContentIDRef>
        <audioContentIDRef>ACO_1001</audioContentIDRef>
      </audioProgramme>
      <audioContent audioContentID="ACO_1001" audioContentName="Atmo">
        <audioObjectIDRef>AO_1001</audioObjectIDRef>
      </audioContent>
      <audioContent audioContentID="ACO_1002" audioContentName="Speech">
        <audioObjectIDRef>AO_1002</audioObjectIDRef>
        <audioObjectIDRef>AO_1003</audioObjectIDRef>
      </audioContent>
      <audioObject audioObjectID="AO_1001" audioObjectName="Forrest Atmo">
        <audioPackFormatIDRef>AP_00010002</audioPackFormatIDRef>
        <audioTrackUIDRef>ATU_00000001</audioTrackUIDRef>
        <audioTrackUIDRef>ATU_00000002</audioTrackUIDRef>
      </audioObject>
      <audioObject audioObjectID="AO_1002" audioObjectName="Alice">
        <audioPackFormatIDRef>AP_00031001</audioPackFormatIDRef>
        <audioTrackUIDRef>ATU_00000003</audioTrackUIDRef>
      </audioObject>
      <audioObject audioObjectID="AO_1003" audioObjectName="Bob">
        <audioPackFormatIDRef>AP_00031002</audioPackFormatIDRef>
        <audioTrackUIDRef>ATU_00000004</audioTrackUIDRef>
      </audioObject>
      <audioPackFormat audioPackFormatID="AP_00031001" audioPackFormatName="Alice" typeLabel="0003" typeDefinition="Objects">
        <audioChannelFormatIDRef>AC_00031001</audioChannelFormatIDRef>
      </audioPackFormat>
      <audioPackFormat audioPackFormatID="AP_00031002" audioPackFormatName="Bob" typeLabel="0003" typeDefinition="Objects">
        <audioChannelFormatIDRef>AC_00031002</audioChannelFormatIDRef>
      </audioPackFormat>
      <audioChannelFormat audioChannelFormatID="AC_00031001" audioChannelFormatName="Alice" typeLabel="0003" typeDefinition="Objects"/>
      <audioChannelFormat audioChannelFormatID="AC_00031002" audioChannelFormatName="Bob" typeLabel="0003" typeDefinition="Objects"/>
      <audioStreamFormat audioStreamFormatID="AS_00031001" audioStreamFormatName="Alice" formatLabel="0001" formatDefinition="PCM">
        <audioChannelFormatIDRef>AC_00031001</audioChannelFormatIDRef>
        <audioTrackFormatIDRef>AT_00031001_01</audioTrackFormatIDRef>
      </audioStreamFormat>
      <audioStreamFormat audioStreamFormatID="AS_00031002" audioStreamFormatName="Bob" formatLabel="0001" formatDefinition="PCM">
        <audioChannelFormatIDRef>AC_00031002</audioChannelFormatIDRef>
        <audioTrackFormatIDRef>AT_00031002_01</audioTrackFormatIDRef>
      </audioStreamFormat>
      <audioTrackFormat audioTrackFormatID="AT_00031001_01" audioTrackFormatName="Alice" formatLabel="0001" formatDefinition="PCM">
        <audioStreamFormatIDRef>AS_00031001</audioStreamFormatIDRef>
      </audioTrackFormat>
      <audioTrackFormat audioTrackFormatID="AT_00031002_01" audioTrackFormatName="Bob" formatLabel="0001" formatDefinition="PCM">
        <audioStreamFormatIDRef>AS_00031002</audioStreamFormatIDRef>
      </audioTrackFormat>
      <audioTrackUID UID="ATU_00000001">
        <audioTrackFormatIDRef>AT_00010001_01</audioTrackFormatIDRef>
        <audioPackFormatIDRef>AP_00010002</audioPackFormatIDRef>
      </audioTrackUID>
      <audioTrackUID UID="ATU_00000002">
        <audioTrackFormatIDRef>AT_00010002_01</audioTrackFormatIDRef>
        <audioPackFormatIDRef>AP_00010002</audioPackFormatIDRef>
      </audioTrackUID>
      <audioTrackUID UID="ATU_00000003">
        <audioTrackFormatIDRef>AT_00031001_01</audioTrackFormatIDRef>
        <audioPackFormatIDRef>AP_00031001</audioPackFormatIDRef>
      </audioTrackUID>
      <audioTrackUID UID="ATU_00000004">
        <audioTrackFormatIDRef>AT_00031002_01</audioTrackFormatIDRef>
        <audioPackFormatIDRef>AP_00031002</audioPackFormatIDRef>
      </audioTrackUID>
    </audioFormatExtended>

As the idea of the common definitons is, that those ADM elements don't need to
be written, even though we added common definition ADM elements to our document
the XML writer does not write them.

Setting  block format durations
-------------------------------

According to ITU-R BS. 2076, multiple AudioBlockFormats in an :cpp:class:`adm::AudioChannelFormat`
should all have a rtime and a duration.

In practice, however, it can be very hard to determine the duration of an `adm::AudioBlockFormat`
during its creation or setup.
This is due to the fact that an :cpp:class:`adm::AudioChannelFormat`, and thus its blocks and their durations,
is bound to the parent :cpp:class:`adm::AudioObject` duration. The lifetime of the :cpp:class:`adm::AudioObject`, if not given explictily, is 
bound to the length of the :cpp:class:`adm::AudioProgramme` or, if that's not set either, to the length of the file.

Thus, it's easy to imagine situations where not all informations are available during the setup of `adm::AudioBlockFormat` s.

This library provides some utility functions that are supposed to postpone the duration setting to a later point in time when all information are available,
and therefore should help in writing standard conforment ADM documents.

Consider the following code:

.. code-block:: cpp

    using namespace adm;
    auto document = Document::create();
    auto programme = AudioProgramme::create(AudioProgrammeName{"main"});
    auto content1 = AudioContent::create(AudioContentName{"main"});
    programme->addReference(content1);
    auto object1 = AudioObject::create(AudioObjectName{"object1"});
    content1->addReference(object1);
    auto pack1 = AudioPackFormat::create(AudioPackFormatName{"pack1"},
                                    TypeDefinition::OBJECTS);
    object1->addReference(pack1);
    auto channel1 = AudioChannelFormat::create(AudioChannelFormatName{"channel1"},
                                          TypeDefinition::OBJECTS);
    channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                          Rtime{std::chrono::milliseconds(0)}));
    channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                          Rtime{std::chrono::milliseconds(100)}));

Neither the referencing :cpp:class:`adm::AudioObject` nor the main :cpp:class:`adm::AudioProgramme` might have a duration or an endtime.
Thus, the duration of the second block added to the :cpp:class:`adm::AudioChannelFormat` `channel1` depends
on the length of the audio signals, which might not be known at this point in time.

When it is known, for example when writing a `BW64` file with the ADM document contained as an `axml` chunk,
one might known the actual length of the file.
Then, one can use the utility function :cpp:func:`adm::updateBlockFormatDurations()` to, well, update all block format durations
with their correct values:

.. code-block:: cpp

    // ... somehow we know that our file will be 5 seconds long

    updateBlockFormatDurations(document, std::chrono::seconds(5));

    // now, continue with writing the xml chunk to disk or something similar


Depending on the use case, the file length might not be nessecery or there might not even be a file with audio signals.
Multiple variants of :cpp:func:`adm::updateBlockFormatDurations` are therefore provided to accomodate all use cases.

