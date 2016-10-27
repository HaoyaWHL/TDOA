#pragma once
#ifndef TAYLOR_ALGO_FILE
#define TAYLOR_ALGO_FILE
/**************泰勒算法二维***************/
/***************TaylorAlgo.h**************/
/********没有天王的读文件的代码自己*******/
/************所以自己写了一个*************/
//                            _ooOoo_  
//                           o8888888o  
//                           88" . "88  
//                           (| -_- |)  
//                            O\ = /O  
//                        ____/`---'\____  
//                      .   ' \\| |// `.  
//                       / \\||| : |||// \  
//                     / _||||| -:- |||||- \  
//                       | | \\\ - /// | |  
//                     | \_| ''\---/'' | |  
//                      \ .-\__ `-` ___/-. /  
//                   ___`. .' /--.--\ `. . __  
//                ."" '< `.___\_<|>_/___.' >'"".  
//               | | : `- \`.;`\ _ /`;.`/ - ` : | |  
//                 \ \ `-. \_ __\ /__ _/ .-` / /  
//         ======`-.____`-.___\_____/___.-`____.-'======  
//                            `=---='  
//  
//         .............................................  
//                  佛祖保佑             永无BUG 
//          佛曰:  
//                  写字楼里写字间，写字间里程序员；  
//                  程序人员写程序，又拿程序换酒钱。  
//                  酒醒只在网上坐，酒醉还来网下眠；  
//                  酒醉酒醒日复日，网上网下年复年。  
//                  但愿老死电脑间，不愿鞠躬老板前；  
//                  奔驰宝马贵者趣，公交自行程序员。  
//                  别人笑我忒疯癫，我笑自己命太贱；  
//                  不见满街漂亮妹，哪个归得程序员？ 
#include <vector>

#include "Matrix.h"
using namespace std;
struct POSITION
{
	double x;
	double y;
	double z;
};

class TAYLOR_ALGO
{
public:
	TAYLOR_ALGO();
	~TAYLOR_ALGO();
	void TaylorCalcXY(double x,double y);
	void TaylorCalcXYZ(double x,double y,double z);
	void ReadTxt(double tdoa[4]);
	bool GetCalcResult(double *desX,double *desY);
	bool GetCalcResult(double *desX,double *desY,double *desZ);
private:
	POSITION resultPosition;
	MAT inputTdoaData;//第一维行第二维列
	MAT tdoaDataNodelay;
	//double MS[3];
	double BS[4][3];
	double delay[4];
	bool calcFlag ;
};
#endif