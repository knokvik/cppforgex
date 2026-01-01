#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>

class TCPClient : public std::enable_shared_from_this<TCPClient> {
public:
    TCPClient(boost::asio::io_context& io) : resolver_(io), socket_(io) {}

    void start(const std::string& host, const std::string& port) {
        resolver_.async_resolve(host, port,
            [self = shared_from_this()](const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::results_type results) {
                if (!ec) {
                    self->do_connect(results);
                }
            });
    }

private:
    void do_connect(boost::asio::ip::tcp::resolver::results_type results) {
        boost::asio::async_connect(socket_, results,
            [self = shared_from_this()](const boost::system::error_code& ec, const boost::asio::ip::tcp::endpoint& /*endpoint*/) {
                if (!ec) {
                    std::cout << "Successfully connected to server!" << std::endl;
                    self->do_write();
                }
            });
    }

    void do_write() {
        tx_buffer_ = "Hello from Client!\n";
        boost::asio::async_write(socket_, boost::asio::buffer(tx_buffer_),
            [self = shared_from_this()](const boost::system::error_code& ec, std::size_t /*length*/) {
                if (!ec) {
                    self->do_read();
                }
            });
    }

    void do_read() {
        socket_.async_read_some(boost::asio::buffer(rx_buffer_),
            [self = shared_from_this()](const boost::system::error_code& ec, std::size_t length) {
                if (!ec) {
                    std::string msg(self->rx_buffer_.data(), length);
                    std::cout << "[Client Received]: " << msg << std::endl;
                }
            });
    }

    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
    std::string tx_buffer_;
    std::array<char, 1024> rx_buffer_;
};

int main() {
    try {
        boost::asio::io_context io;
        auto client = std::make_shared<TCPClient>(io);
        client->start("127.0.0.1", "8000");
        io.run();
    } catch (std::exception& e) {
        std::cerr << "Client Error: " << e.what() << std::endl;
    }
    return 0;
}
