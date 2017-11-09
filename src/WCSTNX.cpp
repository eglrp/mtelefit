/*
 * @file WCSTNX.cpp 定义文件, 基于非标准WCS格式TNX, 计算图像坐标与WCS坐标之间的对应关系
 * @version 0.1
 * @date 2017年11月9日
 * - 从FITS文件头加载WCS TNX参数项
 * - 从文本文件加载WCS TNX参数项
 * - 计算(x,y)对应的WCS坐标(ra, dec)
 */

#include "ADefine.h"
#include "WCSTNX.h"

using namespace AstroUtil;

WCSTNX::WCSTNX() {

}

WCSTNX::~WCSTNX() {

}

bool WCSTNX::LoadImage(const char* filepath) {
	return true;
}


bool WCSTNX::LoadText(const char* filepath) {
	return true;
}

int WCSTNX::XY2WCS(double x, double y, double& ra, double& dec) {
	if (!param_.valid) return -1;

	double xi, eta, xi1, eta1, xi2, eta2;

	image_to_plane(x, y, xi, eta);
	correct(xi, eta, xi1, eta1);
	xi2 = xi + xi1;
	eta2 = eta + eta1;
	plane_to_wcs(xi2, eta2, ra, dec);
	ra  *= D2R;
	dec *= D2R;

	return 0;
}

void WCSTNX::linear_array(double x, int order, double** array) {
	int i;
	double *ptr;

	ptr = (*array) = new double[order];
	ptr[0] = 1.0;
	for (i = 1; i < order; ++i) {
		ptr[i] = x * ptr[i - 1];
	}
}

void WCSTNX::legendre_array(double x, double xmin, double xmax, int order, double** array) {
	int i;
	double *ptr, xnorm;

	ptr = (*array) = new double[order];
	if (fabs(xmin) < AEPS && fabs(xmax) < AEPS) xnorm = x;
	else xnorm = (2 * x - (xmax + xmin)) / (xmax - xmin);

	ptr[0] = 1.0;
	if (order > 1) ptr[1] = xnorm;
	for (i = 2; i < order; ++i) {
		ptr[i] = ((2 * i - 1) * xnorm * ptr[i - 1] - (i - 1) * ptr[i - 2]) / i;
	}
}

void WCSTNX::chebyshev_array(double x, double xmin, double xmax, int order, double** array) {
	int i;
	double *ptr, xnorm;

	ptr = (*array) = new double[order];
	if (fabs(xmin) < AEPS && fabs(xmax) < AEPS) xnorm = x;
	else xnorm = (2 * x - (xmax + xmin)) / (xmax - xmin);

	ptr[0] = 1.0;
	if (order > 1) ptr[1] = xnorm;
	for (i = 2; i < order; ++i) {
		ptr[i] = 2 * xnorm * ptr[i - 1] - ptr[i - 2];
	}
}

void WCSTNX::polyval(int order, int type, double* x, double* y, int& n, double** array) {
	double *ptr, t;
	int i, j, imin(0), imax(order);

	if      (type == TNX_XNONE) n = 2 * order - 1;
	else if (type == TNX_XFULL) n = order * order;
	else if (type == TNX_XHALF) n = order * (order + 1) / 2;
	(*array) = new double[n];

	for (j = 0, ptr = *array; j < order; ++j) {
		if (j) {
			if (type == TNX_XNONE && imax != 1) imax = 1;
			else if (type == TNX_XHALF) imax = order - j;
		}

		for (i = imin, t = y[j]; i < imax; ++i, ++ptr) *ptr = x[i] * t;
	}
}

void WCSTNX::correct(double xi, double eta, double& xi1, double& eta1) {

}

/*
 * @note 图像坐标转换为投影(中间)坐标
 */
void WCSTNX::image_to_plane(double x, double y, double& xi, double& eta) {
	double dx(x - param_.ref_xy.x), dy(y - param_.ref_xy.y);

	xi  = param_.cd[0][0] * dx + param_.cd[0][1] * dy;
	eta = param_.cd[1][0] * dx + param_.cd[1][1] * dy;
}

/*
 * @note TAN投影逆变换, 平面坐标==>球面坐标
 */
void WCSTNX::plane_to_wcs(double xi, double eta, double& ra, double& dec) {

}
