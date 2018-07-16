# --- tools ---

add_executable(read_write_segmenter_combiner read_write_segmenter_combiner.cpp)
target_link_libraries(read_write_segmenter_combiner PRIVATE libadm)

add_executable(resource_embedder resource_embedder.cpp)
