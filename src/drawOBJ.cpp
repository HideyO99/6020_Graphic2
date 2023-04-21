#include "VAOManager/cVAOManager.h"
#include "Shader/cShaderManager.h"
#include "Light/cLightManager.h"
#include "Texture/cTextureManager.h"
#include "boneShader.h"
#include "MeshObj/Model.h"

extern cLightManager* g_pTheLightManager;
extern cTextureManager* g_pTextureManager;
extern BoneShader gBoneShader;
extern glm::vec3 g_cameraEye;
extern glm::vec3 g_cameraTarget;

void drawObj(cMeshObj* pCurrentMeshObject, glm::mat4x4 mat_PARENT_Model, cShaderManager* pShaderManager, cVAOManager* pVAOManager)
{
    // Assume that the frustom planes are pure vertical and horizontal
    {   // Outsize of the horizontal planes

        glm::vec3 eyeNoY = ::g_cameraEye;
        glm::vec3 targetNoY = ::g_cameraTarget;
        glm::vec3 objectNoY = pCurrentMeshObject->position;

        eyeNoY.y = 0.0f;
        targetNoY.y = 0.0f;
        objectNoY.y = 0.0f;

        glm::vec3 vEyeVector = glm::normalize(targetNoY - eyeNoY);

        glm::vec3 vObjectToEye = glm::normalize(objectNoY - eyeNoY);

        // Take dot product of these two vectors
        float dotView = glm::dot(vEyeVector, vObjectToEye);

        // Cull anything below a certian angle.
        // (this is just like the spot light does it, too)

        // Get the cosine of 60 degrees
        float desiredFOVCullAngleValue = glm::cos(glm::radians(30.0f));

        // If greater than, then it's "in front" of the camera in the FOL
        // If less than, it's outside (or behinds the camera)
        if (dotView < desiredFOVCullAngleValue)
        {
            //return;
        }
    }

    // Assume that the frustom planes are pure vertical and horizontal
    {   // Outsize of the vertical planes

        glm::vec3 eyeNoX = ::g_cameraEye;
        glm::vec3 targetNoX = ::g_cameraTarget;
        glm::vec3 objectNoX = pCurrentMeshObject->position;

        eyeNoX.x = 0.0f;
        targetNoX.x = 0.0f;
        objectNoX.x = 0.0f;

        glm::vec3 vEyeVector = glm::normalize(targetNoX - eyeNoX);

        glm::vec3 vObjectToEye = glm::normalize(objectNoX - eyeNoX);

        // Take dot product of these two vectors
        float dotView = glm::dot(vEyeVector, vObjectToEye);

        // Cull anything below a certian angle.
        // (this is just like the spot light does it, too)

        // Get the cosine of 60 degrees
        float desiredFOVCullAngleValue = glm::cos(glm::radians(10.0f));

        // If greater than, then it's "in front" of the camera in the FOL
        // If less than, it's outside (or behinds the camera)
        if (dotView < desiredFOVCullAngleValue)
        {
            //return;
        }
    }

    // Don't draw any "back facing" triangles
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    // Turn on depth buffer test at draw time
    glEnable(GL_DEPTH_TEST);

    //matModel = glm::mat4x4(1.0f);  // identity matrix

#if 0   //flame effect
    pShaderManager->setShaderUniform1f("bIsFlameObject", (GLfloat)GL_TRUE);
    glDepthMask(GL_FALSE);
#endif

    if (pCurrentMeshObject->meshName == "flame")
    {
        pShaderManager->setShaderUniform1f("bUseDiscardTexture", (GLfloat)GL_TRUE);

        if ((pCurrentMeshObject->scale.x < 8.5))
        {
            pCurrentMeshObject->scale += 0.05f;
            g_pTheLightManager->plight[1]->attenuation -= glm::vec4(0.001, 0.0001, 0.00002, 0);
            g_pTheLightManager->plight[2]->attenuation -= glm::vec4(0.001, 0.0001, 0.00002, 0);
            g_pTheLightManager->plight[3]->attenuation -= glm::vec4(0.001, 0.0001, 0.00002, 0);
            g_pTheLightManager->plight[4]->attenuation -= glm::vec4(0.001, 0.0001, 0.00002, 0);
            g_pTheLightManager->plight[5]->attenuation -= glm::vec4(0.001, 0.0001, 0.00002, 0);
            g_pTheLightManager->plight[6]->attenuation -= glm::vec4(0.001, 0.0001, 0.00002, 0);
        }
        else
        {
            pCurrentMeshObject->scale.x = 7;
            g_pTheLightManager->plight[1]->attenuation = glm::vec4(0.7f, 0.1f, 0.2f, 1.0f);
            g_pTheLightManager->plight[2]->attenuation = glm::vec4(0.7f, 0.1f, 0.2f, 1.0f);
            g_pTheLightManager->plight[3]->attenuation = glm::vec4(0.7f, 0.1f, 0.2f, 1.0f);
            g_pTheLightManager->plight[4]->attenuation = glm::vec4(0.7f, 0.1f, 0.2f, 1.0f);
            g_pTheLightManager->plight[5]->attenuation = glm::vec4(0.7f, 0.1f, 0.2f, 1.0f);
            g_pTheLightManager->plight[6]->attenuation = glm::vec4(0.7f, 0.1f, 0.2f, 1.0f);
        }

    }
    else
    {
        pShaderManager->setShaderUniform1f("bUseDiscardTexture", (GLfloat)GL_FALSE);
    }

    GLuint texture07_Number = g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[7]);
    GLuint texture07_Unit = 7;
    glActiveTexture(texture07_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture07_Number);
    pShaderManager->setShaderUniform1i("texture7", texture07_Unit);

    glm::mat4x4 matModel = mat_PARENT_Model;
    // Move the object 
    glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f), pCurrentMeshObject->position);

    //std::cout << pCurrentMeshObject->instanceName << " position x = " << pCurrentMeshObject->position.x << " y = " << pCurrentMeshObject->position.y << " z = " << pCurrentMeshObject->position.z << std::endl;

    //rotate //todo change to quat
    //glm::mat4 matRoationZ = glm::rotate(glm::mat4(1.0f), pCurrentMeshObject->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    //glm::mat4 matRoationY = glm::rotate(glm::mat4(1.0f), pCurrentMeshObject->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    //glm::mat4 matRoationX = glm::rotate(glm::mat4(1.0f), pCurrentMeshObject->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 matRotation = glm::mat4_cast(pCurrentMeshObject->rotation);
    // Scale the object
    glm::vec3 uniformScale = pCurrentMeshObject->scale;
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f), uniformScale);

    matModel = matModel * matTranslation;

    //matModel = matModel * matRoationX;
    //matModel = matModel * matRoationY;
    //matModel = matModel * matRoationZ;

    matModel = matModel * matRotation * matScale;

    pShaderManager->setShaderUniformM4fv("mModel", matModel);
    //pShaderManager->setShaderUniformM4fv("mView", matView);
    //pShaderManager->setShaderUniformM4fv("mProjection", matProjection);

    glm::mat4 mModelInverseTransform = glm::inverse(glm::transpose(matModel));
    pShaderManager->setShaderUniformM4fv("mModelInverseTranspose", mModelInverseTransform);

    if (pCurrentMeshObject->hasBone)
    {
        pShaderManager->setShaderUniform1f("hasBone", (GLfloat)GL_TRUE);
        for (int i = 0; i < pCurrentMeshObject->BoneModelMatrices.size(); i++)
        {
            glUniformMatrix4fv(gBoneShader.BoneMatrices[i], 1, GL_FALSE, glm::value_ptr(pCurrentMeshObject->BoneModelMatrices[i]));
            glUniformMatrix4fv(gBoneShader.BoneRotationMatrices[i], 1, GL_FALSE, glm::value_ptr(pCurrentMeshObject->BoneRotationMatrices[i]));
        }
    }

    // Wireframe
    if (pCurrentMeshObject->isWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // GL_POINT, GL_LINE, GL_FILL
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pShaderManager->setShaderUniform4f("RGBA_Color",
        pCurrentMeshObject->color_RGBA.r,
        pCurrentMeshObject->color_RGBA.g,
        pCurrentMeshObject->color_RGBA.b,
        pCurrentMeshObject->color_RGBA.w);

    if (pCurrentMeshObject->bUse_RGBA_colour)
    {
        pShaderManager->setShaderUniform1f("bUseRGBA_Color", (GLfloat)GL_TRUE);
    }
    else
    {
        pShaderManager->setShaderUniform1f("bUseRGBA_Color", (GLfloat)GL_FALSE);
    }
    pShaderManager->setShaderUniform4f("specularColour",
        pCurrentMeshObject->specular_colour_and_power.r,
        pCurrentMeshObject->specular_colour_and_power.g,
        pCurrentMeshObject->specular_colour_and_power.b,
        pCurrentMeshObject->specular_colour_and_power.w);

    //uniform bool bDoNotLight;	
    if (pCurrentMeshObject->bDoNotLight)
    {
        pShaderManager->setShaderUniform1f("bDoNotLight", (GLfloat)GL_TRUE);
    }
    else
    {
        pShaderManager->setShaderUniform1f("bDoNotLight", (GLfloat)GL_FALSE);
    }

    //set texture0
    GLuint texture00_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[0]);
    GLuint texture00_Unit = 0;
    glActiveTexture(texture00_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture00_Number);
    pShaderManager->setShaderUniform1i("texture0", texture00_Unit);
    //set texture1
    GLuint texture01_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[1]);
    GLuint texture01_Unit = 1;
    glActiveTexture(texture01_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture01_Number);
    pShaderManager->setShaderUniform1i("texture1", texture01_Unit);
    //set texture2
    GLuint texture02_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[2]);
    GLuint texture02_Unit = 2;
    glActiveTexture(texture02_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture02_Number);
    pShaderManager->setShaderUniform1i("texture2", texture02_Unit);
    //set texture3
    GLuint texture03_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[3]);
    GLuint texture03_Unit = 3;
    glActiveTexture(texture03_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture03_Number);
    pShaderManager->setShaderUniform1i("texture3", texture03_Unit);
    //set texture4
    GLuint texture04_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[4]);
    GLuint texture04_Unit = 4;
    glActiveTexture(texture04_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture04_Number);
    pShaderManager->setShaderUniform1i("texture4", texture04_Unit);
    //set texture5
    GLuint texture05_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[5]);
    GLuint texture05_Unit = 5;
    glActiveTexture(texture05_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture05_Number);
    pShaderManager->setShaderUniform1i("texture5", texture05_Unit);
    //set texture6
    GLuint texture06_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[6]);
    GLuint texture06_Unit = 6;
    glActiveTexture(texture06_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture06_Number);
    pShaderManager->setShaderUniform1i("texture6", texture06_Unit);

    pShaderManager->setShaderUniform4f("texRatio_0_3",
        pCurrentMeshObject->textureRatios[0],
        pCurrentMeshObject->textureRatios[1],
        pCurrentMeshObject->textureRatios[2],
        pCurrentMeshObject->textureRatios[3]);

    //cube map texture
    GLuint cubeMapTextureNumber = g_pTextureManager->getTexttureID("SpaceBox");
    GLuint texture30Unit = 30;
    glActiveTexture(texture30Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureNumber);
    pShaderManager->setShaderUniform1i("skyboxTexture", texture30Unit);

    cModelDrawInfo drawingInformation;
