#version 460

//in variables, this are in model coordinates
layout (location = 0) in vec3 VertexInitVel; //Inital Velocity
layout (location = 1) in float VertexBirthTime; //Birth time of particle

//out vector needed for the fragment shader
out float Transp; //Transparency of Particle
out vec2 TexCoord; //Tex Coordinates
 
uniform float Time;                             //Time of the Animation
uniform vec3 Gravity = vec3(0.0, -0.05,0.0);    //Gravity vector in the world's coordinates
uniform float ParticleLifeTime;                 //Max particle lifetime - how long before it is destroyed
uniform float ParticleSize = 1.0;               //Particle size
uniform vec3 EmitterPos;                        //Emiter position in world coordinates


//uniforms for matrices required in the shader
uniform mat4 MV;				//model view projection matrix
uniform mat4 Proj;				//model view projection matrix

const vec3 offsets[] = vec3[](vec3(-0.5,-0.5,0), vec3(0.5,-0.5,0), vec3(0.5,0.5,0), vec3(-0.5,-0.5,0), vec3(0.5,0.5,0), vec3(-0.5,0.5,0));

const vec2 texCoords[] = vec2[](vec2(0,0), vec2(1,0), vec2(1,1), vec2(0,0), vec2(1,1), vec2(0,1));

void main() 
{ 
    //move the particle by an amount
    vec3 cameraPos;

    //calculate the time since birth
    float t = Time - VertexBirthTime;

    //if the particle is yet to be born, or is older than the lifetime, turn it invisible
    if(t >= 0 && t < ParticleLifeTime)
    {
        vec3 pos = EmitterPos + VertexInitVel * t + Gravity * t * t;
        //offset the vertex based on the ID
        cameraPos = (MV * vec4(pos,1)).xyz + (offsets[gl_VertexID] * ParticleSize);

        Transp = mix(1,0,t/ParticleLifeTime);
    }
    else 
    {
        //Particle shouldn't exist, draw as transparent
        cameraPos = vec3(0);
        Transp = 0.0;
    }

    //Set the Texture coordinates 
    TexCoord = texCoords[gl_VertexID];

    //Move the particle
    gl_Position = Proj * vec4(cameraPos,1.0); 
} 