#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <array>
#include <string>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::flush;

std::array<char, 1024> rx_buffer;
std::shared_ptr<boost::asio::ip::tcp::socket> active_socket = nullptr;

// Thread 1: Continuously listens for incoming data from the peer
void listen_for_data(std::shared_ptr<boost::asio::ip::tcp::socket> sock) {
    try {
        while (true) {
            boost::system::error_code ec;
            // This blocks ONLY this background thread, waiting for network data
            size_t length = sock->read_some(boost::asio::buffer(rx_buffer), ec);

            if (ec == boost::asio::error::eof) {
                cout << "\n[System]: Peer disconnected." << endl;
                break;
            } else if (ec) {
                break; 
            }

            string msg(rx_buffer.data(), length);
            cout << "\n[Incoming Message]: " << msg << endl;
            cout << "Type message: " << flush;
        }
    } catch (...) {}
}

// Thread 2: Runs the Asio event loop in the background
void run_io_context(boost::asio::io_context& io) {
    io.run();
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: ./p2p_node [My_Port] [Peer_Port]" << endl;
        cout << "Instance 1 example: ./p2p_node 8001 8002" << endl;
        cout << "Instance 2 example: ./p2p_node 8002 8001" << endl;
        return 1;
    }

    short my_port = std::stoi(argv[1]);
    short peer_port = std::stoi(argv[2]);

    try {
        boost::asio::io_context io;
        
        // 1. Instantly start listening on your assigned port
        boost::asio::ip::tcp::acceptor acceptor(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), my_port));
        auto listen_socket = std::make_shared<boost::asio::ip::tcp::socket>(io);

        // Set up an async accept trap
        acceptor.async_accept(*listen_socket, [&](const boost::system::error_code& ec) {
            if (!ec && !active_socket) {
                active_socket = listen_socket;
                cout << "\n[System]: Connected via incoming connection!" << endl;
                std::thread(listen_for_data, active_socket).detach();
            }
        });

        // Start the Asio network engine in a background thread
        std::thread io_thread(run_io_context, std::ref(io));
        io_thread.detach();

        // 2. Loop and try to connect to the peer's port at the same time
        auto dial_socket = std::make_shared<boost::asio::ip::tcp::socket>(io);
        auto peer_endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), peer_port);

        cout << "Looking for peer on port " << peer_port << "..." << endl;
        while (!active_socket) {
            boost::system::error_code ec;
            dial_socket->connect(peer_endpoint, ec);
            
            if (!ec) {
                if (!active_socket) { // Double check we didn't accept a connection first
                    active_socket = dial_socket;
                    cout << "\n[System]: Connected via outgoing dialer!" << endl;
                    std::thread(listen_for_data, active_socket).detach();
                }
                break;
            }
            // Sleep for half a second before trying to connect again
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        // 3. Main Keyboard Thread: Handles your typing completely dynamically
        string user_input;
        // Small delay to let the connection prints settle down cleanly
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
        
        while (true) {
            cout << "Type message: ";
            std::getline(std::cin, user_input);

            if (user_input == "exit") {
                break;
            }

            if (!user_input.empty() && active_socket) {
                boost::system::error_code ec;
                boost::asio::write(*active_socket, boost::asio::buffer(user_input), ec);
                if (ec) {
                    cout << "[System]: Failed to send message. Peer closed." << endl;
                    break;
                }
            }
        }

        io.stop();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
