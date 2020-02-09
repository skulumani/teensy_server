#include <iostream>
#include <fstream>
#include <sstream>

#include <Eigen/Dense>

int main() {
    std::ifstream teensy_output("/dev/cu.usbmodem5105671");
    std::string line;
    while (std::getline(teensy_output, line)) {
        std::cout << line << std::endl;
    }

    teensy_output.close();

    return 0;
}


