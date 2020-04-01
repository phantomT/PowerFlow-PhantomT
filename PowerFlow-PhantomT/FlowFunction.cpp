/**
******************************************************************************
* @FileName	:	FlowFunction.cpp
* @Brief	:	潮流计算函数
* @Author	:	电气1611 田宝宁 1161181118
* @Date		:	2019.07.06
******************************************************************************
*/

#include "fstream"
#include "iostream"
#include "iomanip"
#include "cmath"

#include "DataForm.h"
#include "NEquation.h"
#include "FlowFunction.h"

/**
* @brief	节点、支路数据读入子程序
* @retval	None
* @input	..\9-Nodes-Data\BusData.txt || BranchData.txt
* @output	Origin Data.txt
*/
void DataRead()
{
	ifstream BusData;
	ifstream BranchData;
	BusData.open("..\\9-Nodes-Data\\BusData.txt", ios::in);
	for (int i = 0; i < 9; ++i)
		BusData >> Bus[i].Num >> Bus[i].BusType >> Bus[i].Voltage >> Bus[i].Phase >> Bus[i].Pl >> Bus[i].Ql >> Bus[i].Pg >> Bus[i].Qg;
	BusData.close();

	ofstream outfile;
	outfile.open("..\\Origin Data.txt", ios::app | ios::out | ios::in);

	//输出原始数据
	outfile << "节点数据	" << endl;
	outfile << "节点" << "\t" << "节点类型" << '\t' << "电压幅值" << '\t' << "电压相位" << '\t' << "有功注入" << '\t' << "无功注入" << '\t' << "有功流出" << '\t' << "无功流出" << endl;
	for (int i = 0; i < 9; ++i)
		outfile << Bus[i].Num << "\t" << Bus[i].BusType << "\t" << Bus[i].Voltage << "\t" << Bus[i].Phase << "\t" << Bus[i].Pg << "\t" << Bus[i].Qg << "\t" << Bus[i].Pl << "\t" << Bus[i].Ql << endl;
	outfile.close();

	BranchData.open("..\\9-Nodes-Data\\BranchData.txt", ios::in);
	for (int i = 0; i < 9; ++i)
		BranchData >> Branch[i].Num >> Branch[i].BusFirst >> Branch[i].BusEnd>> Branch[i].R >> Branch[i].X >> Branch[i].B >> Branch[i].K;
	BranchData.close();

	for(int i = 0;i < BusNum;++i)
	{
		if(Branch[i].K == 0)
			Branch[i].K = 1;
	}

	outfile.open("..\\Origin Data.txt", ios::app | ios::out | ios::in);

	//输出原始数据
	outfile << "支路数据	" << endl;
	outfile << "支路编号" << '\t' << "首端节点" << '\t' << "末端节点" << '\t' << "电阻" << "\t" << "电抗" << "\t" << "电纳" << "\t" << "变比" << endl;
	for (int i = 0; i < 9; ++i)
		outfile << Branch[i].Num << "\t" << Branch[i].BusFirst << "\t" << Branch[i].BusEnd << "\t" << Branch[i].R << "\t" << Branch[i].X << "\t" << Branch[i].B << "\t" << Branch[i].K << endl;
	outfile.close();

}

/**
* @brief	形成节点导纳矩阵子程序
* @retval	None
* @output	Matrix Y.txt
*/
void FormY()
{
	int m, n;	//设两个变量分别储存支路首末节点的编号
	for (int i = 0; i < 9; ++i)
	{
		float GG, BB, B0, G1, G2, B1, B2;

		//处理节点编号和数组下标
		m = Branch[i].BusFirst - 1;
		n = Branch[i].BusEnd - 1;

		//无变压器支路变比是1
		if(Branch[i].K == 0)
			Branch[i].K = 1;

		//以下处理变压器变比不为1时的情况，对于有些节点数据中，变压器变比全为标准变比的情况，以下处理可以省略
		Branch[i].R = Branch[i].K * Branch[i].R;
		Branch[i].X = Branch[i].K * Branch[i].X;

		//将变压器变比引起的对地支路转成电纳值（首段）
		G1 =  (1 - Branch[i].K) / (Branch[i].K * Branch[i].K) / (Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X) * Branch[i].R;
		B1 = -(1 - Branch[i].K) / (Branch[i].K * Branch[i].K) / (Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X) * Branch[i].X;

		//将变压器变比引起的对地支路转成电纳值（末段）
		G2 =  (Branch[i].K - 1) / Branch[i].K / (Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X) * Branch[i].R;
		B2 = -(Branch[i].K - 1) / Branch[i].K / (Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X) * Branch[i].X;
		//变压器非标准变比的情况处理完毕

		//将支路的电阻数据转化成电导数据
		GG =  Branch[i].R / (Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X);
		BB = -Branch[i].X / (Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X);

		//接地数据单独写出
		B0 = Branch[i].B / 2;

		//追加节点导纳
		G[m][m] += GG + G1;
		G[m][n] -= GG;
		G[n][m] -= GG;
		G[n][n] += GG + G2;

		B[m][m] += BB + B0 + B1;
		B[m][n] -= BB;
		B[n][m] -= BB;
		B[n][n] += BB + B0 + B2;
	}

	//输出节点导纳矩阵
	ofstream outfile;
	outfile.open("..\\Matrix Y.txt", ios::app | ios::out | ios::in);
	outfile << "节点导纳矩阵";
	for (int i = 0; i < 9; ++i)
	{
		outfile << endl;
		for (int j = 0; j < 9; ++j)
		{
			if(fabs(G[i][j] - 0) > 1e-8 && fabs(B[i][j] - 0) > 1e-8)
				outfile << setprecision(4) << G[i][j] << " j" << left << setw(7) <<setfill(' ') << B[i][j] << "\t";
			else
				outfile << setprecision(4) << G[i][j] << " j" << left << setw(7) << setfill(' ') << B[i][j] << "\t\t";

		}
	}
	outfile << endl;
}

