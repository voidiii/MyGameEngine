#pragma once
#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace MGE {

	class Renderer
	{
	public:
		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, 
			const std::shared_ptr<Shader>& shader,
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

