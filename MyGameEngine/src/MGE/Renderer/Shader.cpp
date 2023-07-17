 #pragma once

 #include "MGEpch.h"
#include "Shader.h"
#include "Log.h"
#include <glad/glad.h>
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace MGE {

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetRendererAPI())
		{
			case RendererAPI::API::None:    MGE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLShader(vertexSrc, fragmentSrc);
		}
		MGE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:    MGE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLShader(filepath);
		}

		MGE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}