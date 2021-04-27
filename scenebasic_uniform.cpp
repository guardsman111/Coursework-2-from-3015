#include "scenebasic_uniform.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
using glm::vec3;
using glm::mat4;

//constructor for torus
SceneBasic_Uniform::SceneBasic_Uniform()
{
    ogre = ObjMesh::load("media/bs_ears.obj", false, true);
}

//constructor for teapot
//SceneBasic_Uniform::SceneBasic_Uniform() : teapot(13, glm::translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.25f))) {}

void SceneBasic_Uniform::initScene()
{
    if (cycleN == 0) 
    {
        compile();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

        glEnable(GL_DEPTH_TEST);

        float c = 2.5f;
        projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);

        ///////////////// UNIFORMSES /////////////////////
        prog.setUniform("Line.Width", 0.2f);
        prog.setUniform("Line.Color", glm::vec4(0.05f, 1.0f, 0.55f, 1.0f));
        prog.setUniform("Material.Kd", 0.0f, 0.7f, 0.2f);
        prog.setUniform("Material.Ka", 0.2f, 0.3f, 0.8f);
        prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
        prog.setUniform("Material.Shininess", 100.0f);
        prog.setUniform("Light.Intensity", 1.0f, 1.0f, 1.0f);
        prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    if (cycleN == 1) 
    {
        compile();

        glEnable(GL_DEPTH_TEST);

        float c = 3.5f;
        projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);
        glPointSize(10.0f);

        float v[] = { -1.0f, -1.0f, -0.5f, 1.0f, 0.5f, -1.0f, 1.0f, 1.0f };

        GLuint vboHandle;

        glGenBuffers(1, &vboHandle);

        glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), v, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vaoHandle);
        glBindVertexArray(vaoHandle);

        glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        glPatchParameteri(GL_PATCH_VERTICES, 4);

        prog.use();
        prog.setUniform("NumSegments", 50);
        prog.setUniform("NumSegments", 1);
        prog.setUniform("Line.Width", 0.2f);
        prog.setUniform("Line.Color", glm::vec4(0.05f, 1.0f, 0.55f, 1.0f));
        prog.setUniform("Material.Kd", 0.0f, 0.7f, 0.2f);
        prog.setUniform("Material.Ka", 0.2f, 0.3f, 0.8f);
        prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
        prog.setUniform("Material.Shininess", 100.0f);
        prog.setUniform("Light.Intensity", 1.0f, 1.0f, 1.0f);
        prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        solidProg.use();
        solidProg.setUniform("Color", glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
    }
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
        prog.compileShader("shader/Geometry_Shader.geom");
        prog.compileShader("shader/tessellation_control_shader.tcs");
        prog.compileShader("shader/tessellation_evalution.tes");
		prog.link();
		prog.use();

        solidProg.compileShader("shader/solid.vs");
        solidProg.compileShader("shader/solid.fs");
        solidProg.link();

	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
	//update your angle here
}

void SceneBasic_Uniform::render()
{
    if (cycleN == 0) 
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float radius = 10.0f;

        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;

        vec3 cameraPos(camX, 0.0f, camZ);

        view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

        model = mat4(1.0f);

        setMatrices(); //we set matrices 
        //torus.render();     //we render the torus
        //teapot.render();  

        ogre->render();

        glFinish();
    }
    else if(cycleN == 1)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 cameraPos(0.0f, 0.0f, 1.5f);

        view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

        model = mat4(1.0f);

        glBindVertexArray(vaoHandle);
        setMatrices();

        prog.use();
        glDrawArrays(GL_PATCHES, 0, 4);

        solidProg.use();

        glDrawArrays(GL_POINTS, 0, 4);

        glFinish();

    }
}

void SceneBasic_Uniform::setMatrices()
{
    if (cycleN == 0)
    {
        mat4 mv = view * model; //we create a model view matrix
    
        prog.setUniform("ModelViewMatrix", mv); //set the uniform for the model view matrix
    
        prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))); //we set the uniform for normal matrix
    
        prog.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix

        prog.setUniform("ViewportMatrix", viewport);

        prog.setUniform("Cycle", cycleN); //set the cycle number in the shaders
    }
    if (cycleN == 1) 
    {
        mat4 mv = view * model; //we create a model view matrix
        prog.use();
        prog.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix
        solidProg.use();
        solidProg.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix
    }
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);

    float w2 = w / 2.0f;
    float h2 = h / 2.0f;
    viewport = mat4(glm::vec4(w2, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, h2, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));

    //width = w;
    //height = h;
    //projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}
