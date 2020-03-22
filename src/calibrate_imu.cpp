#include <iostream>
#include <iomanip>
#include <chrono>

#include <Eigen/Dense>

#include "serial_port.hpp"
#include "simple.pb.h"
#include "ahrs.pb.h"
#include "hdf5.hpp"

int read_data(SerialPort& serial, HDF5::File& hf, double& max_time 
        std::string& dataset_name) { 

    Eigen::Matrix<float, Eigen::Dynamic, 11> imu_data;
    Eigen::Matrix<float, 1, 11> imu_row;
    
    size_t num_meas = 0;

    SimpleMessage m;
    AHRS::IMUMeasurement imu_msg;
    
    std::cout << "Waiting 2 seconds" << std::endl;
    usleep(2000);

    auto start_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start_time;

    while(elapsed_seconds.count() < max_time) {
        serial.read();
        /* m.ParseFromArray(serial.byte_buffer, sizeof(serial.byte_buffer)); */
        /* std::cout << m.lucky_number() << std::endl; */
        imu_msg.ParseFromArray(serial.byte_buffer, sizeof(serial.byte_buffer));
        std::cout << "\33[2K\r" << "T: " << imu_msg.time() << " " 
            << imu_msg.accel().x() << " " << imu_msg.accel().y() << " " << imu_msg.accel().z() 
            << " " << imu_msg.gyro().x() << " " << imu_msg.gyro().y() << " " << imu_msg.gyro().z() 
            << " " << imu_msg.mag().x() << " " << imu_msg.mag().y() << " " << imu_msg.mag().z() 
            << " " << imu_msg.temp() << std::flush;

        // store the imu data into the array
        imu_row << imu_msg.time(), 
                imu_msg.accel().x(), imu_msg.accel().y(), imu_msg.accel().z(),
                imu_msg.gyro().x(), imu_msg.gyro().y(), imu_msg.gyro().z(),
                imu_msg.mag().x(), imu_msg.mag().y(), imu_msg.mag().z(),
                imu_msg.temp();
        imu_data.conservativeResize(num_meas+1, 11);
        imu_data.row(num_meas) << imu_row;
        num_meas += 1;
        
        elapsed_seconds = std::chrono::system_clock::now() - start_time;

    }
    
    std::cout << std::endl << std::endl;

    // save to HDF5 file
    hf.write(dataset_name, imu_data);

    return 0;
}

int main () {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // create a HDF5 to store the sensor data
    HDF5::File hf = HDF5::File("imu_data.hdf5", HDF5::File::Truncate );
    
    SerialPort serial("/dev/ttyACM0", 115200);
    if (serial.is_open != true) {
        std::cerr << "error with serial port" << std::endl;
        return 1;
    }
    /* serial.list_ports(); */
    
    // stationary data for gyro calibration
    std::cout << "Keep IMU stationary for 60 seconds" << std::endl;

    std::cout << "Waiting 2 seconds" << std::endl;
    usleep(2000);

    while(elapsed_seconds.count() < 60.0) {
        serial.read();
        
        imu_msg.ParseFromArray(serial.byte_buffer, sizeof(serial.byte_buffer));
        std::cout << "\33[2K\r" << "T: " << imu_msg.time() << " " 
            << imu_msg.accel().x() << " " << imu_msg.accel().y() << " " << imu_msg.accel().z() 
            << " " << imu_msg.gyro().x() << " " << imu_msg.gyro().y() << " " << imu_msg.gyro().z() 
            << " " << imu_msg.mag().x() << " " << imu_msg.mag().y() << " " << imu_msg.mag().z() 
            << " " << imu_msg.temp() << std::flush;

        // store the imu data into the array
        imu_row << imu_msg.time(), 
                imu_msg.accel().x(), imu_msg.accel().y(), imu_msg.accel().z(),
                imu_msg.gyro().x(), imu_msg.gyro().y(), imu_msg.gyro().z(),
                imu_msg.mag().x(), imu_msg.mag().y(), imu_msg.mag().z(),
                imu_msg.temp();
        imu_data.conservativeResize(num_meas+1, 11);
        imu_data.row(num_meas) << imu_row;
        num_meas += 1;
        
        elapsed_seconds = std::chrono::system_clock::now() - start_time;
        
    }
    
    std::cout << std::endl << std::endl;
    std::cout << "Finished with stationary period" << std::endl;

    // save to HDF5 file
    hf.write("gyro_data", imu_data);
    
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
