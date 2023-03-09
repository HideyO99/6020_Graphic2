#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>



#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3        (x,y,z)
#include <glm/vec4.hpp> // glm::vec4        (x,y,z,w)
#include <glm/mat4x4.hpp> // glm::mat4
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader/cShaderManager.h"
#include "VAOManager/cVAOManager.h"
#include "MeshObj/cMeshObj.h"
#include "Light/cLightManager.h"
#include "GUI/cGUI.h"
#include "Texture/cTextureManager.h"
#include "FBO/cFBO.h"

#define MODEL_LIST_XML          "asset/model.xml"
#define VERTEX_SHADER_FILE      "src/shader/vertexShader.glsl"
#define FRAGMENT_SHADER_FILE    "src/shader/fragmentShader.glsl"
#define TEXTURE_PATH            "asset/texture"
#define USE_IMGUI true


glm::vec3 g_cameraEye = glm::vec3(0.0, 5.0, 0.0f);
glm::vec3 g_cameraTarget_defualt = glm::vec3(-2.5f, 2.5f, -15.0f);
glm::vec3 g_cameraTarget = g_cameraTarget_defualt;
glm::vec3 g_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
bool bIsWalkAround = false;
bool firstMouse = true;
float yaw = -90.0f;	
float pitch = 0.0f;
float lastX = 1280.0f / 2.0;
float lastY = 800.0 / 2.0;
float fov = 45.0f;

bool toggleblur = false;

cLightManager* g_pTheLightManager = NULL;
static GLFWwindow* window = nullptr;

cTextureManager* g_pTextureManager = NULL;

cFBO* g_FBO_01 = NULL;
cFBO* g_FBO_02 = NULL;
cFBO* g_FBO_03 = NULL;
cFBO* g_FBO_04 = NULL;
cMeshObj* g_MeshBoss = NULL;

extern void error_callback(int error, const char* description);
extern void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
extern void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
extern void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
extern void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
extern void window_size_callback(GLFWwindow* window, int width, int height);

void updateInstanceObj(cShaderManager* pShaderManager, cVAOManager* pVAOManager);
void drawObj(cMeshObj* pCurrentMeshObject, glm::mat4x4 mat_PARENT_Model, cShaderManager* pShaderManager, cVAOManager* pVAOManager);
void light0Setup();
void light1Setup(cVAOManager* pVAOManager);
void light2Setup(cVAOManager* pVAOManager);
void light3Setup();
void light4Setup();

void setFBOPortal(cFBO* fbo, cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::vec3 eye, glm::vec3 target);
void setFBOtoTexture(cFBO* fbo, cShaderManager* pShaderManager, cVAOManager* pVAOManager, std::string projector);
void setFBOCubeMap(cFBO* fbo, cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::vec3 eye);
void setFBOtoTextureCubeMap(cFBO* fbo, cShaderManager* pShaderManager, cVAOManager* pVAOManager, std::string projector);
void setFBO2(cShaderManager* pShaderManager, cVAOManager* pVAOManager);

