/*
 * @Author: Bedrock
 * @Date: 2022-08-05 15:37:35
 * @LastEditors: Bedrock
 * @LastEditTime: 2022-08-09 10:08:22
 * @Description: 
 */
#include <stdio.h>

#include "argparse.h"
#include "rga_using_interface.h"

static const char *const usages[] = {
    "./bedrock_rga_test [-w 1920 -h 1080 -i [file_name]]...",
    NULL,
};

int main(int argc, char const *argv[])
{
    int mode;
    const char *file_name;
    char file[128] = {0};
    struct image_param input_img_param;
    struct image_param output_img_param;
    im_rect 		src_rect;
    printf("Hello word!\n");
    
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_INTEGER('w', "width", &(output_img_param.width),
                    "get width ", NULL, 0, 0),
        OPT_INTEGER('h', "height", &(output_img_param.heigth),
                    "get height", NULL, 0, 0),
        OPT_INTEGER('m', "mode", &(mode),
                    "image processing mode eg: \n\r1(resize) \n\r2(crop)", NULL, 0, 0),
        OPT_STRING('i', "file_name", &(file_name),
                   "file_name.png or xxxxx.jpg", NULL, 0, 0),
        OPT_END(),
    };
    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");
    argc = argparse_parse(&argparse, argc, argv);

    printf("%d %d %s\n", output_img_param.width, output_img_param.heigth, file_name);
    if(output_img_param.heigth < 2 || output_img_param.width < 2 ||
        file_name == NULL) {
        goto FAILD_OUT;
    }
    if(mode == 1) {
        strcpy(file, file_name);
        read_image_from_file(file, &input_img_param);

        output_img_param.fmt = RK_FORMAT_YUYV_422;
        // output_img_param.width = 3840;
        // output_img_param.heigth = 2160;
        rga_resize_test(&input_img_param, &output_img_param);
        release_image_file_buf(&input_img_param);
        release_image_file_buf(&output_img_param);
    } else if(mode == 2) {
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
