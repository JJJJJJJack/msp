#include <Client.hpp>
#include <iostream>
#include <msp_msg.hpp>

int main(int argc, char *argv[]) {
    const std::string device =
        (argc > 1) ? std::string(argv[1]) : "/dev/ttyUSB0";
    const size_t baudrate = (argc > 2) ? std::stoul(argv[2]) : 115200;

    msp::client::Client client;
    client.setLoggingLevel(msp::client::LoggingLevel::WARNING);
    client.setVariant(msp::FirmwareVariant::INAV);
    client.start(device, baudrate);

    msp::FirmwareVariant fw_variant = msp::FirmwareVariant::INAV;

    while(1){
        msp::msg::WayPoint waypoint(fw_variant);
        if(client.sendMessage(waypoint) == 1) {
	  printf("msg1: %6.2f,     msg2: %6.2f,     msg3: %6.2f,     msg4: %6.2f,     msg5: %6.2f,     msg6: %6.2f\n", (double)waypoint.msg1, (double)waypoint.msg2, (double)waypoint.msg3, (double)waypoint.msg4, (double)waypoint.msg5, (double)waypoint.msg6);
        }
        /*msp::msg::Attitude attitude(fw_variant);
        if(client.sendMessage(attitude) == 1)
            std::cout << attitude;*/
        usleep(20000);
    }
    
    client.stop();
}
