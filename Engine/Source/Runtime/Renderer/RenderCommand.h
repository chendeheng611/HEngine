#pragma once

#include "Runtime/Renderer/RendererAPI.h"

namespace HEngine
{
    class RenderCommand
    {
    public:
        static void Init()
        {
            sRendererAPI->Init();
        }

        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            sRendererAPI->SetViewport(x, y, width, height);
        }

        static void SetClearColor(const glm::vec4& color)
        {
            sRendererAPI->SetClearColor(color);
        }

        static void Clear()
        {
            sRendererAPI->Clear();
        }

        static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
        {
            sRendererAPI->DrawIndexed(vertexArray, indexCount);
        }

		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			sRendererAPI->DrawLines(vertexArray, vertexCount);
		}

		static void SetLineWidth(float width)
		{
			sRendererAPI->SetLineWidth(width);
		}
    private:
        static Scope<RendererAPI> sRendererAPI;
    };
}
