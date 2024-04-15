#include <boost/asio.hpp>
#include <boost/system.hpp>
#include <iostream> 

using namespace boost::asio;
io_service service;
void handle_connections() {
    char buff[1024];
    try
    {
        ip::udp::socket sock(service, ip::udp::endpoint(ip::udp::v4(), 3333));
        ip::udp::endpoint STM_ep(ip::address_v4::from_string("169.254.248.1"), 62510);
        while (true)
        {
            int bytes = sock.receive_from(buffer(buff), STM_ep);
            std::string msg(buff, bytes);
            std::cout << msg<<'\n';
            sock.send_to(buffer("I hear you!"), STM_ep);
        }
    }
    catch (boost::system::system_error e)
    {
        std::cout << e.code() << std::endl;
    } 
}
int main(int argc, char* argv[]) {
    handle_connections();
}