/**
* @brief	设置节点电压子程序
* @retval	None
*/
void VoltageInitial()
{
	for(int i = 0;i < BusNum;++i)
	{
		Ue[i]	= Bus[i].Voltage * cos(Bus[i].Phase);
		Uf[i]	= Bus[i].Voltage * sin(Bus[i].Phase);
		U[i]	= Bus[i].Voltage;
	}
}

/**
* @brief	计算功率不平衡量子程序
* @retval	None
* @output	Results.txt
*/
void SetUnbalance()
{
	for(int i = 1;i < BusNum;++i)
	{
		P[i]=0;
		Q[i]=0;
		for(int j = 0;j < BusNum;++j)
		{
			P[i] += Ue[i] * (G[i][j] * Ue[j] - B[i][j] * Uf[j]) + Uf[i] * (G[i][j] * Uf[j] + B[i][j] * Ue[j]);
			Q[i] += Uf[i] * (G[i][j] * Ue[j] - B[i][j] * Uf[j]) - Ue[i] * (G[i][j] * Uf[j] + B[i][j] * Ue[j]);
		}

		if(Bus[i].BusType == 1)
		{
			DeltaP	[i - 1]	= Bus[i].Pg - Bus[i].Pl - P[i];
			DeltaQU2[i - 1] = Bus[i].Qg - Bus[i].Ql - Q[i];
		}
		else if(Bus[i].BusType == 2)
		{
			DeltaP	[i - 1]	= Bus[i].Pg - Bus[i].Pl - P[i];
			DeltaQU2[i - 1] = Bus[i].Voltage * Bus[i].Voltage - (Ue[i] * Ue[i] + Uf[i] * Uf[i]);
		}
	}
	for(int i = 0;i < BusNum - 1;i++)
	{
		Delta[2 * i]		= DeltaP[i];
		Delta[2 * i + 1]	= DeltaQU2[i];
	}

	//输出初始不平衡量向量
	ofstream outfile;
	outfile.open("..\\Results.txt", ios::app | ios::out | ios::in);
	outfile <<"设置不平衡量" << endl;
	outfile << "节点\t" << "DeltaP\t\t\t" << "DeltaQU2" << endl;
	for (int i = 0; i < 2 * (BusNum - 1); i += 2)
	{
		if(i / 2 + 2 == 7)
			outfile << i / 2 + 2 << "\t" << Delta[i] << "\t\t" << Delta[i + 1] << endl;
		else
			outfile << i / 2 + 2 << "\t" << Delta[i] << "\t\t\t" << Delta[i + 1] << endl;
	}
	outfile.close();
}

