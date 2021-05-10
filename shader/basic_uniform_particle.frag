#version 460

//in variable that receives the diffuse calculation from the vertex shader
in float Transp;
in vec2 TexCoord;
uniform sampler2D ParticleTex;

//out variable, this typical for all fragment shaders
layout (location = 0) out vec4 FragColor;

void main()
{
	//Set the frag colour to the texture's colour
	FragColor = texture(ParticleTex, TexCoord);
	FragColor.a *= Transp;
}
