#include "Light/cLightManager.h"
#include "VAOManager/cVAOManager.h"

extern cLightManager* g_pTheLightManager;

void light0Setup() //lamp
{
    //Directional light
    cDirLight* pDirLight = new cDirLight(*::g_pTheLightManager->plight[0]);
    *pDirLight->pDirection = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    *pDirLight->pDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    *pDirLight->pSpecular = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);
    *pDirLight->pTurnON = 0;


    //::g_pTheLightManager->plight[3]->position = glm::vec4(1.95f, 2.7f, -0.75f, 1.f);
    //::g_pTheLightManager->plight[3]->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //::g_pTheLightManager->plight[3]->attenuation = glm::vec4(0.19f, 0.003f, 0.072f, 1.0f);
    //::g_pTheLightManager->plight[3]->type = cLight::LightType::LIGHT_SPOT;
    //::g_pTheLightManager->plight[3]->direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    //// inner and outer angles
    //::g_pTheLightManager->plight[3]->angle.x = 10.0f;     // Degrees
    //::g_pTheLightManager->plight[3]->angle.y = 20.0f;     // Degrees
    //::g_pTheLightManager->plight[3]->turnON = 1;
}

void light1Setup(cVAOManager* pVAOManager)
{
    glm::vec4 atten = glm::vec4(0.7f, 0.1f, 0.2f, 1.0f);
    glm::vec4 diffuse = glm::vec4(20.0f, 7.5f, 0.0f, 1.0f);

    ::g_pTheLightManager->plight[1]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[1]->diffuse = diffuse;
    ::g_pTheLightManager->plight[1]->position = pVAOManager->getInstanceObjPosition("debug_light1");
    ::g_pTheLightManager->plight[1]->attenuation = atten;
    ::g_pTheLightManager->plight[1]->turnON = 1;

    ::g_pTheLightManager->plight[2]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[2]->diffuse = glm::vec4(20.0f, 20.0f, 20.0f, 1.0f);
    ::g_pTheLightManager->plight[2]->position = pVAOManager->getInstanceObjPosition("debug_light2");
    ::g_pTheLightManager->plight[2]->attenuation = atten;
    ::g_pTheLightManager->plight[2]->turnON = 1;

    ::g_pTheLightManager->plight[3]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[3]->diffuse = diffuse;
    ::g_pTheLightManager->plight[3]->position = pVAOManager->getInstanceObjPosition("debug_light3");
    ::g_pTheLightManager->plight[3]->attenuation = atten;
    ::g_pTheLightManager->plight[3]->turnON = 1;

    ::g_pTheLightManager->plight[4]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[4]->diffuse = diffuse;
    ::g_pTheLightManager->plight[4]->position = pVAOManager->getInstanceObjPosition("debug_light4");
    ::g_pTheLightManager->plight[4]->attenuation = atten;
    ::g_pTheLightManager->plight[4]->turnON = 1;

    ::g_pTheLightManager->plight[5]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[5]->diffuse = diffuse;
    ::g_pTheLightManager->plight[5]->position = pVAOManager->getInstanceObjPosition("debug_light5");
    ::g_pTheLightManager->plight[5]->attenuation = atten;
    ::g_pTheLightManager->plight[5]->turnON = 1;

    ::g_pTheLightManager->plight[6]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[6]->diffuse = diffuse;
    ::g_pTheLightManager->plight[6]->position = pVAOManager->getInstanceObjPosition("debug_light6");
    ::g_pTheLightManager->plight[6]->attenuation = atten;
    ::g_pTheLightManager->plight[6]->turnON = 1;
}
void light2Setup(cVAOManager* pVAOManager)
{
    //cDirLight* pDirLight = new cDirLight(*::g_pTheLightManager->plight[5]);
    //*pDirLight->pDirection = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    //*pDirLight->pDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //*pDirLight->pSpecular = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);
    //*pDirLight->pTurnON = 1;

    ::g_pTheLightManager->plight[7]->position = glm::vec4(15.6f, 0.6f, 8.7f, 1.0f);
    ::g_pTheLightManager->plight[7]->diffuse = glm::vec4(2.0f, 0.f, 0.f, 1.0f);
    ::g_pTheLightManager->plight[7]->attenuation = glm::vec4(1.19f, 0.1f, 0.2f, 1.0f);
    ::g_pTheLightManager->plight[7]->type = cLight::LightType::LIGHT_SPOT;
    ::g_pTheLightManager->plight[7]->direction = glm::vec4(-100.f, 0.f, 0.f, 1.0f);
    // inner and outer angles
    ::g_pTheLightManager->plight[7]->angle.x = 5.0f;     // Degrees
    ::g_pTheLightManager->plight[7]->angle.y = 5.0f;     // Degrees
    ::g_pTheLightManager->plight[7]->turnON = 1;

    ::g_pTheLightManager->plight[8]->position = glm::vec4(15.6f, 0.6f, 8.7f, 1.0f);
    ::g_pTheLightManager->plight[8]->diffuse = glm::vec4(0.f, 1.f, 0.f, 1.0f);
    ::g_pTheLightManager->plight[8]->attenuation = glm::vec4(1.19f, 0.1f, 0.2f, 1.0f);
    ::g_pTheLightManager->plight[8]->type = cLight::LightType::LIGHT_SPOT;
    ::g_pTheLightManager->plight[8]->direction = glm::vec4(100.f, -0.5f, 0.f, 1.0f);
    // inner and outer angles
    ::g_pTheLightManager->plight[8]->angle.x = 5.0f;     // Degrees
    ::g_pTheLightManager->plight[8]->angle.y = 5.0f;     // Degrees
    ::g_pTheLightManager->plight[8]->turnON = 1;

    ::g_pTheLightManager->plight[9]->position = glm::vec4(15.6f, 0.6f, 8.7f, 1.0f);
    ::g_pTheLightManager->plight[9]->diffuse = glm::vec4(0.f, 0.f, 1.0f, 1.0f);
    ::g_pTheLightManager->plight[9]->attenuation = glm::vec4(1.19f, 0.1f, 0.2f, 1.0f);
    ::g_pTheLightManager->plight[9]->type = cLight::LightType::LIGHT_SPOT;
    ::g_pTheLightManager->plight[9]->direction = glm::vec4(0.f, -0.5f, 100.f, 1.0f);
    // inner and outer angles
    ::g_pTheLightManager->plight[9]->angle.x = 5.0f;     // Degrees
    ::g_pTheLightManager->plight[9]->angle.y = 5.0f;     // Degrees
    ::g_pTheLightManager->plight[9]->turnON = 1;
}

