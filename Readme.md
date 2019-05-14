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

![Demo v21](https://raw.githubusercontent.com/powernic/DX11-Game-Engine/master/demo/v21.png)

