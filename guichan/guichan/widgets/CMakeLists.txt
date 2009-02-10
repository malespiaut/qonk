include_directories(${QONK_SOURCE_DIR}/guichan)

add_library(guichan_widgets SHARED
	button.cpp checkbox.cpp container.cpp dropdown.cpp icon.cpp label.cpp
	listbox.cpp radiobutton.cpp scrollarea.cpp slider.cpp textbox.cpp
	textfield.cpp window.cpp
)
