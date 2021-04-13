.. reference_elements:

ADM Elements
############

AudioProgramme
--------------

.. doxygenclass:: adm::AudioProgramme
.. doxygenclass:: adm::AudioProgrammeId
.. doxygentypedef:: adm::AudioProgrammeName
.. doxygentypedef:: adm::AudioProgrammeLanguage
.. doxygentypedef:: adm::End
.. doxygentypedef:: adm::MaxDuckingDepth
.. doxygenclass:: adm::AudioProgrammeReferenceScreen

AudioContent
------------

.. doxygenclass:: adm::AudioContent
.. doxygenclass:: adm::AudioContentId
.. doxygentypedef:: adm::AudioContentName
.. doxygentypedef:: adm::AudioContentLanguage
.. doxygentypedef:: adm::DialogueId
.. doxygennamespace:: adm::Dialogue
.. doxygentypedef:: adm::ContentKind
.. doxygentypedef:: adm::NonDialogueContentKind
.. doxygennamespace:: adm::NonDialogueContent
.. doxygentypedef:: adm::DialogueContentKind
.. doxygennamespace:: adm::DialogueContent
.. doxygentypedef:: adm::MixedContentKind
.. doxygennamespace:: adm::MixedContent

AudioObject
-----------

.. doxygenclass:: adm::AudioObject
.. doxygenclass:: adm::AudioObjectId
.. doxygentypedef:: adm::AudioObjectName
.. doxygentypedef:: adm::Interact
.. doxygentypedef:: adm::DisableDucking
.. doxygenclass:: adm::AudioObjectInteraction
.. doxygentypedef:: adm::OnOffInteract
.. doxygentypedef:: adm::GainInteract
.. doxygentypedef:: adm::PositionInteract
.. doxygenclass:: adm::GainInteractionRange
.. doxygenclass:: adm::PositionInteractionRange

AudioTrackUid
-------------

.. doxygenclass:: adm::AudioTrackUid
.. doxygenclass:: adm::AudioTrackUidId
.. doxygentypedef:: adm::SampleRate
.. doxygentypedef:: adm::BitDepth

AudioPackFormat
---------------

.. doxygenclass:: adm::AudioPackFormat
.. doxygenclass:: adm::AudioPackFormatId
.. doxygentypedef:: adm::AudioPackFormatName
.. doxygentypedef:: adm::AbsoluteDistance

AudioChannelFormat
------------------

.. doxygenclass:: adm::AudioChannelFormat
.. doxygenclass:: adm::AudioChannelFormatId
.. doxygentypedef:: adm::AudioChannelFormatName
.. doxygenclass:: adm::Frequency

AudioStreamFormat
-----------------
.. doxygenclass:: adm::AudioStreamFormat
.. doxygenclass:: adm::AudioStreamFormatId
.. doxygentypedef:: adm::AudioStreamFormatName

AudioTrackFormat
----------------
.. doxygenclass:: adm::AudioTrackFormat
.. doxygenclass:: adm::AudioTrackFormatId
.. doxygentypedef:: adm::AudioTrackFormatName

AudioBlockFormat
----------------

As the audioBlockFormat ADM elements are quite different for each typeDefinition
there are five different ``AudioBlockFormat`` classes.

.. warning:: The typeDefinitions ``Matrix`` and ``HOA`` are not completely
  supported yet.


.. doxygenclass:: adm::AudioBlockFormatDirectSpeakers
.. doxygenclass:: adm::CartesianSpeakerPosition
.. doxygenclass:: adm::SphericalSpeakerPosition

.. doxygenclass:: adm::AudioBlockFormatMatrix

.. doxygenclass:: adm::AudioBlockFormatObjects
.. doxygentypedef:: adm::Cartesian
.. doxygentypedef:: adm::Position
.. doxygenclass:: adm::SphericalPosition
.. doxygenclass:: adm::CartesianPosition
.. doxygentypedef:: adm::Width
.. doxygentypedef:: adm::Height
.. doxygentypedef:: adm::Depth
.. doxygenclass:: adm::ScreenEdgeLock
.. doxygentypedef:: adm::ScreenEdge
.. doxygentypedef:: adm::HorizontalEdge
.. doxygentypedef:: adm::VerticalEdge
.. doxygentypedef:: adm::Gain
.. doxygentypedef:: adm::Diffuse
.. doxygenclass:: adm::ChannelLock
.. doxygentypedef:: adm::ChannelLockFlag
.. doxygentypedef:: adm::MaxDistance
.. doxygenclass:: adm::ObjectDivergence
.. doxygenclass:: adm::JumpPosition
.. doxygentypedef:: adm::JumpPositionFlag
.. doxygentypedef:: adm::InterpolationLength
.. doxygentypedef:: adm::ScreenRef

.. doxygenclass:: adm::AudioBlockFormatHoa

.. doxygenclass:: adm::AudioBlockFormatBinaural

Shared Elements
---------------

.. doxygentypedef:: adm::Start
.. doxygenclass:: adm::LoudnessMetadata
.. doxygentypedef:: adm::Importance
.. doxygenclass:: adm::AudioBlockFormatId
.. doxygentypedef:: adm::Rtime
.. doxygentypedef:: adm::Duration
