#pragma once
/**
******************************************************************************
* @FileName	:	FlowFunction.h
* @Brief	:	�������㺯��ͷ�ļ�
* @Describe	:	���㺯��������
* @Author	:	����1611 �ﱦ�� 1161181118
* @Date		:	2019.07.06
******************************************************************************
*/

#ifndef FLOWFUNCTION
#define FLOWFUNCTION

void DataRead();			//�ڵ㡢֧·���ݶ����ӳ���
void FormY();				//�γɽڵ㵼�ɾ����ӳ���
void VoltageInitial();		//���ýڵ��ѹ�ӳ���
void SetUnbalance();		//���㹦�ʲ�ƽ�����ӳ���
void FormJacobi();			//�γ��Ÿ��Ⱦ����ӳ���
void GetRevised();			//����������̵õ���ѹ�����ӳ���
void GetNewVoltageValue();  //�õ��µĵ�ѹ�ӳ���
void BusBalance();			//����ƽ��ڵ㹦���ӳ���
void BrancePQ();			//����֧·�����ӳ���
void BranceLoss();			//������·������ӳ���

#endif // !FLOWFUNCTION
