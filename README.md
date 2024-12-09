# KMU-Computer-Graphic

This repository is a codebase of different exercices requested by Computer Graphics Courses #1 of KMU Uni.

## Current results
<p align="center">
  <img src="https://github.com/user-attachments/assets/493ca0ec-f9e3-4bdd-94b6-ba22a26ac4f4" width="70%">
</p>



## 📑 <samp>Requirements</samp>

### 🔧 <samp>Dependencies</samp>

- [xmake](https://xmake.io/#/)
- Maybe [Vcpkg](https://vcpkg.io/en/) (I don't know if xmake automatically install vcpkg)

## ⚡ <samp>Usage</samp>

### 🔧 <samp>Setup</samp>

Start by cloning this repository

```sh
git clone https://github.com/Miou-zora/Xmake-OpenGl.git
cd Xmake-OpenGl
```

### 👷 <samp>Building</samp>

#### Release

```sh
xmake f -m release
xmake build -y
```

#### Debug

```sh
xmake f -m debug
xmake build -y
```

### 🚀 <samp>Running</samp>

```sh
xmake run
```

### <samp>IDE</samp>

#### VS Code

If you're using VS Code (on windows), don't forget to add these paths to includePath:
```
"path_to_appdata\\Local\\.xmake\\packages\\**",
"path_to_vcpk\\vcpkg\\installed\\x64-windows-static\\**"
```
If you don't know the correct path for you, you can run:
```
xmake l find_package gl3w
```
or
```
xrepo info glm
xrepo info glfw
```
