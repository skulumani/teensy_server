#include <iostream>
#include <stdlib.h>

#include "serial_port.hpp"

SerialPort::SerialPort( void ) {

}

SerialPort::~SerialPort( void ) {

}

int SerialPort::check( enum sp_return result) {
    /* For this example we'll just exit on any error by calling abort(). */
    char *error_message;

    switch (result) {
        case SP_ERR_ARG:
            std::cout << "Error: Invalid argument" << std::endl;
            abort();
        case SP_ERR_FAIL:
            error_message = sp_last_error_message();
            std::cout << "Error: Failed: " << error_message << std::endl;
            sp_free_error_message(error_message);
            abort();
        case SP_ERR_SUPP:
            std::cout << "Error: Not supported." << std::endl;
            abort();
        case SP_ERR_MEM:
            std::cout << "Error: Couldn't allocate memory." << std::endl;
            abort();
        case SP_OK:
            std::cout << "OK" << std::endl;
        default:
            return result;
    }
}

int SerialPort::list_ports( void ) {
    struct sp_port **port_list;

    enum sp_return result = sp_list_ports(&port_list);
    if (result != SP_OK) {
        std::cout << "sp_list_ports() failed!" << std::endl;
        return -1;
    }
    
    struct sp_port *port;
    int ii;
    for (ii=0; port_list[ii] != NULL; ii++) {
        port = port_list[ii];
        std::cout << "Found port: " << sp_get_port_name(port) << std::endl;
    }
    std::cout << "Found " << ii << " ports." << std::endl;
    std::cout << "Freeing port list. " << std::endl;
    sp_free_port_list(port_list);
    
    return 0;
}
