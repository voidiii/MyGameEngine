#pragma once

#include "MGE/Renderer/RendererAPI.h"

namespace MGE {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const Vec4& color) override;
		virtual void Clear() override;
		virtual void Init() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}