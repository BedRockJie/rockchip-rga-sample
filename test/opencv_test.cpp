/*
 * @Author: Bedrock
 * @Date: 2022-08-08 20:26:37
 * @LastEditors: Bedrock
 * @LastEditTime: 2022-08-08 21:12:43
 * @Description: 
 */
#include "opencv2/opencv.hpp"
#include "iostream"
using namespace cv;


int main(int argc, char const *argv[])
{
    Mat img = imread("18.jpg", IMREAD_UNCHANGED);
    std::cout << img.size << std::endl;

    std::cout<<"image width:\t"<<img.cols<<"\theigth:\t"<<img.rows<<"\tformat:"<<img.channels()<<std::endl;
    return 0;
}
