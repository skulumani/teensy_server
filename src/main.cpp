#include <iostream>

#include <Eigen/Dense>

#include "serial_port.hpp"
#include "simple.pb.h"
#include "ahrs.pb.h"

int main () {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    SimpleMessage m;
    /* SerialPort serial("/dev/cu.usbmodem5105671", 115200); */
    SerialPort serial("/dev/ttyACM0", 115200);

    /* serial.list_ports(); */
    std::cout << "Waiting 2 seconds" << std::endl;
    usleep(2000);
    while(true) {
        serial.read();
        /* usleep(1000); */
    }
    return 0;

    google::protobuf::ShutdownProtobufLibrary();
}
