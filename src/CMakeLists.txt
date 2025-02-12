
set(SOURCES simulation.cpp strategies.cpp)

set(LIBRARY_NAME simulation)
set(EXECUTABLE_NAME tttsim)

add_library(${LIBRARY_NAME} ${SOURCES})
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)

add_executable(${EXECUTABLE_NAME} main.cpp)
target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME} tictactoe)

add_subdirectory("game")
