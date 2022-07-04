#!/bin/bash
DIRS="examples include src tests"

IGNORE="
src/elements/time.cpp
src/elements/audio_pack_format.cpp
src/elements/audio_track_format_id.cpp
src/elements/audio_block_format_id.cpp
src/elements/audio_block_format_hoa.cpp
src/document.cpp
src/private/xml_parser.cpp
include/adm/elements/audio_block_format_hoa.hpp
include/adm/elements/audio_pack_format.hpp
include/adm/elements/audio_channel_format.hpp
include/adm/elements/normalization.hpp
include/adm/elements/audio_pack_format_hoa.hpp
include/adm/elements/nfc_ref_dist.hpp
tests/hex_values_tests.cpp
tests/adm_common_definitions_tests.cpp
tests/xml_parser_audio_block_format_hoa_tests.cpp
tests/xml_parser_audio_block_format_objects_tests.cpp
tests/xml_parser_audio_pack_format_tests.cpp
"

find $DIRS \( -iname '*.cpp' -or -iname '*.hpp' \) $(printf "! -wholename %s " $IGNORE) -exec clang-format -style=file -i '{}' +
