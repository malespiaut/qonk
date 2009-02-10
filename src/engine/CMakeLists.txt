add_library(engine STATIC actions.cpp animations.cpp canvas.cpp coordinate.cpp extensions.cpp game.cpp messages.cpp planets.cpp players.cpp selection.cpp ships.cpp stars.cpp timer.cpp universe.cpp)

include_directories(${QONK_SOURCE_DIR}/guichan ${SDL_INCLUDE_DIR})
