cmake_minimum_required(VERSION 3.16)
project(OpenGL_redo)

set(CMAKE_CXX_STANDARD 14)
# Like C++, cmake also has macros that are useful for cross-platforming.
#   Here, I use the macro PROJECT_SOURCE_DIR (more like variable, really) along with '$' to return the project's directory
set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/Application.cpp src/Renderer.h src/Renderer.cpp src/VertexBuffer.h
        src/VertexBuffer.cpp src/IndexBuffer.h src/IndexBuffer.cpp src/VertexBufferLayout.h src/VertexArray.h
        src/VertexArray.cpp src/Shader.h src/Shader.cpp src/vendor/stb_image/stb_image.h src/vendor/stb_image/stb_image.cpp
        src/Texture.h src/Texture.cpp src/vendor/glm src/vendor/imgui/imconfig.h src/vendor/imgui/imgui.cpp src/vendor/imgui/imgui.h src/vendor/imgui/imgui_demo.cpp src/vendor/imgui/imgui_draw.cpp src/vendor/imgui/imgui_impl_glfw.cpp src/vendor/imgui/imgui_impl_glfw.h src/vendor/imgui/imgui_impl_opengl3.h src/vendor/imgui/imgui_impl_opengl3.cpp src/vendor/imgui/imgui_internal.h src/vendor/imgui/imgui_widgets.cpp src/vendor/imgui/imstb_textedit.h src/vendor/imgui/imstb_rectpack.h src/vendor/imgui/imstb_truetype.h src/tests/Test.h src/tests/TestClearColor.h src/tests/TestClearColor.cpp src/tests/Test.cpp src/tests/TestTexture2D.h src/tests/TestTexture2D.cpp src/tests/TestBatcher.h src/tests/TestBatcher.cpp src/tests/TestTextureBatch.h src/tests/TestTextureBatch.cpp)

# This is the equivalent of manually adding include directory in Visual Studio settings for Windows.
#   Also the equivalent of using -Idirectory/goes/here flag while compiling with gcc
include_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/include)
include_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLEW/include)
include_directories(src/vendor)
include_directories(src)


# This step is important
#  Because I did not have a .a file in the "lib" folder from the docs I downloaded from glew's website,
#   I needed to compile the static library myself from the source documents that came with the download.
#   So the parameters specify the name of the lib I want to compile, how I want to compile (static here), and the
#   source files from wich I will compile, specified by their directories.
#   After reloading these changes to the cmake, I go to my configurations on the top right and select the "glew | Debug"
#   config. Finally I compile this configuration and find the libglew.a file in the debug folder of the project.
add_library(glew STATIC
        ${PROJECT_SOURCE_DIR}/Dependencies/GLEW/src/glew.c
        ${PROJECT_SOURCE_DIR}/Dependencies/GLEW/src/glewinfo.c
        ${PROJECT_SOURCE_DIR}/Dependencies/GLEW/src/visualinfo.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# The find_library function takes in a argument to which the directory of the lib will be stored,
#   the name of the specific lib this is being done with respect to
#   and the path to where that lib is found. The result is that the address of the lib is stored in the first parameter.
find_library(GLFW_LIBRARY
        NAMES glfw3
        PATHS ${PROJECT_SOURCE_DIR}/Dependencies/GLFW/lib-macos/)

find_library(GLEW_LIBRARY
        NAMES glew
        PATHS ${PROJECT_SOURCE_DIR}/Dependencies/GLEW/lib)

# Here we simply specify the libraries to be linked. See The Cherno's video for more details on this.
target_link_libraries(${PROJECT_NAME} PUBLIC ${GLFW_LIBRARY}
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo")

target_link_libraries(${PROJECT_NAME} PUBLIC ${GLEW_LIBRARY})