include_directories(${CUDA_INCLUDE_PATH})
add_library(deviceQuery deviceQuery.cpp)
target_link_libraries(deviceQuery ${CUDA_LIBRARY})
