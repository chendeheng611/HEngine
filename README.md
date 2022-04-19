# HEngine
HEngine

Now only support platform Windows

OpenGL 4.5

TODO:
D3D11
D3D12
Vulkan

## �����淶

### ������
ͳһ����Pascal���������ļ��С������ȣ��������������

CMakeLists.txt �ı�������Ҳ���� Pascal �����������磺
```
set(ProjectRootDir "${CMAKE_CURRENT_SOURCE_DIR}")
```
������� ProjectRootDir ���� Pascal ���������� CMAKE �Դ��������֣����� CMAKE_CURRENT_SOURCE_DIR ��

### include ͷ�ļ�˳��
����includeͬ���ļ��������ͬSource�ļ����ٴ�Ϊ������������ȷ������˳��
���� Editor �У�
```
// ͬ���ļ���ͬ���� Editor ��
#include "EditorLayer.h"

// ͬSource�ļ���λ�� Runtime �У�
#include <HEngine.h>
#include <Runtime/Core/EntryPoint.h>	 

// ����������
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
```

### ����淶
HEngine �Ĵ���淶ƫUnreal���ɲο���
https://docs.unrealengine.com/4.27/zh-CN/ProductionPipelines/DevelopmentSetup/CodingStandard/

ԭ�򣺣�����Ĺ������ȼ�����ǰ��Ĺ���
1. ������ʹ���»���
1. ����һ��Сд��ĸ��ͷ
1. �����ھ�̬������ȫ�ֱ������ֲ�����������ǰ���Сд��ĸs
1. ��������ڲ���Ա��������ǰ���Сд��ĸm
1. �����bool���ͱ�������ǰ���Сд��ĸb
1. ���ڳ�Աͳһ���������ĩβ����������ǰ��

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
* Cherno Hazel
* BoomingTech Pilot