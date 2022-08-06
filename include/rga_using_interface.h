/*
 * @Author: Bedrock
 * @Date: 2022-08-05 17:19:30
 * @LastEditors: Bedrock
 * @LastEditTime: 2022-08-05 18:07:01
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

#include "rga.h"
#include "RockchipRga.h"
#include "RgaUtils.h"
#include "im2d.hpp"
#include "im2d.h"

#define ERROR               -1


int rga_resize_test(char *filepatch, struct image_param *p_src, struct image_param *p_dst);
#endif // RGA_USING_INTERFACE_H