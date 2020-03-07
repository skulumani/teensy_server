#include <iostream>
#include <iomanip>

#include <Eigen/Dense>

#include "ahrs.hpp"
#include "hdf5.hpp"

int main () {

    // create a HDF5 to store the sensor data
    /* HDF5::File hf = HDF5::File("imu_data.hdf5", HDF5::Truncate); */

    // read HDF5 with all the IMU data
    // parse all the magnetometer data
    // call ellipsoid fit
    // compute the Mag scale parameters
    // print and write to HDF5
    return 0;
}