/**
* @brief	形成雅各比矩阵子程序
* @retval	None
* @output	Results.txt
*/
void FormJacobi()
{
	float H[BusNum-1][BusNum-1] = {0};
	float N[BusNum-1][BusNum-1] = {0};
	float J[BusNum-1][BusNum-1] = {0};
	float L[BusNum-1][BusNum-1] = {0};

	float a[BusNum],b[BusNum];

	for(int i = 1;i < BusNum;++i)
	{
		a[i] = G[i][i] * Ue[i] - B[i][i] * Uf[i];
		b[i] = G[i][i] * Uf[i] + B[i][i] * Ue[i];
		for(int j = 0;j < BusNum; ++j)
		{
			if(j != i)
			{
				a[i] = a[i] + (G[i][j] * Ue[j] - B[i][j] * Uf[j]);
				b[i] = b[i] + (G[i][j] * Uf[j] + B[i][j] * Ue[j]);
			}
		}
	}
	for(int i = 1;i < BusNum;++i)
	{
		for(int j = 1;j < BusNum;++j)
		{
			if(i == j)
			{
				H[i-1][i-1] = -B[i][i] * Ue[i] + G[i][i] * Uf[i] + b[i];
				N[i-1][i-1] =  G[i][i] * Ue[i] + B[i][i] * Uf[i] + a[i];

				if(Bus[i].BusType == 1)
				{
					J[i-1][i-1] = -G[i][i] * Ue[i] - B[i][i] * Uf[i] + a[i];
					L[i-1][i-1] = -B[i][i] * Ue[i] + G[i][i] * Uf[i] - b[i];
				}
				else if(Bus[i].BusType == 2)
				{
					J[i-1][i-1] = 2 * Uf[i];
					L[i-1][i-1] = 2 * Ue[i];
				}
			}
			else if(i != j)
			{
				H[i-1][j-1] = -B[i][j] * Ue[i] + G[i][j] * Uf[i];
				N[i-1][j-1] =  G[i][j] * Ue[i] + B[i][j] * Uf[i];
				
				if(Bus[i].BusType == 1)
				{
					J[i-1][j-1] = -G[i][j] * Ue[i] - B[i][j] * Uf[i];
					L[i-1][j-1] = -B[i][j] * Ue[i] + G[i][j] * Uf[i];
				}
				else if(Bus[i].BusType == 2)
				{
					J[i-1][j-1] = 0;
					L[i-1][j-1] = 0;
				}

			}
		}
	}
	
	for(int i = 0;i < BusNum - 1;++i)
	{
		for(int j = 0;j < BusNum - 1;++j)
		{
			Jacobi[2 * i][2 * j]		= H[i][j];
			Jacobi[2 * i][2 * j + 1]	= N[i][j];
			Jacobi[2 * i + 1][2 * j]	= J[i][j];
			Jacobi[2 * i + 1][2 * j + 1]= L[i][j];
		}
	}

	//输出雅克比矩阵
	ofstream outfile;
	outfile.open("..\\Results.txt", ios::app | ios::out | ios::in);
	outfile << "雅克比矩阵" << endl;
	for (int i = 0; i < 2 * (BusNum - 1); ++i)
	{
		for (int j = 0; j < 2 * (BusNum - 1); ++j)
			outfile << Jacobi[i][j] << "\t";
		outfile << endl;
	}
	outfile.close();
}

/**
* @brief	求解修正方程得到电压增量子程序
* @retval	None
* @output	Results.txt
*/
void GetRevised()
{
	NEquation Object;
	Object.SetSize(2 * (BusNum - 1));

	for(int i = 0;i < 2 * (BusNum - 1);++i)
	{
		Object.Value(i) = Delta[i];
		for(int j = 0;j < 2 * (BusNum - 1);++j)
			Object.Data(i,j) = Jacobi[i][j];
	}

	Object.Run();

	for(int i = 0;i < BusNum - 1;++i)
	{
		DeltaUe[i] = (float) Object.Value(2 * i + 1);
		DeltaUf[i] = (float) Object.Value(2 * i);
		DeltaU[2 * i] = DeltaUf[i];
		DeltaU[2 * i + 1] =DeltaUe[i];
	}

	//输出电压偏移量
	ofstream outfile;
	outfile.open("..\\Results.txt", ios::app | ios::out | ios::in);

	outfile << "\n电压偏移量" << endl;
	outfile << "节点\t" << "DeltaUe\t\t" << "DeltaUf" << endl;
	for (int i = 0; i < BusNum - 1; ++i)
	{
		if(i + 2 <= 3)
			outfile << i + 2 << "\t" << DeltaUe[i] << "\t\t" << DeltaUf[i] << endl;
		else
			outfile << i + 2 << "\t" << DeltaUe[i] << "\t" << DeltaUf[i] << endl;
	}
	outfile.close();

	float Me, Mf;
	Me = fabs(DeltaUe[0]);
	Mf = fabs(DeltaUf[0]);

	for (int i = 0; i < BusNum - 1; i++)
	{
		if (Me < fabs(DeltaUe[i]))
		{
			Me = fabs(DeltaUe[i]);
		}
		if (Mf < fabs(DeltaUf[i]))
		{
			Mf = fabs(DeltaUf[i]);
		}
	}

	outfile.open("..\\Results.txt", ios::app | ios::out | ios::in);
	outfile << Me << "\t" << Mf << endl;
	outfile.close();

	MMe = Me;
	MMf = Mf;
}

