cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0074 NEW)

project(Pomegranate CXX)

set(CMAKE_CXX_STANDARD 23)

include_directories(Modules/)
add_subdirectory(ThirdParty)
add_subdirectory(Modules/Core)
add_subdirectory(Modules/Platform)
add_subdirectory(Modules/Events)
add_subdirectory(Modules/Framework)
add_subdirectory(Modules/Rendering)
add_subdirectory(Modules/Layout)
add_subdirectory(Modules/UI)

add_subdirectory(Examples)