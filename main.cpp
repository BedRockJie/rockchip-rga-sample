/*
 * @Author: Bedrock
 * @Date: 2022-08-05 15:37:35
 * @LastEditors: Bedrock
 * @LastEditTime: 2022-08-10 09:35:31
 * @Description: 
 */
#include <stdio.h>

#include "argparse.h"
#include "rga_using_interface.h"

using namespace cv;

static const char *const usages[] = {
    "./bedrock_rga_test [-w 1920 -h 1080 -i [file_name]]...",
    NULL,
};

int main(int argc, char const *argv[])
{
    int mode, pix_fmt;;
    const char *file_name;
    char file[128] = {0};
    struct image_param input_img_param;
    struct image_param output_img_param;
    im_rect 		src_rect;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_INTEGER('w', "width", &(output_img_param.width),
                    "get width ", NULL, 0, 0),
        OPT_INTEGER('h', "height", &(output_img_param.heigth),
                    "get height", NULL, 0, 0),
        OPT_INTEGER('m', "mode", &(mode),
                    "image processing mode eg: 1(resize) 2(crop)", NULL, 0, 0),
        OPT_INTEGER('p', "pixsize", &(pix_fmt),
                    "yuv pix size", NULL, 0, 0),
        OPT_STRING('i', "file_name", &(file_name),
                   "file_name.png or xxxxx.jpg", NULL, 0, 0),
        OPT_END(),
    };
    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");
    argc = argparse_parse(&argparse, argc, argv);

    printf("%d %d mode:%d pix:%d %s\n", output_img_param.width, output_img_param.heigth,
                                        mode, pix_fmt, file_name);
    if(output_img_param.heigth < 2 || output_img_param.width < 2 ||
        file_name == NULL) {
        goto FAILD_OUT;
    }
    if(mode == 1) {
        strcpy(file, file_name);
        if(output_img_param.width > 4096 || output_img_param.heigth > 4096) {
            /*todo 是否需要读取透明度信息，传递参数，默认只读取RGB信息*/
            struct timeval start, end;
            long usec1;
            Mat input_img = imread(file);
            input_img_param.width = input_img.cols;
            input_img_param.heigth = input_img.rows;
            int fmt = input_img.channels();
            std::cout<<"image width:\t"<<input_img.cols<<"\theigth:\t"<<input_img.rows<<"\tformat:"<<input_img.channels()<<std::endl;
            if(fmt == 3)
                input_img_param.fmt = RK_FORMAT_RGB_888;
            else if(fmt == 4)
                input_img_param.fmt = RK_FORMAT_RGBA_8888;

            Size outsize = Size(output_img_param.width, output_img_param.heigth);
            Mat output_img;

            gettimeofday(&start, NULL);
            resize(input_img, output_img, outsize, 0, 0, INTER_AREA);
            gettimeofday(&end, NULL);
            usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
            printf("resizing .... cost time %ld us\n", usec1);
            
            int buflen = 0;
            if(pix_fmt == 1)
                buflen = output_img.cols * output_img.rows * 1.5;
            else
                buflen = output_img.cols * output_img.rows * pix_fmt;
            input_img_param.img_data = new unsigned char[buflen];
            Mat yuvimg(output_img.rows, output_img.cols, CV_8UC2);
            printf("resize done\n");
            gettimeofday(&start, NULL);
            if(pix_fmt == 1) {
                cvtColor(output_img, yuvimg, COLOR_BGR2YUV_IYUV);
            } else if(pix_fmt == 3) {
                cvtColor(output_img, yuvimg, COLOR_BGR2YUV);
            } else if(pix_fmt == 2){
                cvtcolor_rgb2yuv422(output_img, yuvimg);
                //rgb2yuv422(output_img, &input_img_param);
            } else {
                std::cout << "pix_fmt faild" << std::endl;
            }
            gettimeofday(&end, NULL);
            usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
            printf("cvtcolor yuv422 .... cost time %ld us\n", usec1);
            FILE* pfile=fopen("img.yuv", "wb");
            memcpy(input_img_param.img_data, yuvimg.data, buflen * sizeof(unsigned char));
            fwrite(input_img_param.img_data, buflen * sizeof(unsigned char), 1, pfile);
            fclose(pfile); 
            delete input_img_param.img_data;

        } else {
            read_image_from_file(file, &input_img_param);

            output_img_param.fmt = RK_FORMAT_RGBA_8888;
            // output_img_param.width = 3840;
            // output_img_param.heigth = 2160;
            rga_resize_test(&input_img_param, &output_img_param);
            release_image_file_buf(&input_img_param);
            release_image_file_buf(&output_img_param);
        }
    } 
    else if(mode == 2) {
        memset(&src_rect, 0, sizeof(src_rect));
        strcpy(file, file_name);
        read_image_from_file(file, &input_img_param);
        output_img_param.fmt = RK_FORMAT_YUYV_422;
        src_rect.width = 100;
        src_rect.height = 100;
        src_rect.x = 0;
        src_rect.y = 0;
        rga_crop_test(&input_img_param, &output_img_param, src_rect);
        release_image_file_buf(&input_img_param);
        release_image_file_buf(&output_img_param);
    }
    
    return 0;

FAILD_OUT:
    printf("input Error exit!\n"); 
    return -1;
}
