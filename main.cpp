/*
 * @Author: Bedrock
 * @Date: 2022-08-05 15:37:35
 * @LastEditors: Bedrock
 * @LastEditTime: 2022-08-08 16:07:10
 * @Description: 
 */
#include <stdio.h>
#include "rga_using_interface.h"

int main(int argc, char const *argv[])
{
    char file_name[128] = {0};
    struct image_param input_img_param;
    struct image_param output_img_param;
    printf("Hello word!\n");
    
    if(argc == 3)
    {
        strcpy(file_name, argv[2]);
        read_image_from_file(file_name, &input_img_param);

        output_img_param.fmt = RK_FORMAT_YUYV_422;
        output_img_param.width = 1920;
        output_img_param.heigth = 1080;
        rga_resize_test(&input_img_param, &output_img_param);
        release_image_file_buf(&input_img_param);
    }
    
    
    // if(argc == 2)
    // {
    //     strcpy(file_name, argv[1]);
    //     input_img_param.fmt = RK_FORMAT_RGBA_8888;
    //     input_img_param.width = 1280;
    //     input_img_param.heigth = 720;

    //     output_img_param.fmt = RK_FORMAT_YUYV_422;
    //     output_img_param.width = 1920;
    //     output_img_param.heigth = 1080;
    //     rga_resize_test(file_name, &input_img_param, &output_img_param);
        
    // }
    return 0;
}
