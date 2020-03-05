#ifndef AHRS_HPP
#define AHRS_HPP
#include <Eigen/Dense>

namespace ELLIPSOID {
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

}
#endif

