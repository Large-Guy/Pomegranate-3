cmake_minimum_required(VERSION 3.5)
project(Pomegranate)

include_directories(Modules/)
add_subdirectory(ThirdParty)
add_subdirectory(Modules/Core)
add_subdirectory(Modules/Platform)

add_subdirectory(Examples)