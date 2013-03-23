/*
matrix.cpp
*/
#include <cstdlib>
#include <iostream>
#include <string>
#include "Number.h"
#include "matrix.h"
using namespace std;

/////////////////////////
// constructors
/////////////////////////
Matrix::Matrix(int rows, int cols): rows(rows), cols(cols)
{
	for (int row = 0; row < rows; row++)
	{
		vector<Number> newRow;
		for (int col = 0; col < cols; col++)
		{
			newRow.push_back(Number());
		}
		push_back(newRow);
	}
}
// identity n x n matrix
Matrix::Matrix(int n): rows(n), cols(n)
{
	for (int row = 0; row < rows; row++)
	{
		vector<Number> newRow;
		for (int col = 0; col < cols; col++)
		{
			if (row == col)
				newRow.push_back(Number(1));
			else
				newRow.push_back(Number(0));
		}
		push_back(newRow);
	}
}
Matrix::Matrix():rows(0), cols(0)
{
}

/////////////////////////
// accessors
/////////////////////////

Number Matrix::getNum(int row, int col) const
{
	
	if (row > rows || col > cols)
	{
		cerr << "Invalid. Trying to set " << row << " row and " << col << " cols but only have " << rows << " rows and " << cols << " cols." << endl;;
		exit(0);
	}
	return at(row).at(col);
}
// return number of rows
int Matrix::getRows() const
{
	return rows;
}
// return number of columns
int Matrix::getCols() const
{
	return cols;
}
// return i'th row
vector<Number> Matrix::getRowAt(int i)
{
	return at(i);
}
// return i'th column
vector<Number> Matrix::getColAt(int i)
{
	vector<Number> column;
	for (int row = 0; row < rows; row++)
	{
		column.push_back(at(row).at(i));
	}
	return column;
}

/////////////////////////
// modifiers
/////////////////////////

void Matrix::setNum(int row, int col, Number setTo)
{
	if (row > rows || col > cols)
	{
		cerr << "Invalid. Trying to set " << row << " row and " << col << " cols but only have " << rows << " rows and " << cols << " cols." << endl;;
		exit(0);
	}
	at(row).at(col) =  setTo;
}

void Matrix::setRows(int r)
{
	if (r<0)
	{
		cerr << "rows cannot be negative\n";
		exit(1);
	}
	rows = r;
	resize(r);
}
void Matrix::setCols(int c)
{
	if (c<0)
	{
		cerr << "cols cannot be negative\n";
		exit(1);
	}
	cols = c;
	for (int row = 0; row < rows; row++)
	{
		at(row).resize(c);
	}
}
/////////////////////////
// methods
/////////////////////////

Matrix Matrix::cofactorMatrix(int i, int j)
{
	Matrix newMatrix;
	for (int k = 0; k < rows; k++)
	{
		if (k != i)
		{
			vector<Number> nRow = getRowAt(k);
			nRow.erase(nRow.begin()+j);
			newMatrix.pushRow(nRow);
		}
	}
	return newMatrix;
}

Number Matrix::cofactor(int i, int j)
{
	Number det = cofactorMatrix(i, j).determinant();

	if ((i+j)%2 == 0)
		return det;
	else
		return -det;
}

// mutually recursive determinant function
Number Matrix::determinant()
{
	if (rows != cols)
	{
		cerr << "can't take determinant of " << rows << " rows and " << cols << " cols." << endl;
		exit(1);
	}
	if (rows == 1)
		return getNum(0,0);
	if (rows == 2)
	{
		return (getNum(0,0)*getNum(1,1) - getNum(1,0)*getNum(0,1));
	}
	Number detSoFar = 0;
	// loop through columns
	for (int j = 0; j < rows; j++)
	{
		// if a(i) is 0
		if (getNum(0, j) == Number())
			continue;
		detSoFar = detSoFar + getNum(0,j)*cofactor(0,j);
	}
	return detSoFar;
}

