#version 460


uniform vec4 Color;

//out variable, this typical for all fragment shaders
out vec4 FragColor;

void main()
{
	FragColor = Color;
}