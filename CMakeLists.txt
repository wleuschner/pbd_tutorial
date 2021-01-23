cmake_minimum_required(VERSION 3.5)

project(PBD LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)

add_executable(PBD
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  glcanvas.h
  glcanvas.cpp
  buffer.h
  buffer.cpp
  shader.h
  shader.cpp
  shaderprogram.h
  shaderprogram.cpp
  camera.h
  camera.cpp
  cloth.cpp
  cloth.h
  lengthconstraint.cpp
  lengthconstraint.h
  spherecollisionconstraint.cpp
  spherecollisionconstraint.h
  utils.h
  vertex.h
)

add_custom_command(
        TARGET PBD POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/effect.vs
                ${CMAKE_CURRENT_BINARY_DIR}/effect.vs
)

add_custom_command(
        TARGET PBD POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/effect.fs
                ${CMAKE_CURRENT_BINARY_DIR}/effect.fs
)

target_link_libraries(PBD PRIVATE Qt5::Widgets Qt5::OpenGL GL GLEW)
