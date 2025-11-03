#include "enet/enet.h"
#include <string>
#include <udp_discovery_peer.hpp>
#include <udp_discovery_peer_parameters.hpp>

namespace GameNet {

extern ENetAddress Address;
extern ENetEvent Event;
extern ENetHost *Host;
extern ENetPeer *Peer;

void CreateServer(enet_uint16 port = 7777, size_t MaxClients = 4,
                  size_t ChannelsCount = 1);

void JoinServer(const char *hostName = "127.0.0.1", enet_uint16 port = 7777,
                size_t ChannelsCount = 1);

void SendPacket(ENetPeer *peer, const void *data, size_t size,
                enet_uint8 channel = 0,
                enet_uint32 flags = ENET_PACKET_FLAG_RELIABLE);

int PollEvents(auto timeout);

void Stop();

} // namespace GameNet

namespace GameLAN {

using DPeer = udpdiscovery::DiscoveredPeer;
extern udpdiscovery::PeerParameters params;

void InitServer(const std::string &user_data);
void InitClient();
bool IsServer();
std::list<DPeer> DiscoverAsClient();
} // namespace GameLAN

// #define GAMENET_IMPL
#ifdef GAMENET_IMPL
#undef GAMENET_IMPL
#include <iostream>
namespace GameNet {

ENetAddress Address;
ENetEvent Event;
ENetHost *Host = nullptr;
ENetPeer *Peer = nullptr;

void CreateServer(enet_uint16 port, size_t MaxClients, size_t ChannelsCount) {

  if (enet_initialize() != 0) {
    std::cerr << "GAMENET: Failed initialization.\n";
    return;
  }

  Address.host = ENET_HOST_ANY;
  Address.port = port;

  Host = enet_host_create(&Address, MaxClients, ChannelsCount, 0, 0);

  if (!Host) {
    std::cerr << "GAMENET: Failed to create server host.\n";
    return;
  }

  std::cout << "GAMENET: Server running on port" << Address.port << ".\n";
}

void JoinServer(const char *hostName, enet_uint16 port, size_t ChannelsCount) {
  if (enet_initialize() != 0) {
    std::cerr << "GAMENET: Failed initialization.\n";
    return;
  }

  Host = enet_host_create(NULL, 1, ChannelsCount, 0, 0);
  if (!Host) {
    std::cerr << "GAMENET: Failed to create client host.\n";
    return;
  }

  enet_address_set_host(&Address, hostName);
  Address.port = port;

  Peer = enet_host_connect(Host, &Address, ChannelsCount, 0);
  if (!Peer) {
    std::cerr << "GAMENET: No available peers.\n";
    return;
  }

  std::cout << "GAMENET: Client running on port" << Address.port << ".\n";
}

void SendPacket(ENetPeer *peer, const void *data, size_t size,
                enet_uint8 channel, enet_uint32 flags) {
  if (!peer || !Host)
    return;

  ENetPacket *packet = enet_packet_create(data, size, flags);
  enet_peer_send(peer, channel, packet);
  enet_host_flush(Host);
}

int PollEvents(auto timeout) {
  if (Event.packet) {
    enet_packet_destroy(GameNet::Event.packet);
    Event.packet = nullptr;
  }
  if (!GameNet::Host) {
    std::cerr << "GAMENET: Polling events without initializaiton!.\n";
    return -1;
  } else
    return enet_host_service(GameNet::Host, &GameNet::Event, timeout);
}

void Stop() {
  if (Host)
    enet_host_destroy(Host);
  enet_deinitialize();
}

} // namespace GameNet

namespace GameLAN {

udpdiscovery::Peer peer;
udpdiscovery::PeerParameters params;
bool isServer = false;

void InitServer(const std::string &user_data) {

  params.set_port(12021); // discovery port
  params.set_application_id(7681412);
  params.set_can_discover(false);     // Can't Discover (only need broadcast)
  params.set_can_be_discovered(true); // Can be discovered
  params.set_can_use_broadcast(true);

  peer.Start(params, user_data);

  isServer = true;

  std::cout << "GAMELAN: Initialized Server.\n";
}

void InitClient() {

  params.set_port(12021);
  params.set_application_id(7681412);
  params.set_can_discover(true); // Can discover (searches servers)
  params.set_can_be_discovered(
      false); // Can't be discovered (nor server discovers)

  peer.Start(params, ""); // Server doesn't care so why put user_data 🥀

  isServer = false;

  std::cout << "GAMELAN: Initialized Client.\n";
}

bool IsServer() { return isServer; }

std::list<DPeer> DiscoverAsClient() {
  return peer.ListDiscovered();
}
} // namespace GameLAN
#endif