add_subdirectory(lisp)
add_subdirectory(engine)
add_subdirectory(ui)

add_definitions(
	-DPKG_DATA_DIR="${DATADIR}"
	-DPACKAGE_STRING="${PACKAGE_STRING}"
)

include_directories(${QONK_SOURCE_DIR}/guichan)

link_directories(
	${QONK_BINARY_DIR}/guichan/guichan
	${QONK_BINARY_DIR}/guichan/guichan/widgets
	${QONK_BINARY_DIR}/guichan/guichan/sdl
	${QONK_BINARY_DIR}/src/engine
	${QONK_BINARY_DIR}/src/ui
	${QONK_BINARY_DIR}/src/lisp
)

add_executable(qonk
 controloptions.cpp fonts.cpp gameoptions.cpp main.cpp menumanager.cpp
 sdl_driver.cpp settings.cpp videooptions.cpp
)

target_compile_options(qonk PRIVATE
		-Wall
		-Wextra
    -Wpedantic
		-Wshadow
		-Werror=return-type
		-Werror=int-conversion
		-Wstrict-aliasing=2
		-Wdouble-promotion
	)

target_link_libraries(qonk
	guichan guichan_widgets guichan_sdl engine lisp ui
	${SDL_LIBRARY} ${SDLTTF_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLGFX_LIBRARY}
)

install(TARGETS qonk DESTINATION ${BINDIR})
