#include <iostream>

#include <Eigen/Dense>

#include "serial_port.hpp"
#include "proto/simple.pb.h"

int main () {
    SerialPort serial("/dev/cu.usbmodem5105671", 115200);

    serial.list_ports();

    while(true) {
        serial.read();
        /* usleep(1000); */
    }
    return 0;
}
