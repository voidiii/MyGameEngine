 #pragma once

#include <string>
#include <unordered_map>
#include "MathAlias.h"


namespace MGE {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat3(const std::string& name, const Vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const Vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const Mat44& value) = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	/*
		so we can store all the shaders file in this class and use it whenever we want in our app like Sandbox.
		it is meant to be user-friendly, etc
	*/
	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}