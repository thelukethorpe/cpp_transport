#include <cstring>
#include <filthyrat/transport/tcp_client_transport.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace filthyrat
{

TcpClientTransport::TcpClientTransport(const Args &args, ClientTransportCallbacks callbacks)
    : callbacks_(std::move(callbacks)), socket_fd_(socket(AF_INET, SOCK_STREAM, 0))
{
    if (socket_fd_ < 0)
    {
        this->disconnectImpl();
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(args.server_port);
    std::memcpy(&server_addr.sin_addr.s_addr, args.server_ip.data(), args.server_ip.size());

    const auto connect_result =
        connect(socket_fd_, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr));
    if (connect_result < 0)
    {
        this->disconnectImpl();
        return;
    }
}

TcpClientTransport::~TcpClientTransport() { this->disconnectImpl(); }

void TcpClientTransport::send(std::span<const std::byte> data)
{
    const auto bytes_sent = ::send(socket_fd_, data.data(), data.size(), 0);
    if (bytes_sent < 0)
    {
        this->disconnectImpl();
    }
}

std::vector<std::byte> TcpClientTransport::receive()
{
    std::vector<std::byte> buffer(1024);
    const auto bytes_received = ::recv(socket_fd_, buffer.data(), buffer.size(), 0);
    if (bytes_received < 0)
    {
        this->disconnectImpl();
        return {};
    }
    buffer.resize(bytes_received);
    return buffer;
}

void TcpClientTransport::tick()
{
    // TODO: socket pooling?
}

void TcpClientTransport::disconnect() { this->disconnectImpl(); }

void TcpClientTransport::disconnectImpl()
{
    if (!is_connected_)
    {
        return;
    }
    if (socket_fd_ >= 0)
    {
        close(socket_fd_);
        socket_fd_ = -1;
    }
    is_connected_ = false;
    if (callbacks_.on_disconnected)
    {
        callbacks_.on_disconnected();
    }
}

} // namespace filthyrat
