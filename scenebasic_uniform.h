#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include "helper/texture.h"
#include "helper/objmesh.h"
#include "helper/torus.h"
#include "helper/teapot.h"
#include <glm/glm.hpp>
#include "helper/grid.h"
#include "helper/utils.h"
#include "helper/particleutils.h"
#include "helper/random.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog, partProg, flatProg; //Prog for the standard shaders, partProg and flatProg for particle shaders

    GLSLProgram solidProg; //unused
    GLuint  startTime, particles, nParticles;

    GLuint initVel = 0;
    std::unique_ptr<ObjMesh> ogre;

    Random rand;
    Grid grid;
    

    glm::mat4 viewport;

    GLuint vaoHandle;
    glm::vec3 emitterPos, emitterDir;

    float angle, time, particleLifeTime;
    void initBuffers();
    float randFloat();

    void setMatrices();
    void setMatricesParticle(GLSLProgram&);

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
