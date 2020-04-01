/**
******************************************************************************
* @FileName	:	PowerFlowMain.cpp
* @Brief	:	潮流主函数
* @Author	:	电气1611 田宝宁 1161181118
* @Date		:	2019.07.06
******************************************************************************
*/

#include "fstream"
#include "iostream"
#include "cmath"

#include "FlowFunction.h"

using namespace std;

#ifndef BusNum
	#define BusNum 9
#endif // !BusNum

#ifndef BranchNum
	#define BranchNum 9
#endif // !BranchNum

#ifndef Precision
	#define Precision 1e-5
#endif // !Precision

extern int real_cnt;
extern float MMe, MMf;

/**
* @brief	潮流计算主函数
* @retval	系统调用返回值
* @output	Results.txt
*/
int main()
{
	DataRead();
	cout << "Data Read Complete." << endl;

	FormY();
	cout << "Matrix Y created." << endl;

	VoltageInitial();
	cout << "First Time Voltage Init Complete." << endl;

	for (int Iteration_Cnt = 1; Iteration_Cnt < 20; ++Iteration_Cnt)
	{
		if (MMe > Precision || MMf > Precision)
		{
			real_cnt++;
			ofstream outfile;
			outfile.open("..\\Results.txt", ios::app | ios::out | ios::in);
			outfile << "\n第" << real_cnt << "次迭代的数据\n" << endl;

			SetUnbalance();
			cout << "第" << Iteration_Cnt << "次设置节点电压初值。\n";

			FormJacobi();
			cout << "第" << Iteration_Cnt << "次求雅可比矩阵。\n";

			GetRevised();
			cout << "第" << Iteration_Cnt << "次求各节点的变化量。\n";

			GetNewVoltageValue();
			cout << "第" << Iteration_Cnt << "次计算各节点电压的新值。\n";
		}
	}

	BusBalance();
	cout << "计算平衡节点功率\n";

	BrancePQ();
	cout << "计算支路功率\n";

	BranceLoss();
	cout << "计算线路损耗\n";

    cout << "Progress Complete\n";
}