# --- adm library ---

include(${PROJECT_SOURCE_DIR}/submodules/rapidxml.cmake)

include(embed_resource)
embed_resource(
        NAMESPACE adm
        BASE_NAME resources
        RESOURCE_FILES ${PROJECT_SOURCE_DIR}/resources/common_definitions.xml)

add_library(adm
  document.cpp
  errors.cpp
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
  elements/audio_pack_format_hoa.cpp
  elements/common_parameters.cpp
  elements/time.cpp
  elements/channel_lock.cpp
  elements/frequency.cpp
  elements/gain_interaction_range.cpp
  elements/jump_position.cpp
  elements/label.cpp
  elements/loudness_metadata.cpp
  elements/object_divergence.cpp
  elements/position.cpp
  elements/position_offset.cpp
  elements/position_interaction_range.cpp
  elements/screen_edge_lock.cpp
  elements/speaker_position.cpp
  elements/type_descriptor.cpp
  elements/format_descriptor.cpp
  elements/headphone_virtualise.cpp
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
  private/base_xml_parser.cpp
  detail/id_assigner.cpp
  parse.cpp
  parse_sadm.cpp
  write.cpp
  frame.cpp
  serial/audio_track.cpp
  serial/frame_format.cpp
  serial/frame_format_id.cpp
  serial/frame_header.cpp
  serial/transport_track_format.cpp
  serial/transport_id.cpp
  serial/sadm_xml_parser.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/resources.hpp
  ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp
)

target_include_directories(adm
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # resources.hpp
  PUBLIC
  # Headers used from source/build location:
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>  # config.h / export.h
  # Headers used from installed location:
    $<INSTALL_INTERFACE:${ADM_INSTALL_INCLUDE_DIR}>
)

target_link_libraries(adm PUBLIC Boost::boost)
target_link_libraries(adm PRIVATE $<BUILD_INTERFACE:rapidxml>)

if (UNIX)
  target_link_libraries(adm PUBLIC dl)
endif()

set_property(TARGET adm PROPERTY POSITION_INDEPENDENT_CODE ON)

if(ADM_HIDE_INTERNAL_SYMBOLS AND BUILD_SHARED_LIBS)
  set_target_properties(adm PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN TRUE)
endif()

############################################################
# enable C++14 support
############################################################
if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
  # Note: this is not a complete list of c++ features required by libadm.
  # What we want is C++14 support and this is a simple way to trigger
  # this for CMake < 3.8
  target_compile_features(adm PUBLIC cxx_generic_lambdas)
else()
  target_compile_features(adm PUBLIC cxx_std_14)
endif()
set_target_properties(adm PROPERTIES CXX_EXTENSIONS OFF)

include(GenerateExportHeader)
set(EXPORT_EXTRA_CONTENT
    "
// use on an extern class template instantiation to force exporting of methods;
// not required on MSVC where exported methods of explicitly instantiated class
// templates are actually exported
#ifdef _MSC_VER
#define ADM_EXPORT_TEMPLATE_METHODS
#else
#define ADM_EXPORT_TEMPLATE_METHODS ADM_EXPORT
#endif
")
generate_export_header(adm
  EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/adm/export.h
  CUSTOM_CONTENT_FROM_VARIABLE EXPORT_EXTRA_CONTENT
)

############################################################
# export package
############################################################
if(ADM_PACKAGE_AND_INSTALL)
  include(CMakePackageConfigHelpers)

  configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/config/admConfig.cmake.in
    ${PROJECT_BINARY_DIR}/admConfig.cmake
    INSTALL_DESTINATION
    ${ADM_INSTALL_CMAKE_DIR}
    PATH_VARS ADM_INSTALL_INCLUDE_DIR ADM_INSTALL_LIB_DIR ADM_INSTALL_CMAKE_DIR
  )

  write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/admConfigVersion.cmake
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
          DESTINATION ${ADM_INSTALL_INCLUDE_DIR}
          PATTERN "private*" EXCLUDE
  )
  install(FILES ${PROJECT_BINARY_DIR}/adm/version.hpp
          DESTINATION ${ADM_INSTALL_INCLUDE_DIR}/adm
  )
  install(FILES ${PROJECT_BINARY_DIR}/adm/export.h
          DESTINATION ${ADM_INSTALL_INCLUDE_DIR}/adm
  )

  install(FILES
    ${PROJECT_BINARY_DIR}/admConfig.cmake
    ${PROJECT_BINARY_DIR}/admConfigVersion.cmake
    DESTINATION ${ADM_INSTALL_CMAKE_DIR}
  )

  if(${BUILD_SHARED_LIBS})
    install(TARGETS adm
      EXPORT admTargetsShared
      LIBRARY DESTINATION "${ADM_INSTALL_LIB_DIR}"
      RUNTIME DESTINATION "${ADM_INSTALL_BIN_DIR}"
      ARCHIVE DESTINATION "${ADM_INSTALL_LIB_DIR}"
      PUBLIC_HEADER DESTINATION "${ADM_INSTALL_INCLUDE_DIR}"
    )
    install(EXPORT admTargetsShared DESTINATION ${ADM_INSTALL_CMAKE_DIR})
    if(MSVC)
      install(FILES
              $<$<CONFIG:Debug,RelWithDebInfo>:$<TARGET_PDB_FILE:adm>>
              DESTINATION
              ${ADM_INSTALL_BIN_DIR})
    endif()

  else()
    install(TARGETS adm
      EXPORT admTargetsStatic
      LIBRARY DESTINATION "${ADM_INSTALL_LIB_DIR}"
      RUNTIME DESTINATION "${ADM_INSTALL_BIN_DIR}"
      ARCHIVE DESTINATION "${ADM_INSTALL_LIB_DIR}"
      PUBLIC_HEADER DESTINATION "${ADM_INSTALL_INCLUDE_DIR}"
    )
    install(EXPORT admTargetsStatic DESTINATION ${ADM_INSTALL_CMAKE_DIR})
  endif()
endif()
