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

sf::Packet& operator<<(sf::Packet& packet, eCommandToHost command)
{
    packet << (Command_t)command;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, eCommandToHost& command)
{
    Command_t c = (Command_t)command;
    packet >> c;
    command = (eCommandToHost)c;
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, eCommandToPeer command)
{
    packet << (Command_t)command;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, eCommandToPeer& command)
{
    Command_t c = (Command_t)command;
    packet >> c;
    command = (eCommandToPeer)c;
    return packet;
}
