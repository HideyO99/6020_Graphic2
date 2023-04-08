#include "TV.h"

TV::TV()
{
	currentChannel = 1;
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
