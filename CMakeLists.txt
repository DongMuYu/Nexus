cmake_minimum_required(VERSION 3.10)
project(Nexus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)  # 强制MSVC使用UTF-8
endif()

# 设置SFML库的路径
set(SFML_ROOT "D:/Libraries/SFML-2.5.1")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")

# 设置ImGui-SFML库的路径
set(IMGUI_SFML_ROOT "D:/Libraries/imgui-sfml")
set(IMGUI_SFML_INCLUDE_DIR "${IMGUI_SFML_ROOT}")
set(IMGUI_SFML_LIBRARY_DIR "${IMGUI_SFML_ROOT}/build/Release")

# 设置ImGui库的路径
set(IMGUI_ROOT "D:/Libraries/imgui")
set(IMGUI_INCLUDE_DIR "${IMGUI_ROOT}")

# 查找SFML库
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# 添加源文件
set(SOURCES
    src/main.cpp
    src/particles/ParticleSystem.cpp
    src/particles/CoreParticle.cpp
    src/particles/Emitters/MouseEmitterController.cpp
    src/particles/Emitters/BaseEmitter.cpp
    src/particles/Emitters/PointEmitter.cpp
    src/rendering/Renderer.cpp
    src/ui/UIManager.cpp
    src/utils/Math/Vector2.cpp
    src/physics/SpatialHash.cpp
    src/physics/Solver.cpp
    src/physics/World.cpp
    src/core/Application.cpp
    src/core/Config.cpp
    src/core/Profiler.cpp
    src/core/ThreadPool.cpp
    src/rendering/DebugDraw.cpp
    src/rendering/PostProcess.cpp
    src/utils/FileIO.cpp
    src/utils/Logger.cpp
    # ImGui源文件
    ${IMGUI_ROOT}/imgui.cpp
    ${IMGUI_ROOT}/imgui_demo.cpp
    ${IMGUI_ROOT}/imgui_draw.cpp
    ${IMGUI_ROOT}/imgui_tables.cpp
    ${IMGUI_ROOT}/imgui_widgets.cpp
)

# 添加头文件目录
include_directories(${SFML_INCLUDE_DIR})
include_directories(${IMGUI_SFML_INCLUDE_DIR})
include_directories(${IMGUI_INCLUDE_DIR})
include_directories(src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 创建可执行文件
add_executable(Nexus ${SOURCES})

# 链接SFML库 - 使用更明确的方式
target_link_libraries(Nexus 
    ${SFML_LIBRARY_DIR}/sfml-graphics.lib
    ${SFML_LIBRARY_DIR}/sfml-window.lib
    ${SFML_LIBRARY_DIR}/sfml-system.lib
    ${IMGUI_SFML_LIBRARY_DIR}/imgui-sfml.lib
    opengl32.lib  # 添加OpenGL库
)