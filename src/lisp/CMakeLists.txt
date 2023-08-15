add_library(lisp STATIC lexer.cpp lisp.cpp parser.cpp writer.cpp)

target_compile_options(lisp PRIVATE ${GAME_COMPILE_OPTIONS})

