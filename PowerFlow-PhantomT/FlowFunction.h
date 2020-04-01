#pragma once
/**
******************************************************************************
* @FileName	:	FlowFunction.h
* @Brief	:	潮流计算函数头文件
* @Describe	:	计算函数的声明
* @Author	:	电气1611 田宝宁 1161181118
* @Date		:	2019.07.06
******************************************************************************
*/

#ifndef FLOWFUNCTION
#define FLOWFUNCTION

void DataRead();			//节点、支路数据读入子程序
void FormY();				//形成节点导纳矩阵子程序
void VoltageInitial();		//设置节点电压子程序
void SetUnbalance();		//计算功率不平衡量子程序
void FormJacobi();			//形成雅各比矩阵子程序
void GetRevised();			//求解修正方程得到电压增量子程序
void GetNewVoltageValue();  //得到新的电压子程序
void BusBalance();			//计算平衡节点功率子程序
void BrancePQ();			//计算支路功率子程序
void BranceLoss();			//计算线路上损耗子程序

#endif // !FLOWFUNCTION