/**
* @brief	得到新的电压子程序
* @retval	None
* @output	Results.txt
*/
void GetNewVoltageValue()
{
	for (int i = 0; i < BusNum - 1; i++)
	{
		Ue[i + 1] += DeltaUe[i];
		Uf[i + 1] += DeltaUf[i];
		U[i + 1] = sqrt(Ue[i + 1] * Ue[i + 1] + Uf[i + 1] * Uf[i + 1]);
	}

	ofstream outfile;
	outfile.open("..\\Results.txt", ios::app | ios::out | ios::in);

	outfile << '\n' << "新的电压值" << endl;

	for (int i = 0; i < BusNum - 1; ++i)
	{
		outfile << Ue[i + 1] << " j" << Uf[i + 1] << endl;
	}
	outfile.close();
}

/**
* @brief	计算平衡节点功率子程序
* @retval	None
* @output	Balance PQ.txt
*/
void BusBalance()
{
	for (int i = 0; i < BusNum; i++)
	{
		if (Bus[i].BusType == 3)
		{
			P[i] = 0.0;
			Q[i] = 0.0;

			for (int j = 0; j < BusNum; j++)
			{
				P[i] += Ue[i] * (G[i][j] * Ue[j] - B[i][j] * Uf[j]) + Uf[i] * (G[i][j] * Uf[j] + B[i][j] * Ue[j]);//有功部分计算；
				Q[i] += Uf[i] * (G[i][j] * Ue[j] - B[i][j] * Uf[j]) - Ue[i] * (G[i][j] * Uf[j] + B[i][j] * Ue[j]);//无功部分计算；
			}
		}
	}

	ofstream outfile;
	outfile.open("..\\Balance PQ.txt", ios::app | ios::out | ios::in);

	outfile << "\n" << "平衡节点功率" << endl;
	for (int i = 0; i < BusNum; ++i)
	{
		if (Bus[i].BusType == 3)
			outfile << P[i] << " j" << Q[i] << "\n" << endl;
	}
	outfile.close();
}

/**
* @brief	计算支路功率子程序
* @retval	None
* @output	BranchPQ.txt
*/
void BrancePQ()
{
	for (int i = 0; i < BusNum; i++)
	{
		G0[i] = G[i][i];
		B0[i] = B[i][i];
		for (int j = 0; j < BusNum; j++)
		{
			if (i != j)
			{
				G0[i] += G[i][j];
				B0[i] += B[i][j];
			}
		}
	}

	for (int i = 0; i < BusNum; ++i)
	{
		for (int j = 0; j < BusNum; ++j)
		{
			BranchP[i][j] = 0;
			BranchQ[i][j] = 0;
			if (B[i][j] != 0 && i != j)
			{
				BranchP[i][j] = Ue[i] * (Ue[i] - Ue[j]) * G[i][j] - Ue[i] * (Uf[i] - Uf[j]) * B[i][j] + Uf[i] * (Ue[i] - Ue[j]) * B[i][j] + Uf[i] * (Uf[i] - Uf[j]) * G[i][j] + U[i] * U[i] * G0[i];
				BranchQ[i][j] =-Uf[i] * (Uf[i] - Uf[j]) * B[i][j] + Uf[i] * (Ue[i] - Ue[j]) * G[i][j] - Ue[i] * (Uf[i] - Uf[j]) * G[i][j] - Ue[i] * (Ue[i] - Ue[j]) * B[i][j] - U[i] * U[i] * B0[i];
			}
		}
	}

	ofstream outfile;
	outfile.open("..\\BranchPQ.txt", ios::app | ios::out | ios::in);
	outfile << "支路功率" << endl;
	
	for (int i = 0; i < BusNum; ++i)
	{
		for (int j = 0; j < BusNum; ++j)
		{
			if(BranchP[i][j] == 0 && BranchQ[i][j] == 0)
				outfile << setprecision(4) << BranchP[i][j] << " j" << BranchQ[i][j] << "\t\t";
			else
				outfile << setprecision(4) << BranchP[i][j] << " j" << BranchQ[i][j] << "\t";
		}
		outfile << endl;
	}
	outfile.close();
}

/**
* @brief	计算线路上损耗子程序
* @retval	None
* @output	BranchLoss.txt
*/
void BranceLoss()
{
	for (int i = 0; i < BusNum; ++i)
	{
		for (int j = 0; j < BusNum; ++j)
		{
			BDP[i][j] = BranchP[i][j] + BranchP[j][i];
			BDQ[i][j] = BranchQ[i][j] + BranchQ[j][i];
		}
	}

	ofstream outfile;
	outfile.open("..\\BranchLoss.txt", ios::app | ios::out | ios::in);

	outfile << "\n" << "总损耗" << endl;
	for (int i = 0; i < BusNum; ++i)
	{
		for (int j = 0; j < BusNum; ++j)
		{
			outfile << BDP[i][j] << " j" << BDQ[i][j] << "\t";
		}
		outfile << endl;
	}
	outfile.close();
}