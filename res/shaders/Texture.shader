#shader vertex
#version 330 core

/* These variables are "coming in" from the vertex buffer (vao) */
layout(location=0) in vec4 a_Position;
layout(location=1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

/* The uniforms "come in" from the cpu */
uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_Position;
    v_TexCoord = a_TexCoord;
}

#shader fragment
#version 330 core

layout(location=0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
    vec4 sampledColor = texture(u_Texture, v_TexCoord);
    o_Color = sampledColor;
}
