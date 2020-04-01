/**
******************************************************************************
* @FileName	:	FlowFunction.cpp
* @Brief	:	�������㺯��
* @Author	:	����1611 �ﱦ�� 1161181118
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
* @brief	�ڵ㡢֧·���ݶ����ӳ���
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

	//���ԭʼ����
	outfile << "�ڵ�����	" << endl;
	outfile << "�ڵ�" << "\t" << "�ڵ�����" << '\t' << "��ѹ��ֵ" << '\t' << "��ѹ��λ" << '\t' << "�й�ע��" << '\t' << "�޹�ע��" << '\t' << "�й�����" << '\t' << "�޹�����" << endl;
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

	//���ԭʼ����
	outfile << "֧·����	" << endl;
	outfile << "֧·���" << '\t' << "�׶˽ڵ�" << '\t' << "ĩ�˽ڵ�" << '\t' << "����" << "\t" << "�翹" << "\t" << "����" << "\t" << "���" << endl;
	for (int i = 0; i < 9; ++i)
		outfile << Branch[i].Num << "\t" << Branch[i].BusFirst << "\t" << Branch[i].BusEnd << "\t" << Branch[i].R << "\t" << Branch[i].X << "\t" << Branch[i].B << "\t" << Branch[i].K << endl;
	outfile.close();

}

/**
* @brief	�γɽڵ㵼�ɾ����ӳ���
* @retval	None
* @output	Matrix Y.txt
*/
void FormY()
{
	int m, n;	//�����������ֱ𴢴�֧·��ĩ�ڵ�ı��
	for (int i = 0; i < 9; ++i)
	{
		float GG, BB, B0, G1, G2, B1, B2;

		//����ڵ��ź������±�
		m = Branch[i].BusFirst - 1;
		n = Branch[i].BusEnd - 1;

		//�ޱ�ѹ��֧·�����1
		if(Branch[i].K == 0)
			Branch[i].K = 1;

		//���´����ѹ����Ȳ�Ϊ1ʱ�������������Щ�ڵ������У���ѹ�����ȫΪ��׼��ȵ���������´������ʡ��
		Branch[i].R = Branch[i].K * Branch[i].R;
		Branch[i].X = Branch[i].K * Branch[i].X;

		//����ѹ���������ĶԵ�֧·ת�ɵ���ֵ���׶Σ�
		G1 =  (1 - Branch[i].K) / (Branch[i].K * Branch[i].K) / (Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X) * Branch[i].R;
		B1 = -(1 - Branch[i].K) / (Branch[i].K * Branch[i].K) / (Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X) * Branch[i].X;

		//����ѹ���������ĶԵ�֧·ת�ɵ���ֵ��ĩ�Σ�
		G2 =  (Branch[i].K - 1) / Branch[i].K / (Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X) * Branch[i].R;
		B2 = -(Branch[i].K - 1) / Branch[i].K / (Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X) * Branch[i].X;
		//��ѹ���Ǳ�׼��ȵ�����������

		//��֧·�ĵ�������ת���ɵ絼����
		GG =  Branch[i].R / (Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X);
		BB = -Branch[i].X / (Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X);

		//�ӵ����ݵ���д��
		B0 = Branch[i].B / 2;

		//׷�ӽڵ㵼��
		G[m][m] += GG + G1;
		G[m][n] -= GG;
		G[n][m] -= GG;
		G[n][n] += GG + G2;

		B[m][m] += BB + B0 + B1;
		B[m][n] -= BB;
		B[n][m] -= BB;
		B[n][n] += BB + B0 + B2;
	}

	//����ڵ㵼�ɾ���
	ofstream outfile;
	outfile.open("..\\Matrix Y.txt", ios::app | ios::out | ios::in);
	outfile << "�ڵ㵼�ɾ���";
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
* @brief	���ýڵ��ѹ�ӳ���
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
* @brief	���㹦�ʲ�ƽ�����ӳ���
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

	//�����ʼ��ƽ��������
	ofstream outfile;
	outfile.open("..\\Results.txt", ios::app | ios::out | ios::in);
	outfile <<"���ò�ƽ����" << endl;
	outfile << "�ڵ�\t" << "DeltaP\t\t\t" << "DeltaQU2" << endl;
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
* @brief	�γ��Ÿ��Ⱦ����ӳ���
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

	//����ſ˱Ⱦ���
	ofstream outfile;
	outfile.open("..\\Results.txt", ios::app | ios::out | ios::in);
	outfile << "�ſ˱Ⱦ���" << endl;
	for (int i = 0; i < 2 * (BusNum - 1); ++i)
	{
		for (int j = 0; j < 2 * (BusNum - 1); ++j)
			outfile << Jacobi[i][j] << "\t";
		outfile << endl;
	}
	outfile.close();
}

/**
* @brief	����������̵õ���ѹ�����ӳ���
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

	//�����ѹƫ����
	ofstream outfile;
	outfile.open("..\\Results.txt", ios::app | ios::out | ios::in);

	outfile << "\n��ѹƫ����" << endl;
	outfile << "�ڵ�\t" << "DeltaUe\t\t" << "DeltaUf" << endl;
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
* @brief	�õ��µĵ�ѹ�ӳ���
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

	outfile << '\n' << "�µĵ�ѹֵ" << endl;

	for (int i = 0; i < BusNum - 1; ++i)
	{
		outfile << Ue[i + 1] << " j" << Uf[i + 1] << endl;
	}
	outfile.close();
}

/**
* @brief	����ƽ��ڵ㹦���ӳ���
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
				P[i] += Ue[i] * (G[i][j] * Ue[j] - B[i][j] * Uf[j]) + Uf[i] * (G[i][j] * Uf[j] + B[i][j] * Ue[j]);//�й����ּ��㣻
				Q[i] += Uf[i] * (G[i][j] * Ue[j] - B[i][j] * Uf[j]) - Ue[i] * (G[i][j] * Uf[j] + B[i][j] * Ue[j]);//�޹����ּ��㣻
			}
		}
	}

	ofstream outfile;
	outfile.open("..\\Balance PQ.txt", ios::app | ios::out | ios::in);

	outfile << "\n" << "ƽ��ڵ㹦��" << endl;
	for (int i = 0; i < BusNum; ++i)
	{
		if (Bus[i].BusType == 3)
			outfile << P[i] << " j" << Q[i] << "\n" << endl;
	}
	outfile.close();
}

/**
* @brief	����֧·�����ӳ���
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
	outfile << "֧·����" << endl;
	
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
* @brief	������·������ӳ���
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

	outfile << "\n" << "�����" << endl;
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