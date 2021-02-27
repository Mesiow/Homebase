#include "NetworkedGame.h"
#include <Game/ResourceManager/ResourceManager.h>
#include <Game/Game.h>

void NetworkedGame::render(sf::RenderTarget& target)
{
	for (size_t i = 0; i < MAX_CONNECTIONS; ++i) {
		if (connects[i]) {
			if (players[i])
				players[i]->render(target);
		}
	}

	for (auto& planet : planets) planet.render(target);
}

void NetworkedGame::update(Peer_t id, float dt)
{
	if(connects[id])
		players[id]->update(game->getWindowHandle(), dt);

	for (auto& planet : planets) planet.update(game->getWindowHandle(), dt);
}

void NetworkedGame::handleInput(Peer_t id, float dt)
{
	if (connects[id])
		players[id]->handleInput(dt);
}

void NetworkedGame::handleEvents(Peer_t id, sf::Event& ev)
{
	if (connects[id])
		players[id]->handleEvents(ev);
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

void NetworkedGame::add(Peer_t id, const EndPoint &endPoint)
{
	connects[id] = true;
	peers[id] = std::make_unique<EndPoint>(endPoint);

	switch (id) {
		case 0: players[id] = std::make_unique<Player>(game, ResourceManager::getTexture("Player1")); break; //Host 
		case 1: players[id] = std::make_unique<Player>(game, ResourceManager::getTexture("Player2")); break;
		case 2: players[id] = std::make_unique<Player>(game, ResourceManager::getTexture("Player3")); break;
		case 3: players[id] = std::make_unique<Player>(game, ResourceManager::getTexture("Player4")); break;
	}
	
}

bool NetworkedGame::isConnected(Peer_t id) const
{
	return connects[id];
}

int NetworkedGame::getFreeSlot() const
{
	for (int i = 0; i < MAX_CONNECTIONS; ++i) {
		if (!connects[i]) return i;
	}
	return -1;
}

int NetworkedGame::peersConnected() const
{
	int count = 0;
	for (int i = 0; i < MAX_CONNECTIONS; ++i) {
		if (connects[i]) count++;
	}
	return count;
}

std::vector<int> NetworkedGame::getAlreadyConnectedSlots() const
{
	std::vector<int> connected_slots;
	connected_slots.clear();
	for (int i = 0; i < MAX_CONNECTIONS; ++i) {
		if (connects[i]) {
			connected_slots.push_back(i);
		}
	}
	return connected_slots;
}

const EndPoint& NetworkedGame::getEndPoint(Peer_t id) const
{
	return *peers[id];
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
	std::fill(connects.begin(), connects.end(), false);
}
