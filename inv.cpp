//to calculate the inv of matrix a,get b finally
#include "inv.h"
double rem(double* a, int i, int j, int n)
{
	int k, m;
	double *pTemp = new double[(n - 1)*(n - 1)];

	for (k = 0; k<i; k++)
	{
		for (m = 0; m<j; m++)	pTemp[k*(n - 1) + m] = a[k*n + m];
		for (m = j; m<n - 1; m++) pTemp[k*(n - 1) + m] = a[k*n + m + 1];
	}
	for (k = i; k<n - 1; k++)
	{
		for (m = 0; m<j; m++)	pTemp[k*(n - 1) + m] = a[(k + 1)*n + m];
		for (m = j; m<n - 1; m++) pTemp[k*(n - 1) + m] = a[(k + 1)*n + m + 1];
	}
	double dResult = (((i + j) % 2 == 1) ? -1 : 1)*det(pTemp, n - 1);
	delete[] pTemp;
	return dResult;
}


double det(double* a, int n)
{
	if (n == 1) return a[0];
	double sum = .0;
	for (int j = 0; j<n; j++) sum += a[0 * n + j] * rem(a, 0, j, n);
	return sum;
}



void inv(double* a, double* b, int n)
{
	double deta = det(a, n); 
	//cout << deta << endl;
	for (int i = 0; i<n; i++)
	for (int j = 0; j<n; j++)
		b[i*n + j] = rem(a, j, i, n) / deta;
}