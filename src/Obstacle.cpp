#include "Obstacle.h"

#include "TextureManager.h"

Obstacle::Obstacle()
{
	TextureManager::Instance()->load("../Assets/textures/obstacle.png", "obstacle");

	const auto size = TextureManager::Instance()->getTextureSize("obstacle");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(OBSTACLE);
}

Obstacle::~Obstacle()
= default;

void Obstacle::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("obstacle", x, y, 0, 255, true);
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
}
