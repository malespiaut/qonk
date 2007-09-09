#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([qonk], [0.0.3-pre], [qonk-dev@lists.sf.net])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src/animations.h])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

AC_LANG_C

AM_PATH_SDL([1.2.0], [:],
	AC_MSG_ERROR([No proper SDL found. Please install at least version 1.2.0!]))


# Checks for libraries.
AC_CHECK_LIB([SDL], [SDL_Init])
AC_CHECK_LIB([SDL_ttf], [TTF_Init], ,AC_MSG_ERROR([SDL_ttf missing. Please install at least version 2.0!]))
AC_CHECK_LIB([SDL_gfx], [pixelColor], ,AC_MSG_ERROR([SDL_gfx missing. Please install at least version 1.2!]))
AC_CHECK_LIB([SDL_image], [IMG_Load], ,AC_MSG_ERROR([SDL_image missing. Please install at least version 1.2!]))

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h SDL/SDL.h SDL/SDL_gfxPrimitives.h SDL/SDL_ttf.h SDL/SDL_image.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.
AC_FUNC_SELECT_ARGTYPES
AC_CHECK_FUNCS([atexit mkdir select sqrt])

AC_CONFIG_FILES([Makefile
                 data/Makefile
                 src/lisp/Makefile
                 src/Makefile
                 guichan/Makefile
                 guichan/guichan/Makefile
                 guichan/guichan/sdl/Makefile
                 guichan/guichan/widgets/Makefile])
AC_OUTPUT
