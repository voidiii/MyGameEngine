//--------------------------
// - Hazel 2D -
// Renderer2D Circle Shader
// --------------------------

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in float a_Thickness;
layout(location = 3) in float a_Fade;
layout(location = 4) in vec3 a_LocalPosition;

uniform mat4 u_ViewProjection;

struct VertexOutput
{
	vec3 Position;
	vec4 Color;
	float Thickness;
	float Fade;
	vec3 LocalPosition;
};

layout (location = 0) out VertexOutput Output;

void main()
{
	Output.Position = a_Position;
	Output.Color = a_Color;
	Output.Thickness = a_Thickness;
	Output.Fade = a_Fade;
	Output.LocalPosition = a_LocalPosition;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec3 Position;
	vec4 Color;
	float Thickness;
	float Fade;
	vec3 LocalPosition;
};

layout (location = 0) in VertexOutput Input;

void main()
{
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(Input.LocalPosition);
    float circle = smoothstep(0.0, Input.Fade, distance);
    circle *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

	if (circle == 0.0)
		discard;

    // Set output color
    o_Color = Input.Color;
	o_Color.a *= circle;
}