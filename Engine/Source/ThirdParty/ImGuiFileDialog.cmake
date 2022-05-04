set(ImGuiFileDialogSourceDir ${ThirdPartyDir}/ImGuiFileDialog)

file(GLOB ImGuiFileDialogSrc
    "${ImGuiFileDialogSourceDir}/ImGuiFileDialog.h"
    "${ImGuiFileDialogSourceDir}/ImGuiFileDialogConfig.h"
    "${ImGuiFileDialogSourceDir}/ImGuiFileDialog.cpp"
)

add_library(ImGuiFileDialog STATIC ${ImGuiFileDialogSrc})
target_include_directories(ImGuiFileDialog 
    PUBLIC "${ImGuiFileDialogSourceDir}"
)
target_link_libraries(ImGuiFileDialog PUBLIC imgui glfw Glad ${OPENGL_LIBRARIES})
target_compile_definitions(ImGuiFileDialog PRIVATE USE_BOOKMARK)