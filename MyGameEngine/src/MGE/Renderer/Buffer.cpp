#include "MGEpch.h"
#include "Renderer/Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

MGE::IndexBuffer* MGE::IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
	switch (Renderer::GetRendererAPI())
	{
	case RendererAPI::API::None:    MGE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return new OpenGLIndexBuffer(indices, count);
	}

	MGE_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

/*
			The code generates a Vertex Buffer Object (VBO) using glGenBuffers.
			A VBO is a buffer that holds vertex data, such as positions, colors, or texture coordinates.
			In this case, it is used to store the vertices of the triangle.
		*/

MGE::VertexBuffer* MGE::VertexBuffer::Create(float* vertices, uint32_t size)
{
	switch (Renderer::GetRendererAPI())
	{
	case RendererAPI::API::None:    MGE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return new OpenGLVertexBuffer(vertices, size);
	}

	MGE_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}