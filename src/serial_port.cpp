#include <iostream>
#include <stdlib.h>

#include "serial_port.hpp"

SerialPort::SerialPort( void ) {

}

SerialPort::SerialPort( std::string port_name_in ) {
    this->port_name = port_name_in.c_str();
    this->init();
}

SerialPort::SerialPort(std::string port_name_in, unsigned int baud_rate_in) {
    this->port_name = port_name_in.c_str();
    this->baud_rate = baud_rate_in;
    
    this->init();
}

void SerialPort::init( void ) {
    // intialize the serial port using member variables
    this->check(sp_get_port_by_name(this->port_name, &(this->port)));

    // print basic info about the port
    std::cout << "Port name: " << sp_get_port_name(this->port) << std::endl;
    std::cout << "Description: " << sp_get_port_description(this->port) << std::endl;

    // open the port
    this->open();

    // set config
    this->save_config();

    this->get_config();

}

void SerialPort::open( void ) {
    std::cout << "Opening port" << std::endl;
    this->check(sp_open(this->port, this->mode));

    this->is_open = true;
}

void SerialPort::close( void ) {
    int result = this->check(sp_close(this->port));
    std::string name(this->port_name);
    if (result != SP_OK) {
        std::cerr << "Cannot close: " << name << std::endl;
    } else {
        std::cout << "Closed: " << name << std::endl;
    }
    sp_free_port(this->port);
    this->is_open = false;
}

SerialPort::~SerialPort( void ) {
    if (this->is_open) {
        this->close();
    }

    sp_free_config(this->config);
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
            /* std::cout << "OK" << std::endl; */
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

void SerialPort::save_config( void ) {
    this->check(sp_new_config(&(this->config)));
    // save config variables
    this->check(sp_set_config_baudrate(this->config, this->baud_rate));
    this->check(sp_set_config_bits(this->config, this->bits));
    this->check(sp_set_config_parity(this->config, this->parity));
    this->check(sp_set_config_stopbits(this->config, this->stop_bits));
    this->check(sp_set_config_flowcontrol(this->config, this->flowcontrol));
}

void SerialPort::set_config( void ) {
    std::cout << "Applying configuration to port" << std::endl;
    this->check(sp_set_config(this->port, this->config));
}

void SerialPort::get_config( void ) {
   struct sp_port_config *initial_config;

   std::cout << "Geting the config for: " << 
   check(sp_get_config(this->port, initial_config));

   // output the config of this serial port
}
