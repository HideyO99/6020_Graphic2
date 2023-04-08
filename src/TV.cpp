#include "TV.h"

void drawObj(cMeshObj* pCurrentMeshObject, glm::mat4x4 mat_PARENT_Model, cShaderManager* pShaderManager, cVAOManager* pVAOManager);
void updateInstanceObj(cShaderManager* pShaderManager, cVAOManager* pVAOManager);
extern double g_CurrentTime;
extern glm::vec3 g_cameraEye;
extern glm::vec3 g_cameraTarget;
extern glm::vec3 g_upVector;
extern float fov;
extern cFBO* g_FBO_02;
extern cFBO* g_FBO_03;
extern cFBO* g_FBO_04;

TV::TV()
{
	m_currentChannel = 7;
	isPwrOn = false;
	meshBody = nullptr;
	meshScreen = nullptr;
	lastTurnOn = 0;
}

TV::~TV()
{
}

void TV::TurnOn()
{
    this->isPwrOn = true;
}

void TV::TurnOff()
{
    this->isPwrOn = false;
}

void TV::incCHN()
{
}

void TV::decCHN()
{
}

void TV::render(cShaderManager* pShaderManager, cVAOManager* pVAOManager)
{

    pShaderManager->setShaderUniform1f("bFullScreen", true);
    if (this->isPwrOn)
    {
        switch (m_currentChannel)
        {
        case 3:
            setupFBO2Texture(g_FBO_02, pShaderManager);
            break;
        case 7:
            setupFBO2Texture(g_FBO_03, pShaderManager);
            break;
        case 11:
            setupFBO2Texture(g_FBO_04, pShaderManager);
            break;
        default:
            pShaderManager->setShaderUniform1f("bStaticScreen", true);
            pShaderManager->setShaderUniform1f("iTime", g_CurrentTime);
            break;
        }
    }

    glm::mat4 scrMAT = glm::mat4(1.f);
    //cMeshObj* scrOBJ = pVAOManager->findMeshObjAddr(projector);
    ////bool result = pVAOManager->setInstanceObjScale("projecter1", 100.f);
    //bool result = pVAOManager->setInstanceObjVisible(projector, true);
    //if (isStaticCHN())
    //{
    //    pShaderManager->setShaderUniform1f("bStaticScreen", true);
    //    pShaderManager->setShaderUniform1f("iTime", g_CurrentTime);
    //}
    //pShaderManager->setShaderUniform1f("bMirror", true);
    drawObj(this->meshScreen, scrMAT, pShaderManager, pVAOManager);
    //result = pVAOManager->setInstanceObjVisible(projector, false);
    //pShaderManager->setShaderUniform1f("bMirror", false);
    pShaderManager->setShaderUniform1f("bStaticScreen", false);
    pShaderManager->setShaderUniform1f("bFullScreen", false);
}

void TV::recieveSignal(cFBO* fbo, cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::vec3 eye, glm::vec3 target)
{
    glm::mat4x4 matProjection;
    glm::mat4x4 matView;

    //GLuint shaderID = pShaderManager->getIDfromName("Shader01"); 
    GLuint shaderID = pShaderManager->getCurrentShaderID();

    //FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
    glViewport(0, 0, fbo->width, fbo->height);
    fbo->clearBuffer(true, true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 mirrorNorm = glm::normalize(target - eye);
    glm::vec3 eyeTemp = g_cameraEye - 2.f * glm::dot(g_cameraEye - eye, mirrorNorm) * mirrorNorm;
    glm::vec3 targetTemp = g_cameraTarget - 2.f * glm::dot(g_cameraTarget - eye, mirrorNorm) * mirrorNorm;
    matView = glm::lookAt(eyeTemp, targetTemp, ::g_upVector);

    GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");

    glUniform4f(eyeLocation_UniLoc, eye.x, eye.y, eye.z, 1.0f);

    float FBO_screenRatio = static_cast<float>(fbo->width) / fbo->height;
    matProjection = glm::perspective(glm::radians(fov), FBO_screenRatio, 0.1f, 10000.f);
    pShaderManager->setShaderUniformM4fv("mView", matView);
    pShaderManager->setShaderUniformM4fv("mProjection", matProjection);

    if (this->isPwrOn)
    {
        updateInstanceObj(pShaderManager, pVAOManager);
    }
}

bool TV::isStaticCHN()
{
    bool result;
    switch (m_currentChannel)
    {
    case 1:  
    case 2:
        result = true;
        break;
    case 3:
        result = false;
        break;
    case 4:
    case 5:
    case 6:
        result = true;
        break;
    case 7:
        result = false;
        break;
    case 8:
    case 9:
    case 10:
        result = true;
        break;
    case 11:
        result = false;
        break;
    case 12:
    case 13:
    default:
        result = true;
        break;
    }
    return result;
}

void TV::setupFBO2Texture(cFBO* fbo, cShaderManager* pShaderManager)
{
    //set FBO to texture
    GLuint texture21_Unit = 21;
    glActiveTexture(texture21_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo->vertexMaterialColorID);
    pShaderManager->setShaderUniform1i("sampler_FBO_vertexMaterialColour", texture21_Unit);

    GLuint texture22_Unit = 22;
    glActiveTexture(texture22_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo->vertexNormalID);
    pShaderManager->setShaderUniform1i("sampler_FBO_vertexNormal", texture22_Unit);

    GLuint texture23_Unit = 23;
    glActiveTexture(texture23_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo->vertexWorldPositionID);
    pShaderManager->setShaderUniform1i("sampler_FBO_vertexWorldPos", texture23_Unit);

    GLuint texture24_Unit = 24;
    glActiveTexture(texture24_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo->vertexSpecularID);
    pShaderManager->setShaderUniform1i("sampler_FBO_vertexSpecular", texture24_Unit);

    GLuint texture25_Unit = 25;
    glActiveTexture(texture25_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo->vertexRefractionID);
    pShaderManager->setShaderUniform1i("sampler_FBO_vertexRefraction", texture25_Unit);
}

void TV::setup(glm::vec4 color, cMeshObj* body, cMeshObj* screen)
{
	meshBody = body;
	meshBody->bUse_RGBA_colour = true;
	meshBody->color_RGBA = color;
	meshBody->rotation = glm::vec4(-1.5f, 0.f, 0.f, 1.f);
	meshBody->scale = glm::vec3(0.3);

	meshScreen = screen;
	meshScreen->rotation = glm::vec4(-1.5f, 0.f, 0.f, 1.f);
	meshScreen->scale = glm::vec3(0.3);
	meshScreen->isVisible = false;
	meshScreen->isStaticScreen = true;
}
