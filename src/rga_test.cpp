/*
 * @Author: Bedrock
 * @Date: 2022-08-05 17:31:06
 * @LastEditors: Bedrock
 * @LastEditTime: 2022-08-12 11:30:42
 * @Description: 
 */
#include "rga_using_interface.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"


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
/**
 * @brief: 释放图像内存
 * @description: 
 * @param {image_param} *p_src
 * @return {*}
 * @use: 
 */
int release_image_file_buf(struct image_param *p_src)
{
    stbi_image_free(p_src->img_data);
    p_src->img_data = NULL;
    return 0;
}
/**
 * @brief: 从文件中读取图像到内存
 * @description: 
 * @param {char} *file_patch    文件存放路径
 * @param {image_param} *p_src  源数据结构体
 * @return {*}
 * @use: 
 */
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
        p_src->fmt = RK_FORMAT_RGBA_8888;
        //output_bufs_data_to_file("outtest.bin", idata, RK_FORMAT_BGRA_8888, iw, ih);
        break;
    default:
        printf("input img RGB < 3, Please input again\n");
        break;
    }

    // stbi_image_free(idata);

    return 0;
}
/**
 * @brief: 图像缩放接口
 * @description: 
 * @param {image_param} *p_src 源数据结构图
 * @param {image_param} *p_dst  传出数据结构体
 * @return {*}
 * @use: 
 */
int rga_resize_test(struct image_param *p_src, struct image_param *p_dst)
{
    int ret, pix_fmt;
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
    //超过走软件缩放  sample 使用stb缩放，弊端：只支持RGB和灰度格式。
    if(p_dst->width > 4096 || p_dst->heigth > 4096) {
        if(p_dst->fmt == RK_FORMAT_RGB_888) {
            pix_fmt = 3;
            dst_buf = (unsigned char*)malloc(p_dst->width*p_dst->heigth*pix_fmt);
        } else if(p_dst->fmt == RK_FORMAT_RGBA_8888) {
            pix_fmt = 4;
            dst_buf = (unsigned char*)malloc(p_dst->width*p_dst->heigth*pix_fmt);
        }
        
        gettimeofday(&start, NULL);
        stbir_resize(src_buf, p_src->width, p_src->heigth, 0, dst_buf, p_dst->width, p_dst->heigth, 0,
        STBIR_TYPE_UINT8, pix_fmt, STBIR_ALPHA_CHANNEL_NONE, 0, STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
        STBIR_FILTER_BOX, STBIR_FILTER_BOX, STBIR_COLORSPACE_SRGB, NULL);
        gettimeofday(&end, NULL);

        usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        printf("resizing .... cost time %ld us, %s\n", usec1, imStrError(STATUS));
        dst.format = 0;
        dst.wstride = 8192;
        dst.hstride = 8192;
        goto RESIZE_END;
    }
    dst_buf = (unsigned char*)malloc(p_dst->width*p_dst->heigth*get_bpp_from_format(p_dst->fmt));
    

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
RESIZE_END:
    if (dst_buf != NULL) {
        output_bufs_data_to_file(OUTPUT_FILE ,dst_buf, dst.format, dst.wstride, dst.hstride);
        printf("%d %d %d\n", dst.format, dst.wstride, dst.hstride);
        p_dst->img_data = dst_buf;
        //free(dst_buf);
        dst_buf = NULL;
    }
    
    return ret;
}
/**
 * @brief: 图像剪裁接口
 * @description: 
 * @param {image_param} *p_src 输入图像
 * @param {image_param} *p_dst 输出图像
 * @param {im_rect} src_rect   图像剪裁参数
 * @return {*}
 * @use: 
 */
