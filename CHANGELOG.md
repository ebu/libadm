# Changelog

## Unreleased

### Changed
- Rename cmake rename library target name from `libadm` to `adm`


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