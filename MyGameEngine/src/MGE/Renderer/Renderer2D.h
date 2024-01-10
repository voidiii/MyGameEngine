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
		static void DrawQuad(const Vec2& position, const Vec4& color, const std::vector<Vec3> vertices);
		static void DrawQuad(const Vec3& position, const Vec4& color, const std::vector<Vec3> vertices);
		static void DrawQuad(const Vec2& position, const Vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const Vec3& position, const Vec2& size, const Ref<Texture2D>& texture);
		static void DrawCircle(const Vec3& position, const Vec4& color, float thickness = 1.0f, float fade = 0.005f);
	};

}

