include_directories(${QONK_SOURCE_DIR}/guichan ${SDL_INCLUDE_DIR})

add_library(guichan_sdl SHARED
	sdl.cpp sdlgraphics.cpp sdlimage.cpp sdlimageloader.cpp sdlinput.cpp
)

target_compile_options(guichan_sdl PRIVATE ${GAME_COMPILE_OPTIONS})

target_link_libraries(guichan_sdl ${SDL_LIBRARY})

install(TARGETS guichan_sdl DESTINATION ${PACKAGE_LIBDIR})
