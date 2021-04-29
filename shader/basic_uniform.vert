#version 460

uniform int Cycle;

//in variables, this are in model coordinates
layout (location = 0) in vec3 VertexPosition; 
layout (location = 1) in vec3 VertexNormal; 

//out vector needed for the fragment shader
out vec3 VPosition;
out vec3 VNormal;  


//uniforms for matrices required in the shader
uniform mat4 ModelViewMatrix;   //model view matrix
uniform mat3 NormalMatrix;		//normal matrix
uniform mat4 MVP;				//model view projection matrix
 
void main() 
{ 
    VPosition = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));

    VNormal = normalize(NormalMatrix * VertexNormal);

    //turns any vertex position into model view projection in preparations to 
    //graphics pipeline processes before fragment shader (clipping)
    gl_Position = MVP * vec4(VertexPosition,1.0); 
    //gl_Position = vec4(VertexPosition, 1.0); 


//  if(Cycle == 1)
//  {
//    gl_Position = vec4(VertexPosition2, 0.0, 1.0); 
//  }
} 