void light3Setup()
{

    ::g_pTheLightManager->plight[6]->position = glm::vec4(15.6f, 0.6f, 8.7f, 1.0f);
    ::g_pTheLightManager->plight[6]->diffuse = glm::vec4(0.8f, .50f, 0.4f, 1.0f);
    ::g_pTheLightManager->plight[6]->attenuation = glm::vec4(0.19f, 0.003f, 0.072f, 1.0f);
    ::g_pTheLightManager->plight[6]->type = cLight::LightType::LIGHT_SPOT;
    ::g_pTheLightManager->plight[6]->direction = glm::vec4(0.07f, -0.5f, 1.0f, 1.0f);
    // inner and outer angles
    ::g_pTheLightManager->plight[6]->angle.x = 10.0f;     // Degrees
    ::g_pTheLightManager->plight[6]->angle.y = 20.0f;     // Degrees
    ::g_pTheLightManager->plight[6]->turnON = 1;

    ::g_pTheLightManager->plight[7]->position = glm::vec4(15.0f, 0.6f, 9.5f, 1.0f);
    ::g_pTheLightManager->plight[7]->diffuse = glm::vec4(0.8f, .50f, 0.4f, 1.0f);
    ::g_pTheLightManager->plight[7]->attenuation = glm::vec4(0.19f, 0.003f, 0.072f, 1.0f);
    ::g_pTheLightManager->plight[7]->type = cLight::LightType::LIGHT_SPOT;
    ::g_pTheLightManager->plight[7]->direction = glm::vec4(0.07f, -0.5f, 1.0f, 1.0f);
    // inner and outer angles
    ::g_pTheLightManager->plight[7]->angle.x = 10.0f;     // Degrees
    ::g_pTheLightManager->plight[7]->angle.y = 20.0f;     // Degrees
    ::g_pTheLightManager->plight[7]->turnON = 1;

}

void light4Setup()
{
    ::g_pTheLightManager->plight[8]->position = glm::vec4(11.9f, 0.4f, 5.f, 1.0f);
    ::g_pTheLightManager->plight[8]->diffuse = glm::vec4(1.f, 1.f, 1.f, 1.0f);
    ::g_pTheLightManager->plight[8]->attenuation = glm::vec4(0.19f, 0.003f, 0.072f, 1.0f);
    ::g_pTheLightManager->plight[8]->type = cLight::LightType::LIGHT_SPOT;
    ::g_pTheLightManager->plight[8]->direction = glm::vec4(0.07f, -0.5f, 1.0f, 1.0f);
    // inner and outer angles    
    ::g_pTheLightManager->plight[8]->angle.x = 10.0f;     // Degrees
    ::g_pTheLightManager->plight[8]->angle.y = 20.0f;     // Degrees
    ::g_pTheLightManager->plight[8]->turnON = 1;

    ::g_pTheLightManager->plight[9]->position = glm::vec4(11.2f, 0.6f, 5.7f, 1.0f);
    ::g_pTheLightManager->plight[9]->diffuse = glm::vec4(1.f, 1.f, 1.f, 1.0f);
    ::g_pTheLightManager->plight[9]->attenuation = glm::vec4(0.19f, 0.003f, 0.072f, 1.0f);
    ::g_pTheLightManager->plight[9]->type = cLight::LightType::LIGHT_SPOT;
    ::g_pTheLightManager->plight[9]->direction = glm::vec4(0.07f, -0.5f, 1.0f, 1.0f);
    // inner and outer angle
    ::g_pTheLightManager->plight[9]->angle.x = 10.0f;     // Degrees
    ::g_pTheLightManager->plight[9]->angle.y = 20.0f;     // Degrees
    ::g_pTheLightManager->plight[9]->turnON = 1;
}




