#ifndef SERIAL_PORT_HPP
#define SERIAL_PORT_HPP

#include <iostream>
#include <unistd.h>

#include <libserialport.h>

class SerialPort {
    
    public:
        // constructors
        SerialPort( void );
        SerialPort(std::string port_name_in);
        SerialPort(std::string port_name_in, unsigned int baud_rate_in);

        virtual ~SerialPort( void );

        int list_ports( void );
        
        // read data from connected port and store in a byte buffer in object
        void read( void );
        void open( void );
        void close( void );
        void info( void );

        char byte_buffer[128]; // store serial data

        bool is_open = false;
    private:
        void init( void );
        sp_return check( enum sp_return result);
        void set_config( void );
        
        void parse_serial( int byte_num);


        // libserialport variables
        const char *port_name = NULL;
        struct sp_port *port;
        struct sp_event_set *event_set;

        sp_return error;

        // configuration variables for the port
        unsigned int baud_rate = 115200;
        int bits = 8;
        int stop_bits = 1;
        sp_flowcontrol flowcontrol = SP_FLOWCONTROL_NONE;
        sp_mode mode = SP_MODE_READ;
        sp_parity parity = SP_PARITY_NONE;

};

#endif
