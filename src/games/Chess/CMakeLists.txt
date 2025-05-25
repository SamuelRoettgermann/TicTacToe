# list all the code files of the module
set(SOURCES game.cpp
        game.cpp
        game.h)

set(LIBRARY_NAME chess)


add_library(${LIBRARY_NAME} ${SOURCES})
target_include_directories(${LIBRARY_NAME} PUBLIC .)
set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)