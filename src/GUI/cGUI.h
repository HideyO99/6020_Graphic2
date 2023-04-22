#pragma once
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <map>
#include <string>
#include "../MeshObj/cMeshObj.h"
#include "../Light/cLightManager.h"
#include <vector>
#include "../cBeholder.h"
#include "../Monster/Monster.h"

class cGUI
{
public:
	cGUI(glm::vec3* camPos, glm::vec3* camTar);
	~cGUI();

	bool ImGUI_init(GLFWwindow* window);
	void ImGUI_shutdown();
	bool ImGUICreateFrame();
	bool ImGUI_render();
	bool ImGUI_render2();
	bool ImGUI_update();

	std::vector<cMeshObj*>* pVecInstanceMeshObj;
	cLight* pLight[MAX_LIGHT_SOURCE];
	glm::vec3* camPos;
	glm::vec3* camTar;
	std::vector<Monster*>* vecMonster;
};

