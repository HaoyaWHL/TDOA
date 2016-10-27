#include "TaylorAlgo.h"
#include "Matrix.h"
#include "DELAY_LOC.h"

#include <math.h>
#include <fstream> 
#include <iostream>
using namespace std;
void TAYLOR_ALGO::ReadTxt(double tdoa[4])
{
	vector<double> inputData;
	inputData.resize(4);
	//char buffer[100];
	//ifstream myfile (filePath);  
	/*if(!myfile)
	{  
		cout << "Unable to open myfile";  
		exit(1); // terminate with error  
	}
	while (myfile.getline(buffer, 100))  
    {  
		inputData[0] = atof(strtok(buffer," "));
		inputData[1] = atof(strtok(NULL," "));
		inputData[2] = atof(strtok(NULL," "));
		inputData[3] = atof(strtok(NULL," "));
		inputTdoaData.push(inputData);
    }  */

	//MS[0] = 2.60;
	//MS[1] = 2.33;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			this->BS[row][col] = ::BS[row][col];
		}
	}
	inputData[0] = tdoa[0];
	inputData[1] = tdoa[1];
	inputData[2] = tdoa[2];
	inputData[3] = tdoa[3];
	inputTdoaData.push(inputData);


	delay[0] = DELAY[0];
	delay[1] = DELAY[1];
	delay[2] = DELAY[2];
	delay[3] = DELAY[3];
	tdoaDataNodelay.resizeMat(inputTdoaData.getRowSize(),inputTdoaData.getColSize());
	for(int row = 0;row < inputTdoaData.getRowSize();row++)
	{
		for(int col = 0;col < inputTdoaData.getColSize();col++)
		{
			tdoaDataNodelay.setValue(row,col,inputTdoaData.at(row,col) + delay[col]);
		}
	}
}
void TAYLOR_ALGO::TaylorCalcXY(double x,double y)
{
	double meanTdoaData[3] = {0,0,0};
	for(int col = 0;col < 3;col++)
	{
		for(int row = 0;row < tdoaDataNodelay.getRowSize();row++)
		{
			meanTdoaData[col] += (tdoaDataNodelay.at(row,col+1)-tdoaDataNodelay.at(row,0));
		}
		meanTdoaData[col] /= tdoaDataNodelay.getRowSize();
	}

	MAT XQ[3] = {};
	for(int row = 0;row < 3;row++)
	{
		XQ[row].resizeMat(1,tdoaDataNodelay.getRowSize());
		for(int col = 0;col < tdoaDataNodelay.getRowSize();col++)
		{
			XQ[row].setValue(0,col,tdoaDataNodelay.at(col,row+1) - tdoaDataNodelay.at(col,0));
		}
	}
	MAT Q(3,3);
	for(int row = 0;row<3;row++)
	{
		for(int col = 0;col<3;col++)
		{
			Q.setValue(row,col,((XQ[row]-(meanTdoaData[row]))*((XQ[col]-(meanTdoaData[col])).MyMatTanspose())).at(0,0));
		}
	}
	MAT QReal = Q.MyMatDivide(XQ[0].getColSize());
	double delta[2] = {10,10};
	double xTaylor = x;
	double yTaylor = y;
	while(fabs(delta[0])+fabs(delta[1]) > 0.000001)
	{
		double RTaylor21 = ((tdoaDataNodelay.MyGetCol(1)-tdoaDataNodelay.MyGetCol(0))*0.3).MyMean(COL,0);
		double RTaylor31 = ((tdoaDataNodelay.MyGetCol(2)-tdoaDataNodelay.MyGetCol(0))*0.3).MyMean(COL,0);
		double RTaylor41 = ((tdoaDataNodelay.MyGetCol(3)-tdoaDataNodelay.MyGetCol(0))*0.3).MyMean(COL,0);

		double X1Taylor = BS[0][0];
		double Y1Taylor = BS[0][1];
		double X2Taylor = BS[1][0];
		double Y2Taylor = BS[1][1];
		double X3Taylor = BS[2][0];
		double Y3Taylor = BS[2][1];
		double X4Taylor = BS[3][0];
		double Y4Taylor = BS[3][1];
		
		double R1Taylor = sqrt( pow(X1Taylor-xTaylor,2.0)+ pow(Y1Taylor-yTaylor,2.0));
		double R2Taylor = sqrt( pow(X2Taylor-xTaylor,2.0)+ pow(Y2Taylor-yTaylor,2.0));
		double R3Taylor = sqrt( pow(X3Taylor-xTaylor,2.0)+ pow(Y3Taylor-yTaylor,2.0));
		double R4Taylor = sqrt( pow(X4Taylor-xTaylor,2.0)+ pow(Y4Taylor-yTaylor,2.0));
		double hTaylorArr[3] = {RTaylor21 - (R2Taylor - R1Taylor),
								RTaylor31 - (R3Taylor - R1Taylor),
								RTaylor41 - (R4Taylor - R1Taylor)};
		MAT hTaylor;
		hTaylor.ArrayToMat_Constructor(hTaylorArr,3,1);
		double GTaylorArr[] = {
				(X1Taylor - xTaylor)/R1Taylor - (X2Taylor - xTaylor)/R2Taylor,(Y1Taylor - yTaylor)/R1Taylor - (Y2Taylor - yTaylor)/R2Taylor,
				(X1Taylor - xTaylor)/R1Taylor - (X3Taylor - xTaylor)/R3Taylor,(Y1Taylor - yTaylor)/R1Taylor - (Y3Taylor - yTaylor)/R3Taylor,
				(X1Taylor - xTaylor)/R1Taylor - (X4Taylor - xTaylor)/R4Taylor,(Y1Taylor - yTaylor)/R1Taylor - (Y4Taylor - yTaylor)/R4Taylor};
		MAT GTaylor;
		GTaylor.ArrayToMat_Constructor(GTaylorArr,3,2);
		MAT QTaylor = QReal;
		double QStub[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
		MAT QStubMat;
		QStubMat.ArrayToMat_Constructor(QStub, 3, 3);
		QTaylor = QStubMat;
		//MAT deltaMat = (GTaylor.MyMatTanspose() *QTaylor.MyInv()*GTaylor).MyInv()  *GTaylor.MyMatTanspose() *QTaylor.MyInv() *hTaylor;
		MAT debugMat1 = (GTaylor.MyMatTanspose() *QTaylor.MyInv()*GTaylor).MyInv() ;
		MAT debugMat2 = debugMat1*GTaylor.MyMatTanspose() ;
		MAT debugMat3 = debugMat2 *QTaylor.MyInv();
		MAT deltaMat = debugMat3*hTaylor;
		delta[0] = deltaMat.at(0,0);
		delta[1] = deltaMat.at(1,0);
		if( (abs(delta[0]) + abs(delta[1])) > 0.000001)
		{
			xTaylor = xTaylor + delta[0];
			yTaylor = yTaylor + delta[1];
		}
	}
	resultPosition.x = xTaylor;
	resultPosition.y = yTaylor;
	calcFlag = true;
}

void TAYLOR_ALGO::TaylorCalcXYZ(double x,double y,double z)
{
	double meanTdoaData[3] = {0,0,0};
	for(int col = 0;col < 3;col++)
	{
		for(int row = 0;row < tdoaDataNodelay.getRowSize();row++)
		{
			meanTdoaData[col] += (tdoaDataNodelay.at(row,col+1)-tdoaDataNodelay.at(row,0));
		}
		meanTdoaData[col] /= tdoaDataNodelay.getRowSize();
	}

	MAT XQ[3] = {};
	for(int row = 0;row < 3;row++)
	{
		XQ[row].resizeMat(1,tdoaDataNodelay.getRowSize());
		for(int col = 0;col < tdoaDataNodelay.getRowSize();col++)
		{
			XQ[row].setValue(0,col,tdoaDataNodelay.at(col,row+1) - tdoaDataNodelay.at(col,0));
		}
	}
	MAT Q(3,3);
	for(int row = 0;row<3;row++)
	{
		for(int col = 0;col<3;col++)
		{
			Q.setValue(row,col,((XQ[row]-(meanTdoaData[row]))*((XQ[col]-(meanTdoaData[col])).MyMatTanspose())).at(0,0));
		}
	}
	MAT QReal = Q.MyMatDivide(XQ[0].getColSize());
	double QStub[9] = { 1,0,0,0,1,0,0,0,1 };
	MAT QStubMat;
	QStubMat.ArrayToMat_Constructor(QStub, 3, 3);
	QReal = QStubMat;
	double delta[3] = {10,10,10};
	double xTaylor = x;
	double yTaylor = y;
	double zTaylor = z;
	while(fabs(delta[0])+fabs(delta[1])+fabs(delta[2])  > 0.000001)
	{
		double RTaylor21 = ((tdoaDataNodelay.MyGetCol(1)-tdoaDataNodelay.MyGetCol(0))*0.3).MyMean(COL,0);
		double RTaylor31 = ((tdoaDataNodelay.MyGetCol(2)-tdoaDataNodelay.MyGetCol(0))*0.3).MyMean(COL,0);
		double RTaylor41 = ((tdoaDataNodelay.MyGetCol(3)-tdoaDataNodelay.MyGetCol(0))*0.3).MyMean(COL,0);

		double X1Taylor = BS[0][0];
		double Y1Taylor = BS[0][1];
		double Z1Taylor = BS[0][2];
		double X2Taylor = BS[1][0];
		double Y2Taylor = BS[1][1];
		double Z2Taylor = BS[1][2];
		double X3Taylor = BS[2][0];
		double Y3Taylor = BS[2][1];
		double Z3Taylor = BS[2][2];
		double X4Taylor = BS[3][0];
		double Y4Taylor = BS[3][1];
		double Z4Taylor = BS[3][2];
		
		double R1Taylor = sqrt( pow(X1Taylor-xTaylor,2.0)+ pow(Y1Taylor-yTaylor,2.0)+pow(Z1Taylor-zTaylor,2.0) );
		double R2Taylor = sqrt( pow(X2Taylor-xTaylor,2.0)+ pow(Y2Taylor-yTaylor,2.0)+pow(Z2Taylor-zTaylor,2.0) );
		double R3Taylor = sqrt( pow(X3Taylor-xTaylor,2.0)+ pow(Y3Taylor-yTaylor,2.0)+pow(Z3Taylor-zTaylor,2.0) );
		double R4Taylor = sqrt( pow(X4Taylor-xTaylor,2.0)+ pow(Y4Taylor-yTaylor,2.0)+pow(Z4Taylor-zTaylor,2.0) );
		double hTaylorArr[3] = {RTaylor21 - (R2Taylor - R1Taylor),
								RTaylor31 - (R3Taylor - R1Taylor),
								RTaylor41 - (R4Taylor - R1Taylor)};
		MAT hTaylor;
		hTaylor.ArrayToMat_Constructor(hTaylorArr,3,1);
		double GTaylorArr[] = {
				(X1Taylor - xTaylor)/R1Taylor - (X2Taylor - xTaylor)/R2Taylor,(Y1Taylor - yTaylor)/R1Taylor - (Y2Taylor - yTaylor)/R2Taylor,(Z1Taylor - zTaylor)/R1Taylor - (Z2Taylor -zTaylor)/R2Taylor,
				(X1Taylor - xTaylor)/R1Taylor - (X3Taylor - xTaylor)/R3Taylor,(Y1Taylor - yTaylor)/R1Taylor - (Y3Taylor - yTaylor)/R3Taylor,(Z1Taylor - zTaylor)/R1Taylor - (Z3Taylor -zTaylor)/R3Taylor,
				(X1Taylor - xTaylor)/R1Taylor - (X4Taylor - xTaylor)/R4Taylor,(Y1Taylor - yTaylor)/R1Taylor - (Y4Taylor - yTaylor)/R4Taylor,(Z1Taylor - zTaylor)/R1Taylor - (Z4Taylor -zTaylor)/R4Taylor};
		MAT GTaylor;
		GTaylor.ArrayToMat_Constructor(GTaylorArr,3,3);
		MAT QTaylor = QReal;
		//MAT deltaMat = (GTaylor.MyMatTanspose() *QTaylor.MyInv()*GTaylor).MyInv()  *GTaylor.MyMatTanspose() *QTaylor.MyInv() *hTaylor;
		MAT debugMat1 = (GTaylor.MyMatTanspose() *QTaylor.MyInv()*GTaylor).MyInv() ;
		MAT debugMat2 = debugMat1*GTaylor.MyMatTanspose() ;
		MAT debugMat3 = debugMat2 *QTaylor.MyInv();
		MAT deltaMat = debugMat3*hTaylor;
		delta[0] = deltaMat.at(0,0);
		delta[1] = deltaMat.at(1,0);
		delta[2] = deltaMat.at(2,0);
		if( (abs(delta[0]) + abs(delta[1]) + abs(delta[2])) > 0.000001)
		{
			xTaylor = xTaylor + delta[0];
			yTaylor = yTaylor + delta[1];
			zTaylor = zTaylor + delta[2];
		}
	}
	resultPosition.x = xTaylor;
	resultPosition.y = yTaylor;
	resultPosition.z = zTaylor;
	calcFlag = true;
}
bool TAYLOR_ALGO::GetCalcResult(double *desX,double *desY)
{
	if(calcFlag == false)
	{
		cout<<"calc fail or you didn't calc!"<<endl;
		return false;
	}
	*desX = resultPosition.x;
	*desY = resultPosition.y;
	return true;
}

bool TAYLOR_ALGO::GetCalcResult(double *desX,double *desY,double *desZ)
{
	if(calcFlag == false)
	{
		cout<<"calc fail or you didn't calc!"<<endl;
		return false;
	}
	*desX = resultPosition.x;
	*desY = resultPosition.y;
	*desZ = resultPosition.z;
	return true;
}

TAYLOR_ALGO::TAYLOR_ALGO()
{
	calcFlag = false;
}

TAYLOR_ALGO::~TAYLOR_ALGO()
{
}

