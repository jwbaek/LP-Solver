#ifndef __LP_H__
#define __LP_H__
#include <iostream>
#include "matrix.h"

class LP
{
	public:
		enum Rel {eq, le, ge};
		LP();

		// accessors
		bool isMax() const;
		bool isMin() const;
		bool isRelSame() const;			// returns true if all constraints are equality
		
		Matrix getObjective() const;
		Matrix getConstraint() const;
		Matrix getRHS() const;
		vector<Rel> getRelVec() const;	// vector of inequalities
		Number getObjConst() const;		// + z

		Rel getRelAt(int row) const;
		Number getRHSAt(int row) const;


		// modifiers
		void setIsMax(bool isMax);
		void setObjective(Matrix m);
		void setConstraint(Matrix m);
		void setRHS(Matrix m);
		void setRelVec(vector<Rel> r);
		void setObjConst(Number r);

		// push_back for each data member
		void pushObjective(Number n);
		void pushConstraint(vector<Number> row);
		void pushRHS(Number n);
		void pushRelVec(Rel r);

		// methods
		LP turnToCanonical(vector<int> basis);
		vector<int> findNewBasis(vector<int> basis);
		LP auxiliaryProblem();
		bool isValidLP();
		bool isSEF();
		static Rel stringToRel(string str);
		static string relToString(Rel r);

	protected:
		bool max;					// true if maximization, false if miniminzation
		Matrix objective;			// n x 1 matrix, n is # of variables
		Number objConst;			// + z in objective function
		Matrix constraint;			// m x n matrix
		Matrix RHS;					// m x 1 matrix, RHS of constraints
		vector<Rel> relVec;

};
ostream & operator<< (std::ostream & stream, const LP::Rel & r);
ostream & operator<< (std::ostream & stream, const LP & lp);
istream & operator>> (std::istream & stream, LP & lp);

#endif
