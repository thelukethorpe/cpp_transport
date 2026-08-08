#ifndef FILTHYRAT_TRANSPORT_TCP_CLIENT_TRANSPORT_H
#define FILTHYRAT_TRANSPORT_TCP_CLIENT_TRANSPORT_H

#include "transport.h"
#include <cstdint>

namespace filthyrat
{

class TcpClientTransport : public ClientTransport
{
  public:
    struct Args
    {
        std::array<std::byte, 4> server_ip;
        std::uint16_t server_port;
    };

    TcpClientTransport(const Args &args, ClientTransportCallbacks callbacks);

    ~TcpClientTransport() override;

    TcpClientTransport(const TcpClientTransport &) = delete;
    TcpClientTransport &operator=(const TcpClientTransport &) = delete;

    TcpClientTransport(TcpClientTransport &&) = delete;
    TcpClientTransport &operator=(TcpClientTransport &&) = delete;

    void send(std::span<const std::byte> data) override;
    std::vector<std::byte> receive() override;
    void disconnect() override;

    void tick();

    [[nodiscard]] ClientConnectionId connectionId() const noexcept
    {
        // TODO
        return 0;
        // return client_server_pair_.client_id;
    }

  private:
    const ClientTransportCallbacks callbacks_;
    bool is_connected_ = true;
    int socket_fd_ = -1;

    void disconnectImpl();
};

} // namespace filthyrat

#endif // FILTHYRAT_TRANSPORT_TCP_CLIENT_TRANSPORT_H
