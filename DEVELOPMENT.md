# Developing with VS Code on Windows

Prerequisites: Git, CMake, Visual Studio Build Tools 2026 with the C++ desktop workload and Windows SDK. The Visual Studio IDE is not required.

Open the repository root (`D:\Coding`) in VS Code. Install the recommended C/C++ and CMake Tools extensions. Restart VS Code after installing tools so it receives the updated PATH.

- Select the `windows` CMake configure preset if prompted.
- Press Ctrl+Shift+B to configure and build Debug.
- Press F5 and select `Sandbox (Debug)` to build and debug.

Command-line equivalents from the repository root:

```powershell
cmake --preset windows
cmake --build --preset debug
Set-Location Sandbox
..\build\Debug\Sandbox.exe
```

The working directory must be `Sandbox` so relative asset paths resolve. Release builds use `cmake --build --preset release`.

Initialize dependencies on a fresh clone with `git submodule update --init --recursive`. Mathter is already tracked directly in the repository.

The default build uses the CPU simulation. Historical CUDA experiments remain in the source but are excluded from the default build; CUDA support is not configured or verified.

The pinned spdlog/fmt version uses an obsolete MSVC checked iterator interface. The build defines `_SECURE_SCL=0` consistently across targets to use its ordinary pointer fallback with current MSVC.
