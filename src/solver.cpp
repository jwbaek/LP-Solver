#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "LP.h"
#include "matrix.h"


/*
 given a LP in SEF and a feasible basis, 
 either 
 1) return an optimal solution
 2) state it is unbounded
 3) state it is infeasible
*/

Matrix findSolution(LP canonical, vector<int> basis)
{
	int numVars = canonical.getConstraint().getCols();
	Matrix solution(numVars, 1);
	for(int i = 0; i < basis.size(); i++)
	{
		solution.setNum(basis.at(i) - 1, 0, canonical.getRHS().getNum(i, 0));
	}
	return solution;
}

void solveGivenFeasibleBasis(LP lp, vector<int> basis)
{
	vector<int> currBasis = basis;
	while(true)
	{
		LP canonical = lp.turnToCanonical(currBasis);
		cout << "Canonical for basis " << currBasis << " is:" << endl;
		cout << canonical << endl << endl;
		if(canonical.getObjective().isNonPositive())
		{
			cout << "Optimal solution: " << endl << findSolution(canonical, currBasis) << endl;
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
}

int main()
{
	/*
	cout << "Enter a matrix: " << endl;
	Matrix m;
	cin >> m;
	cout << m;
	system("Pause");
	*/
	try
	{	
		cout << "Enter an LP: " << endl;
		LP linear;
		ifstream ifs("sample.txt"); //open file
		if(!ifs.is_open()) {
			cerr << "sample LP could not be opened!" << endl;
			system("Pause");
			exit(1);
		}
		ifs >> linear;
		cout << linear << endl;
		cout << "auxiliary: " << endl;
		linear.auxiliaryProblem();
		cout << linear << endl;
		system("Pause");

		int myints[] = {4, 5, 6};
		std::vector<int> b (myints, myints + sizeof(myints) / sizeof(int) );
		LP canon = linear.turnToCanonical(b);
		cout << "new LP: " << endl << canon;
		cout << "solution is " << endl << findSolution(canon, b);
		solveGivenFeasibleBasis(canon,b);
		system("Pause");
	}
	catch (int e)
	{
		cout << "An exception occurred. Exception Nr. " << e << endl;
		system("Pause");
	}
	
	/*
	Matrix m1;
	Matrix m2;
	cout << "Enter Matrix 1: " << endl;
	cin >> m1;
	cout << "Enter Matrix 2: " << endl;
	cin >> m2;
	Matrix m3 = m1*m2;
	cout << "m1*m2 is " << endl;
	cout << m3;
	*/
	system("Pause");
	return 0;
}