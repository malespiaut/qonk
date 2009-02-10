cmake_minimum_required(VERSION 2.6)

project(QONK)
set(QONK_DATA_DIR ${CMAKE_INSTALL_PREFIX}/share/games/qonk)
set(PACKAGE_STRING "Qonk 0.3.0")

enable_language(CXX)

find_package(SDL 1.2 REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_image REQUIRED)
find_library(SDLGFX_LIBRARY SDL_gfx)

add_subdirectory(guichan)
add_subdirectory(src)

