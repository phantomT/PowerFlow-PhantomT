#pragma once
/**
******************************************************************************
* @FileName	:	DataForm.h
* @Brief	:	数据结构与变量头文件
* @Describe	:	声明数据结构与变量，并初始化一些变量
* @Author	:	电气1611 田宝宁 1161181118
* @Date		:	2019.07.06
******************************************************************************
*/

#ifndef DATAFORM
#define DATAFORM

#define BusNum 9
#define BranchNum 9
#define Precision 1e-5

#define PI 3.1415926

struct BusData
{
	int Num, BusType;
	float Voltage, Phase, Pg, Qg, Pl, Ql;

}Bus[BusNum];			//存储节点相关信息

struct BranchData
{
	int Num, BusFirst, BusEnd;
	float R, X, B, K;

}Branch[BranchNum];		//存储线路相关信息

float G[BusNum][BusNum] = { 0 }, B[BusNum][BusNum] = { 0 };				//存储节点导纳数据
float Jacobi[2 * (BusNum - 1)][2 * (BusNum - 1)];		//存储雅可比矩阵
float P[BusNum], Q[BusNum];								//
float BranchP[BusNum][BusNum], BranchQ[BusNum][BusNum];	//支路的有功功率、支路的无功功率
float DeltaP[BusNum - 1];								//有功的不平衡量
float DeltaQU2[BusNum - 1];								//PQ节点存储无功功率不平衡量,PV节点存储电压数值平方的不平衡量
float Delta[2 * (BusNum - 1)];							//全部不平衡量
float DeltaUe[BusNum - 1], DeltaUf[BusNum - 1];			//电压实部的修正量、电压虚部的修正量
float DeltaU[2 * (BusNum - 1)];							//电压的全部修正量
float Ue[BusNum], Uf[BusNum], U[BusNum];				//存储电压实部、虚部、数值

float G0[BusNum], B0[BusNum];
float BDP[BusNum][BusNum], BDQ[BusNum][BusNum];
float MMe = 9999, MMf = 9999;

int real_cnt = 0;
int Iteration_Cnt = 0;

#endif // !DATAFORM
