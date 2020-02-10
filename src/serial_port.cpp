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
    this->set_config();

    /* this->get_config(); */
    
    // event waiting set
    this->check(sp_new_event_set(&this->event_set));
    this->check(sp_add_port_events(this->event_set, this->port, SP_EVENT_RX_READY));

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
    sp_free_event_set(this->event_set);
    if (this->is_open) {
        this->close();
    }

    /* sp_free_config(this->config); */
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

void SerialPort::set_config( void ) {
    std::cout << "Applying configuration to port" << std::endl;
    /* this->check(sp_set_config(this->port, this->config)); */

    this->check(sp_set_baudrate(this->port, this->baud_rate));
    this->check(sp_set_bits(this->port, this->bits));
    this->check(sp_set_stopbits(this->port, this->stop_bits));
    this->check(sp_set_flowcontrol(this->port, this->flowcontrol));
    this->check(sp_set_parity(this->port, this->parity));
}

void SerialPort::read( void ) {
    // read from serial port - and save to buffer in object
    // wait for data
    check(sp_wait(this->event_set, 10000));

    int bytes_waiting = sp_input_waiting(this->port);
    /* memset(&this->byte_buffer[0], 0, sizeof(this->byte_buffer)); */
    int byte_num = 0;

    if (bytes_waiting > 0) {
        byte_num = sp_nonblocking_read(this->port, this->byte_buffer, sizeof(this->byte_buffer));
        this->parse_serial(byte_num);
    }
}

void SerialPort::parse_serial(int byte_num) {
    for (int ii = 0; ii < byte_num; ii++) {
        std::cout << this->byte_buffer[ii];
    }
    std::cout << std::endl;
}
