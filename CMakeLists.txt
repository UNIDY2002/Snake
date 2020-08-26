# Generated with qt-init
# See: https://github.com/UNIDY2002/qt-init

cmake_minimum_required(VERSION 3.15.4)
project(Snake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_COMPILER C:/Qt/Tools/mingw810_64/bin/gcc)
set(CMAKE_CXX_COMPILER C:/Qt/Tools/mingw810_64/bin/g++)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH C:/Qt/5.15.0/mingw81_64)

find_package(Qt5 COMPONENTS Core Gui Multimedia Widgets REQUIRED)

aux_source_directory(. DIRS_SRCS)

add_executable(Snake main.cpp game.cpp game.h constants.h types.h utils.cpp utils.h)

target_link_libraries(Snake Qt5::Core Qt5::Gui Qt5::Multimedia Qt5::Widgets)
