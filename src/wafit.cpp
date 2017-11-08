/*!
 * @file wafit.cpp 大视场星象位置拟合, 定义文件
 * @version 0.1
 * @date 2017年11月8日
 * @note
 * 建立投影坐标系与图像坐标系的匹配拟合关系
 */

#include "wafit.h"
#include "AMath.h"

using namespace AstroUtil;

void image_object::calc_relpos(double ra0, double dc0) {
	ProjectForward(ra0, dc0, ra, dc, xi, eta);
}
