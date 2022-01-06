#include <iostream>
#include <cmath>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "sophus/se3.hpp"

using namespace std;
using namespace Eigen;

int main(int argc, char **argv)
{
  Matrix3d R = AngleAxisd(M_PI / 2, Vector3d(0,0,1)).toRotationMatrix();
  Quaterniond q(R);
  Sophus::SO3d SO3_R(R);
  Sophus::SO3d SO3_q(q);

  cout.precision(3);
  cout << "SO(3) from Matrix: \n" << SO3_R.matrix() << endl;
  cout << "SO(3) from quaternion: \n" << SO3_q.matrix() << endl;

  Vector3d so3 = SO3_R.log();         //  直接log就可以，不需要vee操作
  cout << "so3 = " << so3.transpose() << endl;
  cout << "so3 hat= \n" << Sophus::SO3d::hat(so3) << endl;
  cout << "so3 hat vee= \n" << Sophus::SO3d::vee(Sophus::SO3d::hat(so3)).transpose() << endl;

  Vector3d update_so3(1e-4, 0, 0);
  //
  Sophus::SO3d SO3_updated = Sophus::SO3d::exp(update_so3) * SO3_R;   // exp中并没有进行hat操作; 
  cout << "SO3 updated = \n" << SO3_updated.matrix() << endl;

  cout << "_______________________________________________"<< endl;

  Vector3d t(1,0,0);
  Sophus::SE3d SE3_Rt(R, t);  // SE3这个t是平移向量，se3中的那个不是，之间差个J
  Sophus::SE3d SE3_qt(q, t);

  cout << "SE3 from R,t= \n" << SE3_Rt.matrix() << endl;
  cout << "SE3 from q,t= \n" << SE3_qt.matrix() << endl;
  cout << "SE3 translation: "<< SE3_Rt.translation() << endl;

  typedef Eigen::Matrix<double, 6, 1> Vector6d;
  Vector6d se3 = SE3_Rt.log();
  cout << "se3 = " << se3.transpose() << endl;
  cout << "se3 hat = \n" <<Sophus::SE3d::hat(se3) << endl;
  cout << "se3 hat vee = " <<Sophus::SE3d::vee(Sophus::SE3d::hat(se3)).transpose() << endl;
 
  Vector6d update_se3;
  update_se3.setZero();
  update_se3(0,0) = 1e-3;
  Sophus::SE3d SE3_updated = Sophus::SE3d::exp(update_se3) * SE3_Rt;
  cout << "SE3 updated = " << endl << SE3_updated.matrix() << endl;

}