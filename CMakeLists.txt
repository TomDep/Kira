cmake_minimum_required(VERSION 3.10)
project(Kira VERSION 1.0)

set(LIB_DIRECTORY "lib")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Configuration file for the Version number
#configure_file(Config.h.in Config.h)

# ImGui
set(IMGUI_SRC ${LIB_DIRECTORY}/imgui/src)
set(IMGUI_BACKEND ${IMGUI_SRC}/backends)
set(IMGUI_FILES ${IMGUI_SRC}/imgui_demo.cpp
                ${IMGUI_SRC}/imgui_draw.cpp
                ${IMGUI_SRC}/imgui_tables.cpp
                ${IMGUI_SRC}/imgui_widgets.cpp
                ${IMGUI_SRC}/imgui.cpp
                ${IMGUI_BACKEND}/imgui_impl_opengl2.cpp
                ${IMGUI_BACKEND}/imgui_impl_sdl.cpp)

file(GLOB SOURCES 
    "src/*.cpp"
    "src/ui/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_FILES})
set(HEADER_FILES $("includes/*.h"))
include_directories("includes" ${LIB_DIRECTORY}/imgui/includes)

# Link against wiringPi
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
target_link_libraries(${PROJECT_NAME} ${WIRINGPI_LIBRARIES})

# Link against SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# Link against GLUT
find_package(GLUT REQUIRED)
target_link_libraries(${PROJECT_NAME} GLUT::GLUT)

# Link against GLM
find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME} glm::glm)

# Link against GLEW
find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)

# Link against OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})

# Link against STDC++
target_link_libraries(${PROJECT_NAME} stdc++)
