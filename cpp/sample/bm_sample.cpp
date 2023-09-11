//
// Created by Tunm-Air13 on 2023/9/11.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include "log.h"
#include "recognition_module/simd.h"
//#include <Eigen/Dense>

int main() {

    int N = 512;
    int vectorSize = 512; // 向量的长度
    {
        // 创建一个 Nx512 的 CV_32F 类型的矩阵，并填充随机数
        cv::Mat mat(N, vectorSize, CV_32F);
        cv::randu(mat, cv::Scalar(0), cv::Scalar(1)); // 在 [0, 1) 范围内生成随机浮点数

        // 创建一个 512x1 的 CV_32F 类型的矩阵，并填充随机数
        cv::Mat one(vectorSize, 1, CV_32F);
        cv::randu(one, cv::Scalar(0), cv::Scalar(1)); // 在 [0, 1) 范围内生成随机浮点数

        std::cout << mat.size << std::endl;
        std::cout << one.size << std::endl;

        auto timeStart = (double) cv::getTickCount();

        cv::Mat cosineSimilarities;
        cv::gemm(mat, one, 1, cv::Mat(), 0, cosineSimilarities);

        double cost = ((double) cv::getTickCount() - timeStart) / cv::getTickFrequency() * 1000;
        LOGD("Matrix COST: %f", cost);

    }

    {
        // 设置随机数种子
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        // 创建一个使用 std::vector<std::vector<float>> 表示的矩阵，并填充随机数
        std::vector<std::vector<float>> matrix(N, std::vector<float>(vectorSize));
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < vectorSize; ++j) {
                matrix[i][j] = static_cast<float>(std::rand()) / RAND_MAX; // 生成随机浮点数
            }
        }

        // 创建一个使用 std::vector<float> 表示的向量，并填充随机数
        std::vector<float> vectorOne(vectorSize);
        for (int i = 0; i < vectorSize; ++i) {
            vectorOne[i] = static_cast<float>(std::rand()) / RAND_MAX; // 生成随机浮点数
        }

        auto timeStart = (double) cv::getTickCount();
        // dot
        for (const auto &v: matrix) {
            simd_dot(v.data(), vectorOne.data(), vectorSize);
        }

        double cost = ((double) cv::getTickCount() - timeStart) / cv::getTickFrequency() * 1000;
        LOGD("Vector COST: %f", cost);
    }

//    {
//        Eigen::initParallel();
//        // 创建一个 Nx512 的矩阵，并填充随机数
//        Eigen::MatrixXf mat(N, vectorSize);
//        mat = Eigen::MatrixXf::Random(N, vectorSize);
//
//        std::cout << mat.rows() << " x " << mat.cols() << std::endl;
//
//
//        Eigen::VectorXf one(vectorSize);
//        one = Eigen::VectorXf::Random(vectorSize);
//
//        auto timeStart = (double) cv::getTickCount();
//        Eigen::VectorXf result = mat * one;
//
//        double cost = ((double) cv::getTickCount() - timeStart) / cv::getTickFrequency() * 1000;
//        LOGD("Eigen COST: %f", cost);
//    }

    return 0;
}