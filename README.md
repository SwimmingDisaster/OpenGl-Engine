O.Ž.

# Overview
- [Info](#info)
- [Requrements](#requrements)
- [Download](#download)
- [Compilation](#compilation)
	- [On Linux](#on-linux)
	- [On Windows](#on-windows)
	- [On MacOS](#on-macos)



# Info
### This is just a little project i am working on in my free time.

Materials used while making this engine:  
- [This series on youtube](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT).  
- [LearnOpengl](https://learnopengl.com/)  
- [OGLdev](https://ogldev.org/)  

Libraries used:

- [glfw](https://www.glfw.org/) + [glad](https://glad.dav1d.de/) -> for graphics rendering using OpenGL.  
- [nfd](https://github.com/btzy/nativefiledialog-extended) -> for a multi platform file dialog.  
- [assimp](https://www.assimp.org/) -> for loading extermal 3D models.  
- [physx](https://developer.nvidia.com/physx-sdk) -> for physics.  
- [imgui](https://github.com/ocornut/imgui) -> for GUI.  
- [imguizmo](https://github.com/CedricGuillemet/ImGuizmo) -> for showing GUI gizmos.  
- [yaml-cpp](https://github.com/jbeder/yaml-cpp) -> for parsing yaml files.  
- [glm](https://github.com/g-truc/glm) -> for math that is guaranteed to be compatable with OpenGl.  
- [stb_image](https://github.com/nothings/stb) -> for loading images.  

# Requrements

In order to download and build this engine you need:  
- [cmake](https://cmake.org/)  
- [make](https://www.gnu.org/software/make/) (for linux) or [visual studio](https://visualstudio.microsoft.com/) (for windows)  
- [git](https://git-scm.com/) + [git-lfs](https://git-lfs.github.com/) (git-lfs must be initialized so that the git clone will download the lib files needed for compilaton)  
- A working c/c++ compiler. Currently the supported compilers are:
	- [clang](https://clang.llvm.org/)  
	- [gcc](https://gcc.gnu.org/)  
	- [msvc](https://en.wikipedia.org/wiki/Microsoft_Visual_C%2B%2B) (included with visual studio)  


# Download

To download this project, in the terminal/command prompt navigate to the folder you want to place this engine and type this command:
```
git lfs clone https://github.com/SwimmingDisaster/OpenGl-Engine
```

# Compilation

## On linux

at the root of the project in the terminal write these commands one by one:
```
mkdir build
cd build
cmake .. -G"Unix Makefiles"
make
```
that should generate an executable named *main* in the build/ folder.

## On windows
At the root of the project in the command prompt 
write these commands one by one:

```
mkdir build
cd build
cmake -G"Visual Studio 16 2019"
make
``` 
### **NOTE:** Change "Viual Studo 16 2019" to whatever version of visual studio you have installed. Supported versions can be found [here](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#visual-studio-generators) 

That should gnerate a visual studio solution named main.sln. Open that solution and build the project named *ALL_BUILD*. After that you should see and executable named main.exe in the build/Debug folder.

##On MacOS
This project can`t be built on the MacOS platform.
