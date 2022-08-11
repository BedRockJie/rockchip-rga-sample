/*
 * @Author: Bedrock
 * @Date: 2022-08-05 17:19:30
 * @LastEditors: Bedrock
 * @LastEditTime: 2022-08-11 10:18:18
 * @Description: 
 */
#ifndef __RGA_USING_INTERFACE_H__
#define __RGA_USING_INTERFACE_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>

#include "rga.h"
#include "RockchipRga.h"
#include "RgaUtils.h"
#include "im2d.hpp"
#include "im2d.h"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"


#define ERROR               -1


struct image_param{
    int width;
    int heigth;
    RgaSURF_FORMAT fmt;
    unsigned char *img_data;
};
int read_image_from_file(char *file_patch, struct image_param *p_src);
int release_image_file_buf(struct image_param *p_src);
int rga_resize_test(struct image_param *p_src, struct image_param *p_dst);
int rga_crop_test(struct image_param *p_src, struct image_param *p_dst, im_rect src_rect);

void cvtcolor_rgb2yuv422(cv::Mat& rgb, cv::Mat& yuv);
int rgb2yuv422(cv::Mat& img, struct image_param *yuv422);
#endif // RGA_USING_INTERFACE_H