

#version 330 core  // version 3, not 440 >> 450 //GLSL 3, core = non-depricated

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP; // model view projection matrix uniform handle

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
}