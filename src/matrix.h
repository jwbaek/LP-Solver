#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>
#include <vector>
#include "Number.h"
using namespace std;

class Matrix: public vector< vector <Number> >
{
    public:
        // constructor
        Matrix(int rows, int cols);
		Matrix(int n);				// identity n x n matrix
		Matrix();	

        // accessors
        Number getNum(int row, int col) const;
        int getRows() const;
        int getCols() const;
		vector<Number> getRowAt(int i);
		vector<Number> getColAt(int i);

		// modifiers
		void setNum(int row, int col, Number setTo);
        void setRows(int r);
        void setCols(int c);

		// methods
		Matrix cofactorMatrix(int i, int j);	// returns cofactor matrix
		Number cofactor(int i, int j);			// returns cofactor
		Number determinant();					// returns determinant
		Matrix inverse();						// returns inverse
		Matrix transpose();
		Number toNum();							// turn 1x1 matrix to number

		Matrix matrixBasis(vector<int> basis);

		void append(Matrix toAppend);
		void appendBottom(Matrix toAppend);
		void pushRow(vector<Number> row);
		void printRowAt(int row);
		bool isValidMatrix(); // not used
		void setCorrectSize(); // not used
		bool isNonPositive();
		bool oneColNonPositive();

        // operator overloads
        Matrix operator+(const Matrix & toAdd);
        Matrix operator-(const Matrix & toSub);
        Matrix operator-();
        Matrix operator*(const Matrix & toMul);

	protected:
        int rows;
        int cols;
};
// streams
ostream & operator<< (std::ostream & stream, const vector<Number> & row);
ostream & operator<< (std::ostream & stream, const vector<int> & row);
ostream & operator<< (std::ostream & stream, const Matrix & m);
istream & operator>> (std::istream & stream, Matrix & m);
#endif
