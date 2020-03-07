#ifndef AHRS_HPP
#define AHRS_HPP
#include <Eigen/Dense>

namespace Ellipsoid {
class EllipsoidParameters {
    public:
        Eigen::Vector3d center;
        Eigen::Vector3d semiaxis;

        void print( void );

        EllipsoidParameters( void );
        virtual ~EllipsoidParameters( void );
};

enum class EllipsoidType {
    Arbitrary,
    XYEqual,
    XZEqual,
    Sphere,
    Aligned,
    AlignedXYEqual,
    AlignedXZEqual,
}

Parameters fit(const Eigen::Matrix<double, Eigen::Dynamic, 3>& data, 
        EllipsoidType type = EllipsoidType::Arbitrary);

Eigen::Matrix<double, Eigen::Dynamic, 3> generate(const Parameters& parameters, size_t samples = 1000);

}
#endif

