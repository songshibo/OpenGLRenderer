## OpenGL Renderer Startup Template

For simplicity, I've added most of dependencies as git submodules and compiled them from source with CMake. (in ```external/CMakeLists.txt```)

- Compiled from source
  - GLFW: On MacOS, ols use Clang to compile. (Tested on Big Sur)
  - ImGui: backends using OpenGL+GLFW3+GLAD
  - Eigen: head-only, add in as INTERFACE library
  - spdlog: head-only, add in as INTERFACE library
- Others
  - GLAD: download from their [website](https://glad.dav1d.de/) and put it into ```external/glad/include(src)``` manually.
  - Assimp: install it via homebrew on MacOS. For windows, pls build it manually and put the include and lib files into ```external/assimp```. 

