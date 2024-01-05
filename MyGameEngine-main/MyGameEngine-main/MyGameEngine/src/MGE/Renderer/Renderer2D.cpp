#include "MGEpch.h"
#include "Renderer2D.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/RenderCommand.h"


namespace MGE {

	struct QuadVertex
	{
		Vec3 Position;
		Vec4 Color;
		Vec2 TexCoord;
		// TODO: texid
	};

	struct CircleVertex
	{
		Vec3 Position;
		Vec4 Color;
		float Thickness;
		float Fade;
		Vec3 LocalPosition;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 100000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture;

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		Mat44 ViewProjection;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// circle
		s_Data.CircleVertexArray = VertexArray::Create();

		s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
		s_Data.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"      },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"          },
			{ ShaderDataType::Float3, "a_LocalPosition" }
			});
		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(quadIB); // Use quad IB
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.QuadShader = Shader::Create("assets/shaders/Renderer2D_Quad.glsl");
		s_Data.CircleShader = Shader::Create("assets/shaders/Renderer2D_Circle.glsl");
	}

	void Renderer2D::Shutdown()
	{
		
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		// s_Data.QuadShader->Bind();
		// s_Data.QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		// 
		// s_Data.CircleShader->Bind();
		// s_Data.CircleShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.ViewProjection = camera.GetViewProjectionMatrix();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		// uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		// s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		// RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
			// Bind textures
		
			s_Data.QuadShader->Bind();
			s_Data.QuadShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		}

		if (s_Data.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

			s_Data.CircleShader->Bind();
			s_Data.CircleShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
		}
	}

	void Renderer2D::DrawQuad(const Vec2& position, const Vec4& color, const std::vector<Vec3> vertices)
	{
		DrawQuad({ position.x, position.y, 0.0f }, color, vertices);
	}

	void Renderer2D::DrawQuad(const Vec3& position, const Vec4& color, const std::vector<Vec3> vertices)
	{
		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = Vec3{ position.x, position.y, 0.0f } + vertices[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
			s_Data.QuadVertexBufferPtr++;
		}
		
		// s_Data.QuadVertexBufferPtr->Position = Vec3{ position.x + size.x / 2.0f , position.y + size.y / 2.0f, 0.0f };
		// s_Data.QuadVertexBufferPtr->Color = color;
		// s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		// s_Data.QuadVertexBufferPtr++;
		// 
		// s_Data.QuadVertexBufferPtr->Position = Vec3{ position.x - size.x / 2.0f , position.y + size.y / 2.0f, 0.0f };
		// s_Data.QuadVertexBufferPtr->Color = color;
		// s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		// s_Data.QuadVertexBufferPtr++;
		// 
		// s_Data.QuadVertexBufferPtr->Position = Vec3{ position.x - size.x / 2.0f , position.y - size.y / 2.0f, 0.0f };
		// s_Data.QuadVertexBufferPtr->Color = color;
		// s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		// s_Data.QuadVertexBufferPtr++;
		// 
		// s_Data.QuadVertexBufferPtr->Position = Vec3{ position.x + size.x / 2.0f , position.y - size.y / 2.0f, 0.0f };
		// s_Data.QuadVertexBufferPtr->Color = color;
		// s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		// s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}
	void Renderer2D::DrawQuad(const Vec3& position, const Vec2& size, const Ref<Texture2D>& texture) 
	{
		// s_Data.TextureShader->Bind();
		texture->Bind();
		Mat44 transform = (Mat44)mathter::Translation(position) * (Mat44)mathter::Scale(Vec3{ size.x, size.y, 1.0f });
		s_Data.QuadShader->SetMat4("u_Transform", transform);

		

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}

	void Renderer2D::DrawCircle(const Vec3& position, const Vec4& color, float thickness, float fade)
	{
		// HZ_PROFILE_FUNCTION();

		// TODO: implement for circles
		// if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		// 	NextBatch();

		Vec3 QuadVertexPositions[4];

		float radius = 0.5f;

		QuadVertexPositions[0] = { -radius, -radius, 0.0f };
		QuadVertexPositions[1] = { radius, -radius, 0.0f };
		QuadVertexPositions[2] = { radius,  radius, 0.0f };
		QuadVertexPositions[3] = { -radius,  radius, 0.0f };

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->Position = position + QuadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->LocalPosition = QuadVertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;
	}

}