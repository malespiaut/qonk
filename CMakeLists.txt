cmake_minimum_required(VERSION 3.27)

project(QONK)
set(PACKAGE_STRING "Qonk 0.3.0")

set(DATAROOTDIR "${CMAKE_INSTALL_PREFIX}/share")
set(DATADIR "${DATAROOTDIR}/games/qonk")
set(BINDIR "${CMAKE_INSTALL_PREFIX}/bin")
set(LIBDIR "${CMAKE_INSTALL_PREFIX}/lib")
set(PACKAGE_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib/qonk")

set(CMAKE_INSTALL_RPATH "${PACKAGE_LIBDIR}")

enable_language(CXX)
set(CMAKE_CXX_STANDARD 23)

set(GAME_COMPILE_OPTIONS
  -Wall
  -Wextra
  -Wpedantic
  -Wshadow
  -Werror=return-type
  -Wstrict-aliasing=2
  -Wdouble-promotion
  )

find_package(SDL 1.2 REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_image REQUIRED)
find_library(SDLGFX_LIBRARY SDL_gfx)

add_subdirectory(guichan)
add_subdirectory(src)
add_subdirectory(data)

