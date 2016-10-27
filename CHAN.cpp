#include "CHAN.h"
#include <iostream>
#include "DELAY_LOC.h"
#include <cmath>
#include <iomanip>
using namespace std;

XYTDOA::XYTDOA()
{
	for (int i = 0; i < 4; ++i){data[i]=0;}
	for (int i = 0; i < 3; ++i){h[i]=0;}
	for (int i = 0; i < 3; ++i){Za0[i]=0;}
	for (int i = 0; i < 3; ++i){Za1[i]=0;}
	for (int i = 0; i < 3; ++i){ sh[i]=0;}

	R21=0;R31=0;R41=0;
	K1=0;K2=0;K3=0;K4=0;
	Za2[0]=0;Za2[1]=0;


	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			Ga[i][j]=0;
			FI[i][j]=0;
			Q[i][j]=0;
			B[i][j]=0;
			covZa[i][j]=0;
			Ba2[j][i]=0;
			sFI[i][j]=0;
		}
	}


	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			POS[i][j]=0;
		}
	}



	Q[0][0]= 1;	Q[1][1]= 1;	Q[2][2]= 1;

	sGa[0][0]= 1 ;
	sGa[0][1]= 0 ;

	sGa[1][0]= 0 ;
	sGa[1][1]= 1 ;

	sGa[2][0]= 1 ;
	sGa[2][1]= 1 ;

	BS_X_MAX=BS[0][0];
	BS_Y_MAX=BS[0][1];
	BS_X_MIN=BS[0][0];
	BS_Y_MIN=BS[0][1];


	for (int i = 0; i < 3; ++i)
	{
		if (BS_X_MAX<BS[i + 1][0])
		{
			BS_X_MAX=BS[i+1][0];
		}

		if (BS_Y_MAX<BS[i + 1][1])
		{
			BS_Y_MAX = BS[i + 1][1];
		}

		if (BS_X_MIN>BS[i+1][0])
		{
			BS_X_MIN = BS[i+1][0];
		}
		
		if (BS_Y_MIN>BS[i + 1][1])
		{
			BS_Y_MIN=BS[i+1][1];
		}
	}



}

XYTDOA::~XYTDOA()
{
	for (int i = 0; i < 4; ++i)
	{
		data[i]=0;
	}
}

double XYTDOA::GetData(double inputData[])
{
	for (int i = 0; i < 4; ++i)
	{
		data[i]=inputData[i];
	}
	return 0;
}

void XYTDOA::displayData()
{
	cout<<"the input data is:"<<endl;
	for (int i = 0; i < 4; ++i)
	{
		cout<<setprecision(10)<<data[i]<<"  ";
	}
	cout<<endl;
}

void XYTDOA::displayPOS()
{
	cout<<"the position of the object is:"<<endl;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			cout<<setprecision(5)<<POS[i][j]<<"  ";
		}
		cout<<endl;
	}

}

void XYTDOA::displayFinal_XY(double pos[2])
{
	pos[0] = POS_X;
	pos[1] = POS_Y;
	cout << "the chan algorithm : X & Y" << endl;
	cout << "POS_X=" << POS_X << endl;
	cout << "POS_Y=" << POS_Y << endl;
};




