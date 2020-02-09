#ifndef SERIAL_PORT_HPP
#define SERIAL_PORT_HPP

#include <libserialport.h>
#include <iostream>

class SerialPort {
    
    public:
        // constructors
        SerialPort( void );
        SerialPort(std::string port_name_in);
        SerialPort(std::string port_name_in, unsigned int baud_rate_in);

        virtual ~SerialPort( void );


    private:
        bool is_open = false;
        std::string port_name;
        unsigned int baud_rate;

        void init( void );
        void open( void );
        
}

#endif
