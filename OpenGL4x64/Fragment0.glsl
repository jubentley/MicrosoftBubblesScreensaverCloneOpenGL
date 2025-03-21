#version 330 core  //also known as pixel shader // version 3, not 440 >> 450

// GLSL 3, core = non-deprecated
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord) * u_Color;
    // vec4 texColor = texture(u_Texture, v_TexCoord);
    // color = u_Color;  // Emissive
    color = texColor;
}