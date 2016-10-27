class XYTDOA
{
public:
	XYTDOA();
	~XYTDOA();
	double GetData(double data[4]);
	void displayData();
	void displayPOS();
	void displayFinal_XY(double pos[2]);
	double Calculate();
	double Calculate_inital_Z(double pos[3]);
private:
	double xTaylor;
	double yTaylor; 
	double data[4];

	double R21,R31,R41;
	double K1,K2,K3,K4;

	double x21,x31,x41;
	double y21,y31,y41;
	double z21,z31,z41;

	double Ga[3][3];
	double Q[3][3];
	double h[3];

	double hTaylor[3];
	double GTaylor[3][2];
	double K1Taylor;
	double K2Taylor;
	double K3Taylor;
	double K4Taylor;
	
	double Za0_Cal();

	int mulMatri(double x[3][3], double y[3][3], double z[3][3]);
	int tMatrix(double x[3][3], double c[3][3]);

	double Za0[3],Za1[3];
	double B[3][3];
	double FI[3][3];
	double covZa[3][3];
	double Ba2[3][3];
	double sFI[3][3];
	double sGa[3][2];
	double sh[3];
	double Za2[2];
	double POS[4][2];

	double BS_X_MAX;
	double BS_Y_MAX;
	double BS_X_MIN;
	double BS_Y_MIN;
	//new 
	double POS_X,POS_Y; 
	
	//to calculate the inv of matrix a,get b finally
	double rem(double* a, int i, int j, int n);
	double det(double* a, int n);
	void inv(double* a, double* b, int n);

	//
	void Matrix2Vector(double matrixA[3][3],double vectorB[3*3]);
	void Vector2Matrix(double vectorC[3 * 3], double matrixD[3][3]);

	//
	void MatrixPlusVextor(double matrixE[3][3],double matrixF[3],double vectorC[3]);
	void FI_Cal();
	void Za1_Cal();
	void sFI_Cal();
	void Za2_Cal();

	double mulMatri_23X33(double x[2][3], double y[3][3], double z[2][3]);
	void Matrix2Vector_22(double matrixA[2][2],double vectorB[2*2]);
	void Vector2Matrix_22(double vectorC[2*2],double matrixD[2][2]);
	void POS_Cal();



};




