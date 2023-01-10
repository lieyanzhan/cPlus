#include <iostream>
#include <cmath>
#include <Eigen/Core>
#include <Eigen/Geometry>


using namespace std;
using namespace Eigen;

int main() {
    Matrix3d rotation_matrix = Matrix3d::Identity();
    cout << rotation_matrix << endl;

    AngleAxisd rotation_vector(M_PI / 4, Vector3d(0, 0, 1));
    // 表示输出小数位精度
    cout.precision(2);
    cout << rotation_vector.matrix() << endl;

    rotation_matrix = rotation_vector.toRotationMatrix();
    cout << "rotation_matrix from vector: " << rotation_matrix << endl;

    Vector3d v(1, 0, 0);
    Vector3d v_rotated = rotation_vector * v;
    // v_rotated.transpose() 表示转置
    cout << "(1, 0, 0) after rotation = "<< v_rotated.transpose() << endl;

    // 欧拉角 将旋转矩阵转换为欧拉角 ZYX方式
    Vector3d euler_angles = rotation_matrix.eulerAngles(2, 1, 0);
    cout << "eulerAngles: " << euler_angles.transpose()<< endl;


    // 欧式变换矩阵
    Eigen::Isometry3d T = Isometry3d::Identity();
    T.rotate(rotation_vector); 
    T.pretranslate(Vector3d(1, 3, 4));
    cout << "Transform matrix = \n" << T.matrix() << endl;

    Vector3d v_transformed = T * v;
    cout << "v tranformed = " << v_transformed.transpose() << endl;

    // 旋转向量 转化为 四元数
    Quaterniond q = Quaterniond(rotation_vector);
    // x , y z , w(实部)
    cout << "quaternion from vector = " << q.coeffs().transpose() << endl;
    // also
    q = Quaterniond(rotation_matrix);
    cout << "quaternion from matrix = " << q.coeffs().transpose() << endl;

    // v_rotated = q * v;
    v_rotated = rotation_matrix * v;
    cout << "(1, 0, 0) after rotation = " << v_rotated.transpose() << endl;
    cout << "should be equal to " << (q * Quaterniond(0, 1, 0, 0) * q.inverse()) << endl;
    return 0;
}