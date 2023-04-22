#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include "cAI_Map.h"
#include <glm/glm.hpp>
#include "../VAOManager/cVAOManager.h"

struct PathNode
{
	PathNode(std::string name)
		: name(name)
		, parent(nullptr)
		, f(std::numeric_limits<float>::infinity())
		, g(std::numeric_limits<float>::infinity())
		, h(std::numeric_limits<float>::infinity())
	{
	}

	glm::vec3 point;
	PathNode* parent;
	int Id;
	std::string name;
	float distanceFromStart;
	std::vector<PathNode*> neighbours;

	float f;
	float g;
	float h;

	bool operator() (const PathNode* l, const PathNode* r) const
	{
		return l->distanceFromStart > r->distanceFromStart;
	}

	PathNode() : name("") {}
};

struct Graph
{
	void SetNeighbours(int a, int b);
	PathNode* getNodeByName(std::string name);

	std::vector<PathNode*> nodes;
};

class AI_path
{
public:
	AI_path();
	~AI_path();

	void loadBITMAP(std::string fileName);
	void createGraph(cVAOManager* pVAOManager);
	PathNode* createPathNode(const glm::vec3& pos, std::string name);
	void checkAndCreateNeighbour();

	bool BreadthFirstSearch(Graph* graph, PathNode* start, PathNode* end);
	bool DepthFirstSearch(Graph* graph, PathNode* start, PathNode* end);
	bool DijkstraSearch(Graph* graph, PathNode* start, PathNode* end);
	bool AStarSearch(Graph* graph, PathNode* start, PathNode* end);

	void findPath();
	
	glm::vec3 getStartPos();
	std::vector<glm::vec3> path_pos;

private:
	std::vector<PathNode*>::iterator lowestFValue(std::vector<PathNode*>& openList);
	float heuristic(PathNode* nodeA, PathNode* nodeB);
	void reconstructPath(PathNode* goal);

	cAI_Map* m_theMap;
	Graph m_Graph;

	std::vector<PathNode*> m_path;

	//unsigned int m_OpenMaterialId;
	//unsigned int m_ClosedMaterialId;
	//unsigned int m_UnvisitedMaterialId;
	//unsigned int m_FoundPathMaterialId;

	std::vector<PathNode*> m_OpenSet;
	std::vector<PathNode*> m_ClosedSet;
	PathNode* m_StartNode;
	PathNode* m_EndNode;

};

