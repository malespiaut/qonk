add_library(ui STATIC menuaction.cpp menu.cpp menuentry.cpp menusystem.cpp)
add_definitions(-DPKG_DATA_DIR="${QONK_DATA_DIR}")

include_directories(${QONK_SOURCE_DIR}/guichan)

