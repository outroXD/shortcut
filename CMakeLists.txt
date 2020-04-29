cmake_minimum_required(VERSION 3.10)
project(shortcut)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS
        Core
        Widgets
        Gui
        REQUIRED)

include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

# TODO あまり良い書き方ではないように思うのであとでなおす
find_library(xcb
        NAMES libxcb.so
        HINTS /usr/lib/x86_64_linux_gnu
        REQUIRED)
find_library(xcb-keysyms
        NAMES libxcb-keysyms.so
        HINTS /usr/lib/x86_64_linux_gnu
        REQUIRED)

add_library(shortcut SHARED
        src/shortcut_global.h
        src/shortcut_exception.h
        src/shortcut_exception.cpp
        src/shortcut_keysequence.h
        src/shortcut_keysequence.cpp
        src/shortcut_hotkeymap.h
        src/shortcut.h
        src/shortcut.cpp)

target_link_libraries(shortcut LINK_PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        xcb
        xcb-keysyms)
