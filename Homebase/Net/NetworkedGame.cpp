#include "NetworkedGame.h"
#include <Game/ResourceManager/ResourceManager.h>
#include <Game/Game.h>

void NetworkedGame::render(sf::RenderTarget& target)
{
	for (size_t i = 0; i < MAX_CONNECTIONS; ++i) {
		if(players[i])
			players[i]->render(target);
	}

	for (auto& planet : planets) planet.render(target);
}

void NetworkedGame::update(Peer_t id, float dt)
{
	players[id]->update(game->getWindowHandle(), dt);

	for (auto& planet : planets) planet.update(game->getWindowHandle(), dt);
}

void NetworkedGame::shoot(const PlayerData& data)
{
	players[data.id]->shoot(data.x, data.y, data.dx, data.dy);
}

void NetworkedGame::setupPlanetLocations()
{
	Planet moon(Planet::Type::Moon, 200, 400);
	moon.loadTexture(ResourceManager::getTexture("Moon1"));

	Planet terran(Planet::Type::Planet, 500, 600);
	terran.loadTexture(ResourceManager::getTexture("Terran_Cloudy"));

	Planet redgiant(Planet::Type::Planet, 1200, -500);
	redgiant.loadTexture(ResourceManager::getTexture("Gas_Giant_Red"));

	planets.emplace_back(moon);
	planets.emplace_back(terran);
	planets.emplace_back(redgiant);
}

void NetworkedGame::add(Peer_t id)
{
	players[id] = std::make_unique<Player>();
}

Player& NetworkedGame::getPlayerById(Peer_t id)
{
	return *players[id];
}

void NetworkedGame::setup()
{
	zero();

	//Setup entities
	setupPlanetLocations();
}

void NetworkedGame::zero()
{
	std::fill(peers.begin(), peers.end(), nullptr);
	std::fill(players.begin(), players.end(), nullptr);
}
