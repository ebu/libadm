# Changelog

## Unreleased

### Added
- library can now also be used as a CMake subproject
- new CMake option `ADM_HIDE_INTERNAL_SYMBOLS`
- new CMake option `ADM_PACKAGE_AND_INSTALL`

### Changed
- Rename cmake rename library target name from `libadm` to `adm`
- Renamed CMake option `UNIT_TESTS` to `ADM_UNIT_TESTS`
- Renamed CMake option `EXAMPLES` to `ADM_EXAMPLES`

### Fixed
- An unresolvable reference will now result in an exception instead of a segfault when parsing XML.

## 0.10.0 - 2018-11-30

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


## 0.9.0 - 2018-07-24

Initial release
