# Mini Game Engine on DirectX11

## Configuration

- Environment: [Visual Studio 2019](https://visualstudio.microsoft.com)
- SDK: [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=8109)
- Application: Win32 (x86)
- Property Pages -> Configuration Properties -> 
  - General
    -  Output Directory: `$(SolutionDir)lib\`
    -  Intermediate Directory: `$(SolutionDir)obj\$(Configuration)\`
  - VC++ Directories
    - Include Directories: `$(DXSDK_DIR)Include\`
    - Library Directories: `$(DXSDK_DIR)lib\x86\`
  - C/C++ -> Language ( If you got Error C4596: 'GetLeft': illegal qualified name in member declaration.)
    - Confirmance mode: No
  - Linker for Game Engine.
    - Property Pages -> Configuration Properties 
      - VC++ Directories -> Include Directories: `$(SolutionDir)`
      - Linker -> General -> Advanced Include Directories: `$(SolutionDir);%(AdditionalLibraryDirectories)` 
      - Linker -> Input -> Additional Dependencies : `$(SolutionDir)lib\D3D Framework_d.lib;%(AdditionalDependencies)`

## Demo

### Logs
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/221e2685877f5c9abe7acf200aadb924b726bed5)

![Logs](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v1.png)

### Window Render And DirectX11
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/78380fbe1a9f68aa07444a3fe7b317f1f16a7143)

![Window Render](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v2.png)

### Triangle
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/8e3dabfb)

![Triangle](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v3.png)

### Cube
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/c4fec842)

![Cube](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v4.png)

### Cube animation
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/b7ded76c)

![Cube animation](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v5.png)

### Light sources
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/3757a5ba)

![Light sources](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v6.png)

### Texture
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/5423ce12)

![Texture](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v7.png)

### Mesh
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/e6919854)

![Mesh](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v8.png)

### Mesh transform
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/a05ec9c80f7ff20b2d8eff93b8cfe752587ed351)

![Mesh transform](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v9.png)

### 2D Render
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/5115859d)

![2D Render](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v10.png)

### Render States
[Rasterize State Commit](https://github.com/powernic/DX11-Game-Engine/tree/fb5989a5)

![Rasterized State](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v11_1.png)

[Blend State Commit](https://github.com/powernic/DX11-Game-Engine/tree/5115859d)
![Blend State](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v11_2.png)

### Text
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/ed684f9f)

![Text](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v12.png)


### Point Light
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/ea622311)

![Point light](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v13.png)

### Spot Light
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/7afb9027)

![Spot Light](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v14.png)

### Ambient
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/85251994)

![Ambient](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v15.png)

### Dynamic text
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/7b4a6af08a7f3c04783e0ddada6f2efc5659038f)

![Dynamic text](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/16.png)

### FPS
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/51b10385)

![FPS](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v17.png)

### Frustum
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/909caeed)

![Frustum](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v18.png)

### Multitesturing
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/2e524afbc4d0d7d2d63b1bee0a88b6ed2ae88491)

![Multitesturing](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v19.png)

### Lightmap
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/2e524afbc4d0d7d2d63b1bee0a88b6ed2ae88491)

![Lightmap](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v20.png)

### Alphablending
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/f5cc6184b1484bc410b9ace15c944b723d6a38a1)

![Lightmap](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v21.png)

### Render to texture
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/601ccf039bb8aa4107c498e7921c5371706cbed7)

![Render to texture](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v22.png)

### Texture offset
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/edf85cef)

![Texture offset](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v23.gif)

### Billboard
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/0f148132c7c53ae30c1fceef6a66d34dd184911a)

![Billboard](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v24.png)

### MultiLight sources
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/10caabaf1579b6fb2b47f485deb536150f478e80)

![MultiLight sources](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v25.png)

### Shadow mapping
[Commit](https://github.com/powernic/DX11-Game-Engine/tree/318c7eca081b9b7246e454bdee3c477359a6f987)

![Shadow mapping](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v26.gif)

### Multilight Shadow mapping
![multilight Shadow mapping](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v27.png)

