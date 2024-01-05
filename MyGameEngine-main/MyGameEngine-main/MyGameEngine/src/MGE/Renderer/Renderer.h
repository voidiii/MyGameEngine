#pragma once
#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"
#include "MGE/Core.h"

namespace MGE {

	class Renderer
	{
	public:
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void Submit(const Ref<VertexArray>& vertexArray, 
			const Ref<Shader>& shader,
			const Mat44& transform = mathter::Identity());

		inline static RendererAPI::API GetRendererAPI() { return RendererAPI::GetRendererAPI(); }

	private:
		struct SceneData
		{
			Mat44 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};


}

