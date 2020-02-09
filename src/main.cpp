#include <iostream>

#include <Eigen/Dense>

#include "serial_port.hpp"

int main () {
    SerialPort serial;

    serial.list_ports();
    return 0;
}
