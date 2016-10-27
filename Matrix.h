#pragma once
#ifndef MATRIX_FILE
#define MATRIX_FILE
/*Matrix.h*/
#include <vector>
using namespace std;

enum DIM
{
	ROW = 0,
	COL,
	BUTT
};

class MAT
{
public:
	MAT(){}

	MAT(int row,int col)
	{resizeMat(row,col);}

	MAT(const MAT& inMat)
	{mMat = inMat.getMyMat();}

	void operator =(vector<vector<double>> value);

	~MAT(){}

	int getRowSize()
	{return mMat.size();}

	vector<vector<double>> getMyMat()const
	{return mMat;}

	void push(vector<double> inVec)
	{mMat.push_back(inVec);}

	int getColSize()
	{return mMat[0].size();}

	void setValue(int row,int col,double value)
	{mMat[row][col] = value;}

	void resizeMat(int row,int col);
	double at(int row,int col);
    MAT MyMatrixDiffConstValue (double conValue);
	MAT MyMatTanspose();
	MAT MyMatDivide(double value);
	MAT MyInv ();
	MAT MyGetCol(int colNum);
	MAT MyGetRow(int rowNum);
	double MyMean(DIM inDim,int index);
	MAT operator -(MAT &matRight);
	MAT operator -(double value);
	MAT operator *(MAT &matRight);
	MAT operator *(double multiValue);
	bool MyMatMatch_DimEqual(MAT &matRight);
	bool MyMatMatch_DimMulti(MAT &matRight);
	void ArrayToMat_Constructor(double *inArray,int rowSize,int colSize);
private:
	vector<vector<double>> mMat;
};



#endif