// transpose matrix
Matrix Matrix::transpose()
{
	rows = this->getRows();
	cols = this->getCols();
	Matrix Transposed(cols, rows);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols ; j++)
		{
			Number newNum = at(i).at(j);
			Transposed.setNum(j,i,newNum);
		}	
	}
	return Transposed;
}

Number Matrix::toNum()
{
	if ( rows != 1 || cols != 1)
	{
		cerr << "matrix must be 1x1 for toNum. There are " << rows << " rows and " << cols << " cols." << endl;
		exit(1);
	}
	return at(0).at(0);

}

// return inverse of matrix

Matrix Matrix::inverse()
{
	Number det = determinant();
	if (determinant() == Number())
	{
		cerr << "Matrix not invertible." << endl;
		exit(1);
	}
	Matrix inv(rows);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			inv.setNum(i,j,cofactor(j,i)/det);
		}
	}
	return inv;
}

// return only columns in basis
// ie. basis = (1, 3)
// matrix = (1,2,3)
//          (4,5,6)
// returns:
// (1,3)
// (4,6)
Matrix Matrix::matrixBasis(vector<int> basis)
{
	Matrix mB;
	for (int i = 0; i < rows; i++)
	{
		vector<Number> newRow;
		for (vector<int>::iterator itr = basis.begin(); itr != basis.end(); ++itr)
		{
			newRow.push_back(getNum(i,*itr));
		}
		mB.pushRow(newRow);
	}
	return mB;
}
// append toAppend on the right side
void Matrix::append(Matrix toAppend)
{
	if (rows != toAppend.getRows())
	{
		cerr << "mismatching number of rows to append" << endl;
		exit(1);
	}
	cols += toAppend.getCols();
	for (int row = 0; row < rows; row++)
	{
		for (int pos = 0; pos < toAppend.getCols(); pos++)
		{
			Number newNum = toAppend.getNum(row, pos);
			at(row).push_back(toAppend.getNum(row, pos));
		}
	}
}
// append toAppend to the bottom
void Matrix::appendBottom(Matrix toAppend)
{
	if (cols != toAppend.getCols())
	{
		cerr << "mismatching number of cols to append" << endl;
		exit(1);
	}
	rows += toAppend.getRows();
	for (int row = 0; row < toAppend.getRows(); row++)
	{
		vector<Number> newRow;
		for (int col = 0; col < toAppend.getCols(); col++)
		{
			Number newNum = toAppend.getNum(row,col);
			newRow.push_back(newNum);
		}
		push_back(newRow);
	}
}

void Matrix::pushRow(vector<Number> row)
{
	if (size() != 0 && cols != row.size())
	{
		cerr << "mismatching number of columns." << endl;
		exit(1);
	}
	push_back(row);
	cols = row.size();
	rows++;
}

void Matrix::printRowAt(int row)
{
	cout << "|";
	for (int j = 0; j < at(row).size() ; j++)
	{
		Number num = at(row).at(j);
		// if positive
		if (num >= Number())
		{
			cout << " ";
		}
		// if whole number
		if (num.denominator() == 1)
		{
			cout << "   " << num.numerator() << " ";
		}
		else
		{
			cout << " " << num << " ";
		}
	}
	cout << "|";
}

bool Matrix::isValidMatrix()
{
	// all rows must have same size
	int columns = at(0).size();
	for (int i = 0; i < size(); i++)
	{
		if (at(i).size() != columns)
			return false;
	}
	return true;
}

void Matrix::setCorrectSize()
{
	rows = size();
	if (size() == 0)
		cols = 0;
	cols = at(0).size();
}

bool Matrix::isNonPositive()
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (getNum(row, col) > 0)
			{
				return false;
			}
		}
	}
	return true;
}

bool Matrix::oneColNonPositive()
{
	for (int col = 0 ; col < cols; col++)
	{
		bool negative = true;
		for(int row = 0; row < rows; row++)
		{
			if (getNum(row, col) > 0)
			{
				negative = false;
				break;
			}
		}
		if (negative)
		{
			return true;
		}
	}
	return false;
}


