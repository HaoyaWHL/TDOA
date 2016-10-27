#include "Matrix.h"
#include "debugModule.h"
#include "inv.h"
#include <vector>
#include <assert.h>
#include <iostream>
using namespace std;


void MAT::resizeMat(int row,int col)
{
	mMat.resize(row);
	for(int curRow = 0;curRow < row;curRow++)
	{
		mMat[curRow].resize(col);
	}
}

double MAT::at(int row,int col)
{
	return mMat[row][col];
}




MAT MAT::MyMatDivide(double value)
{
	MAT outMat(getRowSize(),getColSize());
	for(int row = 0;row < outMat.getRowSize();row++)
	{
		for(int col=0;col<outMat.getColSize();col++)
		{
			outMat.setValue(row,col,at(row,col)/value);
		}
	}
	return outMat;
}
MAT MAT::MyMatTanspose()
{
	MAT outMat(this->getColSize(),this->getRowSize());
	for(int row = 0;row < outMat.getRowSize();row++)
	{
		for(int col = 0;col < outMat.getColSize();col++)
		{
			outMat.setValue(row,col,this->at(col,row));
		}
	}
	return outMat;
}

MAT MAT::MyMatrixDiffConstValue (double conValue)
{
	MAT outMat(getRowSize(),getColSize());
	for(int row = 0;row < getRowSize(); row++)
	{
		for(int col = 0;col <getColSize();col++)
		{
			outMat.setValue(row,col,mMat[row][col] - conValue);
		}
	}
	return outMat;
}

MAT MAT::MyInv ()
{
	if(this->getColSize() != this->getRowSize())
	{
		MyAssert(true,"inv mat row != col !");
	}
	const int rowSize = this->getColSize();
	double inMatArray[9] = {} ;
	double outMatArray[9] = {}; 
	for(int row = 0;row <rowSize; row++)
	{
		for(int col =0;col<rowSize;col++)
		{
			inMatArray[row*rowSize+col] = this->at(row,col);
		}
	}
	inv(inMatArray,outMatArray,rowSize);
	MAT outMatMat(rowSize,rowSize);
	for(int row = 0;row <rowSize; row++)
	{
		for(int col =0;col<rowSize;col++)
		{
			outMatMat.setValue(row,col,(outMatArray[row*rowSize+col]));
		}
	}
	return outMatMat;
}

MAT MAT::MyGetCol(int colNum)
{
	MAT outMat(getRowSize(),1);
	for(int row = 0;row<getRowSize();row++)
	{
		outMat .setValue(row,0,at(row,colNum));
	}
	return outMat;
}
MAT MAT::MyGetRow(int rowNum)
{
	MAT outMat(1,getColSize());
	for(int col = 0;col<getRowSize();col++)
	{
		outMat .setValue(0,col,at(rowNum,col));
	}
	return outMat;
}


void MAT::operator =(vector<vector<double>> value)
{
	this->resizeMat(value.size(),value[0].size());
	for(int row = 0;row<getRowSize();row++)
	{
		for(int col = 0;col<getColSize();col++)
		{
			this->setValue(row,col,value[row][col]);
		}
	}
}


double MAT::MyMean(DIM inDim,int index)
{
	double meanValue = 0;
	if(inDim == COL)
	{
		for(int row = 0;row<this->getRowSize();row++)
		{
			meanValue += this->at(row,index);
		}
		meanValue /= this->getRowSize();
	}
	else
	{
		for(int col = 0;col<this->getColSize();col++)
		{
			meanValue += this->at(index,col);
		}
		meanValue /= this->getColSize();
	}
	return meanValue;
}

MAT MAT::operator -(MAT &matRight)
{
	if(!this->MyMatMatch_DimEqual( matRight))
	{
		MyAssert(1,"MAT not match!");
	}
	MAT outMat (this->getRowSize(),this->getColSize());
	for(int row = 0;row<this->getRowSize();row++)
	{
		for(int col =0;col<this->getColSize();col++)
		{
			outMat.setValue(row,col,this->at(row,col) - matRight.at(row,col));
		}
	}
	return outMat;
}

MAT MAT::operator -(double value)
{
	MAT outMat(getRowSize(),getColSize());
	for(int row = 0;row < getRowSize(); row++)
	{
		for(int col = 0;col <getColSize();col++)
		{
			outMat.setValue(row,col,mMat[row][col] - value);
		}
	}
	return outMat;
}

MAT MAT:: operator *(MAT &matRight)
{
	if((this->getColSize() != matRight.getRowSize()))
	{
		cout<<"matrix left col not equals matrix right row.";
		assert(1);
	}
	MAT outMatrix(this->getRowSize(),matRight.getColSize());
	
	for(int row = 0;row < this->getRowSize();row++)
	{
		for(int col = 0;col < matRight.getColSize();col++)
		{
			double tempValue = 0;
			for(int index = 0;index < this->getColSize();index++)
			{
				tempValue += this->at(row,index)*matRight.at(index,col);
			}
			outMatrix.setValue(row,col,tempValue);
		}
	}
	return outMatrix;
}

MAT MAT::operator *(double multiValue)
{
	MAT outMat(*this);
	for(int row = 0;row<getRowSize();row++)
	{
		for(int col = 0;col<getColSize();col++)
		{
			outMat.setValue(row,col,at(row,col)*multiValue);
		}
	}
	return outMat;
}

bool MAT::MyMatMatch_DimEqual(MAT &matRight)
{
	if(this->getRowSize() == matRight.getRowSize() && this->getColSize() == matRight.getColSize())
	{
		return true;
	}
	else
	{
		return false;
	}
}





bool MAT::MyMatMatch_DimMulti(MAT &matRight)
{
	if(this->getColSize() == matRight.getRowSize())
	{
		return true;
	}
	else
	{
		return false;
	}
}



void MAT::ArrayToMat_Constructor(double *inArray,int rowSize,int colSize)
{
	this->resizeMat(rowSize,colSize);
	for(int row=0;row<rowSize;row++)
	{
		for(int col=0;col<colSize;col++)
		{
			this->setValue(row,col,inArray[row*colSize + col]);
		}
	}
	return ;
}

