# Changelog

## Unreleased

### Added
- Added support for Cartesian speaker positions.
- Added support for AudioChannelFormatIDRef in AudioTrackUID as per BS.2076-2
- Added support for dB gains. `Gain{1.0}` should be replaced with `Gain::fromLinear(1.0)`, and `b.get<Gain>().get()` should be replaced with `b.get<Gain>().asLinear()`.
- Added support for headphoneVirtualise in audioBlockFormat as per BS.2076-2.

### Changed
- SpeakerPosition is now a boost::variant that can be either a CartesianSpeakerPosition or a SphericalSpeakerPosition
- The previous SpeakerPosition type has been renamed to SphericalSpeakerPosition.
- included mono (0+1+0) to the common definitions lookup tables
- corrected (0+5+0) to point to 5.1 pack (AP_00010003) in common definitions lookup table
- included LFE in common definitions lookup table
- multiple incorrect references to LFE1 changed to LFE in common definitions lookup tables
- corrected B-045 AudioTrackFormat reference in common definitions lookup table
- fixed erroneous test acceptance data
- updated required C++ standard from C++11 to C++14
- implemented fractional time format from BS.2076-2
- audioProgramme and audioContent may now have multiple loudnessMetadata elements, as per BS.2076-2

### Fixed
- updateBlockFormatDurations now throws an exception when given an audioChannelFormat with no audioBlockFormats, rather than segfaulting

## 0.12.0 (April 18, 2020)

### Added
- new `addSimpleCommonDefinitionsObjectTo` function
- new `addSimpleObjectTo` function
- added support to lookup HOA common definitions AudioPackFormatIDs and AudioTrackFormatIDs
- added missing ITU-R BS.2051 setups 0+7+0 and 4+7+0 to common definition lookup tables

### Changed
- improved `AudioChannelFormat::assignId` logic - huge performance increase for large documents

### Fixed
- fixed bug were not all references were removed if AudioPackFormat was removed from document

## 0.11.0 (Oktober 11, 2019)

### Added
- library can now also be used as a CMake subproject
- new CMake option `ADM_HIDE_INTERNAL_SYMBOLS`
- new CMake option `ADM_PACKAGE_AND_INSTALL`
- new CMake option `BUILD_SHARED_LIBS`
- `audioPackFormat` now supports typeDefinition `HOA`

### Changed
- Renamed CMake library target name from `libadm` to `adm`
- Renamed CMake option `UNIT_TESTS` to `ADM_UNIT_TESTS`
- Renamed CMake option `EXAMPLES` to `ADM_EXAMPLES`
- properly implemented the `LoudnessMetadata` class
- improved common definitions handling
- drastically improved performance by enhancing hex and ID parsing
- boost will automatically be found when finding libadm
- hide symbols only is shared library is build

### Fixed
- An unresolvable reference will now result in an exception instead of a segfault when parsing XML.
- Always return true for values with default values in has<...>() methods.
- The dialogue subelement will now be written by the xml writer

## 0.10.0 (November 30, 2018)

### Added
- Added helper function to access optional properties from elements, return a supplied default value if it hasn't been set
- Add utility functions to (re-)calculate block format durations
- Added `adm::ReaderOption` to select `AudioFormatExtended` node search mode

### Changed
- Use `Catch2` instead of `Boost.Test` for unit testing
- Refactored XmlParser tests to use separate files for test data
- Improved search for `AudioFormatExtended` node when parsing XML

### Fixed
- Documentation fixes and clarifications
- Fixed visibility issues of some methods that prevented linking with the shared library


## 0.9.0 (July 23, 2018)

Initial release
