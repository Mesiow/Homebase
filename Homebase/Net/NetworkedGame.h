#pragma once
#include <Game/Net/Net.h>
#include "../Entities/Player.h"
#include "Packets.h"
#include <algorithm>
#include <memory>
#include <array>

#define MAX_CONNECTIONS 4

struct PlayerData {
	float x, y;
	float dx, dy;
	Peer_t id;
};

/*
	Struct that renders all networked entities contains functions used
	in the multiplayer environment
*/

struct NetworkedGame {

	void render(sf::RenderTarget& target);

	/*
	  Shoot from a networked player's location using its' data
	*/
	void shoot(const PlayerData &data);

	/*
	  Add new player to the players list
	*/
	void add(Peer_t id);

	Player& getPlayerById(Peer_t id);

	/*
		Clears memory from all arrays
	*/
	void zero();


	std::array<std::unique_ptr<EndPoint>, MAX_CONNECTIONS> peers;
	std::array<std::unique_ptr<Player>, MAX_CONNECTIONS> players;
};