//todo doing
    std::string meshToDraw = pCurrentMeshObject->meshName;

    // Check if this object has a Level of Detail list of meshes
    if (!pCurrentMeshObject->vecLODs.empty())
    {
        // go through the vector looking for the closest LOD from the camera
        float distanceToEye = glm::distance(pCurrentMeshObject->position, g_cameraEye);

        for (std::vector<sLODInfo>::iterator itLOD = pCurrentMeshObject->vecLODs.begin();
            itLOD != pCurrentMeshObject->vecLODs.end(); itLOD++)
        {
            if (distanceToEye < itLOD->minDistance)
            {
                meshToDraw = itLOD->meshName;
            }
        }
    }

    if (pVAOManager->FindDrawInfo(meshToDraw, drawingInformation))
    { 
        glBindVertexArray(drawingInformation.VAO_ID);

        glDrawElements(GL_TRIANGLES, drawingInformation.numberOfIndices, GL_UNSIGNED_INT, (void*)0);

        glBindVertexArray(0);

    }
    //else
    //{
    //    // Didn't find that model
    //    std::cout << "Error: didn't find model to draw." << std::endl;

    //}

    for (std::vector<cMeshObj* >::iterator itCurrentMesh = pCurrentMeshObject->vecChildMesh.begin();
        itCurrentMesh != pCurrentMeshObject->vecChildMesh.end();
        itCurrentMesh++)
    {
        cMeshObj* pCurrentChildMeshObject = *itCurrentMesh;
        drawObj(pCurrentChildMeshObject, matModel, pShaderManager, pVAOManager);
    }
    if (pCurrentMeshObject->hasBone)
    {
        pShaderManager->setShaderUniform1f("hasBone", (GLfloat)GL_FALSE);
    }
}
