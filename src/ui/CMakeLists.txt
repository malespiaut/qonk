add_library(ui STATIC menuaction.cpp menu.cpp menuentry.cpp menusystem.cpp)
add_definitions(-DPKG_DATA_DIR="${DATADIR}")

include_directories(${QONK_SOURCE_DIR}/guichan)

