/*#####################################################################
#
#	Self-Container Inline .h Shader File
#
#	Author:		Justin Bentley
#	Date:		03/2019
#	Version:	1.0
#
######################################################################*/



#ifndef Shaders
#define Shaders

//nvidia 760 = 4.3, 960 = 4.4, 1080 = 4.5


char* vertexShader =
"#version 330 core\n"	//version 3, not 440 >> 450	
"\n"					//GLSL 3, core = non-depricated
"layout(location = 0) in vec4 position;\n"
"layout(location = 1) in vec2 texCoord;\n"
"\n"
"out vec2 v_TexCoord;\n"
"\n"
"uniform mat4 u_MVP;\n"//model view projection matrix uniform handle
"\n"
"void main()\n"
"{\n"
"	gl_Position = u_MVP * position;\n"
"	v_TexCoord = texCoord;\n"
"}\n";

char* fragmentShader =	//also known as pixel shader
"#version 330 core\n"	//version 3, not 440 >> 450
"\n"					//GLSL 3, core = non-depricated
"layout(location = 0) out vec4 color;\n"
"\n"
"in vec2 v_TexCoord;\n"
"\n"
"uniform vec4 u_Color;\n"
"uniform sampler2D u_Texture;\n"
"\n"
"void main()\n"
"{\n"
"	vec4 texColor = texture(u_Texture, v_TexCoord) * u_Color;\n"//
//"	vec4 texColor = texture(u_Texture, v_TexCoord);\n"
//"	color = u_Color;\n"//emmisive
"	color = texColor;\n"
"}\n";

// need to research inline in .h it copies in code instead of being code?

static inline unsigned int CompileShader(unsigned int type, const char* source) {

	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	int result; 

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);//debug, use bp, result 0 = shader compile failed
	printf("Shader Compile: %d\n", result);//output 1 for correct compile

	return id;
}

//static = doesnt leak into other functions
static inline unsigned int CreateShader(const char* vertexShader, const char* fragmentShader) {

	//Creats a shader "program" that could be hot swapped if needed for other shaders
	unsigned int program = glCreateProgram();//change to GLuint

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	//detach and replace a shader
	//	glDetachShader(vs,fs);//supposed to be called, not sure where

	glDeleteShader(vs);//delete intermediatories
	glDeleteShader(fs);

	return program;
}




#endif