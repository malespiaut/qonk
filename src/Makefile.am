SUBDIRS = lisp ui

INCLUDES = -I$(top_srcdir)/guichan

AM_CPPFLAGS=-DPKG_DATA_DIR="\"$(pkgdatadir)/\""

bin_PROGRAMS = qonk

qonk_LDADD = lisp/liblisp.a \
	ui/libui.a \
	$(top_builddir)/guichan/guichan/sdl/libguichan_sdl.a \
	$(top_builddir)/guichan/guichan/widgets/libguichan_widgets.a \
	$(top_builddir)/guichan/guichan/libguichan.a

qonk_SOURCES = \
	actions.cpp \
	actions.h \
	animations.cpp \
	animations.h \
	canvas.cpp \
	canvas.h \
	coordinate.cpp \
	coordinate.h \
	controloptions.cpp \
	controloptions.h \
	extensions.cpp \
	extensions.h \
	fonts.cpp \
	fonts.h \
	menumanager.cpp \
	menumanager.h \
	game.cpp \
	game.h \
	gameoptions.cpp \
	gameoptions.h \
	input.h \
	main.cpp \
	main.h \
	messages.cpp \
	messages.h \
	planets.cpp \
	planets.h \
	players.cpp \
	players.h \
	sdl_driver.cpp \
	sdl_driver.h \
	selection.cpp \
	selection.h \
	settings.cpp \
	settings.h \
	ships.cpp \
	ships.h \
	stars.cpp \
	stars.h \
	timer.cpp \
	timer.h \
	universe.cpp \
	universe.h \
	videooptions.cpp \
	videooptions.h
