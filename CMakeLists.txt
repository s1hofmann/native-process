cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

project(native-process)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set(LIBS "-framework Foundation -framework Cocoa")
set(INCLUDES "")

if (${MACOSX})
    set(LIBS "${LIBS}" "-framework Foundation -framework Cocoa")
endif()

# cmake-js
set(INCLUDES "${INCLUDES}" "${CMAKE_JS_INC}")
set(LIBS "${LIBS}" "${CMAKE_JS_LIB}")

# N-API
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api")
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# BUILD
target_include_directories(${PROJECT_NAME} PRIVATE "${INCLUDES}")
target_link_libraries(${PROJECT_NAME} "${LIBS}")
