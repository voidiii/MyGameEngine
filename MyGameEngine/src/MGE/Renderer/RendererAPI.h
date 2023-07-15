#pragma once

#include "MathAlias.h"

#include "VertexArray.h"
#include "MGE/Core.h"

namespace MGE {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual void SetClearColor(const Vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetRendererAPI() { return s_API; }
	private:
		static API s_API;
	};

}