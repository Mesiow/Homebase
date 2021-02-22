#pragma once
#include <SFML/Network/Packet.hpp>
#include <cstdint>

using Peer_t = uint8_t;
using Packet_t = uint8_t;
using Command_t = uint8_t;
using Command = bool;

constexpr static unsigned int commandIdentifier = 49049;

enum class eCommandToHost : Command_t {
	ConnectRequest = 0,
	DisconnectRequest,
	Broadcast
};

enum class eCommandToPeer : Command_t {
	ConnectResponse = 0,
	DisconnectResponse,
	BroadcastResponse
};

enum class ePacket : Packet_t {
	PeerState = 0, /* Position, Rotation */
	PeerDied,
	PeerShot
};

sf::Packet& operator<<(sf::Packet& packet, ePacket pack);
sf::Packet& operator>>(sf::Packet& packet, ePacket& pack);
sf::Packet& operator<<(sf::Packet& packet, eCommandToHost command);
sf::Packet& operator>>(sf::Packet& packet, eCommandToHost& command);
sf::Packet& operator<<(sf::Packet& packet, eCommandToPeer command);
sf::Packet& operator>>(sf::Packet& packet, eCommandToPeer& command);