/*
 * @file WCSTNX.h 声明文件, 基于非标准WCS格式TNX, 计算图像坐标与WCS坐标之间的对应关系
 * @version 0.1
 * @date 2017年11月9日
 * - 从FITS文件头加载WCS TNX参数项
 * - 从文本文件加载WCS TNX参数项
 * - 计算(x,y)对应的WCS坐标(ra, dec)
 *
 * @note
 * 畸变改正参数项数量(order>=1， 二元)
 * none:   2*order-1
 * half:   order*(order+1)/2
 * full:   order*order
 *
 * @note
 * 畸变改正项排列顺序(order>=1, 二元)
 * X0Y0
 * X1Y0
 * ...
 * X(order-1)Y0
 * X1Y1
 * X2Y1
 * ...
 * X(order-1)Y1
 * ...
 * X0Y(order-1)
 */

#ifndef WCSTNX_H_
#define WCSTNX_H_

#include <string>
#include "ADefine.h"

using std::string;
using AstroUtil::PT2F;

class WCSTNX {
public:
	WCSTNX();
	virtual ~WCSTNX();

protected:
	/* 数据结构 */
	enum {// 平面畸变拟合多项式类型
		TNX_CHEB = 1,	//< 契比雪夫多项式
		TNX_LEG,			//< 勒让德多项式
		TNX_POLY			//< 线性多项式
	};

	enum {// 多项式交叉系数类型
		TNX_XNONE,		//< 无交叉项
		TNX_XFULL,		//< 全交叉
		TNX_XHALF		//< 半交叉
	};

	struct param_tnx {// TNX参数
		bool valid;		//< 参数有效性标志
		int surface;		//< 多项式类型
		int cross;		//< 交叉类型
		PT2F ref_xymean;		//< 参考点: 平均XY坐标
		PT2F ref_wcsmean;	//< 参考点: 平均WCS坐标, 量纲: 角度
		string pixsystem;	//< 图像像素坐标系名称
		string coosystem;	//< WCS坐标系名称
		string projection;	//< 投影模式
		PT2F ref_xy;			//< 参考点: XY坐标
		PT2F ref_wcs;		//< 参考点: WCS坐标, 量纲: 角度
		string function;		//< 畸变改正函数类型
		PT2F shift;			//< 投影坐标偏移量
		PT2F mag;			//< ??
		PT2F rotation;		//< 旋转角, 量纲: 角度
		PT2F rms_wcs;		//< 统计误差??
		PT2F rms;			//< 统计误差??
		double cd[2][2];		//< 旋转矩阵. 由平均关系获得
	};

protected:
	/* 成员变量 */
	param_tnx param_;	//< TNX参数

public:
	/* 接口 */
	/*!
	 * @brief 从FITS文件头加载WCS参数
	 * @param filepath FITS文件路径
	 * @return
	 * 参数加载结果
	 */
	bool LoadImage(const char* filepath);
	/*!
	 * @brief 从文本文件加载WCS参数
	 * @param filepath 文本文件路径
	 * @return
	 * 参数加载结果
	 */
	bool LoadText(const char* filepath);
	/*!
	 * @brief 计算与图像坐标(x,y)对应的WCS坐标(ra,dec)
	 * @param x   X轴坐标
	 * @param y   Y轴坐标
	 * @param ra  赤经, 量纲: 角度
	 * @param dec 赤纬, 量纲: 角度
	 * @return
	 * 计算结果
	 *  0: 正确
	 * -1: 错误(未加载参数项)
	 */
	int XY2WCS(double x, double y, double& ra, double& dec);

protected:
	/* 功能 */
	/*!
	 * @brief 计算一元多阶线性数组
	 * @param x      自变量
	 * @param order  阶次
	 * @param array  输出数组
	 * @note
	 * 函数创建数组存储空间
	 */
	void linear_array(double x, int order, double** array);
	/*!
	 * @brief 计算一元多阶勒让德数组
	 * @param x      自变量
	 * @param xmin   自变量有效范围最小值
	 * @param xmax   自变量有效范围最大值
	 * @param order  阶次
	 * @param array  输出数组
	 * @note
	 * 函数创建数组存储空间
	 */
	void legendre_array(double x, double xmin, double xmax, int order, double** array);
	/*!
	 * @brief 计算一元多阶契比雪夫数组
	 * @param x      自变量
	 * @param xmin   自变量有效范围最小值
	 * @param xmax   自变量有效范围最大值
	 * @param order  阶次
	 * @param array  输出数组
	 * @note
	 * 函数创建数组存储空间
	 */
	void chebyshev_array(double x, double xmin, double xmax, int order, double** array);
	/*!
	 * @brief 计算二元多项式变量数组
	 * @param order  阶次
	 * @param type   交叉项类型
	 * @param x      第一个变量数组, 其长度为order
	 * @param y      第二个变量数组, 其长度为order
	 * @param array  输出数组, 其长度与order和type有关
	 */
	void polyval(int order, int type, double* x, double* y, int& n, double** array);
	/*!
	 * @brief 计算畸变修正项
	 * @param xi    输入xi坐标
	 * @param eta   输入eta坐标
	 * @param xi1   xi修正量
	 * @param eta1  eta修正量
	 */
	void correct(double xi, double eta, double& xi1, double& eta1);
	/*!
	 * @brief 图像坐标转换为投影平面平坐标
	 * @param x    图像X坐标
	 * @param y    图像Y坐标
	 * @param xi   投影xi坐标
	 * @param eta  投影eta坐标
	 */
	void image_to_plane(double x, double y, double& xi, double& eta);
	/*!
	 * @brief TAN投影逆变换, 将平面坐标转换为球面坐标
	 * @param xi   平面坐标1
	 * @param eta  平面坐标2
	 * @param ra   赤经, 量纲: 弧度
	 * @param dec  赤纬, 量纲: 弧度
	 */
	void plane_to_wcs(double xi, double eta, double& ra, double& dec);
};

#endif /* WCSTNX_H_ */
