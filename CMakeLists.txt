cmake_minimum_required(VERSION 3.25)
project(yt_dl_gui_1_2_0)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(yt_dl_gui_1_2_0
        main.cpp
        Window.cpp
        Window.h)

target_link_libraries(yt_dl_gui_1_2_0
        Qt6::Core
        Qt6::Widgets)
