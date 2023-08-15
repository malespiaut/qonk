include_directories(${QONK_SOURCE_DIR}/guichan)

add_definitions(-DPKG_DATA_DIR="${DATADIR}")

add_library(ui STATIC menuaction.cpp menu.cpp menuentry.cpp menusystem.cpp)

target_compile_options(ui PRIVATE ${GAME_COMPILE_OPTIONS})

