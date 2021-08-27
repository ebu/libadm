set(current_dir ${CMAKE_CURRENT_LIST_DIR})
function(embed_resource)
  # parse function arguments
  set(options "")
  set(oneValueArguments NAMESPACE BASE_NAME)
  set(multiValueArguments RESOURCE_FILES)
  cmake_parse_arguments(EMBED "${options}" "${oneValueArguments}"
                        "${multiValueArguments}" ${ARGN})
  foreach(arg ${oneValueArguments} ${multiValueArguments})
    if(NOT EMBED_${arg})
      message(WARNING "Argument ${arg} not defined in call to embed_resource")
    endif()
  endforeach()

  set_property(
    DIRECTORY
    APPEND
    PROPERTY CMAKE_CONFIGURE_DEPENDS ${EMBED_RESOURCE_FILES})

  set(byte_array_template
      "const char file_@FILE_COUNT@[] = { @FILE_BYTE_ARRAY@ }")
  set(lookup_template
      "if (fileName == \"@FILE_NAME@\") { stream.write(file_@FILE_COUNT@, @ARRAY_SIZE@)\; return true\; }"
  )

  set(FILE_COUNT 1)
  foreach(file_path ${EMBED_RESOURCE_FILES})
    # read resource file as hex with no byte separator
    file(READ ${file_path} file_contents HEX)
    # split into 2 char hex bytes
    string(REGEX MATCHALL "(..)" file_bytes ${file_contents})
    # need length for call to ostream.write()
    list(LENGTH file_bytes ARRAY_SIZE)

    # add 0x prefix and , separator TRANSFORM only valid in cmake 3.14+ or we
    # could just do list(TRANSFORM file_bytes PREPEND "0x") list(JOIN file_bytes
    # "," FILE_BYTE_ARRAY)
    list(POP_FRONT file_bytes first_byte)
    string(PREPEND first_byte "0x")
    list(PREPEND file_bytes ${first_byte})
    list(JOIN file_bytes ",0x" FILE_BYTE_ARRAY)

    # strip path from data file, leaving name
    get_filename_component(FILE_NAME "${file_path}" NAME)
    # generate single byte array from template
    string(CONFIGURE ${byte_array_template} byte_array @ONLY)
    # add the trailing semicolon here as it gets interpreted as a list seperator
    # if in template
    list(APPEND byte_arrays "${byte_array}\;")
    # generate single lookup entry
    string(CONFIGURE "${lookup_template}" lookup @ONLY)
    list(APPEND lookups "${lookup}")
    math(EXPR FILE_COUNT "${FILE_COUNT} + 1")
  endforeach()

  # join entries with newline for readability
  if(WIN32)
    set(line_end "\n\r")
  else()
    set(line_end "\n")
  endif()
  list(JOIN byte_arrays "${line_end}" EMBED_BYTE_ARRAYS)
  list(JOIN lookups "${line_end}" EMBED_LOOKUPS)

  # generate files
  configure_file("${current_dir}/embedded_resource.hpp.in"
                 "${CMAKE_CURRENT_BINARY_DIR}/${EMBED_BASE_NAME}.hpp" @ONLY)
  configure_file("${current_dir}/embedded_resource.cpp.in"
                 "${CMAKE_CURRENT_BINARY_DIR}/${EMBED_BASE_NAME}.cpp" @ONLY)
endfunction()