double XYTDOA::Calculate()
{
	for (int i = 0; i < 4; ++i)
	{
		data[i]=data[i]+DELAY[i];
	}

	R21=(data[1]-data[0])*0.3;
	R31=(data[2]-data[0])*0.3;
	R41=(data[3]-data[0])*0.3;	
    
	K1=pow(BS[0][0],2)+pow(BS[0][1],2);
	K2=pow(BS[1][0],2)+pow(BS[1][1],2);
	K3=pow(BS[2][0],2)+pow(BS[2][1],2);
	K4=pow(BS[3][0],2)+pow(BS[3][1],2);


	h[0]=0.5*(pow(R21,2)-K2+K1);
	h[1]=0.5*(pow(R31,2)-K3+K1);
	h[2]=0.5*(pow(R41,2)-K4+K1);

	x21=BS[1][0]-BS[0][0];
	x31=BS[2][0]-BS[0][0];
	x41=BS[3][0]-BS[0][0];

	y21=BS[1][1]-BS[0][1];
	y31=BS[2][1]-BS[0][1];
	y41=BS[3][1]-BS[0][1];

	Ga[0][0]= -x21;
	Ga[0][1]= -y21;
	Ga[0][2]= -R21;

	Ga[1][0]= -x31;
	Ga[1][1]= -y31;
	Ga[1][2]= -R31;

	Ga[2][0]= -x41;
	Ga[2][1]= -y41;	
	Ga[2][2]= -R41;

	Za0_Cal();

	B[0][0]= sqrt( pow( (BS[1][0]-Za0[0]),2 ) + pow( (BS[1][1]-Za0[1]),2 ) ); //wait for sqrt func!
	B[1][1]= sqrt( pow( (BS[2][0]-Za0[0]),2 ) + pow( (BS[2][1]-Za0[1]),2 ) ); 
	B[2][2]= sqrt( pow( (BS[3][0]-Za0[0]),2 ) + pow( (BS[3][1]-Za0[1]),2 ) ); 

	FI_Cal();
	Za1_Cal();

	Ba2[0][0]= Za1[0]-BS[0][0];
	Ba2[1][1]= Za1[1]-BS[0][1];
	Ba2[2][2]= Za1[2]; 

	sFI_Cal();

	sh[0]=pow(  (Za1[0]-BS[0][0])  ,2);
	sh[1]=pow(  (Za1[1]-BS[0][1])  ,2);
	sh[2]=pow(   Za1[2]            ,2);

	Za2_Cal();

	//now we can calculate the position of the object 
 	POS_Cal();









	return 0;
}



double XYTDOA::Za0_Cal()
{	
	double temp0[3][3]={0};//get transfer of Ga
	double temp1[3][3]={0};
	double temp2[3][3]={0};
	double temp3[3*3]={0};
	double temp4[3*3]={0};
	double temp5[3][3]={0};
	double temp6[3][3]={0};

	double temp7[3][3]={0};


	tMatrix(Ga,temp0);//Ga'=temp0
	mulMatri(temp0,Q,temp1);//Ga' * inv(Q) =temp1
	mulMatri(temp1, Ga, temp2);//Ga' * inv(Q) * Ga =temp2=temp3

	Matrix2Vector(temp2,temp3);

	inv(temp3, temp4, 3);//inv(Ga' * inv(Q) * Ga )=temp4=temp5
	Vector2Matrix(temp4, temp5);




	mulMatri(temp5,temp0,temp6);//inv()*Ga'=temp6
	mulMatri(temp6, Q, temp7);//inv()*Ga'*inv(Q)=temp7

	MatrixPlusVextor(temp7,h,Za0);	//inv()*Ga'*inv(Q)*h=temp8=Za0;


	return 0;
}

int XYTDOA::mulMatri(double x[3][3], double y[3][3], double z[3][3])
{
	int i, j, k;
	int m=3;
	int n=3;
	for (i = 0; i<m; i++)
		for (j = 0; j<m; j++){
		z[i][j] = 0;
	for (k = 0; k<n; k++)
		z[i][j] += x[i][k] * y[k][j];
	}
	return 0;
}

int XYTDOA::tMatrix(double x[3][3], double c[3][3])
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			c[i][j]=x[j][i];
		}
	}
	return 0;
}


//to calculate the inv of matrix a,get b finally
double XYTDOA::rem(double* a, int i, int j, int n)
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


double XYTDOA::det(double* a, int n)
{
	if (n == 1) return a[0];
	double sum = .0;
	for (int j = 0; j<n; j++) sum += a[0 * n + j] * rem(a, 0, j, n);
	return sum;
}



void XYTDOA::inv(double* a, double* b, int n)
{
	double deta = det(a, n); 
	//cout << deta << endl;
	for (int i = 0; i<n; i++)
	for (int j = 0; j<n; j++)
		b[i*n + j] = rem(a, j, i, n) / deta;
}




void XYTDOA::Matrix2Vector(double matrixA[3][3],double vectorB[3*3])
{
	int k=0;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			vectorB[k]=matrixA[i][j];
			k++;
		}
	}
}


void XYTDOA::Vector2Matrix(double vectorC[3*3],double matrixD[3][3])
{
	int k=0;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			matrixD[i][j]=vectorC[k];
			k++;
		}
	}
}


