#pragma once
/**
******************************************************************************
* @FileName	:	NEquation.h
* @Brief	:	解方程的库文件
* @Describe	:	包含解方程的类和方法
* @Author	:	老师给的
* @Date		:	我也不知道呀
******************************************************************************
*/

#if !defined(AFX_NEQUATION_H__7D6DCBC7_B370_4F2E_A720_4FB8BA7B55D9__INCLUDED_)
#define AFX_NEQUATION_H__7D6DCBC7_B370_4F2E_A720_4FB8BA7B55D9__INCLUDED_
using namespace std;

using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CHECKERROR

class NEquation
{
public:
	NEquation();
	virtual ~NEquation();
public:
	void		SetSize(int size);
	float&		Data(int lhs, int rhs);
	double&		Value(int lhs);
	int			Run();
protected:
	int			m_nNumber;
	float*		m_nDataBuffer;
	double*		m_nValue;

	void		Factorial();
	void		Forward();
	void		Backward();

	virtual float		ZEROD();
	virtual double		ZEROV();
};


NEquation::NEquation()
{
	m_nDataBuffer	= NULL;
	m_nValue		= NULL;
	m_nNumber		= 0;
}

NEquation::~NEquation()
{
	if (m_nDataBuffer != NULL) 
		delete[]m_nDataBuffer;
	if (m_nValue != NULL) 
		delete[]m_nValue;
}

float NEquation::ZEROD() { return 0.0; }
double NEquation::ZEROV() { return 0.0; }

void NEquation::SetSize(int size)
{
	if (size < 1)return;
	if (m_nDataBuffer != NULL) delete[]m_nDataBuffer;
	if (m_nValue != NULL) delete[]m_nValue;
	m_nDataBuffer = new float[size * size];
	m_nValue = new double[size];
	for (int i = 0; i < size * size; i++)m_nDataBuffer[i] = ZEROD();
	for (int i = 0; i < size; i++)m_nValue[i] = ZEROV();
	m_nNumber = size;
}

float& NEquation::Data(int lhs, int rhs)
{
#if defined(CHECKERROR)
	if ((lhs < m_nNumber) && (lhs >= 0) && (rhs < m_nNumber) && (rhs >= 0))
#endif
		return m_nDataBuffer[lhs * m_nNumber + rhs];
#if defined(CHECKERROR)
	else
		//		AfxMessageBox("Error");
		cout << "Error" << endl;
	return m_nDataBuffer[0];
#endif
}

double& NEquation::Value(int lhs)
{
#if defined(CHECKERROR)
	if ((lhs < m_nNumber) && (lhs >= 0))
#endif
		return m_nValue[lhs];
#if defined(CHECKERROR)
	else
		//		AfxMessageBox("Error");
		cout << "Error" << endl;
	return m_nValue[0];
#endif
}


int NEquation::Run()
{
	Factorial();
	Forward();
	Backward();
	return 1;
}

float reverse(float ff) { return (float)1.0 / ff; };

void NEquation::Factorial()
{
	int i, j, k;
	for (i = 0; i < m_nNumber; i++)
	{
		//	规格化format line;
		Data(i, i) = reverse(Data(i, i));
		for (j = i + 1; j < m_nNumber; j++) Data(i, j) = Data(i, i) * Data(i, j);
		//	消去
		for (j = i + 1; j < m_nNumber; j++)
		{
			for (k = i + 1; k < m_nNumber; k++)
			{
				Data(j, k) = Data(j, k) - Data(j, i) * Data(i, k);
			}
		}
	}
}

void NEquation::Forward()
{
	int i, j;
	for (i = 0; i < m_nNumber; i++)
	{
		Value(i) = Data(i, i) * Value(i);
		for (j = i + 1; j < m_nNumber; j++)
			Value(j) = Value(j) - Data(j, i) * Value(i);
	}
}

void NEquation::Backward()
{
	int i, j;
	for (i = 1; i < m_nNumber; i++)
	{
		for (j = 1; j < i + 1; j++)
			Value(m_nNumber - i - 1) -= Data(m_nNumber - i - 1, m_nNumber - j) * Value(m_nNumber - j);
	}
}

#endif // !defined(AFX_NEQUATION_H__7D6DCBC7_B370_4F2E_A720_4FB8BA7B55D9__INCLUDED_)
