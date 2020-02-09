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

        void list_ports();
        
        // read data
        void read( void );
        void open( void );
        void close( void );

    private:
        void init( void );
        void check( enum sp_return result);

        bool is_open = false;
        unsigned int baud_rate;

        // libserialport variables
        const char *port_name = NULL;
        struct sp_port *port;
        sp_return error;

        sp_mode mode = SP_MODE_READ;
        sp_parity parity = SP_PARITY_NONE;




        
}

#endif
