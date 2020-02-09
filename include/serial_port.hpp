#ifndef SERIAL_PORT_HPP
#define SERIAL_PORT_HPP

#include <iostream>

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


    private:
        void init( void );
        int check( enum sp_return result);
        void save_config( void );
        void set_config( void );
        void get_config( void );

        bool is_open = false;

        // libserialport variables
        const char *port_name = NULL;
        struct sp_port *port;

        sp_return error;

        // configuration variables for the port
        unsigned int baud_rate = 115200;
        int bits = 8;
        int stop_bits = 1;
        sp_flowcontrol flowcontrol = SP_FLOWCONTROL_NONE;
        sp_mode mode = SP_MODE_READ;
        sp_parity parity = SP_PARITY_NONE;

        struct sp_port_config *config;




        
};

#endif
