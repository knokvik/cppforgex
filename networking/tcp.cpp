#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using namespace boost;

int main() {
    asio::io_context io;
    asio::ip::tcp::socket socket(io);
    asio::ip::tcp::resolver network_resolver(io);

    // to connect
    // asio::ip::tcp::endpoint server_endpoint(asio::ip::make_address("127.0.0.1"),8080);
    // socket.connect(server_endpoint);
    asio::ip::tcp::resolver::results_type endpoints = 
        network_resolver.resolve("google.com","http");

    for( const auto& entry : endpoints ) {
        cout << "Found IP: " << entry.endpoint() << endl;
    }

    return 0;
}