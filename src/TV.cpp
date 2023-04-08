#include "TV.h"

void drawObj(cMeshObj* pCurrentMeshObject, glm::mat4x4 mat_PARENT_Model, cShaderManager* pShaderManager, cVAOManager* pVAOManager);
extern double g_CurrentTime;

TV::TV()
{
	m_currentChannel = 1;
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
}

void TV::TurnOff()
{
}

void TV::incCHN()
{
}

void TV::decCHN()
{
}

void TV::render(cFBO* fbo, cShaderManager* pShaderManager, cVAOManager* pVAOManager)
{
    pShaderManager->setShaderUniform1f("bFullScreen", true);
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

    //pShaderManager->setShaderUniform1f("blurAmount", 0.5f);
    glm::mat4 scrMAT = glm::mat4(1.f);
    //cMeshObj* scrOBJ = pVAOManager->findMeshObjAddr(projector);
    ////bool result = pVAOManager->setInstanceObjScale("projecter1", 100.f);
    //bool result = pVAOManager->setInstanceObjVisible(projector, true);
    if (isStaticCHN())
    {
        pShaderManager->setShaderUniform1f("bStaticScreen", true);
        pShaderManager->setShaderUniform1f("iTime", g_CurrentTime);
    }
    //pShaderManager->setShaderUniform1f("bMirror", true);
    drawObj(this->meshScreen, scrMAT, pShaderManager, pVAOManager);
    //result = pVAOManager->setInstanceObjVisible(projector, false);
    //pShaderManager->setShaderUniform1f("bMirror", false);
    pShaderManager->setShaderUniform1f("bStaticScreen", false);
    pShaderManager->setShaderUniform1f("bFullScreen", false);
}

bool TV::isStaticCHN()
{
    bool result;
    switch (m_currentChannel)
    {
    case 1:  
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    default:
        result = true;
        break;
    }
    return result;
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