void XYTDOA::MatrixPlusVextor(double matrixE[3][3],double matrixF[3],double vectorC[3])
{
	vectorC[0]=matrixE[0][0]*matrixF[0]+matrixE[0][1]*matrixF[1]+matrixE[0][2]*matrixF[2];
	vectorC[1]=matrixE[1][0]*matrixF[0]+matrixE[1][1]*matrixF[1]+matrixE[1][2]*matrixF[2];
	vectorC[2]=matrixE[2][0]*matrixF[0]+matrixE[2][1]*matrixF[1]+matrixE[2][2]*matrixF[2];
}



void XYTDOA::FI_Cal()
{
	double temp0[3][3]={0};
	mulMatri(B,Q,temp0); //B*Q=temp0
	mulMatri(temp0,B,FI);//B*Q*B=temp1
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			FI[i][j]=pow(30000000,2)*FI[i][j];
		}
	}

}



void XYTDOA::Za1_Cal()
{
	double temp0[3][3]={0};//get transfer of Ga
	double temp1[3*3]={0};

	double temp2[3*3]={0};
	double temp3[3][3]={0};

	double temp4[3][3]={0};
	double temp5[3][3]={0};
	double temp6[3*3]={0};
	double temp7[3*3]={0};
	double temp8[3][3]={0};
	double temp9[3][3]={0};
	double temp10[3][3]={0};


	tMatrix(Ga,temp0);//Ga'=temp0

	Matrix2Vector(FI,temp1);//inv(FI)=temp1=temp2=temp3
	inv(temp1, temp2, 3);
	Vector2Matrix(temp2, temp3);

	mulMatri(temp0,temp3,temp4);//Ga' * inv(FI)=temp4
	mulMatri(temp4,Ga,temp5);// Ga' * inv(FI) * Ga =temp5


	Matrix2Vector(temp5,temp6);//inv(Ga' * inv(FI) * Ga)=temp6=temp7=temp8
	inv(temp6, temp7, 3);
	Vector2Matrix(temp7, temp8);

	mulMatri(temp8,temp0,temp9);//inv(Ga' * inv(FI) * Ga)*Ga'=temp9
	mulMatri(temp9,temp3,temp10);//inv(Ga' * inv(FI) * Ga)*Ga'*inv(FI)=temp10

	MatrixPlusVextor(temp10,h,Za1);

	if (Za1[2]<0)
	{
		Za1[2]=-Za1[2];
	}

	/////////////////////
	//calculate the covZa
	/////////////////////
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			covZa[i][j]=temp8[i][j];
		}
	}

}



void XYTDOA::sFI_Cal()
{
	double temp0[3][3]={0};
	double temp1[3][3]={0};



	mulMatri(Ba2,covZa,temp0);//Ba2*covZa=temp0
	mulMatri(temp0,Ba2,temp1);//Ba2*covZa*Ba2=temp1

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			sFI[i][j]=4*temp1[i][j];
		}
	}


}


void XYTDOA::Za2_Cal()
{


	double temp1[3*3]={0};
	double temp2[3*3]={0};
	double temp3[3][3]={0};

	double temp4[2][3]={0};
	double temp5[2][2]={0};

	double temp6[2*2]={0};
	double temp7[2*2]={0};
	double temp8[2][2]={0};

	double temp9[2][3]={0};
	//Za2 = inv(sGa'*inv(sFI)*sGa)*sGa'*inv(sFI)*sh;

	double temp0[2][3]={{1,0,1},{0,1,1}}; //sGa' =temp0

	Matrix2Vector(sFI,temp1);//inv(sFI)=temp1=temp2=temp3
	inv(temp1, temp2, 3);
	Vector2Matrix(temp2, temp3);

	mulMatri_23X33(temp0,temp3,temp4);//sGa'*inv(sFI)=temp4  2,3

	//sGa'*inv(sFI)*sGa=temp5
	temp5[0][0]=temp4[0][0]+temp4[0][2];
	temp5[0][1]=temp4[0][1]+temp4[0][2];
	temp5[1][0]=temp4[1][0]+temp4[1][2];
	temp5[1][1]=temp4[1][1]+temp4[1][2];

	Matrix2Vector_22(temp5,temp6);//inv(sGa'*inv(sFI)*sGa)
	inv(temp6, temp7, 2);//=temp5=temp6=temp7=temp8
	Vector2Matrix_22(temp7, temp8);

	//inv(sGa'*inv(sFI)*sGa)  *  sGa'*inv(sFI)=temp8*temp4=temp9
	for (int i = 0; i<2; i++)
	{
		for (int j = 0; j<3; j++)
		{
			for (int k = 0; k<2; k++)
				{temp9[i][j] += temp8[i][k] * temp4[k][j];}
		}
	}


	Za2[0]=temp9[0][0]*sh[0]+temp9[0][1]*sh[1]+temp9[0][2]*sh[2];
	Za2[1]=temp9[1][0]*sh[0]+temp9[1][1]*sh[1]+temp9[1][2]*sh[2];





}


