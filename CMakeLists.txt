cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

project(nut-native-bridge)

if ("${CMAKE_HOST_APPLE}")
    file(GLOB PLATTFORM "${CMAKE_SOURCE_DIR}/src/apple/*.cpp")
elseif ("${CMAKE_HOST_UNIX}")
    file(GLOB PLATTFORM "${CMAKE_SOURCE_DIR}/src/linux/*.cpp")
elseif ("${CMAKE_HOST_WIN32}")
    file(GLOB PLATTFORM "${CMAKE_SOURCE_DIR}/src/win32/*.cpp")
endif()

file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
set(SOURCE_FILES "${SOURCE_FILES}" "${PLATTFORM}")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set(LIBS "")
set(INCLUDES "")

if ("${CMAKE_HOST_APPLE}")
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
