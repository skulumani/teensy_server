#include <iostream>

#include <Eigen/Dense>

#include "serial_port.hpp"

int main () {
    SerialPort serial("/dev/cu.usbmodem5105671", 115200);

    serial.list_ports();
    return 0;
}
