# --- examples ---

add_executable(create_from_scratch create_from_scratch.cpp)
target_link_libraries(create_from_scratch PRIVATE adm)

add_executable(parse_xml parse_xml.cpp)
target_link_libraries(parse_xml PRIVATE adm)

add_executable(foreground_background foreground_background.cpp)
target_link_libraries(foreground_background PRIVATE adm)

add_executable(multiple_languages_1 multiple_languages_1.cpp)
target_link_libraries(multiple_languages_1 PRIVATE adm)

add_executable(multiple_languages_2 multiple_languages_2.cpp)
target_link_libraries(multiple_languages_2 PRIVATE adm)

add_executable(multiple_storylines multiple_storylines.cpp)
target_link_libraries(multiple_storylines PRIVATE adm)

add_executable(voice_over voice_over.cpp)
target_link_libraries(voice_over PRIVATE adm)

add_executable(create_sadm_from_scratch create_sadm_from_scratch.cpp)
target_link_libraries(create_sadm_from_scratch PRIVATE adm)
