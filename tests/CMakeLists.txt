include(${PROJECT_SOURCE_DIR}/submodules/catch2.cmake)

set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test_data")
configure_file(test_config.hpp.in test_config.hpp)

# working directory for tests containing misc. outputs
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/test_data_out")

# --- unit tests ---
function(add_adm_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name}
    PRIVATE
    adm
    catch2
  )
  target_include_directories(${name} PRIVATE
    ${PROJECT_SOURCE_DIR}/submodules
    # for including test_config.h
    ${CMAKE_CURRENT_BINARY_DIR}
  )
  add_test(
    NAME ${name}
    COMMAND $<TARGET_FILE:${name}>
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/test_data_out"
  )
endfunction()

add_adm_test("adm_auto_parenting_tests")
add_adm_test("adm_common_definitions_tests")
add_adm_test("adm_document_tests")
add_adm_test("adm_id_tests")
add_adm_test("adm_time_tests")
add_adm_test("audio_block_format_binaural_tests")
add_adm_test("audio_block_format_direct_speakers_tests")
add_adm_test("audio_block_format_hoa_tests")
add_adm_test("audio_block_format_matrix_tests")
add_adm_test("audio_block_format_objects_tests")
add_adm_test("audio_channel_format_tests")
add_adm_test("audio_content_tests")
add_adm_test("audio_object_interaction_tests")
add_adm_test("audio_object_tests")
add_adm_test("audio_pack_format_tests")
add_adm_test("audio_pack_format_hoa_tests")
add_adm_test("audio_programme_tests")
add_adm_test("audio_stream_format_tests")
add_adm_test("audio_track_format_tests")
add_adm_test("audio_track_uid_tests")
add_adm_test("block_duration_fixing_tests")
add_adm_test("channel_lock_tests")
add_adm_test("dialogue_tests")
add_adm_test("enum_bitmask_options_tests")
add_adm_test("format_descriptor_tests")
add_adm_test("frequency_tests")
add_adm_test("gain_interaction_range_tests")
add_adm_test("hex_values_tests")
add_adm_test("jump_position_tests")
add_adm_test("loudness_metadata_tests")
add_adm_test("named_type_tests")
add_adm_test("object_creation_tests")
add_adm_test("object_divergence_tests")
add_adm_test("position_interaction_range_tests")
add_adm_test("position_tests")
add_adm_test("route_tracer_tests")
add_adm_test("screen_edge_lock_tests")
add_adm_test("speaker_position_tests")
add_adm_test("type_descriptor_tests")
add_adm_test("xml_parser_audio_block_format_direct_speakers_tests")
add_adm_test("xml_parser_audio_block_format_objects_tests")
add_adm_test("xml_parser_audio_block_format_hoa_tests")
add_adm_test("xml_parser_audio_channel_format_tests")
add_adm_test("xml_parser_audio_content_tests")
add_adm_test("xml_parser_audio_object_tests")
add_adm_test("xml_parser_audio_pack_format_tests")
add_adm_test("xml_parser_audio_programme_tests")
add_adm_test("xml_parser_audio_stream_format_tests")
add_adm_test("xml_parser_audio_track_format_tests")
add_adm_test("xml_parser_audio_track_uid_tests")
add_adm_test("xml_parser_common_definitions_tests")
add_adm_test("xml_parser_unresolved_references_tests")
add_adm_test("xml_parser_find_audio_format_extended_tests")
add_adm_test("xml_parser_tests")
add_adm_test("xml_writer_audio_object_interaction_tests")
add_adm_test("xml_writer_audio_programme_tests")
add_adm_test("xml_writer_audio_block_format_tests")
add_adm_test("xml_writer_audio_content_tests")
add_adm_test("xml_writer_objects_creation_tests")
add_adm_test("xml_writer_tests")