int rga_crop_test(struct image_param *p_src, struct image_param *p_dst, im_rect src_rect)
{
    int ret;
    im_rect 		dst_rect;
    //接口返回值
    IM_STATUS 		STATUS;
    rga_buffer_t 	src;
    rga_buffer_t 	dst;

    unsigned char* src_buf = NULL;
    unsigned char* dst_buf = NULL;

    //memset(&src_rect, 0, sizeof(src_rect));
	memset(&dst_rect, 0, sizeof(dst_rect));
    memset(&src, 0, sizeof(src));
	memset(&dst, 0, sizeof(dst));

    src_buf = p_src->img_data;
    //src_buf = (char*)malloc(p_src->width*p_src->heigth*get_bpp_from_format(p_src->fmt));
    dst_buf = (unsigned char*)malloc(p_dst->width*p_dst->heigth*get_bpp_from_format(p_dst->fmt));
    memset(dst_buf,0x00,p_dst->width*p_dst->heigth*get_bpp_from_format(p_dst->fmt));

    src = wrapbuffer_virtualaddr(src_buf, p_src->width, p_src->heigth, p_src->fmt);
    dst = wrapbuffer_virtualaddr(dst_buf, p_dst->width, p_dst->heigth, p_dst->fmt);
    if(src.width == 0 || dst.width == 0) {
        printf("%s, %s\n", __FUNCTION__, imStrError());
        return ERROR;
    }
    ret = imcheck(src, dst, src_rect, dst_rect, IM_CROP);
    if (IM_STATUS_NOERROR != ret) {
	                printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
	                return -1;
	            }
    gettimeofday(&start, NULL);

    STATUS = imcrop(src, dst, src_rect);

    gettimeofday(&end, NULL);
    usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    printf("cropping .... cost time %ld us, %s\n", usec1, imStrError(STATUS));
    if (dst_buf != NULL) {
        output_bufs_data_to_file(OUTPUT_FILE ,dst_buf, dst.format, dst.wstride, dst.hstride);
        p_dst->img_data = dst_buf;
        //free(dst_buf);
        dst_buf = NULL;
    }
    return 0;
}
class Cvtcolor_rgb2yuv422_Loop : public cv::ParallelLoopBody
{
public:
    // dstimg is yuvimg 
    Cvtcolor_rgb2yuv422_Loop(cv::Mat &srcimg, cv::Mat &dstimg):
    ParallelLoopBody(), src_img(srcimg), dst_img(dstimg)
    {
    }
    
