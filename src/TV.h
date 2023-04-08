#pragma once
#include "MeshObj/cMeshObj.h"
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
	unsigned int currentChannel;
	bool isPwrOn;
	cMeshObj* meshBody;
	cMeshObj* meshScreen;

	double lastTurnOn;

private:
};

