#shader vertex
#version 330 core

/* These variables are "coming in" from the vertex buffer (vao) */
layout(location=0) in vec4 a_Position;
layout(location=1) in vec4 a_Color;
layout(location=2) in vec2 a_TexCoord;
layout(location=3) in float a_TexIndex;

out vec2 v_TexCoord;
out float v_TexIndex;

/* The uniforms "come in" from the cpu */
uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_Position;

    v_TexIndex = a_TexIndex;
    v_TexCoord = a_TexCoord;
}

#shader fragment
#version 330 core

layout(location=0) out vec4 o_Color;

in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[2];

void main()
{
    int index = int(v_TexIndex);
    vec4 sampledColor = texture(u_Textures[index], v_TexCoord);
    o_Color = sampledColor;
}
