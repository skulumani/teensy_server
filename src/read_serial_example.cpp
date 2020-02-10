#include <iostream>
#include <unistd.h>

#include <libserialport.h>
#include <Eigen/Dense>

void list_ports() {
    int ii;
    struct sp_port **ports;

    sp_return error = sp_list_ports(&ports);
    if (error == SP_OK) {
        for (int ii =0; ports[ii]; ii++) {
            std::cout << "Found port: " << sp_get_port_name(ports[ii]) << std::endl;
        }
        sp_free_port_list(ports);
    } else {
        std::cout << "No serial devices detected" << std::endl;
    }
}

void  parse_serial(char *byte_buffer, int byte_num) {
    for (int ii = 0; ii < byte_num; ii++) {
        std::cout << byte_buffer[ii];
    }
    std::cout << std::endl;
}

int main() {
    list_ports();
    /* const char* desired_port = "/dev/cu.Bluetooth-Incoming-Port"; */
    const char* desired_port = "/dev/cu.usbmodem5105671";

    std::cout << "Opening port: " << desired_port << std::endl;

    struct sp_port *port;
    sp_return error = sp_get_port_by_name(desired_port, &port);
    if (error == SP_OK) {
        error = sp_open(port, SP_MODE_READ);
        if (error == SP_OK) {
            sp_set_baudrate(port, 115200);

            /* struct sp_port_config *initial_config; */
            /* int result = sp_get_config(port, initial_config); */
            /* std::cout << "result : " << result << std::endl; */

            /* int baudrate, bits, stopbits; */
            /* enum sp_parity parity; */
            /* sp_get_config_baudrate(initial_config, &baudrate); */
            /* sp_get_config_bits(initial_config, &bits); */
            /* sp_get_config_stopbits(initial_config, &stopbits); */
            /* sp_get_config_parity(initial_config, &parity); */

            /* std::cout << "Baudrate: " << baudrate << ", data bits: " << bits << ", parity: " << parity << ", stopbits: " << stopbits << std::endl; */


            while(true) {
                usleep(500);
                int bytes_waiting = sp_input_waiting(port);
                if (bytes_waiting > 0) {
                    std::cout << "Bytes waiting " << bytes_waiting << std::endl;
                    char byte_buffer[128];
                    int byte_num = 0;
                    byte_num = sp_nonblocking_read(port, byte_buffer, 128);
                    parse_serial(byte_buffer, byte_num);
                }

            }
            

            sp_close(port);
        } else {
            std::cout << "Error opening serial device" << std::endl;
        }

    } else {
        std::cout << "Error opening serial device" << std::endl;
    }

    return 0;
}


