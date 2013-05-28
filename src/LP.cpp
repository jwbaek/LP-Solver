#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "LP.h"
#include "matrix.h"

using namespace std;


LP::LP()
{
	Number objConst;
	max = true;
	vector<Number> emptyVec;
	Matrix objective;
	objective.pushRow(emptyVec);
	Matrix constraint;
	Matrix RHS;
	RHS.pushRow(emptyVec);

}

// accessors
bool LP::isMax() const
{
	return max;
}
bool LP::isMin() const
{
	return !max;
}
// returns true if all the relations are the same
bool LP::isRelSame() const
{
	for(vector<LP::Rel>::const_iterator it = relVec.begin() ; it != relVec.end(); ++it)
	{
		if (*it != relVec.at(0))
			return false;
	}
	return true;
}
Matrix LP::getObjective() const
{
	return objective;
}
Matrix LP::getConstraint() const
{
	return constraint;
}
Matrix LP::getRHS() const
{
	return RHS;
}
vector<LP::Rel> LP::getRelVec() const
{
	return relVec;
}
Number LP::getObjConst() const
{
	return objConst;
}

LP::Rel LP::getRelAt(int row) const
{
	return relVec.at(row);
}
Number LP::getRHSAt(int row) const
{
	return RHS.at(row).at(0);
}


// modifiers
void LP::setIsMax(bool isMax)
{
	max = isMax;
}
void LP::setObjective(Matrix m)
{
	objective = m;
}
void LP::setConstraint(Matrix m)
{
	constraint = m;
}
void LP::setRHS(Matrix m)
{
	RHS = m;
}
void LP::setRelVec(vector<Rel> r)
{
	relVec = r;
}
void LP::setObjConst(Number r)
{
	objConst = r;
}

// push back functions
void LP::pushObjective(Number n)
{
	vector<Number> newRow;
	newRow.push_back(n);
	objective.pushRow(newRow);
}

void LP::pushConstraint(vector<Number> row)
{
	constraint.pushRow(row);
}
void LP::pushRHS(Number n)
{
	vector<Number> newRow;
	newRow.push_back(n);
	RHS.pushRow(newRow);
}
void LP::pushRelVec(LP::Rel r)
{
	relVec.push_back(r);
}
// methods

// precondition: LP is in SEF
LP LP::turnToCanonical(vector<int> basis)
{
	/*
	if (basis.size() != objective.size())
	{
		cerr << "invalid basis" << endl;
		exit(1);
	}
	*/
	for(int i = 0; i < basis.size(); ++i)
	{
		basis[i]--;
	}
	Matrix C_b = objective.transpose().matrixBasis(basis).transpose();
	Matrix A_b = constraint.matrixBasis(basis);
	Matrix y = A_b.inverse().transpose() * C_b;

	Matrix y_t = y.transpose();


	LP newLP;
	Matrix obj = y_t*RHS;
	Number newObjConstraint = objConst + (y.transpose() * RHS).toNum();
	newLP.setObjConst(newObjConstraint);
	Matrix newObjective = (objective.transpose() - y.transpose()*constraint).transpose();
	newLP.setObjective(newObjective);
	Matrix newConstraint = A_b.inverse()*constraint;
	newLP.setConstraint(A_b.inverse()*constraint);
	newLP.setRHS(A_b.inverse()*RHS);

	newLP.setRelVec(relVec);

	return newLP;
}

vector<int> LP::findNewBasis(vector<int> basis)
{
	sort (basis.begin(), basis.end());
	// find entering variable (smallest subscript)
	int entering = -1;
	for (int i = 0; i < objective.getRows(); i++)
	{
		if(objective.getNum(i,0) > 0)
		{
			entering = i;
			break;
		}
	}
	if (entering == -1)
	{
		cerr << "LP is already optimal" << endl;
		system("Pause");
		exit(0);
	}
	int toLeave = -1;
	Number lowestRatio;
	// put all ratios into a vector
	for (int i = 0; i < constraint.getRows(); i++)
	{
		if (constraint.getNum(i, entering) > 0)
		{
			// calculate ratio
			Number currRatio =  RHS.getNum(i,0)/constraint.getNum(i, entering);
			if ((toLeave == -1) || (currRatio < lowestRatio))
			{
				lowestRatio = currRatio;
				toLeave = basis.at(i);
			}
		}
	}
	entering++;
	cout << "To leave is " << toLeave << endl;
	cout << "Entering is " << entering << endl << endl;
	int myints[] = {toLeave, entering};
	std::vector<int> newBasis(basis);
	newBasis.push_back(entering);
	for(vector<int>::iterator itr = newBasis.begin(); itr != newBasis.end(); ++itr)
	{
		if (*itr == toLeave)
		{
			newBasis.erase(itr);
			break;
		}
	}
	sort (newBasis.begin(), newBasis.end());
	return newBasis;
}

