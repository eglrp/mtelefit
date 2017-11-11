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
#include "WCSTNX.h"
#include "AMath.h"

using std::string;
using namespace AstroUtil;

int main(int argc, char** argv) {
/* 验证相同XY在不同图像生成的.accrel关系下, 对应的KE坐标
	if (argc < 4) {
		printf("Usage:\n\t mtelefit coords.db x y\n");
		return -1;
	}

	string pathacc = argv[1];
	double x = atof(argv[2]);
	double y = atof(argv[3]);
	double ksi, eta;
	WCSTNX tnx;

	if (!tnx.LoadText(pathacc.c_str())) {
		printf("failed to load coords.db<%s>\n", pathacc.c_str());
		return -2;
	}

	tnx.XY2WCS(x, y, ksi, eta);
	printf("center<%7.2f  %7.2f> = <%12.8f  %12.8f>\n", x, y, ksi * R2D, eta * R2D);
*/

/* 为ccmap拟合生成XY-KE对应关系
	if (argc < 3) {
		printf("Usage:\n\t mtelefit coords.db xytoradec\n");
		return -1;
	}

	string pathacc = argv[1];
	string pathdata = argv[2];
	WCSTNX tnx;
	double x0(2068.5), y0(2068.5), x, y;
	double ra0, dec0, ksi, eta; // 弧度
	double ra, dec; // 角度
	FILE *fp, *rslt;
	const int lnsize(200);
	char line[lnsize];

	if (!tnx.LoadText(pathacc.c_str())) {
		printf("failed to load coords.db<%s>\n", pathacc.c_str());
		return -2;
	}
	// 使用.acc计算视场中心坐标(ra0, dec0)
	tnx.XY2WCS(x0, y0, ra0, dec0);
	printf("center<%7.2f  %7.2f> = <%12.8f  %12.8f>\n", x0, y0, ra0 * R2D, dec0 * R2D);

	if ((fp = fopen(pathdata.c_str(), "r")) == NULL) {
		printf("failed to open data<%s>\n", pathdata.c_str());
		return -3;
	}
	rslt = fopen("result1.txt", "w");

	while(!feof(fp)) {
		if (fgets(line, lnsize, fp) == NULL) continue;
		sscanf(line, "%lf %lf %lf %lf", &x, &y, &ra, &dec);
		// 计算.axycc中各(ra,dec)相对(ra0,dec0)的坐标(ksi, eta)
		RotateForward(ra0, dec0, ra * D2R, dec * D2R, ksi, eta);
		printf("%7.2f %7.2f %12.8f %12.8f %12.8f %12.8f\n",
				x, y, ra, dec, ksi * R2D, eta * R2D);
		fprintf(rslt, "%7.2f %7.2f %12.8f %12.8f %12.8f %12.8f\n",
				x, y, ra, dec, ksi * R2D, eta * R2D);
	}

	fclose(rslt);
	fclose(fp);
*/

///*
	if (argc < 3) {
		printf("Usage:\n\t mtelefit coords.db xytoradec\n");
		return -1;
	}

	string pathacc = argv[1];
	string pathdata = argv[2];
	WCSTNX tnx;
	FILE *fp, *rslt;
	const int lnsize(200);
	char line[lnsize];
	double x, y, ra1, dec1, ra2, dec2, era, edec;

	if (!tnx.LoadText(pathacc.c_str())) {
		printf("failed to load coords.db<%s>\n", pathacc.c_str());
		return -2;
	}
	if ((fp = fopen(pathdata.c_str(), "r")) == NULL) {
		printf("failed to open data<%s>\n", pathdata.c_str());
		return -3;
	}
	rslt = fopen("result1.txt", "w");

	while(!feof(fp)) {
		if (fgets(line, lnsize, fp) == NULL) continue;
		sscanf(line, "%lf %lf %lf %lf", &x, &y, &ra1, &dec1);
		tnx.XY2WCS(x, y, ra2, dec2);
		ra2 *= R2D;
		dec2 *= R2D;
		era  = ra2 - ra1;
		edec = dec2 - dec1;
		if (era > 180.0) era -= 360.0;
		else if (era < -180.0) era += 360.0;
		printf("%7.2f   %7.2f   %9.5f   %9.5f   %9.5f   %9.5f   %7.1f   %7.1f\n",
				x, y, ra1, dec1, ra2, dec2, era * 3600.0, edec * 3600.0);
		fprintf(rslt, "%7.2f   %7.2f   %9.5f   %9.5f   %9.5f   %9.5f   %7.1f   %7.1f\n",
				x, y, ra1, dec1, ra2, dec2, era * 3600.0, edec * 3600.0);
	}

	fclose(rslt);
	fclose(fp);
//*/
	return 0;
}
