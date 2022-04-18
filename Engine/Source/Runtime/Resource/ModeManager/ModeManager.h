#pragma once

#include "Runtime/Core/Base/PublicSingleton.h"

namespace HEngine
{
    // From UE4.27.2
    // TODO
    enum class EditMode
    {
        Select = 0,
        Landscape = 1,
        Foliage = 2,
        BrushEditing = 3,
        MeshPaint = 4
    };

    class ModeManager final : public PublicSingleton<ModeManager>
    {
    public:
        static bool IsEditState() { return ModeManager::GetInstance().bEditState; }
        static void ChangeState() { ModeManager::GetInstance().bEditState = !ModeManager::GetInstance().bEditState; }
    private:
        static bool bEditState;
        static EditMode mEditMode;
    };
}