# puddi
3D graphics engine.

CMake is required.

Required libraries: gomp(OpenMP) GLEW GL SDL2 png jpeg z SDL2_image assimp

This project compiles to a static library called libpuddi.a. There is no executable.

To build Debug:
  mkdir Debug
  cd Debug
  cmake -D CMAKE_BUILD_TYPE=Debug ..
  make

To build Release:
  mkdir Release
  cd Relase
  cmake -D CMAKE_BUILD_TYPE=Release ..
  make

