# --- examples ---

add_executable(create_from_scratch create_from_scratch.cpp)
target_link_libraries(create_from_scratch
  PRIVATE
  libadm
)

add_executable(parse_xml parse_xml.cpp)
target_link_libraries(parse_xml
  PRIVATE
  libadm
)

add_executable(foreground_background foreground_background.cpp)
target_link_libraries(foreground_background
  PRIVATE
  libadm
)

add_executable(multiple_languages_1 multiple_languages_1.cpp)
target_link_libraries(multiple_languages_1
  PRIVATE
  libadm
)

add_executable(multiple_languages_2 multiple_languages_2.cpp)
target_link_libraries(multiple_languages_2
  PRIVATE
  libadm
)

add_executable(multiple_storylines multiple_storylines.cpp)
target_link_libraries(multiple_storylines
  PRIVATE
  libadm
)

add_executable(voice_over voice_over.cpp)
target_link_libraries(voice_over
  PRIVATE
  libadm
)
