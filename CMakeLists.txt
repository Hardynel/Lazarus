cmake_minimum_required(VERSION 3.20)

project(OpenGL_Project)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(SOURCE_EXE ${CMAKE_PROJECT_NAME})

include(${CMAKE_SOURCE_DIR}/cmake/lighting/sources_list.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/lighting/shaders_list.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/images_list.cmake)


include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

if(WIN32)
    set(LIBS glfw3 opengl32)
elseif(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
    find_package(OpenGL REQUIRED)
    add_definitions(${OPENGL_DEFINITIONS})
    find_package(X11 REQUIRED)
    set(LIBS ${GLFW3_LIBRARY} X11 GL)
else()
    set(LIBS )
endif(WIN32)

add_library(STB_IMAGE ${CMAKE_SOURCE_DIR}/src/stb_image.cpp)
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD ${CMAKE_SOURCE_DIR}/src/glad.c)
set(LIBS ${LIBS} GLAD)

add_executable(${SOURCE_EXE} ${SOURCES})
target_include_directories(${SOURCE_EXE} PUBLIC ${CMAKE_SOURCE_DIR}/include/)
target_link_directories(${SOURCE_EXE} PUBLIC ${CMAKE_SOURCE_DIR}/lib/)
target_link_libraries(${SOURCE_EXE} ${LIBS})

if(WIN32)
    set_target_properties(${SOURCE_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
    set_target_properties(${SOURCE_EXE} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Debug")
elseif(UNIX AND NOT APPLE)
    set_target_properties(${SOURCE_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
endif(WIN32)

foreach(SHADER ${SHADERS})
    add_custom_command(TARGET ${SOURCE_EXE} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${SOURCE_EXE}>)
endforeach(SHADER)

foreach(IMAGE ${IMAGES})
    add_custom_command(TARGET ${SOURCE_EXE} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${IMAGE} $<TARGET_FILE_DIR:${SOURCE_EXE}>)
endforeach(IMAGE)
#   file(COPY ${SHADER}i DESTINATION ${CMAKE_SOURCE_DIR}/build/src)
