/*
 * @Author: Bedrock
 * @Date: 2022-08-05 17:31:06
 * @LastEditors: Bedrock
 * @LastEditTime: 2022-08-08 16:11:41
 * @Description: 
 */
#include "rga_using_interface.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define OUTPUT_FILE "/root/out1.bin"

struct timeval start, end;
long usec1;

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
int get_bufs_from_file(char *file_patch, void *buf, int f, int sw, int sh)
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
int output_bufs_data_to_file(char *file_patch, void *buf, int f, int sw, int sh)
{
    FILE *file = fopen(file_patch, "wb+");
    if (!file) {
        fprintf(stderr, "Could not open %s\n", file_patch);
        return false;
    } else
        fprintf(stderr, "open %s and write ok\n", file_patch);

    fwrite(buf, get_buf_size_by_w_h_f(sw, sh, f), 1, file);
    fclose(file);

    return 0;
}
int release_image_file_buf(struct image_param *p_src)
{
    stbi_image_free(p_src->img_data);
    p_src->img_data = NULL;
    return 0;
}
int read_image_from_file(char *file_patch, struct image_param *p_src)
{
    int iw,ih,n;
    p_src->img_data = stbi_load(file_patch, &iw, &ih, &n, 0);
    printf(
        "This image :  width: %d \n \r heigth: %d \n \r format: %d\n\r", iw, ih, n);
    printf("\n");
    
    switch (n)
    {
    case 3:
        p_src->width = iw;
        p_src->heigth = ih;
        p_src->fmt = RK_FORMAT_RGB_888;
        //output_bufs_data_to_file("outtest.bin", idata, RK_FORMAT_RGB_888, iw, ih);
        break;
    case 4:
        p_src->width = iw;
        p_src->heigth = ih;
        p_src->fmt = RK_FORMAT_BGRA_8888;
        //output_bufs_data_to_file("outtest.bin", idata, RK_FORMAT_BGRA_8888, iw, ih);
        break;
    default:
        printf("input img RGB < 3, Please input again\n");
        break;
    }

    // stbi_image_free(idata);

    return 0;
}

int rga_resize_test(struct image_param *p_src, struct image_param *p_dst)
{
    int ret;
    im_rect 		src_rect;
    im_rect 		dst_rect;
    //接口返回值
    IM_STATUS 		STATUS;
    rga_buffer_t 	src;
    rga_buffer_t 	dst;
    
    unsigned char* src_buf = NULL;
    unsigned char* dst_buf = NULL;

    memset(&src_rect, 0, sizeof(src_rect));
	memset(&dst_rect, 0, sizeof(dst_rect));
    memset(&src, 0, sizeof(src));
	memset(&dst, 0, sizeof(dst));

    src_buf = p_src->img_data;
    //src_buf = (char*)malloc(p_src->width*p_src->heigth*get_bpp_from_format(p_src->fmt));
    dst_buf = (unsigned char*)malloc(p_dst->width*p_dst->heigth*get_bpp_from_format(p_dst->fmt));

    //ret = get_bufs_from_file(filepatch, src_buf, p_src->fmt, p_src->width, p_src->heigth);

    memset(dst_buf,0x00,p_dst->width*p_dst->heigth*get_bpp_from_format(p_dst->fmt));

    src = wrapbuffer_virtualaddr(src_buf, p_src->width, p_src->heigth, p_src->fmt);
    dst = wrapbuffer_virtualaddr(dst_buf, p_dst->width, p_dst->heigth, p_dst->fmt);

    if(src.width == 0 || dst.width == 0) {
        printf("%s, %s\n", __FUNCTION__, imStrError());
        return ERROR;
    }
    
    ret = imcheck(src, dst, src_rect, dst_rect);
    if (IM_STATUS_NOERROR != ret) {
	                printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
	                return -1;
	            }

	gettimeofday(&start, NULL);

    STATUS = imresize(src, dst);

    gettimeofday(&end, NULL);

    usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    printf("resizing .... cost time %ld us, %s\n", usec1, imStrError(STATUS));

    // if (src_buf != NULL) {
    //     free(src_buf);
    //     src_buf = NULL;
    // }
    if (dst_buf != NULL) {
        output_bufs_data_to_file(OUTPUT_FILE ,dst_buf, dst.format, dst.wstride, dst.hstride);
        free(dst_buf);
        dst_buf = NULL;
    }
    
    return ret;
}

