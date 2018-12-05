# --- adm library ---

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/resources.hpp
  COMMAND resource_embedder common_definitions.xml > ${CMAKE_BINARY_DIR}/resources.hpp
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/resources
  DEPENDS resource_embedder ${CMAKE_SOURCE_DIR}/resources/common_definitions.xml
  COMMENT "Running resource embedder."
  VERBATIM
)

add_library(adm
  document.cpp
  common_definitions.cpp
  elements/audio_programme.cpp
  elements/audio_content.cpp
  elements/audio_object.cpp
  elements/audio_pack_format.cpp
  elements/audio_channel_format.cpp
  elements/audio_stream_format.cpp
  elements/audio_track_format.cpp
  elements/audio_track_uid.cpp
  elements/audio_programme_id.cpp
  elements/audio_content_id.cpp
  elements/audio_object_id.cpp
  elements/audio_pack_format_id.cpp
  elements/audio_channel_format_id.cpp
  elements/audio_block_format_id.cpp
  elements/audio_track_format_id.cpp
  elements/audio_stream_format_id.cpp
  elements/audio_track_uid_id.cpp
  elements/audio_block_format_direct_speakers.cpp
  elements/audio_block_format_matrix.cpp
  elements/audio_block_format_objects.cpp
  elements/audio_block_format_hoa.cpp
  elements/audio_block_format_binaural.cpp
  elements/audio_object_interaction.cpp
  elements/time.cpp
  elements/channel_lock.cpp
  elements/frequency.cpp
  elements/gain_interaction_range.cpp
  elements/jump_position.cpp
  elements/object_divergence.cpp
  elements/position.cpp
  elements/position_interaction_range.cpp
  elements/screen_edge_lock.cpp
  elements/speaker_position.cpp
  elements/type_descriptor.cpp
  elements/format_descriptor.cpp
  utilities/block_duration_assignment.cpp
  utilities/copy.cpp
  utilities/id_assignment.cpp
  utilities/object_creation.cpp
  path.cpp
  private/copy.cpp
  private/rapidxml_wrapper.cpp
  private/rapidxml_formatter.cpp
  private/xml_writer.cpp
  private/xml_parser.cpp
  detail/hex_values.cpp
  detail/id_assigner.cpp
  parse.cpp
  write.cpp
  ${CMAKE_BINARY_DIR}/resources.hpp
)

target_include_directories(adm
  PUBLIC
  # Headers used from source/build location:
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>  # config.h / export.h
  # Headers used from installed location:
    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

target_include_directories(adm SYSTEM
  PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/submodules>
)

target_link_libraries(adm PUBLIC Boost::boost)

if (UNIX)
  target_link_libraries(adm PUBLIC dl)
endif()

include(GenerateExportHeader)
generate_export_header(adm
  EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/adm/export.h
)

############################################################
# export package
############################################################
include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_SOURCE_DIR}/config/admConfig.cmake.in
  ${CMAKE_BINARY_DIR}/admConfig.cmake
  INSTALL_DESTINATION
  ${INSTALL_CMAKE_DIR}
  PATH_VARS INSTALL_INCLUDE_DIR INSTALL_LIB_DIR INSTALL_CMAKE_DIR
)

write_basic_package_version_file(
  ${CMAKE_BINARY_DIR}/admConfigVersion.cmake
  COMPATIBILITY SameMajorVersion
)


############################################################
# install adm library
############################################################
set_target_properties(adm PROPERTIES
  VERSION "${PROJECT_VERSION}"
  SOVERSION "${PROJECT_VERSION_MAJOR}"
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION ${INSTALL_INCLUDE_DIR}
        PATTERN "private*" EXCLUDE
)
install(FILES ${PROJECT_BINARY_DIR}/adm/version.hpp
        DESTINATION ${INSTALL_INCLUDE_DIR}/adm
)
install(FILES ${PROJECT_BINARY_DIR}/adm/export.h
        DESTINATION ${INSTALL_INCLUDE_DIR}/adm
)

install(FILES
  ${CMAKE_BINARY_DIR}/admConfig.cmake
  ${CMAKE_BINARY_DIR}/admConfigVersion.cmake
  DESTINATION ${INSTALL_CMAKE_DIR}
)

if(${BUILD_SHARED_LIBS})
  install(TARGETS adm
    EXPORT admTargetsShared
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
    RUNTIME DESTINATION "${INSTALL_LIB_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
  )
  install(EXPORT admTargetsShared DESTINATION ${INSTALL_CMAKE_DIR})
else()
  install(TARGETS adm
    EXPORT admTargetsStatic
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
    RUNTIME DESTINATION "${INSTALL_LIB_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
  )
  install(EXPORT admTargetsStatic DESTINATION ${INSTALL_CMAKE_DIR})
endif()
