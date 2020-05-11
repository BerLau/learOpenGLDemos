cmake_minimum_required(VERSION 3.5)


project(OpenGLDemo)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLFW3 3.3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp 5.0 REQUIRED)
# find_package(glew REQUIRED)

include_directories(./include ./imgui/include)

link_directories(/usr/local/Cellar/assimp/5.0.1/lib) 

aux_source_directory(. DIR_SRC) 
aux_source_directory(./imgui DIR_IMGUI) 

add_executable(OpenGLDemo ${DIR_SRC} ${DIR_IMGUI})

target_link_libraries(OpenGLDemo  glfw assimp)
