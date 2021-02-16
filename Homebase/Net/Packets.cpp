#include "Packets.h"

sf::Packet& operator<<(sf::Packet& packet, ePacket pack)
{
    packet << (Packet_t)pack;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, ePacket& pack)
{
    Packet_t p = (Packet_t)pack;
    packet >> p;
    pack = (ePacket)p;
    return packet;
}
