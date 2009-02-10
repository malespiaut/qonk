cmake_minimum_required(VERSION 2.6)

project(QONK)
set(PACKAGE_STRING "Qonk 0.3.0")

set(DATAROOTDIR "${CMAKE_INSTALL_PREFIX}/share" CACHE)
set(DATADIR "${DATAROOTDIR}/games/qonk")
set(BINDIR "${CMAKE_INSTALL_PREFIX}"/bin)

enable_language(CXX)

find_package(SDL 1.2 REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_image REQUIRED)
find_library(SDLGFX_LIBRARY SDL_gfx)

add_subdirectory(guichan)
add_subdirectory(src)

