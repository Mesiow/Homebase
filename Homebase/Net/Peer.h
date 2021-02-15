#pragma once
#include <Thor/Math.hpp>
#include "Multiplayer.h"

class Peer {
public:
	Peer();
	~Peer();

private:
	void initSocket();

private:
	Peer_t _id;
	Multiplayer _multiplayer;
};