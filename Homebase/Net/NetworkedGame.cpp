#include "NetworkedGame.h"

void NetworkedGame::render(sf::RenderTarget& target)
{
	for (size_t i = 0; i < MAX_CONNECTIONS; ++i) {
		if(players[i])
			players[i]->render(target);
	}
}

void NetworkedGame::shoot(const PlayerData& data)
{
	
}

void NetworkedGame::add(Peer_t id)
{
	players[id] = std::make_unique<Player>();
}

Player& NetworkedGame::getPlayerById(Peer_t id)
{
	return *players[id];
}

void NetworkedGame::zero()
{
	std::fill(peers.begin(), peers.end(), nullptr);
	std::fill(players.begin(), players.end(), nullptr);
}
