add_subdirectory(sdl)
add_subdirectory(widgets)

include_directories(${QONK_SOURCE_DIR}/guichan)
link_directories(
  ${QONK_BINARY_DIR}/guichan/guichan/widgets
  ${QONK_BINARY_DIR}/guichan/guichan/sdl
)

add_library(guichan SHARED
	actionevent.cpp basiccontainer.cpp cliprectangle.cpp color.cpp
	defaultfont.cpp event.cpp exception.cpp focushandler.cpp font.cpp
	graphics.cpp guichan.cpp gui.cpp image.cpp imagefont.cpp inputevent.cpp
	key.cpp keyevent.cpp keyinput.cpp mouseevent.cpp mouseinput.cpp
	rectangle.cpp widget.cpp
)
target_link_libraries(guichan guichan_sdl guichan_widgets)

install(TARGETS guichan LIBRARY DESTINATION ${PACKAGE_LIBDIR})
