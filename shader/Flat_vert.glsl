#version 460

layout (location = 0) in vec3 VertexPosition; 
uniform mat4 MV;				//model view projection matrix
uniform mat4 Proj;	

void main()
{
	gl_Position = Proj * MV * vec4(VertexPosition, 1.0);
}