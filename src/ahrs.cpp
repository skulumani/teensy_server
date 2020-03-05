#include "ahrs.hpp"

#include <iostream>

namespace ELLIPSOID {
    EllipsoidParameters( void ) {
        center = Eigen::Vector3d::Zero(3);
        semiaxis = Eigen::Vector3d::Zero(3);
    }

    void EllipsoidParameters::print( void ) {
        std::cout << "center: " << center.transpose() << std::endl;
        std::cout << "semiaxis: " << semiaxis.transpose() << std::endl;
    }

    Parameters fit(const Eigen::Matrix<double, Eigen::Dynamic, 3>& data,
            EllipsoidType type) {

    }
}
