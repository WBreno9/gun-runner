cmake_minimum_required(VERSION 3.5.1)
project(gun_runner)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES include/inc.h
        src/main.cpp src/entity.cpp src/game.cpp
        include/entity.h include/game.h
        src/camera.cpp include/camera.h
        src/inputglsl.cpp include/inputglsl.h
        src/renderer.cpp include/renderer.h
        src/model.cpp include/model.h
        src/empty.cpp include/empty.h
        src/light.cpp include/light.h)

set(DEPS_ROOT "" CACHE PATH "Dependencies directory")
set(USE_INSTALLED_LIBS ON CACHE BOOL "Use installed dependencies")

if(NOT USE_INSTALLED_LIBS)
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
                
        add_subdirectory("${DEPS_ROOT}/glfw-3.2.1")
                
        set(ZLIB_LIBRARIES zlibstatic)
        set(ENABLE_BOOST_WORKAROUND ON)
        set(BUILD_STATIC_LIB ON)
        set(BUILD_ASSIMP_TOOLS  ON)
        set(ASSIMP_BUILD_STATIC_LIB ON)
                
        add_subdirectory("${DEPS_ROOT}/assimp-3.3.1")
                
        include_directories(
                "${DEPS_ROOT}/glew-2.1.0/include"
                "${DEPS_ROOT}/glm"
                "${DEPS_ROOT}/glfw-3.2.1/include"
                "${DEPS_ROOT}/assimp-3.3.1/include"
                )
                
        set(GLEW_INCLUDE_DIRS "${DEPS_ROOT}/glew-2.1.0/include")
        set(GLEW_LIBRARIES "${DEPS_ROOT}/glew-2.1.0/lib/Release/x64/glew32.lib")
else()
        find_package(glfw3 REQUIRED)
        find_package(GLEW REQUIRED)
endif()

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

include_directories(
        "${CMAKE_SOURCE_DIR}/include" 
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
        ${ASSIMP_INCLUDE_DIRS}
        )

add_executable(gun_runner ${SOURCES})
set_target_properties(gun_runner PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
target_link_libraries(
        gun_runner
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        glfw
        assimp
	)
