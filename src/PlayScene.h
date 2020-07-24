#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "DebugKeys.h"
#include "Obstacle.h"
#include "PathNode.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// PRIVATE FUNCTIONS
	void m_buildGrid();

	void m_displayGrid();

	void m_displayGridLOS();

	void m_setGridLOS();

	void m_pBuildPratrolPath();

	void m_PrintPath();

	void seeking(int i);


	// MEMBER VARIABLES
	glm::vec2 m_mousePosition;

	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	bool m_playerFacingRight;
	bool m_bPlayerHasLOS;

	Obstacle* m_pObstacle;

	bool m_bDebugMode;
	bool m_bPatrolMode;

	bool m_bDebugKeys[NUM_OF_DEBUG_KEYS];

	std::vector<PathNode*> m_pGrid;
	std::vector<PathNode*> m_pPathPatrol;

	float m_angle;
	int i;
	glm::vec2 velx;
	
};

#endif /* defined (__PLAY_SCENE__) */