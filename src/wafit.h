/*!
 * @file wafit.h 大视场星象位置拟合, 声明文件
 * @version 0.1
 * @date 2017年11月8日
 * @note
 * 建立投影坐标系与图像坐标系的匹配拟合关系
 */

#ifndef WIDEANGLE_FIT_H
#define WIDEANGLE_FIT_H

struct image_object {// 从图像中识别目标的基本信息
	double x, y;		//< XY坐标
	double flux;		//< 流量
	double ra, dc;	//< 赤道坐标, 量纲: 弧度
	double xi, eta;	//< 相对参考点的平面投影坐标

public:
	/*!
	 * @brief 计算相对(ra0, dc0)的位置
	 * @param ra0 参考点赤经, 量纲: 弧度
	 * @param dc0 参考点赤纬, 量纲: 弧度
	 */
	void calc_relpos(double ra0, double dc0);
};

#endif
