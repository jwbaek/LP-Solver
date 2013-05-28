#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include "LP.h"
#include "matrix.h"

using namespace std;

Matrix findSolutionGivenCanonical(LP canonical, vector<int> basis)
{
	int numVars = canonical.getConstraint().getCols();
	Matrix solution(numVars, 1);
	for(int i = 0; i < basis.size(); i++)
	{
		solution.setNum(basis.at(i) - 1, 0, canonical.getRHS().getNum(i, 0));
	}
	return solution;
}

/*
 given a LP in SEF and a feasible basis,
 either
 1) return an optimal solution
 2) state it is unbounded
 3) state it is infeasible
*/

vector<int> solveGivenFeasibleBasis(LP lp, vector<int> basis)
{
	vector<int> currBasis = basis;
	while(true)
	{
		LP canonical = lp.turnToCanonical(currBasis);
		cout << "Canonical for basis " << currBasis << " is:" << endl;
		cout << canonical << endl << endl;
		if(canonical.getObjective().isNonPositive())
		{
			Matrix sol = findSolutionGivenCanonical(canonical, currBasis);
			cout << "Optimal solution: " << endl << sol << endl;
			cout << "With optimal value: " << canonical.getObjConst() << endl;
			break;
		}
		else if(canonical.getConstraint().oneColNonPositive())
		{
			cout << "Unbounded." << endl;
			break;
		}
		else
		{
			currBasis = canonical.findNewBasis(currBasis);
		}
	}
	return currBasis;
}

vector<int> solveLP(LP lp)
{
	if(!lp.isSEF())
	{
		cerr << "LP is not in SEF" << endl;
		exit(1);
	}
	LP aux = lp.auxiliaryProblem();
	int numVars = lp.getConstraint().getCols();
	int numConstraints = lp.getConstraint().getRows();
	vector<int> initialBasis;
	// initialBasis = {3,4,5}
	for(int i=numVars+1; i<numVars+numConstraints+1; i++)
	{
		initialBasis.push_back(i);
	}
	// solve auxiliary problem
	vector<int> feasibleAuxiliaryBasis = solveGivenFeasibleBasis(aux, initialBasis);
	LP auxiliaryCanonical = aux.turnToCanonical(feasibleAuxiliaryBasis);
	Matrix auxiliarySol = findSolutionGivenCanonical(auxiliaryCanonical,feasibleAuxiliaryBasis);
	if(auxiliaryCanonical.getObjConst() != 0)
	{
		cout << "infeasible" << endl;
	}

	vector<int> feasibleBasis;
	int deficient = 0;			// holds the number of elements in basis that are not
								// in original LP (ie. from auxiliary problem)
	for(int i=0; i<feasibleAuxiliaryBasis.size(); i++)
	{
		int curr = feasibleAuxiliaryBasis.at(i);
		if(curr <= numVars)
		{
			feasibleBasis.push_back(curr);
		}
		else
		{
			deficient++;
		}
	}
	int j =0;
	// add elements from the original LP that were set to 0
	while(deficient>0)
	{
		if (auxiliarySol.getNum(j,0) == 0)
		{
			feasibleBasis.push_back(j+1);
			deficient--;
		}
		j++;
	}
	return solveGivenFeasibleBasis(lp, feasibleBasis);
}

int main(int argc, char* argv[]) {
	try{
	    cout << "File: " << argv[1] << endl << endl;;
		LP linear;
		ifstream ifs(argv[1]);
		if(!ifs.is_open()) {
			cerr << "sample LP could not be opened!" << endl;
			exit(1);
		}
		ifs >> linear;
		cout << "Original linear program is:" << endl;
		cout << linear << endl;
		vector<int> basis = solveLP(linear);
	}
	catch (int e)
	{
		cout << "An exception occurred. Exception Nr. " << e << endl;
		exit(1);
	}
	return 0;
}
