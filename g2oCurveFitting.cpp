#include <iostream>
#include <istream>
#include <g2o/core/g2o_core_api.h>
#include <g2o/core/base_vertex.h>
#include <g2o/core/base_unary_edge.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/core/optimization_algorithm_dogleg.h>
#include <g2o/solvers/dense/linear_solver_dense.h>
#include <Eigen/Core>
#include <opencv2/core/core.hpp>
#include <cmath>
#include <chrono>

using namespace std;
using namespace Eigen;
using namespace g2o;

// 定义图结点, 需要继承于BaseVertex类
// BaseVertex<int D, typename T> D: 结点的最小维度，T用来表示估计值的类型
class CurveFittingVertex : public g2o::BaseVertex<3, Eigen::Vector3d> {
public:
  // Eigen对齐: https://zhuanlan.zhihu.com/p/389391536
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  
  // 设置初始值
  virtual void setToOriginImpl() override {
    _estimate << 0,0,0;
  }

  // 更新函数
  virtual void oplusImpl(const double* update) override {
    // 强制类型转换
    _estimate += Eigen::Vector3d(update);
  }

  virtual bool read(std::istream& is) override {
  }

  virtual bool write(std::ostream& os) const override {
  }
};

// BaseUnaryEdge: 基本一元边
// 误差模型
class CurveFittingEdge : public g2o::BaseUnaryEdge<1, double, CurveFittingVertex> {
public:    
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    CurveFittingEdge(double x) : BaseUnaryEdge(), _x(x){};

    // 计算模型误差
    virtual void computeError() override {
        const CurveFittingVertex *v = static_cast<const CurveFittingVertex *>(_vertices[0]);
        const Eigen::Vector3d abc = v->estimate();
        _error[0] = _measurement - std::exp(abc(0, 0) * _x * _x + abc(1, 0) * _x + abc(2, 0));
    }

    // 定义加法，计算雅克比
    virtual void linearizeOplus() override {
        const CurveFittingVertex *v = static_cast<const CurveFittingVertex *>(_vertices[0]);
        const Eigen::Vector3d abc = v->estimate();
        double y = exp(abc[0] * _x * _x + abc[1] * _x + abc[2]);
        _jacobianOplusXi[0] = -_x*_x*y;
        _jacobianOplusXi[1] = -_x*y;
        _jacobianOplusXi[2] = -y;
    }

    virtual bool read(istream& is) override {}
    virtual bool write(ostream& os) const override {}
public:
    double _x; // x值
};


int main() {
    double ar = 1.0, br = 2.0, cr = 1.0;         // 真实参数值
    double ae = 2.0, be = -1.0, ce = 5.0;        // 估计参数值
    int N = 100;                                 // 数据点
    double w_sigma = 1.0;                        // 噪声Sigma值
    double inv_sigma = 1.0 / w_sigma;
    cv::RNG rng;                                 // OpenCV随机数产生器

    vector<double> x_data, y_data;      // 数据
    for (int i = 0; i < N; i++) {
        double x = i / 100.0;
        x_data.push_back(x);
        y_data.push_back(exp(ar * x * x + br * x + cr) + rng.gaussian(w_sigma * w_sigma));
    }
    // BlockSolverTraits<误差项优化变量维度, 误差值维度> 
    typedef g2o::BlockSolver<g2o::BlockSolverTraits<3,1>> BlockSolverType;
    // 定义线性求解器类型
    typedef g2o::LinearSolverDense<BlockSolverType::PoseMatrixType> LinearSolverType; 
    // make_unique 返回一个指向该对象的智能指针
    auto solver = new g2o::OptimizationAlgorithmGaussNewton(g2o::make_unique<BlockSolverType>(g2o::make_unique<LinearSolverType>()));

    // 图模型
    g2o::SparseOptimizer optimizer;
    optimizer.setAlgorithm(solver);
    // 打开调试
    optimizer.setVerbose(false);

    // 向图中增加顶点，顶点为待优化变量
    CurveFittingVertex *v = new CurveFittingVertex();
    v->setEstimate(Eigen::Vector3d(ae, be, ce));
    v->setId(0);
    optimizer.addVertex(v);

    // 往图中增加边, 误差为边
    for (int i = 0; i < N; i++) {
        CurveFittingEdge *edge = new CurveFittingEdge(x_data[i]);
        edge->setId(i);
        // 该模型中只有一个顶点
        edge->setVertex(0, v);                // 设置连接的顶点
        edge->setMeasurement(y_data[i]);      // 观测数值
        edge->setInformation(Eigen::Matrix<double, 1, 1>::Identity() * 1 / (w_sigma * w_sigma)); // 信息矩阵：协方差矩阵之逆
        optimizer.addEdge(edge);
    }

    // 执行优化
    cout << "start optimization" << endl;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    optimizer.initializeOptimization();
    optimizer.optimize(10);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << "solve time cost = " << time_used.count() << " seconds. " << endl;

    // 输出优化值
    Eigen::Vector3d abc_estimate = v->estimate();
    cout << "estimated model: " << abc_estimate.transpose() << endl;
    return 0;
}