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
        
        const auto& x = data.col(0);
        const auto& y = data.col(1);
        const auto& z = data.col(2);

        auto x_sq = x.pow(2);
        auto y_sq = y.pow(2);
        auto z_sq = z.pow(2);

        // fit ellipsoid 
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> D;
        switch (type) {

            case EllipsoidType::Arbitrary:
                D.resize(data.rows(), 9);
                D.col(0) = x_sq + y_sq - 2 * z_sq;
                D.col(1) = x_sq + z_sq - 2 * y_sq;
                D.col(2) = 2 * x.cwiseProduct(y);
                D.col(3) = 2 * x.cwiseProduct(z);
                D.col(4) = 2 * y.cwiseProduct(z);
                D.col(5) = 2 * x;
                D.col(6) = 2 * y;
                D.col(7) = 2 * z;
                D.col(8).setOnes();
                break;
            case EllipsoidType::XYEqual:
                D.resize(data.rows(), 8);
                D.col(0) = x_sq + y_sq - 2 * z_sq;
                D.col(1) = 2 * x.cwiseProduct(y);
                D.col(2) = 2 * x.cwiseProduct(z);
                D.col(3) = 2 * y.cwiseProduct(z);
                D.col(4) = 2 * x;
                D.col(5) = 2 * y;
                D.col(6) = 2 * z;
                D.col(7).setOnes();
                break;
            case EllipsoidType::XZEqual:
                D.resize(data.rows(), 8);
                D.col(0) = x_sq + z_sq - 2 * y_sq;
                D.col(1) = 2 * x.cwiseProduct(y);
                D.col(2) = 2 * x.cwiseProduct(z);
                D.col(3) = 2 * y.cwiseProduct(z);
                D.col(4) = 2 * x;
                D.col(5) = 2 * y;
                D.col(6) = 2 * z;
                D.col(7).setOnes();
                break;
            case EllipsoidType::Sphere:
                D.resize(data.rows(), 4);
                D.col(0) = 2 * x;
                D.col(1) = 2 * y;
                D.col(2) = 2 * z;
                D.col(3).setOnes();
                break;
            case EllipsoidType::Aligned:
                D.resize(data.rows(), 6);
                D.col(0) = x_sq + y_sq - 2 * z_sq;
                D.col(1) = x_sq + z_sq - 2 * y_sq;
                D.col(2) = 2 * x;
                D.col(3) = 2 * y;
                D.col(4) = 2 * z;
                D.col(5).setOnes();
                break;
            case EllipsoidType::AlignedXYEqual:
                D.resize(data.rows(), 5);
                D.col(0) = x_sq + y_sq - 2 * z_sq;
                D.col(1) = 2 * x;
                D.col(2) = 2 * y;
                D.col(3) = 2 * z;
                D.col(4).setOnes();
                break;
            case EllipsoidType::AlignedXZEqual:
                D.resize(data.rows(), 5);
                D.col(0) = x_sq + z_sq - 2 * y_sq;
                D.col(1) = 2 * x;
                D.col(2) = 2 * y;
                D.col(3) = 2 * z;
                D.col(4).setOnes();
                break; 
        }
        
        // solve normal system of equations
        Eigen::Matrix<double, Eigen::Dynamic, 1> d2 = (x_sq + y_sq + z_sq);
        Eigen::Matrix<double, Eigen::Dynamic, 1> u = (D.transpose() * D).bdcSvd(Eigen::ComputeFullU | Eigen::ComputeFullV).solve(D.transpose() * d2);

        // find parameters of ellipsoid
        Eigen::Matrix<double, 10, 1> v;
        switch (type) {
            case EllipsoidType::Arbitrary:
                v(0) = u(0) + u(1) - 1;
                v(1) = u(0) - 2 * u(1) - 1;
                v(2) = u(1) - 2 * u(0) - 1;
                v.segment<7>(3) = u.segment<7>(2);
                break;
            case EllipsoidType::XYEqual:
                v(0) = u(0) - 1;
                v(1) = u(0) - 1;
                v(2) = -2 * u(0) - 1;
                v.segment<7>(3) = u.segment<7>(1);
                break;
            case EllipsoidType::XZEqual:
                v(0) = u(0) - 1;
                v(1) = -2 * u(0) - 1;
                v(2) = u(0) - 1;
                v.segment<7>(3) = u.segment<7>(1);
                break;
            case EllipsoidType::Sphere:
                v.segment<3>(0).setConstant(-1);
                v.segment<3>(3).setZero();
                v.segment<4>(6) = u.segment<4>(0);
                break;
            case EllipsoidType::Aligned:
                v(0) = u(0) +      u(1) - 1;
                v(1) = u(0) - 2 * u(1) - 1;
                v(2) = u(1) - 2 * u(0) - 1;
                v.segment<3>(3).setZero();
                v.segment<4>(6) = u.segment<4>(2);
                break;
            case EllipsoidType::AlignedXYEqual:
                v(0) = u(0) - 1;
                v(1) = u(0) - 1;
                v(2) = -2 * u(0) - 1;
                v.segment<3>(3).setZero();
                v.segment<4>(6) = u.segment<4>(1);
                break;
            case EllipsoidType::AlignedXZEqual:
                v(0) = u(0) - 1;
                v(1) = -2 * u(0) - 1;
                v(2) = u(0) - 1;
                v.segment<3>(3).setZero();
                v.segment<4>(6) = u.segment<4>(1);
                break;
        }

        Eigen::Matrix4d A;
        A << v(0), v(3), v(4), v(6),
          v(3), v(1), v(5), v(7),
          v(4), v(5), v(2), v(8),
          v(6), v(7), v(8), v(9);

        // center
        params.center = -A.block<3,3>(0,0).bdcSvd(Eigen::ComputerFullU | Eigen::ComputeFullV).solve(v.segment<3>(6));
        // translation matrix
        Eigen::Matrix4d T(Eigen::Matrix4d::Identity());
        T.block<1, 3>(3, 0) = params.center.transpose();

        // translate to center;
        auto R = (T * A * T.transpose()).eval();
        // solve the eigenproblem
        Eigen::EigenSolver<Eigen::Matrix3d> solver(R.block<3, 3>(0, 0) / -R(3, 3));
        params.radii = solver.eigenvalues().cwiseAbs().cwiseInverse().cwiseSqrt();
        for (size_t ii = 0; ii < 3; ii++) {
            if (solver.eigenvalues()(ii).real() < 0) {
                params.radii(ii) = -params.radii(ii);
            }
        }

        return params;
    }

    Eigen::Matrix<double, Eigen::Dynamic, 3> generate(const Parameters& parameters, size_t samples) {
        Eigen::Matrix<double, Eigen::Dynamic, 3> points;
        points.resize(samples, 3);

        for (size_t ii =0; ii < samples; ii++) {
            Eigen::Vector3d point;

            // random spherical angles
            double theta = ((std::rand() - RAND_MAX/2) / double(RAND_MAX)) * M_PI; // -pi/2 < theta < pi/2
            double phi = ((std::rand() _ RAND_MAX/2) / double(RAND_MAX)) * 2 * M_PI; // -pi < phi < pi

            point.x() = parameters.center.x() + parameters.radii.x() * std::cos(theta) * std::cos(phi);
            point.y() = parameters.center.y() + parameters.radii.y() * std::cost(theta) * std::sin(phi);
            point.z() = parameters.center.z() + parameters.radii.z() * std::sin(theta);

            points.row(ii) = point.transpose();
        }

        return points;
    }
}
