/*
 * @Author: Bedrock
 * @Date: 2022-08-05 17:31:06
 * @LastEditors: Bedrock
 * @LastEditTime: 2022-08-05 20:57:40
 * @Description: 
 */
#include "rga_using_interface.h"

struct image_param{
    int width;
    int heigth;
    RgaSURF_FORMAT fmt;
};


int get_buf_size_by_w_h_f(int w, int h, int f)
{
    float bpp = get_bpp_from_format(f);
    int size = 0;

    size = (int)w * h * bpp;
    return size;
}
/**
 * @brief: 
 * @description: 
 * @param {char} *file_patch    文件路径
 * @param {void} *buf           内存指针
 * @param {int} f               图像格式
 * @param {int} sw              图像宽度
 * @param {int} sh              图像高度
 * @return {*}
 * @use: 
**/
int get_buf_from_file(char *file_patch, void *buf, int f, int sw, int sh)
{
    //char file_patch[100];
    int ret = 0;

    FILE *file = fopen(file_patch, "rb");
    if (!file) {
        fprintf(stderr, "Could not open %s\n", file_patch);
        return -EINVAL;
    }
    fread(buf, get_buf_size_by_w_h_f(sw, sh, f), 1, file);
    fclose(file);

    return ret;
}
int rga_resize_test(char *filepatch, struct image_param *p_src, struct image_param *p_dst)
{
    int ret;
    //接口返回值
    IM_STATUS 		STATUS;
    rga_buffer_t 	src;
    rga_buffer_t 	dst;
    
    char* src_buf = NULL;
    char* dst_buf = NULL;

    memset(&src, 0, sizeof(src));
	memset(&dst, 0, sizeof(dst));

    src_buf = (char*)malloc(p_src->width*p_src->heigth*get_bpp_from_format(p_src->fmt));
    dst_buf = (char*)malloc(p_dst->width*p_dst->heigth*get_bpp_from_format(p_dst->fmt));

    ret = get_buf_from_file(filepatch, src_buf, p_src->fmt, p_src->width, p_src->heigth);

    memset(dst_buf,0x00,p_dst->width*p_dst->heigth*get_bpp_from_format(p_dst->fmt));

    src = wrapbuffer_virtualaddr(src_buf, p_src->width, p_src->heigth, p_src->fmt);
    dst = wrapbuffer_virtualaddr(dst_buf, p_dst->width, p_dst->heigth, p_dst->fmt);

    if(src.width == 0 || dst.width == 0) {
        printf("%s, %s\n", __FUNCTION__, imStrError());
        return ERROR;
    }
    
    return ret;
}