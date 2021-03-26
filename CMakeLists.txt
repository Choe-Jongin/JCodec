get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)

string(REPLACE " " "_" ProjectId ${ProjectId})

project(${ProjectId} C CXX)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

cmake_minimum_required(VERSION 2.8)
find_package( OpenCV REQUIRED )
 
file(GLOB SOURCES  *.cpp)
 
add_executable(${PROJECT_NAME} ${SOURCES}  )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )

