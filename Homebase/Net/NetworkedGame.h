#pragma once
#include <Game/Net/Net.h>
#include "../Entities/Player.h"
#include "../Entities/Planet.h"
#include "Packets.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <array>

#define MAX_CONNECTIONS 4

struct PlayerData {
	float x, y;
	float dx, dy;
	Peer_t id;
};


class Game;

/*
	Struct that renders all networked entities contains functions used
	in the multiplayer environment
*/

class NetworkedGame {
public:
	void render(sf::RenderTarget& target);
	void update(Peer_t id, float dt);
	void handleInput(Peer_t id, float dt);
	void handleEvents(Peer_t id, sf::Event& ev);

	/*
	  Shoot from a networked player's location using its' data
	*/
	void shoot(const PlayerData &data);
	void setupPlanetLocations();

	/*
	  Add new player to the players list
	*/
	void add(Peer_t id, const EndPoint &endPoint);
	void add(Peer_t id);
	bool isConnected(Peer_t id)const;
	int getFreeSlot()const;
	int peersConnected()const;
	/* Returns id's of already connected players */
	std::vector<int> getAlreadyConnectedSlots()const;
	const EndPoint& getEndPoint(Peer_t id)const;
	Player& getPlayerById(Peer_t id);

	/*
		Setup all networked game entities
	*/
	void setup();
	/*
		Clears memory from all arrays
	*/
	void zero();


	std::array<std::unique_ptr<EndPoint>, MAX_CONNECTIONS> peers;
	std::array<std::unique_ptr<Player>, MAX_CONNECTIONS> players;
	std::array<bool, MAX_CONNECTIONS> connects;
	std::vector<Planet> planets;

	Game* game;
};