double XYTDOA::mulMatri_23X33(double x[2][3], double y[3][3], double z[2][3])
{
	int i, j, k;
	int m=2;
	int n=3;
	for (i = 0; i<m; i++)
	{
		for (j = 0; j<n; j++)
		{
			z[i][j] = 0;
			for (k = 0; k<n; k++)
				{z[i][j] += x[i][k] * y[k][j];}
		}
	}
	return 0;
}


void XYTDOA::Matrix2Vector_22(double matrixA[2][2],double vectorB[2*2])
{
	int k=0;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			vectorB[k]=matrixA[i][j];
			k++;
		}
	}
}


void XYTDOA::Vector2Matrix_22(double vectorC[2*2],double matrixD[2][2])
{
	int k=0;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			matrixD[i][j]=vectorC[k];
			k++;
		}
	}
}



	/*
	POS1=sqrt(Za2)+BS(1,:)';
	POS2=-sqrt(Za2)+BS(1,:)';
	POS3=sqrt(Za2)-BS(1,:)';
	POS4=-sqrt(Za2)-BS(1,:)';
	*/
void XYTDOA::POS_Cal()
{
	POS[0][0] = sqrt(Za2[0]) + BS[0][0];
	POS[0][1] = sqrt(Za2[1]) + BS[0][1];

	POS[1][0] = -sqrt(Za2[0]) + BS[0][0];
	POS[1][1] = -sqrt(Za2[1]) + BS[0][1];

	POS[2][0] = sqrt(Za2[0]) - BS[0][0];
	POS[2][1] = sqrt(Za2[1]) - BS[0][1];

	POS[3][0] = -sqrt(Za2[0]) - BS[0][0];
	POS[3][1] = -sqrt(Za2[1]) - BS[0][1];

	for (int i = 0; i < 4; ++i)
	{
		if ((POS[i][0]<BS_X_MAX) && (POS[i][0]>BS_X_MIN))
		{
			POS_X = POS[i][0];
		}
	}



	for (int i = 0; i < 4; ++i)
	{
		if ((POS[i][1]<BS_Y_MAX) && (POS[i][1]>BS_Y_MIN))
		{
			POS_Y = POS[i][1];
		}
	}

}


double XYTDOA::Calculate_inital_Z(double pos[3])
{
	xTaylor=pos[0];
	yTaylor=pos[1];

	hTaylor[0]=hTaylor[0]-x21*xTaylor-y21*yTaylor;
	hTaylor[1]=hTaylor[1]-x31*xTaylor-y31*yTaylor;
	hTaylor[2]=hTaylor[2]-x41*xTaylor-y41*yTaylor;


	double A=0;
	double B=0;
	double temp=0;
	double pos_z[3]={0};

	temp=GTaylor[0][1]/GTaylor[1][1];
	A=GTaylor[0][0]-temp*GTaylor[1][0];
	B=hTaylor[0]-hTaylor[1]*temp;
	pos_z[0]=B/A;


	temp=GTaylor[0][1]/GTaylor[2][1];
	A=GTaylor[0][0]-temp*GTaylor[2][0];
	B=hTaylor[0]-hTaylor[2]*temp;
	pos_z[1]=B/A;


	temp=GTaylor[1][1]/GTaylor[2][1];
	A=GTaylor[1][0]-temp*GTaylor[2][0];
	B=hTaylor[1]-hTaylor[2]*temp;
	pos_z[2]=B/A;

	for (int i = 0; i < 2; i++)
	{
		if (pos_z[0]<pos_z[1])
		{
		}
		else
		{
			temp = pos_z[0];
			pos_z[0] = pos_z[1];
			pos_z[1] = temp;
		}


		if (pos_z[1]<pos_z[2])
		{
		}
		else
		{
			temp = pos_z[1];
			pos_z[1] = pos_z[2];
			pos_z[2] = temp;
		}
	}

	temp=pos_z[1];
	return temp;
}
