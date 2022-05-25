# Introduction

LightBringer is a realtime software rasterizer.



# Features

* Fixed Function Pipeline：

  * MVP + Viewport transform

  * Homogeneous Clipping

  * Back-face culling

  * Depth-Buffer

  * Rasterization

  - Perspective correction
  - Blinn–Phong reflection model


- Texture：

  - Diffuse、Specular、Normal
  - Shadowmap
  - Cubemap && Skybox
  - Nearest、Bilinear interpolation




# Build

## Windows

### Visual Studio

Run the Windows.bat in the root directory directly, or use the following command to build in the console:

```pseudocode
cd LightBringer
mkdir build
cmake -B build
```

Open build/LightBringer.sln, set App/LightBringer as the startup item, build and run it.



### MinGW

For MinGW, you can use the following command:

```pseudocode
cd LightBringer
mkdir build
cmake -B build -G"MinGW Makefiles"
cmake --build build
```

then run bin/LightBringer.exe.

