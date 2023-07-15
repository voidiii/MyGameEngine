#pragma once

#include "MGE/Renderer/Shader.h"
#include "MathAlias.h"

namespace MGE {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const Vec2& value);
		void UploadUniformFloat3(const std::string& name, const Vec3& value);
		void UploadUniformFloat4(const std::string& name, const Vec4& value);

		void UploadUniformMat3(const std::string& name, const Mat33& matrix);
		void UploadUniformMat4(const std::string& name, const Mat44& matrix);
	private:
		uint32_t m_RendererID;
	};

}

