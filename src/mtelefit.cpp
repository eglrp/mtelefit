/*
 Name        : mtelefit.cpp
 Author      : Xiaomeng Lu
 Version     : 0.1
 Copyright   : SVOM Group, NAOC
 Description : 单转台多镜头系统坐标拟合

 @note
 建立坐标转换系统, 实现由某一个镜头中心WCS坐标和其它镜头XY坐标, 计算
 该XY位置对应的WCS坐标

 @note
 定义:
 1. 两个镜头参考中心的转换关系(旋转)参数
   镜头0中心位置: (x0, y0); 中心坐标(alpha_0, delta_0)
   镜头1中心位置: (x1, y1); 中心坐标(alpha_1, delta_1)
   (alpha_0, delta_0; alpha_1, delta_1) ==> (lamda, beta)
   (alpha_0, delta_0; lamda, beta) ==> (alpha_1, delta_1)

 2. 镜头1中待测量位置与该镜头中心的关系(旋转)参数
   待测量位置: (x, y); 待测量坐标(alpha, delta)
   (alpha_1, delta_1; alpha, delta) ==> (fai, theta)
   (alpha_1, delta_1; fai, theta) ==> (alpha, delta)

 3. 待测量位置相对坐标的投影参数
   (fai, theta) ==> (xi, eta)
   (xi, eta) ==> (fai, theta)

 4. 待测量位置投影参数与XY坐标系的拟合关系
   (xi, eta)<--(x, y; x0, y0)

 @note
 建立模型:
   (alpha_0, delta_0; alpha_1, delta_1) ==> (lamda, beta)
   (alpha_1, delta_1; alpha, delta) ==> (fai, theta)
   (fai, theta) ==> (xi, eta)
   (xi, eta)<--(x, y; x0, y0)
             f

 @note
 计算XY对应的WCS坐标
                  f
   (x, y; x0, y0)-->(xi, eta)
   (xi, eta)==>(fai, theta)
   (alpha_0, delta_0; lamda, beta)==>(alpha_1, delta_1)
   (alpha_1, delta_1; fai, theta)==>(alpha, delta)
 */

#include <stdio.h>
#include <string>

using std::string;

int main(int argc, char** argv) {

	return 0;
}
