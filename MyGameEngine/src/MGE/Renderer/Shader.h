 #pragma once

#include <string>
#include "MathAlias.h"

namespace MGE {

	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name, const Mat44& matrix);
	private:
		uint32_t m_RendererID;
	};

}