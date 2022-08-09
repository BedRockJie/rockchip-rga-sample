<!--
 * @Author: Bedrock
 * @Date: 2022-08-09 10:49:07
 * @LastEditors: Bedrock
 * @LastEditTime: 2022-08-09 11:03:46
 * @Description: 
-->
# rockchip-rga-sample

`RGA (Raster Graphic Acceleration Unit)是一个独立的2D硬件加速器，可用于加速点/线绘制，执行图像缩放、旋转、bitBlt、alpha混合等常见的2D图形操作。`

这是一个调用Rockchip RGA硬件加速接口的例子。

可以通过 Opencv 或 libstb 库实现图片读取加载。并通过RGA实现图片的硬件加速处理。


# 使用方法

参考 aarch64.toolchain.cmake 配置设置交叉编译链环境。  已在RK3566、RK3568、RK3588平台测试。

# 说明
`main` 为调用 `tsb` 解码图片。
`test\opencv_test` 为调用opencv解码图片

# 小技巧
Opencv库没有自己编译，直接使用的rknn中依赖的Opencv，使用方法见 `test` 中CmakeLists.