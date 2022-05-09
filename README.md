# HEngine
HEngine: Hbh Game Engine

[![License: APACHE](https://img.shields.io/hexpm/l/apa)](https://opensource.org/licenses/MIT)
[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/c%2B%2B-20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20)

![Screenshot](https://user-images.githubusercontent.com/60227429/167453635-f2c1e98e-1863-47c5-ab39-f3e5db26f558.png)

Now only support platform Windows

OpenGL 4.5

TODO:
D3D11
D3D12
Vulkan

## 命名规范

### 命名法
统一采用Pascal命名法（文件夹、类名等），第三方库除外

CMakeLists.txt 的变量命名也采用 Pascal 命名法，比如：
```
set(ProjectRootDir "${CMAKE_CURRENT_SOURCE_DIR}")
```
这里变量 ProjectRootDir 采用 Pascal 命名法，与 CMAKE 自带变量区分（比如 CMAKE_CURRENT_SOURCE_DIR ）

### include 要求
#### 顺序
首先include同级文件，其次是同Source文件，再次为第三方依赖，最后为stl库（确保依赖顺序）

且彼此直接需要以空格隔开

比如 Editor 中：
```
// 同级文件（同属于 Editor ）
#include "EditorLayer.h"

// 同Source文件（位于 Runtime 中）
#include <HEngine.h>
#include <Runtime/Core/EntryPoint.h>	 

// 第三方依赖
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
```
#### 路径
一律以 Source 起的路径开头（第三方依赖除外），比如 "Runtime/..."

### 代码规范
HEngine 的代码规范偏Unreal，可参考：
https://docs.unrealengine.com/4.27/zh-CN/ProductionPipelines/DevelopmentSetup/CodingStandard/

原则：（后面的规则优先级高于前面的规则）
1. 尽量不使用下划线
1. 变量一律小写字母开头
1. 非类内静态变量（全局变量、局部变量），在前面加小写字母s
1. 如果是类内部成员变量，在前面加小写字母m
1. 如果是bool类型变量，在前面加小写字母b
1. 类内成员统一放在类的最末尾（方法置于前）
1. 默认情况下 std::string 为相对路径，而 std::filesystem::path 为绝对路径，相对路径到绝对路径需要由 AssetManager::GetFullPath() 去获取

## Getting Started
**1. Downloading the repository**
`git clone git@github.com:hebohang/HEngine.git`

**2. You can choose one of the following methods to build HEngine:**

2.1 Run the Win-GenProjects.bat 

2.2 Run the following commands:
```
cd HEngine
cmake -B build
cmake --build build --parallel 4
```

2.3 Visual Studio: Open Folder, then choose HEngine folder)

## Credits
* Cherno [Hazel](https://github.com/TheCherno/Hazel)
* BoomingTech [Pilot](https://github.com/BoomingTech/Pilot)
* [LearnOpenGL](https://github.com/JoeyDeVries/LearnOpenGL)
* [MoravaEngine](https://github.com/dtrajko/MoravaEngine)
* [Ogre](https://github.com/OGRECave/ogre)
* [Pixel](https://github.com/pixel-Teee/Pixel)
