cmake_minimum_required(VERSION 3.16)
project(MC-Chess-Engine LANGUAGES CXX)


set(SOURCES
  main.cpp
  util/setup.cpp
  util/bitboards.cpp
  util/hash_keys.cpp
  board/board.cpp
  board/board_data.cpp
  util/attack.cpp
  util/notation.cpp
  move/move_generator.cpp
  test/validate.cpp
  move/make_move.cpp
  test/perft.cpp
  util/misc.cpp
)

add_executable(main ${SOURCES})

target_compile_features(main PRIVATE cxx_std_17)
target_include_directories(main PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/board
)