int main(void)
{

    bool result;
    std::cout << "starting..\n";
    //GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    //GLint mvp_location, vpos_location, vcol_location;
    GLuint shaderID = 0;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWmonitor* pMainScreen = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(pMainScreen);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    //window = glfwCreateWindow(mode->width, mode->height, "6020 Graphic2", pMainScreen, NULL); //full screen
    window = glfwCreateWindow(1280, 800, "6020 Graphic2", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    std::cout << "created window" << std::endl;

    glfwSetWindowSizeCallback(window, window_size_callback);

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

#if USE_IMGUI
    //initialize imgui
    cGUI* gui_ = new cGUI(&g_cameraEye,&g_cameraTarget);
    result = gui_->ImGUI_init(window);
    if (!result)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
#endif

    ::g_FBO_01 = new cFBO();
    ::g_FBO_02 = new cFBO();
    ::g_FBO_03 = new cFBO();
    ::g_FBO_04 = new cFBO();
    int screenW = 0;
    int screenH = 0;
    glfwGetFramebufferSize(window, &screenW, &screenH);
    std::string error;
    result = ::g_FBO_01->init(screenW, screenH, error); // for full screen quad
    result = ::g_FBO_02->init(screenW, screenH, error); // scene 1
    result = ::g_FBO_03->init(screenW, screenH, error); // scene 2
    result = ::g_FBO_04->init(screenW, screenH, error); // scene 3
    if (!result)
    {
        std::cout << "error: FBO initialization = " << error << std::endl;
    }
    else
    {
        std::cout << "FBO initialization -> done " << error << std::endl;
    }

    //create shader program
    cShaderManager* pShaderManager = new cShaderManager();
    cShaderManager::cShader vertexShader;
    cShaderManager::cShader fragmentShader;

    vertexShader.fileName = VERTEX_SHADER_FILE;
    fragmentShader.fileName = FRAGMENT_SHADER_FILE;
    result = pShaderManager->createShaderProgram("Shader01", vertexShader, fragmentShader);
    if (!result)
    {
        std::cout << "error: Shader compilation fail" << std::endl;

        glfwDestroyWindow(window);

        delete pShaderManager;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "shader compilation OK" << std::endl;
    }

    pShaderManager->useShaderPRogram("Shader01");
    shaderID = pShaderManager->getIDfromName("Shader01");
    glUseProgram(shaderID);

    //todo lighting
    ::g_pTheLightManager = new cLightManager();


    
    ::g_pTheLightManager->loadLightUniformLocation(shaderID);
#if USE_IMGUI
    for (size_t i = 0; i < MAX_LIGHT_SOURCE; i++)
    {
        gui_->pLight[i] = ::g_pTheLightManager->plight[i];
    }
#endif

    //load model
    cVAOManager* pVAOManager = new cVAOManager();

    result = pVAOManager->loadModelList(MODEL_LIST_XML, shaderID);
    if (!result)
    {
        std::cout << "cannot load model list" << std::endl;

        glfwDestroyWindow(window);

        delete pVAOManager;
        delete pShaderManager;

        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    ::g_cameraEye = pVAOManager->cameraEyeFromXML;

#if USE_IMGUI
    //gui_->pMapInstanceNametoMeshObj = &pVAOManager->mapInstanceNametoMeshObj;
    gui_->pVecInstanceMeshObj = &pVAOManager->pVecInstanceMeshObj;
#endif

    //load texture
    ::g_pTextureManager = new cTextureManager();
    ::g_pTextureManager->setBasePath(TEXTURE_PATH);
    ::g_pTextureManager->create2DTextureFromBMP("Dungeons_2_Texture_01_A.bmp");
    ::g_pTextureManager->create2DTextureFromBMP("lroc_color_poles_4k.bmp");
    ::g_pTextureManager->create2DTextureFromBMP("glowing-fire-flame.bmp");
    ::g_pTextureManager->create2DTextureFromBMP("glowing-fire-flame_bw.bmp");
    ::g_pTextureManager->create2DTextureFromBMP("photos_2018_7_4_fst_water-blue.bmp"); 
    ::g_pTextureManager->create2DTextureFromBMP("Beholder_Base_color.bmp");

    std::string load_texture_error = "";
    if (g_pTextureManager->createCubeTextureFromBMP("SpaceBox",
        "SpaceBox_right1_posX.bmp", /* positive X */
        "SpaceBox_left2_negX.bmp",  /* negative X */
        "SpaceBox_top3_posY.bmp",    /* positive Y */
        "SpaceBox_bottom4_negY.bmp",  /* negative Y */
        "SpaceBox_front5_posZ.bmp",  /* positive Z */
        "SpaceBox_back6_negZ.bmp", /* negative Z */
        true, load_texture_error))
    {
        std::cout << "Loaded the tropical sunny day cube map OK" << std::endl;
    }
    else
    {
        std::cout << "ERROR: Didn't load the tropical sunny day cube map.->" << load_texture_error << std::endl;
    }
    //setup object

   result = pVAOManager->setDungeonTexture("floorA", "Dungeons_2_Texture_01_A.bmp");
   result = pVAOManager->setTexture("moon", "lroc_color_poles_4k.bmp", 0);
   result = pVAOManager->setInstanceObjPosition("moon", glm::vec4(200.f,200.f,-100.f,0.f));
   result = pVAOManager->setInstanceObjScale("moon", 10);
   result = pVAOManager->setInstanceObjLighting("moon", false);
   result = pVAOManager->setTexture("water", "photos_2018_7_4_fst_water-blue.bmp", 0);
   result = pVAOManager->setInstanceObjRGB("water", glm::vec4(1.f, 1.f, 1.f, 0.5f));
   result = pVAOManager->setTorchTexture("flame", "glowing-fire-flame.bmp", "glowing-fire-flame_bw.bmp");
   
   result = pVAOManager->setIslandModelFlag("water", true);

   result = pVAOManager->setTexture("boss", "Beholder_Base_color.bmp", 0);

    result = pVAOManager->setSkyBoxFlag("skybox",true);
    //full screen quad
    result = pVAOManager->setInstanceObjVisible("projecter1", false); 
    result = pVAOManager->setInstanceObjScale("projecter1", 0.03f);
    result = pVAOManager->setUseRGBColorFlag("projecter1", false);
    result = pVAOManager->setInstanceObjPosition("projecter1", glm::vec4(0.f));

    result = pVAOManager->setInstanceObjVisible("projecter2", false);
    result = pVAOManager->setInstanceObjScale("projecter2", 5.f);
    result = pVAOManager->setUseRGBColorFlag("projecter2", false);
    result = pVAOManager->setInstanceObjPosition("projecter2", glm::vec4(-12.5f,2.5f,-15.f,1.f));

    result = pVAOManager->setInstanceObjVisible("projecter3", true);
    result = pVAOManager->setInstanceObjScale("projecter3", 5.f);
    result = pVAOManager->setUseRGBColorFlag("projecter3", false);
    result = pVAOManager->setInstanceObjPosition("projecter3", glm::vec4(-2.5f, 2.5f, -15.f, 1.f));

    result = pVAOManager->setInstanceObjVisible("projecter4", true);
    result = pVAOManager->setInstanceObjScale("projecter4", 5.f);
    result = pVAOManager->setUseRGBColorFlag("projecter4", false);
    result = pVAOManager->setInstanceObjPosition("projecter4", glm::vec4(7.5f, 2.5f, -15.f, 1.f));

    result = pVAOManager->setInstanceObjPosition("orb", glm::vec4(-12.f, 2.5f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjScale("orb", 2.5);
    result = pVAOManager->setInstanceObjRGB("orb", glm::vec4(1.f, 1.f, 1.f, 0.3f));
    result = pVAOManager->setIslandModelFlag("orb", true);
    result = pVAOManager->setInstanceObjVisible("orb", true);

    result = pVAOManager->setInstanceObjScale("barrel1", 0.05);
    result = pVAOManager->setInstanceObjVisible("barrel1", true);
    result = pVAOManager->setInstanceObjRGB("barrel1", glm::vec4(1.f, 1.f, 1.f, 1.f));
    result = pVAOManager->setTexture("barrel1", "Dungeons_2_Texture_01_A.bmp", 0);
    result = pVAOManager->setInstanceObjLighting("barrel1", false);
    //result = pVAOManager->setInstanceObjPosition("barrel1", glm::vec4(-12.5f, 2.5f, -15.f, 1.f));

    result = pVAOManager->setInstanceObjPosition("boss", glm::vec4(-2.3f, 1.f, 0.f, 1.f));
    //result = pVAOManager->set("boss", glm::vec4(-2.3f, 1.f, 0.f, 1.f));
    g_MeshBoss = pVAOManager->findMeshObjAddr("boss");
    light0Setup(); // Dir light
    light1Setup(pVAOManager);// torch
    light2Setup(pVAOManager); //beholder eye
    //light3Setup();
    //light4Setup();



    while (!glfwWindowShouldClose(window))
    {
        ::g_pTheLightManager->setLightToShader(shaderID);

        float ratio;
        int width, height;
        //glm::mat4x4 matModel;
        glm::mat4x4 matProjection;
        glm::mat4x4 matView;

        //glfwGetFramebufferSize(window, &width, &height);
        //ratio = width / (float)height;

        //glViewport(0, 0, width, height);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        setFBO2(pShaderManager, pVAOManager);
        setFBOPortal(::g_FBO_03, pShaderManager, pVAOManager, glm::vec3(-2.5f, 2.5f, -15.f), glm::vec3(-2.5f,0.f,0.f));
        //setFBOCubeMap(::g_FBO_04, pShaderManager, pVAOManager, glm::vec3(-12.f, 2.5f, 0.f));
        //g_cameraEye = glm::vec4(0.f);
        //g_cameraTarget = glm::vec4(200.f, 200.f, -100.f, 0.f);
        //////////////////////////////////////////////////////////////
        //FBO                                                       //
        //////////////////////////////////////////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, ::g_FBO_01->ID);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, ::g_FBO_01->width, ::g_FBO_01->height);
        ::g_FBO_01->clearBuffer(true, true);

        //glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(g_cameraEye - g_cameraTarget);
        
        glm::vec3 cameraRight = glm::normalize(glm::cross(g_upVector, cameraDirection));
        if (!bIsWalkAround)
        {
            g_cameraTarget = g_cameraTarget_defualt;
            matView = glm::lookAt(::g_cameraEye, g_cameraTarget, ::g_upVector);
        }
        else
        {
            g_cameraTarget = glm::length(g_cameraTarget - g_cameraEye) * g_cameraFront;
            matView = glm::lookAt(::g_cameraEye, g_cameraTarget, ::g_upVector);
        }
        GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");

        glUniform4f(eyeLocation_UniLoc, ::g_cameraEye.x, ::g_cameraEye.y, ::g_cameraEye.z, 1.0f);

        float FBO_screenRatio = static_cast<float>(::g_FBO_01->width) / ::g_FBO_01->height;
        matProjection = glm::perspective(glm::radians(fov), FBO_screenRatio, 0.1f, 10000.f);
        //matProjection = glm::perspective(glm::radians(fov), ratio, 0.1f, 10000.0f);
        ////matProjection = glm::perspective(0.6f, ratio, 0.1f, 10000.0f);
        ////glUniformMatrix4fv(mView_location, 1, GL_FALSE, glm::value_ptr(matView));
        ////glUniformMatrix4fv(mProjection_location, 1, GL_FALSE, glm::value_ptr(matProjection));

        pShaderManager->setShaderUniformM4fv("mView", matView);
        pShaderManager->setShaderUniformM4fv("mProjection", matProjection);
       
        setFBOtoTexture(g_FBO_02, pShaderManager, pVAOManager, "projecter2");
        setFBOtoTexture(g_FBO_03, pShaderManager, pVAOManager, "projecter3");
        updateInstanceObj(pShaderManager, pVAOManager);
        //setFBOtoTexture(g_FBO_04, pShaderManager, pVAOManager, "projecter4");

        //////////////////////////////////////////////////////////////
        //main buffer                                               //
        //////////////////////////////////////////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 oldEye = ::g_cameraEye;
        glm::vec3 oldAt = ::g_cameraTarget;
        glm::vec3 oldCamFront = ::g_cameraFront;

        ::g_cameraEye = glm::vec3(0.0f, 0.0f, 6.0f);
        ::g_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        ::g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        pShaderManager->setShaderUniform2f("FBO_width_height", g_FBO_01->width, g_FBO_01->height);
        pShaderManager->setShaderUniform2f("screen_width_height", (float)width, (float)height);
        matProjection = glm::perspective(glm::radians(fov), ratio, 0.1f, 10000.f);
        glViewport(0, 0, width, height);

        //if (!bIsWalkAround)
        {

            matView = glm::lookAt(::g_cameraEye, ::g_cameraTarget, ::g_upVector);
        }
        //else
        //{
        //    matView = glm::lookAt(::g_cameraEye, ::g_cameraEye + ::g_cameraFront, ::g_upVector);
        //}

        glUniform4f(eyeLocation_UniLoc, ::g_cameraEye.x, ::g_cameraEye.y, ::g_cameraEye.z, 1.0f);
        pShaderManager->setShaderUniformM4fv("mView", matView);

        pShaderManager->setShaderUniform1f("bFullScreen", true);
        //set FBO to texture
        GLuint texture21_Unit = 21;
        glActiveTexture(texture21_Unit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_FBO_01->vertexMaterialColorID);
        pShaderManager->setShaderUniform1i("sampler_FBO_vertexMaterialColour", texture21_Unit);

        GLuint texture22_Unit = 22;
        glActiveTexture(texture22_Unit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_FBO_01->vertexNormalID);
        pShaderManager->setShaderUniform1i("sampler_FBO_vertexNormal", texture22_Unit);

        GLuint texture23_Unit = 23;
        glActiveTexture(texture23_Unit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_FBO_01->vertexWorldPositionID);
        pShaderManager->setShaderUniform1i("sampler_FBO_vertexWorldPos", texture23_Unit);

        GLuint texture24_Unit = 24;
        glActiveTexture(texture24_Unit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_FBO_01->vertexSpecularID);
        pShaderManager->setShaderUniform1i("sampler_FBO_vertexSpecular", texture24_Unit);

        GLuint texture25_Unit = 25;
        glActiveTexture(texture25_Unit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_FBO_01->vertexRefractionID);
        pShaderManager->setShaderUniform1i("sampler_FBO_vertexRefraction", texture25_Unit);

        GLuint texture40_Unit = 40;
        glActiveTexture(texture40_Unit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_FBO_01->vertexCubeMapID);
        pShaderManager->setShaderUniform1i("sampler_FBO_vertexCubeMap", texture40_Unit);

        if(toggleblur)
        { 
            pShaderManager->setShaderUniform1f("blurAmount", 0.5f);
        }
        else
        {
            pShaderManager->setShaderUniform1f("blurAmount", 0.f);
        }
        glm::mat4 scrMAT = glm::mat4(1.f);
        cMeshObj* scrOBJ = pVAOManager->findMeshObjAddr("projecter1");
        result = pVAOManager->setInstanceObjScale("projecter1", 100.f);
        result = pVAOManager->setInstanceObjVisible("projecter1", true);
        drawObj(scrOBJ, scrMAT, pShaderManager, pVAOManager);
        result = pVAOManager->setInstanceObjVisible("projecter1", false);
       //updateInstanceObj(pShaderManager, pVAOManager);
#if USE_IMGUI
        gui_->ImGUICreateFrame();
#endif
        ::g_cameraEye = oldEye;
        ::g_cameraTarget = oldAt;
        ::g_cameraFront = oldCamFront;

        pShaderManager->setShaderUniform1f("bFullScreen", false);

        glfwSwapBuffers(window);
        glfwPollEvents();

        //set window title
        //std::stringstream 

    }

#if USE_IMGUI
    gui_->ImGUI_shutdown();
#endif

    glfwDestroyWindow(window);

    delete pVAOManager;
    delete pShaderManager;

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void updateInstanceObj(cShaderManager* pShaderManager, cVAOManager* pVAOManager)
{
    glm::mat4x4 matModel;

    for (std::map<std::string, cMeshObj* >::iterator itCurrentMesh = pVAOManager->mapInstanceNametoMeshObj.begin();
        itCurrentMesh != pVAOManager->mapInstanceNametoMeshObj.end();
        itCurrentMesh++)
    {
        cMeshObj* pCurrentMeshObject = (itCurrentMesh->second);        // * is the iterator access thing

        //pCurrentMeshObject->textureRatios[0] -= 0.001f;
        //pCurrentMeshObject->textureRatios[1] += 0.001f;

        if (!pCurrentMeshObject->isVisible)
        {
            // Skip the rest of the loop
            continue;
        }
        if (pCurrentMeshObject->instanceName == "boss")
        {
            g_pTheLightManager->plight[7]->position = glm::vec4(pCurrentMeshObject->position, 1) + glm::vec4(-0.4f, 1.4f, 0, 0);
            g_pTheLightManager->plight[8]->position = glm::vec4(pCurrentMeshObject->position, 1) + glm::vec4(0.7f, 1.2f, -0.3f, 0);
            g_pTheLightManager->plight[9]->position = glm::vec4(pCurrentMeshObject->position, 1) + glm::vec4(-2.5f, -0.2f, 0, 0);
            //std::cout << "Beholder position" << pCurrentMeshObject->position.x << ", " << pCurrentMeshObject->position.y << ", " << pCurrentMeshObject->position.z << std::endl;
            //std::cout << "cameraEye " << g_cameraEye.x << ", " << g_cameraEye.y << ", " << g_cameraEye.z << std::endl;
            //std::cout << "cameraTarget " << g_cameraTarget.x << ", " << g_cameraTarget.y << ", " << g_cameraTarget.z << std::endl;
            //std::cout << "cameraFront " << g_cameraFront.x << ", " << g_cameraFront.y << ", " << g_cameraFront.z << std::endl;
        }
        if (pCurrentMeshObject->isIslandModel)
        {
            pShaderManager->setShaderUniform1f("bIsIlandModel", (GLfloat)GL_TRUE);
        }
        if (pCurrentMeshObject->isSkybox)
        {
            pShaderManager->setShaderUniform1f("bIsSkyboxObject", (GLfloat)GL_TRUE);
            pCurrentMeshObject->position = ::g_cameraEye;
            pCurrentMeshObject->scale = 7500.f;
        }
        matModel = glm::mat4x4(1.0f);

#if MOVINGTEXTURE
        if (pCurrentMeshObject->isMovingTexture)
        {
            pShaderManager->setShaderUniform1f("bMovingTexture", (GLfloat)GL_TRUE);
            pShaderManager->setShaderUniform1f("time", pCurrentMeshObject->time);
        }
#endif

        drawObj(pCurrentMeshObject, matModel, pShaderManager, pVAOManager);

#if MOVINGTEXTURE
        pCurrentMeshObject->time += 0.01f;
#endif
        if (pCurrentMeshObject->isSkybox)
        {
            pShaderManager->setShaderUniform1f("bIsSkyboxObject", (GLfloat)GL_FALSE);
        }
        if (pCurrentMeshObject->isIslandModel)
        {
            pShaderManager->setShaderUniform1f("bIsIlandModel", (GLfloat)GL_FALSE);
        }
    }
#if MOVINGTEXTURE
    pShaderManager->setShaderUniform1f("bMovingTexture", (GLfloat)GL_FALSE);
#endif
}

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, GLFW_TRUE);
//
//    //move camera
//    // AWSD AD-Left, Right
//    //      WS-Forward, Back
//    const float CAMERA_MOVE_SPEED = 5.f;
//    if (key == GLFW_KEY_LEFT)
//    {
//        //::g_cameraEye.x -= CAMERA_MOVE_SPEED;
//        //::g_cameraEye += (glm::normalize(glm::cross(g_upVector, (::g_cameraFront * glm::vec3(1, 0, 1)) * CAMERA_MOVE_SPEED)));
//        g_MeshBoss->position.x -= 0.1;
//    }
//    if (key == GLFW_KEY_RIGHT)
//    {
//        //::g_cameraEye.x += CAMERA_MOVE_SPEED;
//        //::g_cameraEye -= (glm::normalize(glm::cross(g_upVector, (::g_cameraFront * glm::vec3(1, 0, 1)) * CAMERA_MOVE_SPEED)));
//        g_MeshBoss->position.x += 0.1;
//    }
//    if (key == GLFW_KEY_UP)
//    {
//        //::g_cameraEye.z -= CAMERA_MOVE_SPEED;
//        //::g_cameraEye += ((::g_cameraFront * glm::vec3(1, 0, 1)) * CAMERA_MOVE_SPEED);
//        g_MeshBoss->position.z -= 0.1;
//    }
//    if (key == GLFW_KEY_DOWN)
//    {
//        //::g_cameraEye.z += CAMERA_MOVE_SPEED;
//        //::g_cameraEye -= ((::g_cameraFront * glm::vec3(1, 0, 1)) * CAMERA_MOVE_SPEED);
//        g_MeshBoss->position.z += 0.1;
//    }
//    if (key == GLFW_KEY_A)
//    {
//        ::g_cameraEye += (glm::normalize(glm::cross(g_upVector, (::g_cameraFront * glm::vec3(1, 0, 1)) * CAMERA_MOVE_SPEED)));
//    }
//    if (key == GLFW_KEY_D)
//    {
//        ::g_cameraEye -= (glm::normalize(glm::cross(g_upVector, (::g_cameraFront * glm::vec3(1, 0, 1)) * CAMERA_MOVE_SPEED)));
//    }
//    if (key == GLFW_KEY_W)
//    {
//        ::g_cameraEye += ((::g_cameraFront * glm::vec3(1, 0, 1)) * CAMERA_MOVE_SPEED);
//    }
//    if (key == GLFW_KEY_S)
//    {
//        ::g_cameraEye -= ((::g_cameraFront * glm::vec3(1, 0, 1)) * CAMERA_MOVE_SPEED);
//    }
//    if (key == GLFW_KEY_Q)
//    {
//        ::g_cameraEye.y -= CAMERA_MOVE_SPEED;
//    }
//    if (key == GLFW_KEY_E)
//    {
//        ::g_cameraEye.y += CAMERA_MOVE_SPEED;
//    }
//    if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
//    {
//        toggleblur = !toggleblur;
//    }
//    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
//    {
//        //::g_cameraEye = glm::vec3(-5.5f, -3.4f, 15.0f);
//        //::g_cameraEye = glm::vec3(0.0, 100.0, 300.0f);
//        //::g_cameraTarget = glm::vec3(5.0f, 0.0f, 0.0f);
//        bIsWalkAround = !bIsWalkAround;
//
//    }
//}
//
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//    float xpos = static_cast<float>(xposIn);
//    float ypos = static_cast<float>(yposIn);
//
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//    if (bIsWalkAround)
//    {
//
//
//        float xoffset = xpos - lastX;
//        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//        lastX = xpos;
//        lastY = ypos;
//
//        float sensitivity = 0.1f; // change this value to your liking
//        xoffset *= sensitivity;
//        yoffset *= sensitivity;
//
//        yaw += xoffset;
//        pitch += yoffset;
//
//        // make sure that when pitch is out of bounds, screen doesn't get flipped
//        if (pitch > 89.0f)
//            pitch = 89.0f;
//        if (pitch < -89.0f)
//            pitch = -89.0f;
//
//        glm::vec3 front;
//        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//        front.y = sin(glm::radians(pitch));
//        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//        ::g_cameraFront = glm::normalize(front);
//    }
//    else
//    {
//        ::g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//    }
//}
//
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
//{
//    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
//    {
//        std::cout << "R click" << std::endl;
//    }
//    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//    {
//        std::cout << "L click" << std::endl;
//    }
//
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    fov -= (float)yoffset;
//    if (fov < 1.0f)
//        fov = 1.0f;
//    if (fov > 45.0f)
//        fov = 45.0f;
//}
//
//void window_size_callback(GLFWwindow* window, int width, int height)
//{
//    std::string error = "";
//    bool result = ::g_FBO_01->reset(width, height, error);
//    
//    if (!result)
//    {
//        std::cout << "Error: FBO cannot reset: " << error << std::endl;
//    }
//}

void setFBOPortal(cFBO* fbo, cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::vec3 eye, glm::vec3 target)
{
    glm::mat4x4 matProjection;
    glm::mat4x4 matView;

    GLuint shaderID = pShaderManager->getIDfromName("Shader01");

    //FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
    glViewport(0, 0, fbo->width, fbo->height);
    fbo->clearBuffer(true, true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //std::cout << "g_cameraEye" << g_cameraEye.x << " : " << g_cameraEye.y << " : " << g_cameraEye.z << std::endl;

        //glm::vec3 eyeTemp = glm::normalize(glm::cross(g_upVector, (glm::normalize(g_cameraEye-eye) )));
    glm::vec3 mirrorNorm = glm::normalize(target - eye);
    glm::vec3 eyeTemp = g_cameraEye - 2.f * glm::dot(g_cameraEye - eye, mirrorNorm) * mirrorNorm;
    glm::vec3 targetTemp = g_cameraTarget - 2.f * glm::dot(g_cameraTarget - eye, mirrorNorm) * mirrorNorm;
    matView = glm::lookAt(eyeTemp, targetTemp, ::g_upVector);
    //}
    //else
    //{
    //    matView = glm::lookAt(eye, eye + target, ::g_upVector);
    //}
    GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");

    glUniform4f(eyeLocation_UniLoc, eye.x, eye.y, eye.z, 1.0f);

    float FBO_screenRatio = static_cast<float>(fbo->width) / fbo->height;
    matProjection = glm::perspective(glm::radians(90.f), FBO_screenRatio, 0.1f, 10000.f);
    pShaderManager->setShaderUniformM4fv("mView", matView);
    pShaderManager->setShaderUniformM4fv("mProjection", matProjection);

    updateInstanceObj(pShaderManager, pVAOManager);

}

void setFBOtoTexture(cFBO* fbo, cShaderManager* pShaderManager, cVAOManager* pVAOManager, std::string projector)
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
    cMeshObj* scrOBJ = pVAOManager->findMeshObjAddr(projector);
    //bool result = pVAOManager->setInstanceObjScale("projecter1", 100.f);
    bool result = pVAOManager->setInstanceObjVisible(projector, true);
    pShaderManager->setShaderUniform1f("bMirror", true);
    drawObj(scrOBJ, scrMAT, pShaderManager, pVAOManager);
    result = pVAOManager->setInstanceObjVisible(projector, false);
    pShaderManager->setShaderUniform1f("bMirror", false);
    pShaderManager->setShaderUniform1f("bFullScreen", false);
}

void setFBOCubeMap(cFBO* fbo, cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::vec3 eye)
{
    glm::mat4x4 matProjection;
    glm::mat4x4 matView;

    GLuint shaderID = pShaderManager->getIDfromName("Shader01");

    //FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
    glViewport(0, 0, fbo->width, fbo->height);
    fbo->clearBuffer(true, true);
    for (int i = 0; i < 6; i++)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, fbo->ID, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //std::cout << "g_cameraEye" << g_cameraEye.x << " : " << g_cameraEye.y << " : " << g_cameraEye.z << std::endl;
        switch (GL_TEXTURE_CUBE_MAP_POSITIVE_X + i)
        {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
            matView = glm::lookAt(eye, eye + glm::vec3(10, 0, 0), glm::vec3(0, -1, 0));
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
            matView = glm::lookAt(eye, eye + glm::vec3(-10, 0, 0), glm::vec3(0, -1, 0));
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
            matView = glm::lookAt(eye, eye + glm::vec3(0, 10, 0), glm::vec3(0, 0, 1));
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
            matView = glm::lookAt(eye, eye + glm::vec3(0, -10, 0), glm::vec3(0, 0, -1));
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
            matView = glm::lookAt(eye, eye + glm::vec3(0, 0, 10), glm::vec3(0, -1, 0));
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            matView = glm::lookAt(eye, eye + glm::vec3(0, 0, -10), glm::vec3(0, -1, 0));
            break;
        default:
            break;
        }

        GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");

        glUniform4f(eyeLocation_UniLoc, eye.x, eye.y, eye.z, 1.0f);

        float FBO_screenRatio = static_cast<float>(fbo->width) / fbo->height;
        matProjection = glm::perspective(glm::radians(90.f), 1.f, 1.f, 10000.f);

        pShaderManager->setShaderUniformM4fv("mView", matView);
        pShaderManager->setShaderUniformM4fv("mProjection", matProjection);

        updateInstanceObj(pShaderManager, pVAOManager);
    }
}

void setFBOtoTextureCubeMap(cFBO* fbo, cShaderManager* pShaderManager, cVAOManager* pVAOManager, std::string projector)
{
    pShaderManager->setShaderUniform1f("bDynCubeMap", true);
    //set FBO to texture

    GLuint texture40_Unit = 40;
    glActiveTexture(texture40_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, fbo->vertexCubeMapID);
    pShaderManager->setShaderUniform1i("sampler_FBO_vertexCubeMap", texture40_Unit);

    //pShaderManager->setShaderUniform1f("blurAmount", 0.5f);
    glm::mat4 scrMAT = glm::mat4(1.f);
    cMeshObj* scrOBJ = pVAOManager->findMeshObjAddr(projector);
    //bool result = pVAOManager->setInstanceObjScale("projecter1", 100.f);
    bool result = pVAOManager->setInstanceObjVisible(projector, true);
    drawObj(scrOBJ, scrMAT, pShaderManager, pVAOManager);
    result = pVAOManager->setInstanceObjVisible(projector, false);
    pShaderManager->setShaderUniform1f("bDynCubeMap", false);
}

void setFBO2(cShaderManager* pShaderManager, cVAOManager* pVAOManager)
{
    glm::mat4x4 matProjection;
    glm::mat4x4 matView;

    GLuint shaderID = pShaderManager->getIDfromName("Shader01");

    //FBO
    glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_02->ID);
    glViewport(0, 0, g_FBO_02->width, g_FBO_02->height);
    g_FBO_02->clearBuffer(true, true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::vec3 eye = glm::vec3(-3.5f, 5.f, 11.f);
    //std::cout << "g_cameraEye" << g_cameraEye.x << " : " << g_cameraEye.y << " : " << g_cameraEye.z << std::endl;
    //if (!bIsWalkAround)
    //{
    matView = glm::lookAt(eye, g_cameraTarget, ::g_upVector);
    //}
    //else
    //{
    //    matView = glm::lookAt(eye, eye + target, ::g_upVector);
    //}
    GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");

    glUniform4f(eyeLocation_UniLoc, eye.x, eye.y, eye.z, 1.0f);

    float FBO_screenRatio = static_cast<float>(g_FBO_02->width) / g_FBO_02->height;
    matProjection = glm::perspective(glm::radians(90.f), FBO_screenRatio, 0.1f, 10000.f);
    pShaderManager->setShaderUniformM4fv("mView", matView);
    pShaderManager->setShaderUniformM4fv("mProjection", matProjection);

    glm::mat4 scrMAT = glm::mat4(1.f);
    cMeshObj* scrOBJ = pVAOManager->findMeshObjAddr("barrel1");
    pShaderManager->setShaderUniform1f("blurAmount", 0.f);
    bool result = pVAOManager->setInstanceObjVisible("barrel1", true);
    drawObj(scrOBJ, scrMAT, pShaderManager, pVAOManager);
    result = pVAOManager->setInstanceObjVisible("barrel1", false);
    
    //updateInstanceObj(pShaderManager, pVAOManager);
}