    virtual void operator()(const cv::Range& range) const CV_OVERRIDE
    {
        //printf("%d   %d\n",range.start, range.end);
        for(int row = range.start; row < range.end; row++)
        {   
            for(int col = 0; col < src_img.cols; col+=2)
            {
                cv::Vec3b p0_in = src_img.at<cv::Vec3b>(row, col);
                cv::Vec3b p1_in = src_img.at<cv::Vec3b>(row, col+1);
                cv::Vec2b p0_out, p1_out;
                p0_out.val[0] = p0_in.val[0];
                p0_out.val[1] = p0_in.val[1];
                p1_out.val[0] = p1_in.val[0];
                p1_out.val[1] = p0_in.val[2];
                dst_img.at<cv::Vec2b>(row, col) = p0_out;
                dst_img.at<cv::Vec2b>(row, col+1) = p1_out;
            }
            //printf("%d ", row);
        }
        
    }
    Cvtcolor_rgb2yuv422_Loop& operator=(const Cvtcolor_rgb2yuv422_Loop &) {
        return *this;
    };
private:
    cv::Mat &src_img;
    cv::Mat &dst_img;
};
void cvtcolor_rgb2yuv422(cv::Mat& rgb, cv::Mat& yuv422) {
	cv::Mat yuv444(rgb.rows, rgb.cols, CV_8UC3);
	cv::cvtColor(rgb, yuv444, CV_BGR2YUV);
    // chroma subsampling: yuv444 -> yuv422;

    
    cv::parallel_for_(cv::Range(0, yuv444.rows), [&](const cv::Range& range){
    for (int row = range.start; row < range.end; row++)
    {
        uchar* p_in  = yuv444.ptr(row, 0);
        uchar* p_out = yuv422.ptr(row, 0);

        for (int col = 0; col < yuv444.cols / 2; col++ )
        {
#if 1
            p_out[0] = p_in[0];
            p_out[1] = p_in[1];
            p_out[2] = p_in[3];
            p_out[3] = p_in[2];

            p_in  += 6; // Y1,U1,V1,Y2,U2,V2
            p_out += 4; // Y1,U1,Y2,V1
#else
            p_out[0] =  p_in[0];
            p_out[1] = (p_in[1] >> 1) + ( p_in[4] >> 1);
            p_out[2] =  p_in[3];
            p_out[3] = (p_in[2] >> 1) + ( p_in[5] >> 1);

            p_in  += 6; // Y1,U1,V1,Y2,U2,V2
            p_out += 4; // Y1,(U1+U2)/2,Y2,(V1+V2)/2
#endif
        }
    }
    });

    // cv::parallel_for_(cv::Range(0, yuv444.rows),
    //                 Cvtcolor_rgb2yuv422_Loop(yuv444, yuv));

    // for (int row = 0; row < yuv444.rows; row++) {
    //     for (int col = 0; col < yuv444.cols; col+=2) {
    //     	cv::Vec3b p0_in = yuv444.at<cv::Vec3b>(row, col);
    //     	cv::Vec3b p1_in = yuv444.at<cv::Vec3b>(row, col+1);
    //     	cv::Vec2b p0_out, p1_out;
    //         p0_out.val[0] = p0_in.val[0];
    //         p0_out.val[1] = p0_in.val[1];
    //         p1_out.val[0] = p1_in.val[0];
    //         p1_out.val[1] = p0_in.val[2];
    //         yuv.at<cv::Vec2b>(row, col) = p0_out;
    //         yuv.at<cv::Vec2b>(row, col+1) = p1_out;
    //     }
    // }


}
int rgb2yuv422(cv::Mat& img, struct image_param *yuv422) {



	if (!img.data) {
		std::cout << "Image reading unsuccessful! Exiting.." << std::endl;
		return -1;
	}
	CV_Assert(img.depth() == CV_8U);

	int nRows = img.rows;
	int nCols = img.cols;
	//int nChannels = img.channels();

	int nPixels = nRows * nCols;
	int buffsize;

	if (nPixels % 2 == 0)
		buffsize = 2 * nPixels;
	else
		buffsize = 2 * nPixels + 1;
    
    yuv422->width = img.cols;
    yuv422->heigth = img.rows;
	//yuv422->buf_size = buffsize;

	unsigned char *t = yuv422->img_data;

	float B1, G1, R1;
	unsigned char  Y1, U1, V1;
	unsigned char * p = img.data;

	bool writeFull = true;

	//This loop converts RGB image to YUV 4:2:2
	for (unsigned int i = 0; i!= uint(nPixels); ++i) {
		B1 = float(*p++);
		G1 = float(*p++);
		R1 = float(*p);

		Y1 = uchar(round(0.257 * R1 + 0.504 * G1 + 0.098 * B1 + 16));
		U1 = uchar(round(-0.148 * R1 - 0.291 * G1 + 0.439 * B1 + 128));
		V1 = uchar(round(0.439 * R1 - 0.368 * G1 - 0.071 * B1 + 128));

		if (writeFull) {
			*t++ = U1;
			*t++ = Y1;
			*t = V1;
		} else
			*t = Y1;

		writeFull = !writeFull;
		if (i < uint(nPixels)-1){
		t++;
		p++;
		}
	}
/*
    uchar *buff_pointer = (uchar*)yuv422->buf;
	cout << "(function) Writing YUV 4:2:2 image data: " << endl;
	for (unsigned int i = 0; i!=uint(yuv422->buf_size); ++i){
		cout << int(*buff_pointer) << endl;
		buff_pointer++;
	}
*/
	return 0;
}