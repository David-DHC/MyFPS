cmake_minimum_required(VERSION 3.21)
project(MyFPS)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-D_USE_MATH_DEFINES -DNOMINMAX -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
include_directories(imgui)
include_directories(imgui/backends)

find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

aux_source_directory(imgui IMGUI_SRC)
set(IMGUI_BACKEND_SRC imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)

add_executable(MyFPS main.cpp stb_image.cpp camera.h constant.h ${IMGUI_SRC} ${IMGUI_BACKEND_SRC})
set_target_properties(MyFPS PROPERTIES CXX_STANDARD 20)
#target_include_directories(MyFPS PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(MyFPS PRIVATE glad::glad glfw glm::glm)
