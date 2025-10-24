#include "enet/types.h"
#include <cstddef>
#include <enet/enet.h>
#include <iostream>

namespace Fnet {

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

} // namespace Fnet

#ifdef FNET_IMPL

namespace Fnet {

ENetAddress Address;
ENetEvent Event;
ENetHost *Host = nullptr;
ENetPeer *Peer = nullptr;

void CreateServer(enet_uint16 port, size_t MaxClients, size_t ChannelsCount) {

  if (enet_initialize() != 0) {
    std::cerr << "FNET: Failed initialization.\n";
    return;
  }

  Address.host = ENET_HOST_ANY;
  Address.port = port;

  Host = enet_host_create(&Address, MaxClients, ChannelsCount, 0, 0);

  if (!Host) {
    std::cerr << "FNET: Failed to create server host.\n";
    return;
  }

  std::cout << "FNET: Server running on port" << Address.port << ".\n";
}

void JoinServer(const char *hostName, enet_uint16 port, size_t ChannelsCount) {
  if (enet_initialize() != 0) {
    std::cerr << "FNET: Failed initialization.\n";
    return;
  }

  Host = enet_host_create(NULL, 1, ChannelsCount, 0, 0);
  if (!Host) {
    std::cerr << "FNET: Failed to create client host.\n";
    return;
  }

  enet_address_set_host(&Address, hostName);
  Address.port = port;

  Peer = enet_host_connect(Host, &Address, ChannelsCount, 0);
  if (!Peer) {
    std::cerr << "FNET: No available peers.\n";
    return;
  }

  std::cout << "FNET: Client running on port" << Address.port << ".\n";
}

void SendPacket(ENetPeer *peer, const void *data, size_t size,
                enet_uint8 channel,
                enet_uint32 flags) {
  if (!peer || !Host)
    return;

  ENetPacket *packet = enet_packet_create(data, size, flags);
  enet_peer_send(peer, channel, packet);
  enet_host_flush(Host);
}

int PollEvents(auto timeout) {
  if (Event.packet)
    enet_packet_destroy(Fnet::Event.packet);
  return enet_host_service(Fnet::Host, &Fnet::Event, timeout);
}

void Stop() {
  if (Host)
    enet_host_destroy(Host);
  enet_deinitialize();
}

} // namespace Fnet
#endif