LP LP::auxiliaryProblem()
{
	LP auxiliary;
	Matrix identity = Matrix(constraint.getRows());

	Matrix newConstraint = constraint;
	newConstraint.append(identity);
	auxiliary.setConstraint(newConstraint);

	Matrix newObjective(objective.getRows()+constraint.getRows(),objective.getCols());
	for (int i=0; i<objective.getRows(); i++)
	{
		newObjective.setNum(i,0,Number());
	}
	for (int j=objective.getRows(); j < newObjective.getRows(); j++)
	{
		newObjective.setNum(j,0,Number(-1));
	}
	auxiliary.setObjective(newObjective);

	auxiliary.setRHS(RHS);
	auxiliary.setRelVec(relVec);
	auxiliary.setObjConst(objConst);
	return auxiliary;
}

bool LP::isValidLP()
{
	if (!constraint.isValidMatrix())
		return false;

	if (constraint.getRows() != RHS.size() || constraint.getCols() != objective.size())
		return false;

	return true;
}

bool LP::isSEF()
{
	return isValidLP() && isMax() && isRelSame() && relVec[0] == 0;
}

// static functions
LP::Rel LP::stringToRel(string str)
{
	if (str == "=")
		return (LP::Rel) 0;
	if (str == "<=")
		return (LP::Rel) 1;
	if (str == ">=")
		return (LP::Rel) 2;
	cerr << "Illegal relation " << str << endl;
	exit(1);
}
string LP::relToString(LP::Rel r) 
{
	switch ((int) r)
	{
	case(0):
		return "=";
	case(1):
		return "<=";
	case(2):
		return ">=";
	default:
		cerr << "invalid relation" << endl;
		exit(1);
	}	
}

ostream & operator<< (std::ostream & stream, const LP::Rel & r)
{
	string rel = LP::relToString(r);
	if (rel == "=")
		stream << " ";
	stream << rel;
	return stream;
}

// printing LP
ostream & operator<< (std::ostream & stream, const LP & lp)
{
	// print objective function
	string maxOrMin = lp.isMax() ? "max " : "min ";
	Matrix cT = lp.getObjective().transpose();
	stream << maxOrMin << cT << "x";
	if (lp.getObjConst() != Number(0))
	{
		Number constant = lp.getObjConst();
		if (constant < Number(0))
			stream << " - " << constant;
		else
			stream << " + " << constant;
	}
	stream << endl;
	
	// print constraints
	Matrix cons = lp.getConstraint();
	for (int row = 0; row < cons.getRows(); row++)
	{
		stream << "    ";

		stream << cons.getRowAt(row);
		if (row != cons.getRows()/2)
			stream << "  ";
		else
			stream << "x ";


		stream << lp.getRelAt(row) << " " << lp.getRHSAt(row);
		stream << endl;
	}
	return stream;
}
// inputting LP
// eg.
// max (1 3)x
// (1 -3/2 4)x <= 5
// (2, 1, -5)x = -3/4
// (-4/3 3/2 3)x < 2/7
istream & operator>> (std::istream & stream, LP & lp)
{	
	// set max or min LP
	string maxOrMin;
	stream >> maxOrMin;
	if (maxOrMin == "max" || maxOrMin == "maximize" || maxOrMin == "Max" || maxOrMin == "Maximize")
		lp.setIsMax(true);
	else // minimize
		lp.setIsMax(false);

	// read in line by line
	string line;
	bool firstLine = true;
	while (getline(stream, line))
	{
		vector<Number> newRow;
		istringstream iss(line);
		string word;

		// read word by word in line
		while (iss >> word)
		{
			char first = word[0];
			// if relational
			if (first == '<' || first == '>' || first == '=')
			{
				LP::Rel newRel = LP::stringToRel(word);
				lp.pushRelVec(newRel);
				// ignore for now
				iss >> word;
				Number newNum(word);
				lp.pushRHS(newNum);
			}
			// if objective function and there is a constant
			else if (firstLine && (word == "+" || word == "-"))
			{
				iss >> word; // constant (z)
				Number newNum(word);
				lp.setObjConst(newNum);
			}
			else
			{
				Number newNum(word);
				newRow.push_back(newNum);
			}
		}

		// if this was first line, row is objective function
		if (firstLine)
		{
			for (int i=0 ; i < newRow.size(); i++)
			{
				lp.pushObjective(newRow.at(i));
			}
			firstLine = false;
		}

		// else push row into constraint 
		else
		{
			lp.pushConstraint(newRow);
		}
	}
	return stream;
}
