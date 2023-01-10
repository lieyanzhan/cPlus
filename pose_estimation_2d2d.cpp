#include <iostream>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/calib3d.hpp"

using namespace std;
using namespace cv;

void find_feature_matches(const Mat &img1, const Mat &img2, vector<KeyPoint>& keypoint1, 
                        vector<KeyPoint>& keypoint2, vector<DMatch>& matches);

void post_estimation_2d2d(vector<KeyPoint>& keypoint1, vector<KeyPoint>& keypoint2, vector<DMatch>& matches, Mat R, Mat t);

Point2d pixel2cam(const Point2d &p, const Mat &K);

int main(){
    cout << "start pose estimation!" << endl;
    Mat img1 = imread("./data/1.png");
    Mat img2 = imread("./data/2.png");
    if (img1.empty() || img2.empty()) {
        cout << "image is empty!" << endl;
        return 0;
    }
    vector<KeyPoint> keypoint1, keypoint2;
    vector<DMatch> matches;
    find_feature_matches(img1, img2, keypoint1, keypoint2, matches);

    // 估计运动
    Mat R, t;
    post_estimation_2d2d(keypoint1, keypoint2, matches, R, t);

    // cv::Mat image_show2;
    // cv::drawMatches(img1, keypoint1, img2, keypoint2, matches, image_show2);
    // cv::imshow("matches_self", image_show2);
    // cv::waitKey(0);
    return 1;
}

void find_feature_matches(const Mat &img1, const Mat &img2, vector<KeyPoint>& keypoint1, 
                        vector<KeyPoint>& keypoint2, vector<DMatch>& matches){
    Mat descriptors_1, descriptors_2;
    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> descriptor = ORB::create();
   
    int threshold = 40;
    cv::FAST(img1, keypoint1, threshold);
    cv::FAST(img2, keypoint2, threshold);

    // cal BRIEF
    descriptor->compute(img1, keypoint1, descriptors_1);
    descriptor->compute(img2, keypoint2, descriptors_2);


    vector<DMatch> tmpMatch;
    // match
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors_1, descriptors_2, tmpMatch);

    // 匹配点筛选
    // 256 BRIEF
    double min_dist = 10000, max_dist = 0;
    for (int i=0; i < tmpMatch.size(); ++i) {
        double dist = tmpMatch[i].distance;
        if (dist < min_dist) {
            min_dist = dist;
        }
        if(dist > max_dist) {
            max_dist = dist;
        }
    }

    printf("-- Max dist : %f \n", max_dist);
    printf("-- Min dist : %f \n", min_dist);

    // 筛选高质量匹配点
    for(int i = 0;i<tmpMatch.size();++i) { 
        // 比较时类型必相同
        if (tmpMatch[i].distance <= max(2*min_dist, 30.0)) {
            matches.push_back(tmpMatch[i]);
        }
    }
    cout << matches.size() << endl;
}

// 像素坐标转为归一化坐标
Point2d pixel2cam(const Point2d &p, const Mat &K){
    return Point2d
    (
      (p.x - K.at<double>(0, 2)) / K.at<double>(0, 0),
      (p.y - K.at<double>(1, 2)) / K.at<double>(1, 1)
    );
}


/// @brief pose
/// @param keypoint1 
/// @param keypoint2 
/// @param matches 
/// @param R 
/// @param t 
void post_estimation_2d2d(vector<KeyPoint>& keypoint1, vector<KeyPoint>& keypoint2, 
                        vector<DMatch>& matches, Mat R, Mat t){
    // K
    Mat k = (Mat_<double>(3, 3) << 520.9, 0, 325.1, 
                                   0, 521.0, 249.7,
                                   0,   0,   1);
    cout << "K: " << endl << k << endl;
    vector<Point2f> point1;
    vector<Point2f> point2;
    for (int i = 0;i< matches.size(); ++i) {
        point1.push_back(keypoint1[matches[i].queryIdx].pt);
        point2.push_back(keypoint2[matches[i].queryIdx].pt);
    }
    // 计算基础矩阵
    Mat fundamental_matrix;
    fundamental_matrix = findFundamentalMat(point1, point2, FM_8POINT);
    cout << "fundamental_matrix is " << endl << fundamental_matrix << endl;

    // 计算本质矩阵
    Point2d principal_point(325.1, 249.7); // 相机光心, TUM dataset标定值
    // 相机焦距
    double f = 521;
    Mat essential_matrix = findEssentialMat(point1, point2, f, principal_point);
    cout << "essential_matrix is " << endl << essential_matrix << endl;

    // 计算单应矩阵
    Mat homography = findHomography(point1, point2, RANSAC, 3);
    cout << "homography is " << endl << homography << endl;

    // 本质矩阵恢复参数
    recoverPose(essential_matrix, point1, point2, R, t, f, principal_point);
    cout << "R is: " << endl << R << endl;
    cout << "t is: " << endl << t << endl;

    // 验证E t^R
    Mat t_x = (Mat_<double>(3, 3) << 0, -t.at<double>(2, 0), t.at<double>(1, 0), 
                                    t.at<double>(2, 0), 0, -t.at<double>(0,0), 
                                    -t.at<double>(1, 0), t.at<double>(0,0), 0);
                                    
    cout << "E = t^ * R: " << endl << t_x * R << endl;
    cout << "essential_matrix" << endl << essential_matrix << endl;
    
    // 验证对极约束
    for(DMatch m :matches) {
        Point2d pt1  = pixel2cam(keypoint1[m.queryIdx].pt, k);
        Mat y1 = (Mat_<double>(3, 1) << pt1.x, pt1.y, 1);

        Point2d pt2 = pixel2cam(keypoint2[m.queryIdx].pt, k);
        Mat y2 = (Mat_<double>(3, 1) << pt2.x, pt2.y, 1);
        
        // 对极约束: x2' * t^R * x1 = 0
        Mat d = y2.t() * t_x * R * y1;
        
        cout << "epipolar constraint: " << d << endl;
    }
    return;
}