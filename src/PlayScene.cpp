#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();

	if(m_bDebugMode)
	{
		auto LOSColour = (!m_bPlayerHasLOS) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

		Util::DrawLine(m_pPlayer->getTransform()->position, m_pPlaneSprite->getTransform()->position, LOSColour);

		Util::DrawRect(m_pPlayer->getTransform()->position - glm::vec2(m_pPlayer->getWidth() * 0.5f, m_pPlayer->getHeight() *0.5f),
			m_pPlayer->getWidth(), m_pPlayer->getHeight());

		Util::DrawRect(m_pPlaneSprite->getTransform()->position - glm::vec2(m_pPlaneSprite->getWidth() * 0.5f, m_pPlaneSprite->getHeight() * 0.5f),
			m_pPlaneSprite->getWidth(), m_pPlaneSprite->getHeight());

		Util::DrawRect(m_pObstacle->getTransform()->position - glm::vec2(m_pObstacle->getWidth() * 0.5f, m_pObstacle->getHeight() * 0.5f),
			m_pObstacle->getWidth(), m_pObstacle->getHeight());

		m_displayGrid();

		//m_displayGridLOS();
	}
}

void PlayScene::update()
{
	updateDisplayList();

	m_bPlayerHasLOS = CollisionManager::LOSCheck(m_pPlayer, m_pPlaneSprite, m_pObstacle);

	CollisionManager::AABBCheck(m_pPlayer, m_pPlaneSprite);

	CollisionManager::AABBCheck(m_pPlayer, m_pObstacle);

	m_setGridLOS();

	if (m_bPatrolMode) {
		seeking(i);
		if (m_pPathPatrol[i]->getTransform()->position == m_pPlaneSprite->getTransform()->position)
		{
			i++;
			if (i == 4) {
				i = 0;
			}
		}
	}
	std::cout << m_angle << "\n";
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;

				m_pPlayer->getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
				m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
				m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;

				m_pPlayer->getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
				m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
				m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;

			m_pPlayer->getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;

			m_pPlayer->getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}

	// H KEY Section
	
	if(!m_bDebugKeys[H_KEY])
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
		{
			// toggles Debug Mode
			m_bDebugMode = !m_bDebugMode;

			m_bDebugKeys[H_KEY] = true;

			if (m_bDebugMode)
			{
				std::cout << "DEBUG Mode On" << std::endl;
				m_pPlaneSprite->update();
			
			}
			else
			{
				std::cout << "DEBUG Mode Off" << std::endl;
			}
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_H))
	{
		m_bDebugKeys[H_KEY] = false;
	}

	// K KEY Section
	
	if (!m_bDebugKeys[K_KEY])
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K))
		{
			std::cout << "DEBUG: Enemies taking damage!" << std::endl;

			m_bDebugKeys[K_KEY] = true;
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_K))
	{
		m_bDebugKeys[K_KEY] = false;
	}

	// P KEY Section

	if (!m_bDebugKeys[P_KEY])
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_P))
		{
			// toggles Patrol Mode
			m_bPatrolMode = !m_bPatrolMode;

			m_bDebugKeys[P_KEY] = true;

			if (m_bPatrolMode)
			{
				std::cout << "DEBUG: Patrol Mode On" << std::endl;
			
			}
			else
			{
				std::cout << "DEBUG: Patrol Mode Off" << std::endl;

			}
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_P))
	{
		m_bDebugKeys[P_KEY] = false;
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::m_buildGrid()
{
	// Logic to add PathNodes to the scene
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			auto pathNode = new PathNode();
			pathNode->getTransform()->position = glm::vec2(pathNode->getWidth() * col + Config::TILE_SIZE * 0.5, pathNode->getHeight() * row + Config::TILE_SIZE * 0.5);
			m_pGrid.push_back(pathNode);
		}
	}

	std::cout << "Number of Nodes: " << m_pGrid.size() << std::endl;
}

void PlayScene::m_displayGrid()
{
	// Logic to add PathNodes to the scene
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			auto Colour = (m_pGrid[row * Config::COL_NUM + col]->getLOS()) ? glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) : glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

			Util::DrawRect(m_pGrid[row * Config::COL_NUM + col]->getTransform()->position - glm::vec2(m_pGrid[row * Config::COL_NUM + col]->getWidth() * 0.5f, m_pGrid[row * Config::COL_NUM + col]->getHeight() * 0.5f),
				Config::TILE_SIZE, Config::TILE_SIZE);

			Util::DrawRect(m_pGrid[row * Config::COL_NUM + col]->getTransform()->position,
				5, 5, Colour);
		}
	}
}

void PlayScene::m_displayGridLOS()
{
	for (auto node : m_pGrid)
	{
		if(!node->getLOS())
		{
			auto colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

			Util::DrawLine(node->getTransform()->position, m_pPlayer->getTransform()->position, colour);
		}
		
	}
}

void PlayScene::m_setGridLOS()
{
	for (auto node : m_pGrid)
	{
		node->setLOS(CollisionManager::LOSCheck(node, m_pPlayer, m_pObstacle));
	}
}

void PlayScene::m_pBuildPratrolPath()
{
	//move left
	/*for (auto i = 0; i < Config::COL_NUM; i++) {
		m_pPathPatrol.push_back(m_pGrid[i]);
	}
	//move down
	for (auto i = 1; i < Config::ROW_NUM; i++) {
		m_pPathPatrol.push_back(m_pGrid[Config::COL_NUM * i]);
	}
	//move right
	for (auto i = 1; i < Config::COL_NUM; i++) {
		m_pPathPatrol.push_back(m_pGrid[Config::COL_NUM - i]);
	}
	//move up
	for (auto i = 1; i < Config::ROW_NUM; i++) {
		m_pPathPatrol.push_back(m_pGrid[Config::ROW_NUM - i]);
	}*/
	m_pPathPatrol.push_back(m_pGrid[1]);
	m_pPathPatrol.push_back(m_pGrid[8]);
	m_pPathPatrol.push_back(m_pGrid[58]);
	m_pPathPatrol.push_back(m_pGrid[51]);
}

void PlayScene::m_PrintPath()
{
	for (auto i : m_pPathPatrol) {
		std::cout << i->getTransform()->position.x << " "<< i->getTransform()->position.y << "\n";
	}
}

void PlayScene::seeking(int i)
{
	float dx = m_pPathPatrol[i]->getTransform()->position.x - m_pPlaneSprite->getTransform()->position.x;
	float dy = m_pPathPatrol[i]->getTransform()->position.y - m_pPlaneSprite->getTransform()->position.y;
	velx = m_pPlaneSprite->getRigidBody()->velocity;
	m_angle = atan2(dy,dx);
	m_pPlaneSprite->getTransform()->position += glm::vec2((5) * cos(m_angle), (5) * sin(m_angle));
}


void PlayScene::start()
{
	i = 0;
	m_bPlayerHasLOS = false;

	m_buildGrid();

	m_pBuildPratrolPath();
	m_PrintPath();
	
	m_bDebugMode = false;
	m_bPatrolMode = false;
	
	// Plane Sprite
	m_pPlaneSprite = new Plane();
	m_pPlaneSprite->getTransform()->position = m_pPathPatrol[0]->getTransform()->position;
	addChild(m_pPlaneSprite);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Obstacle Texture
	m_pObstacle = new Obstacle();
	addChild(m_pObstacle);

}


