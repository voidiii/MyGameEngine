#include "MGEpch.h"

#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace MGE {

	void OpenGLRendererAPI::SetClearColor(const Vec4& color)
	{
		glClearColor(color[0], color[1], color[2], color[3]);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}