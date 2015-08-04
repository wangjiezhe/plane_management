add_library(Queue SHARED Queue.c)
add_library(PriorityQueue SHARED PriorityQueue.c)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/include)
