#include <enet/enet.h>
#include <iostream>

namespace Fnet {

ENetAddress Address;
ENetEvent Event;
ENetHost *Host = nullptr;
ENetPeer *Peer = nullptr;

void CreateServer() {

  if (enet_initialize() != 0) {
    std::cerr << "FNET: Failed initialization.\n";
    return;
  }

  Address.host = ENET_HOST_ANY;
  Address.port = 7777;

  // upto 4 clients can join
  // 1 channel just for movement
  // 0, 0, for unlimited bandwith
  Host = enet_host_create(&Address, 4, 1, 0, 0);

  if (!Host) {
    std::cerr << "FNET: Failed to create server host.\n";
    return;
  }

  std::cout << "FNET: Server running on port" << Address.port << ".\n";
}

void JoinServer() {
  if (enet_initialize() != 0) {
    std::cerr << "FNET: Failed initialization.\n";
    return;
  }

  Host = enet_host_create(NULL, 1, 2, 0, 0);
  if (!Host) {
    std::cerr << "FNET: Failed to create client host.\n";
    return;
  }

  enet_address_set_host(&Address, "127.0.0.1");
  Address.port = 7777;

  Peer = enet_host_connect(Host, &Address, 1, 0);
  if (!Peer) {
    std::cerr << "FNET: No available peers.\n";
    return;
  }

  std::cout << "Running on port 7777.\n";
}

void SendPacket(ENetPeer *peer, const void *data, size_t size,
                enet_uint8 channel = 0,
                enet_uint32 flags = ENET_PACKET_FLAG_RELIABLE) {
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