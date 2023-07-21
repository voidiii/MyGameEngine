#pragma once

#include "Camera.h"
#include "Texture.h"

namespace MGE {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Flush();
		

		// Primitives
		static void DrawQuad(const Vec2& position, const Vec2& size, const Vec4& color);
		static void DrawQuad(const Vec3& position, const Vec2& size, const Vec4& color);
		static void DrawQuad(const Vec2& position, const Vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const Vec3& position, const Vec2& size, const Ref<Texture2D>& texture);
	};

}

