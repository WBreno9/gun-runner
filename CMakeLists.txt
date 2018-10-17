cmake_minimum_required(VERSION 3.5.1)
project(gun_runner)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES include/inc.h
        src/main.cpp src/gutils.cpp src/entity.cpp src/game.cpp
        include/gutils.h include/entity.h include/game.h
        src/camera.cpp include/camera.h
        src/inputglsl.cpp include/inputglsl.h
        src/renderer.cpp include/renderer.h
        src/model.cpp include/model.h
        src/empty.cpp include/empty.h)

find_package(glfw3 3.1.2 REQUIRED)

include_directories("include/")
add_executable(gun_runner ${SOURCES})
target_link_libraries(gun_runner pthread GL GLU GLEW glfw assimp)
