#pragma once
/**
******************************************************************************
* @FileName	:	DataForm.h
* @Brief	:	���ݽṹ�����ͷ�ļ�
* @Describe	:	�������ݽṹ�����������ʼ��һЩ����
* @Author	:	����1611 �ﱦ�� 1161181118
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

}Bus[BusNum];			//�洢�ڵ������Ϣ

struct BranchData
{
	int Num, BusFirst, BusEnd;
	float R, X, B, K;

}Branch[BranchNum];		//�洢��·�����Ϣ

float G[BusNum][BusNum] = { 0 }, B[BusNum][BusNum] = { 0 };				//�洢�ڵ㵼������
float Jacobi[2 * (BusNum - 1)][2 * (BusNum - 1)];		//�洢�ſɱȾ���
float P[BusNum], Q[BusNum];								//
float BranchP[BusNum][BusNum], BranchQ[BusNum][BusNum];	//֧·���й����ʡ�֧·���޹�����
float DeltaP[BusNum - 1];								//�й��Ĳ�ƽ����
float DeltaQU2[BusNum - 1];								//PQ�ڵ�洢�޹����ʲ�ƽ����,PV�ڵ�洢��ѹ��ֵƽ���Ĳ�ƽ����
float Delta[2 * (BusNum - 1)];							//ȫ����ƽ����
float DeltaUe[BusNum - 1], DeltaUf[BusNum - 1];			//��ѹʵ��������������ѹ�鲿��������
float DeltaU[2 * (BusNum - 1)];							//��ѹ��ȫ��������
float Ue[BusNum], Uf[BusNum], U[BusNum];				//�洢��ѹʵ�����鲿����ֵ

float G0[BusNum], B0[BusNum];
float BDP[BusNum][BusNum], BDQ[BusNum][BusNum];
float MMe = 9999, MMf = 9999;

int real_cnt = 0;
int Iteration_Cnt = 0;

#endif // !DATAFORM
