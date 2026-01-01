#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <iostream>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

// Must inherit from enable_shared_from_this because we are doing async chains!
struct StreamingSession : std::enable_shared_from_this<StreamingSession> {
    tcp::resolver resolver;
    beast::tcp_stream stream;
    beast::flat_buffer buffer;

    // Fixed typo: Added missing double colon '::'
    http::parser<false, http::string_body> parser;
    http::request<http::empty_body> req; // Outbound request object

    // Constructor to pass the central io_context brain down to the network tools
    StreamingSession(net::io_context& ioc) 
        : resolver(ioc), stream(ioc) {}

    // START HERE: This is where we define WHO we are talking to
    void start_crawl(const std::string& host, const std::string& target) {
        // 1. Build the HTTP request instructions
        req.version(11); // HTTP/1.1
        req.method(http::verb::get);
        req.target(target);
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "BeastStreamer/1.0");

        // 2. Resolve the domain name to an IP address
        auto self = shared_from_this();
        resolver.async_resolve(host, "80",
            [self](beast::error_code ec, tcp::resolver::results_type results) {
                if(!ec) {
                    // 3. Connect the physical socket stream to that IP address
                    self->stream.async_connect(results,
                        [self](beast::error_code ec, tcp::resolver::results_type::endpoint_type) {
                            if(!ec) {
                                // 4. Send our HTTP Request down the wire to the server
                                http::async_write(self->stream, self->req,
                                    [self](beast::error_code ec, std::size_t) {
                                        if(!ec) {
                                            // 5. NOW WE FINALLY HAVE DATA COMING IN! Read the headers.
                                            self->read_headers();
                                        }
                                    });
                            }
                        });
                }
            });
    }

    void read_headers() {
        auto self = shared_from_this();
        
        // This pulls bytes from the connected socket stream, extracts headers, 
        // and dumps the metadata into our parser state machine.
        http::async_read_header(stream, buffer, parser, 
            [self](beast::error_code ec, std::size_t) {
                if(!ec) {
                    if(self->parser.get().result() == http::status::ok) {
                        std::cout << "[Step 1 Done] Content-Type Header: " 
                                  << self->parser.get()[http::field::content_type] << "\n";
                        
                        // Proceed to grab the body since status is 200 OK
                        self->read_body();
                    } else {
                        std::cout << "Bad Status: " << self->parser.get().result_int() << ". Aborting!\n";
                        self->stream.close(); 
                    }
                }
            }); // Fixed missing semicolon typo here
    }

    void read_body() {
        auto self = shared_from_this();
        
        // This resumes reading from the stream to pull down the actual HTML body data chunks.
        http::async_read(stream, buffer, parser,
            [self](beast::error_code ec, std::size_t) {
                if(!ec) {
                    std::cout << "[Step 2 Done] Body size: " 
                              << self->parser.get().body().size() << " bytes\n";
                    
                    // Gracefully shutdown the socket
                    beast::error_code se;
                    self->stream.socket().shutdown(tcp::socket::shutdown_both, se);
                }
            });
    }
}; // Fixed missing semicolon typo after struct definition

int main() {
    net::io_context ioc;
    
    // CORRECTION FOR MAIN: 
    // Objects using shared_from_this MUST be heap-allocated via a shared_ptr.
    // If you stack-allocate it (like: StreamingSession stream;), it will CRASH instantly!
    auto session = std::make_shared<StreamingSession>(ioc);
    
    // Kickoff the session with a live domain and route
    session->start_crawl("www.google.com", "/");
    
    // Start the event loop processor engine
    ioc.run();
    
    return 0;
}
