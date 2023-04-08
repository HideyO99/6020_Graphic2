#pragma once
#include "MeshObj/cMeshObj.h"
#include "Shader/cShaderManager.h"
#include "FBO/cFBO.h"
#include "VAOManager/cVAOManager.h"
#include "timer.h"

class TV
{
public:
	TV();
	~TV();

	//TV(int id);

	int ID;
	void setup(glm::vec4 color, cShaderManager* shader, cVAOManager* VAO, int ID);
	void TurnOn();
	void TurnOff();
	void incCHN();
	void decCHN();
	void render();
	void recieveSignal(cFBO* fbo, cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::vec3 eye, glm::vec3 target);
	void update();
	bool isPwrOn;
	cMeshObj* meshBody;
	cMeshObj* meshScreen;
	cShaderManager* pShaderManager;
	cVAOManager* pVAOManager;

	double lastTurnOn;

	unsigned int m_currentChannel;
private:
	double m_CurrentTime;
	double m_lastCall;
	Timer* m_timer;
	bool isStaticCHN();
	void setupFBO2Texture(cFBO* fbo, cShaderManager* pShaderManager);
};

