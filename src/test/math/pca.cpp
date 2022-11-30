//
// Created by TD on 2022/9/23.
//

#include <doodle_core/doodle_core.h>

#include <boost/test/unit_test.hpp>

#include "Eigen/Eigen"
#include <Eigen/src/Core/Matrix.h>
#include <Eigen/src/SVD/JacobiSVD.h>
#include <cmath>
#include <crtdbg.h>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <main_fixtures/lib_fixtures.h>
#include <ostream>

Eigen::MatrixXf pca_fun(Eigen::MatrixXf& in_mat) {
  // 计算平均
  std::cout << "mat " << in_mat << std::endl;
  Eigen::VectorXf Average = in_mat.colwise().mean();
  std::cout << "Average " << Average << std::endl;
  Eigen::VectorXf ones{in_mat.rows()};
  ones.setOnes();

  in_mat -= ones * Average.transpose();
  std::cout << "org " << in_mat << std::endl;

  auto k = in_mat.rows() / 3;
  // svd 采样
  Eigen::JacobiSVD<Eigen::MatrixXf> svd{in_mat, Eigen::ComputeThinU | Eigen::ComputeThinV};
  Eigen::VectorXf EigenVector = svd.singularValues();
  std::cout << "The accuracy is " << EigenVector(k - 1) / EigenVector.sum() * 1.0 << std::endl;

  Eigen::MatrixXf ONB = svd.matrixV().block(0, k - 1, k, 1);
  std::cout << ONB;
  // Eigen::VectorXf proj = ONB.transpose() * in_mat;
  // return ONB * proj + Average;
  return {};
};

Eigen::MatrixXf pca_fun2(const Eigen::MatrixXf& in_mat) {
  Eigen::JacobiSVD<Eigen::MatrixXf> l_svd{in_mat, Eigen::ComputeThinU | Eigen::ComputeThinV};

  Eigen::MatrixXf l_u = l_svd.matrixU();
  Eigen::MatrixXf l_v = l_svd.matrixV();
  Eigen::VectorXf l_s = l_svd.singularValues();
  std::size_t l_com   = l_s.size();

  std::cout << "U: \n" << l_u << "\nV: \n" << l_v << "\nS: \n" << l_s << std::endl;

  auto l_rows = in_mat.rows() / 2;

  for (auto i = 0ll; i < l_com; ++i) {
    const std::float_t l_muliplier = l_s(i);
    for (auto j = 0ll; j < l_rows; ++j) {
      l_u(i, j) *= l_muliplier;
    }
  }
  std::cout << "U: \n" << l_u << std::endl;
  return {};
}

BOOST_AUTO_TEST_CASE(test_pca) {
  Eigen::MatrixXf l_mat{3, 3};
  l_mat << 2, 2, 3, 4, 5, 7, 2, 1, 4;
  pca_fun2(l_mat);
}