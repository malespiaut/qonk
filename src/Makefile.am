SUBDIRS = lisp ui engine

AM_CPPFLAGS = -DPKG_DATA_DIR="\"$(pkgdatadir)/\"" -I$(top_srcdir)/guichan

bin_PROGRAMS = qonk

qonk_LDADD = lisp/liblisp.a \
	ui/libui.a \
	engine/libengine.a \
	$(top_builddir)/guichan/guichan/sdl/libguichan_sdl.a \
	$(top_builddir)/guichan/guichan/widgets/libguichan_widgets.a \
	$(top_builddir)/guichan/guichan/libguichan.a

qonk_SOURCES = \
	controloptions.cpp \
	controloptions.h \
	fonts.cpp \
	fonts.h \
	menumanager.cpp \
	menumanager.h \
	gameoptions.cpp \
	gameoptions.h \
	input.h \
	main.cpp \
	main.h \
	sdl_driver.cpp \
	sdl_driver.h \
	settings.cpp \
	settings.h \
	videooptions.cpp \
	videooptions.h
