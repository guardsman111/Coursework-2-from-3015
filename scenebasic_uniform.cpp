#include "scenebasic_uniform.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
using glm::vec3;
using glm::mat4;

//constructor for the scene and the particle fountain
SceneBasic_Uniform::SceneBasic_Uniform() : time(0), particleLifeTime(50.5f), nParticles(8000), emitterPos(-50, 0, -50), emitterDir(0, 2, -7)
{
    ogre = ObjMesh::load("media/bs_ears.obj", false, true); //Load the ogre object
}

//Initialize the scene
void SceneBasic_Uniform::initScene()
{
    compile();

    //Particle stuff first
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    angle = glm::half_pi<float>(); //set angle for rotation of particles
    //angle = 0;
    initBuffers();

    glActiveTexture(GL_TEXTURE0); //load texture to buffer 0
    Texture::loadTexture("media/bluewater.png");

    //Set the uniforms of the particle vert and frag shaders
    partProg.use();
    partProg.setUniform("ParticleTex", 0);
    partProg.setUniform("ParticleLifeTime", particleLifeTime);
    partProg.setUniform("ParticleSize", 0.005f);
    partProg.setUniform("Gravity", vec3(0.0f, -0.2f, 0.0f));
    partProg.setUniform("EmitterPos", emitterPos);

    flatProg.use();
    flatProg.setUniform("Color", glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));

    //Ogre stuff second
    compile(); //Running compile a second time else it doesn't show the particles for some reason :/

    glEnable(GL_DEPTH_TEST);

    float c = 3.5f; //distance of camera from model of ogre
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f); //setting the projection of the camera

    glPointSize(10.0f);

    float v[] = { -1.0f, -1.0f, -0.5f, 1.0f, 0.5f, -1.0f, 1.0f, 1.0f };

    //Set the uniforms of the basic frag and vert shaders
    prog.setUniform("Line.Width", 0.2f);
    prog.setUniform("Line.Color", glm::vec4(0.05f, 1.0f, 0.55f, 1.0f));
    prog.setUniform("Material.Kd", 0.0f, 0.7f, 0.2f);
    prog.setUniform("Material.Ka", 0.2f, 0.3f, 0.8f);
    prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Material.Shininess", 100.0f);
    prog.setUniform("Light.Intensity", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Light.L", vec3(0.9f));
    prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    
}

//Initialize buffers for the particle system
void SceneBasic_Uniform::initBuffers()
{
    glGenBuffers(1, &initVel);
    glGenBuffers(1, &startTime);

    //Setup/initialize the particles
    int size = nParticles * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferData(GL_ARRAY_BUFFER, size * 3, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

    glm::mat3 emitterBasis = ParticleUtils::makeArbitraryBasis(emitterDir);
    vec3 v(0.0f);
    float velocity, theta, phi;
    std::vector<GLfloat> data(nParticles * 3);

    //Set the initial velocity of all particles, as well as their direction on spawn
    for (uint32_t i = 0; i < nParticles; i++) {
        theta = glm::mix(0.0f, glm::pi<float>() / 20.0f, randFloat());
        phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

        v.x = sinf(theta) * cosf(phi);
        v.y = cosf(theta);
        v.z = sinf(theta) * sinf(phi);

        velocity = glm::mix(1.25f, 1.5f, randFloat());
        v = glm::normalize(emitterBasis * v) * velocity;

        data[3 * i] = v.x;
        data[3 * i + 1] = v.y;
        data[3 * i + 2] = v.z;
    }

    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3, data.data());

    //Set the rate at which the particles will spawn - this affects the density of the fountain
    float rate = particleLifeTime / nParticles;
    for (int i = 0; i < nParticles; i++) {
        data[i] = rate * i;
    }

    //Apply these to buffers in the shaders
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &particles);
    glBindVertexArray(particles);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);

    glBindVertexArray(0);
}

//Returns a random float using the random.h script
float SceneBasic_Uniform::randFloat() {
    return rand.nextFloat();
}

//compiling the shaders
void SceneBasic_Uniform::compile()
{
	try {
        //compile standard shaders for the ogre wiremesh
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
        prog.compileShader("shader/Geometry_Shader.geom");
		prog.link();
		prog.use();

        //compile the shaders for the particle fountain
        partProg.compileShader("shader/basic_uniform_particle.vert");
        partProg.compileShader("shader/basic_uniform_particle.frag");
        partProg.link();
        partProg.use();

        //compile shaders for the grid
        flatProg.compileShader("shader/Flat_frag.glsl");
        flatProg.compileShader("shader/Flat_vert.glsl");

        flatProg.link();

	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    //update time and angle for particles
    time = t;
    angle = std::fmod(angle + 0.01f, glm::two_pi<float>());
}

void SceneBasic_Uniform::render()
{
    //clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //sets the view for the particles in the fountain - this code makes the camera rotate
    view = glm::lookAt(vec3(3.0f * cos(angle), 1.5f, 3.0f * sin(angle)), vec3(0.0f, 1.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    model = mat4(1.0f);

    //sets matrices for the grid, but the grid is disabled
    flatProg.use();
    setMatricesParticle(flatProg);
    //grid.render();                    Grid is disabled here because i don't want it showing - it slices through the ogre head and doesn't look great.

    //disable the depth mask for the particles
    glDepthMask(GL_FALSE);

    //set matrices for the particle shaders
    partProg.use();
    setMatricesParticle(partProg); //we set matrices 
    partProg.setUniform("Time", time);

    //bind the particles to a vertex array and then draw them, finally we re-enable the depth mask
    glBindVertexArray(particles);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);

    model = mat4(1.0f);

    //Do some calculations to rotate the camera around the ogre head
    const float radius = 10.0f;

    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;

    vec3 cameraPos(camX, -1.0f, camZ);

    //Adjust the view by the cameraPos set above
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));


    setMatrices(); //we set matrices 

    ogre->render(); //render the ogre head

    glFinish();
}

//Sets the Matices for the standard shaders on the ogre head
void SceneBasic_Uniform::setMatrices()
{
        mat4 mv = view * model; //we create a model view matrix

        prog.use();
    
        prog.setUniform("ModelViewMatrix", mv); //set the uniform for the model view matrix
    
        prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))); //we set the uniform for normal matrix
    
        prog.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix

        prog.setUniform("ViewportMatrix", viewport);

        prog.setUniform("Cycle", cycleN); //set the cycle number in the shaders
    
}

//Sets the Matrices for the particle shaders
void SceneBasic_Uniform::setMatricesParticle(GLSLProgram& p)
{
    mat4 mv = view * model; //we create a model view matrix

    p.setUniform("MV", mv); //set the uniform for the model view matrix

    p.setUniform("Proj", projection); //we set the uniform for normal matrix

}

//Resizes the viewport accordingly
void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);

    float w2 = w / 2.0f;
    float h2 = h / 2.0f;
    viewport = mat4(glm::vec4(w2, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, h2, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));

}