/////////////////////////
// operator overloads
/////////////////////////
Matrix Matrix::operator+(const Matrix & toAdd)
{
	if (rows != toAdd.getRows() || cols != toAdd.getCols())
	{		
		cerr << "Invalid Dimensions" << endl;
		exit(0);
	}	
	Matrix sum(rows,cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols ; j++)
		{
			Number sumNum = at(i).at(j)+ toAdd.getNum(i, j);
			sum.setNum(i, j, sumNum);
		}	
	}
	return sum;
}
Matrix Matrix::operator-(const Matrix & toSub)
{
	if (rows != toSub.getRows() || cols != toSub.getCols())
	{		
		cerr << "Invalid Dimensions" << endl;
		exit(0);
	}	
	Matrix diff(rows,cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols ; j++)
		{
			Number diffNum = at(i).at(j) - toSub.getNum(i, j);
			diff.setNum(i, j, diffNum);
		}	
	}
	return diff;
}

Matrix Matrix::operator-()
{
	Matrix negate(rows,cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols ; j++)
		{
			Number num = at(i).at(j);
			num.numerator( -1*(num.numerator()) );
		}	
	}
	return negate;
}

Matrix Matrix::operator*(const Matrix & toMul)
{
	if (cols != toMul.getRows())
	{
		cerr << "Invalid Dimensions to multiply" << endl;
		exit(0);	
	}
	Matrix prod(rows, toMul.getCols());
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < toMul.getCols(); j++)
		{
			Number num(0);
			for (int k = 0; k < cols; k++)
			{
				num = num + at(i).at(k)*(toMul.getNum(k, j));
			}
			prod.setNum(i, j, num);
		}

	}
	return prod;
}


/////////////////////////
// streams
/////////////////////////
istream & operator>>(istream & stream, Matrix &m)
{
	// get number of rows and columns first
	int rows, cols;
	stream >> rows;
	stream >> cols;
	m.setRows(rows);
	m.setCols(cols);
	
	for (int i = 0; i < rows ; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			Number newNum;
			stream >> newNum;
			m.setNum(i,j,newNum);
		}	
	}
	return stream;
}

ostream & operator<< (std::ostream & stream, const vector<Number> numVec)
{
	cout << "(" ;
	for (int j = 0; j < numVec.size(); j++)
	{
		Number num = numVec.at(j);
		// if positive
		if (num >= Number())
		{
			stream << " ";
		}
		// if whole number
		if (num.denominator() == 1)
		{
			stream << "   " << num.numerator() << " ";
		}
		else
		{
			stream << " " << num << " ";
		}
	}
	stream << ")";
	return stream;
}
ostream & operator<< (std::ostream & stream, const vector<int> & vec)
{
	stream << "(";
	for(int i = 0; i < vec.size(); i++)
	{
		stream << " " << vec.at(i) << " ";
	}
	stream << ")";
	return stream;
}


ostream & operator<<(ostream & stream, const Matrix &m)
{
	string div1 = (m.getRows() == 1) ? "(" : "|";
	string div2 = (m.getRows() == 1) ? ")" : "|";
	for (int i = 0; i < m.getRows(); i++)
	{
		stream << div1;
		for (int j = 0; j < m.getCols(); j++)
		{
			Number num = m.getNum(i, j);
			// if positive
			if (num >= Number())
			{
				stream << " ";
			}
			// if whole number
			if (num.denominator() == 1)
			{
				stream << "   " << num.numerator() << " ";
			}
			else
			{
				stream << " " << num << " ";
			}
		}
		stream << div2;
		
		if (i != m.getRows() -1)
			stream << endl;
	}	
	return stream;
}

ostream & operator<<(ostream & stream, const vector<Number> & row)
{
	stream << "|";
	for (int j = 0; j < row.size(); j++)
	{
		Number num = row.at(j);
		// if positive
		if (num >= Number())
		{
			stream << " ";
		}
		// if whole number
		if (num.denominator() == 1)
		{
			stream << "   " << num.numerator() << " ";
		}
		else
		{
			stream << " " << num << " ";
		}
	}
	stream << "|";
	return stream;
}