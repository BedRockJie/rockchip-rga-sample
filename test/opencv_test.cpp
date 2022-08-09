/*
 * @Author: Bedrock
 * @Date: 2022-08-08 20:26:37
 * @LastEditors: Bedrock
 * @LastEditTime: 2022-08-09 21:17:18
 * @Description: 
 */
#include "opencv2/opencv.hpp"
#include "iostream"
#include <sys/time.h>
using namespace cv;


int main(int argc, char const *argv[])
{
    struct timeval start, end;
    long usec1;
    Mat img = imread("9.bmp");
    std::cout << img.size << std::endl;

    std::cout<<"image width:\t"<<img.cols<<"\theigth:\t"<<img.rows<<"\tformat:"<<img.channels()<<std::endl;

    Size outsize = Size(8192,8192);
    //Size outsize = Size(4096,4096);
    Mat outimg;

    gettimeofday(&start, NULL);
    resize(img,outimg,outsize,0,0,INTER_AREA);
    gettimeofday(&end, NULL);
    usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    printf("resizing .... cost time %ld us\n", usec1);
    Mat yuvimg;
    int buflen = img.cols * img.rows * 2;
    unsigned char* yuvbuf = new unsigned char[buflen];

    //CV_BGR2YUV_IYUV
    cvtColor(outimg, yuvimg, COLOR_BGR2YUV_IYUV);
    FILE* pfile=fopen("img.yuv", "wb");
    memcpy(yuvbuf, yuvimg.data, buflen * sizeof(unsigned char));
    fwrite(yuvbuf, buflen * sizeof(unsigned char), 1, pfile);
	fclose(pfile);
    // outimg.data
    //imwrite("output.png", yuvimg);
    return 0;
}
