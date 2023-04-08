#pragma once
#include "MeshObj/cMeshObj.h"
#include "Shader/cShaderManager.h"
#include "FBO/cFBO.h"
#include "VAOManager/cVAOManager.h"


class TV
{
public:
	TV();
	~TV();

	void setup(glm::vec4 color, cMeshObj* body, cMeshObj* screen);
	void TurnOn();
	void TurnOff();
	void incCHN();
	void decCHN();
	void render(cShaderManager* pShaderManager, cVAOManager* pVAOManager);
	void recieveSignal(cFBO* fbo, cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::vec3 eye, glm::vec3 target);
	bool isPwrOn;
	cMeshObj* meshBody;
	cMeshObj* meshScreen;

	double lastTurnOn;

private:
	unsigned int m_currentChannel;
	bool isStaticCHN();
	void setupFBO2Texture(cFBO* fbo, cShaderManager* pShaderManager);
};

