#include <iostream>
#include <climits>
#include <stdlib.h>
#include "Number.h"

// helper function. returns gcd of int a and b
int gcdFunction(int a, int b)
{
	int temp;
	while (a!=0)
	{
		temp = a;
		a = b%a;
		b = temp;
	}
	return b;
}
// helper function. returns lcm of int a and b
int lcmFunction(int a, int b)
{
	int gcd = gcdFunction(a, b);
	int partial = a/gcd;
	return partial*b;
}
// helper private method to reduce number
void Number::reduce()
{
	int numerator = num;
	int denominator = denom;
	// check for division by 0
	if (denominator == 0)
	{
		std::cerr << "Divide by 0\n";
		exit(1);	
	}
	int factor = gcdFunction(abs(numerator), abs(denominator));
	
	num = numerator/factor;
	denom = denominator/factor;
	// if denominator is negative, transfer that to numerator
	if (denominator < 0)
	{
		num *= -1;
		denom *= -1;
	}
}
// helper function
// finds number needed to fill the number to 1 or -1
// ie. 5/7 -> 2/7
//    -2/5 -> -3/5
Number Number::fill()
{
	int numerator = (num<0) ? -(denom+num) : denom - num;
	return Number(numerator, denom);
}

////////////////////////////
// CONSTRUCTORS/DESTRUCTOR
////////////////////////////
Number::Number():num(0), denom(1)
{}
Number::Number(int n):num(n), denom(1)
{}
Number::Number(int n, int d)
{
	if (d==0)
	{
		std::cerr << "Divide by 0\n";
		exit(1);	
	}
	// reduce with absolute value, not caring about sign	
	int factor = gcdFunction(abs(n), abs(d));
	
	num = n/factor;
	denom = d/factor;
	// if denom is negative, transfer that to numerator
	if (denom<0)
	{
		num *= -1;
		denom *= -1;
	}
	if (num == 0)
	{
		denom = 1;
	}
	
}
// turn "(98/32,sdaf3)" into "98/323"
std::string removeExtra(std::string stringNum)
{
	std::string removed = "";
	for (int pos = 0; pos < stringNum.length(); pos++)
	{
		char charPos = stringNum[pos];
		if (charPos == '0' || charPos == '1' || charPos == '2' || charPos == '3' || charPos == '4' || charPos == '5' || charPos == '6' || charPos == '7' || charPos == '8' || charPos == '9' || charPos == '/'|| charPos == '-')
		{
			removed.push_back(charPos);
		}
	}
	return removed;
}
// turn "98" into int 98
int stringToInt(std::string stringNum)
{
	int intSoFar = 0;
	int tensSoFar = 1;
	bool negative = false;
	if (stringNum[0] == '-')
	{
		negative = true;
		stringNum = stringNum.substr(1);;
	}
	for (int i = stringNum.size()-1; i >= 0; i--)
	{
		int place = stringNum[i] - '0';
		intSoFar += place*(tensSoFar);
		tensSoFar *= 10;
	}
	if (negative)
		intSoFar *= -1;
	return intSoFar;
}
Number::Number(std::string stringNum)
{
	std::string removed = removeExtra(stringNum);
	size_t found = removed.find('/');
	// no fraction sign
	if (found == std::string::npos)
	{
		num = stringToInt(removed);
		denom = 1;
	}
	else
	{
		num= stringToInt(removed.substr(0, found));
		denom = stringToInt(removed.substr(found+1));
	}
	reduce();
}
// copy constructor
Number::Number(const Number &c) :
	num(c.numerator()), denom(c.denominator())
{}
// destructor
Number::~Number()
{}

//////////////////////////
//  CLASS METHODS
//////////////////////////

// get numerator
int Number::numerator() const
{
	return num;
}
// set numerator
int Number::numerator(int n)
{
	int prev = num;
	int factor = gcdFunction(abs(n), abs(denom));
	
	num = n/factor;
	denom = denom/factor;
	if (num == 0)
		denom = 1;
	return prev;
}
// get denominator
int Number::denominator() const
{
	return denom;
}
// set denominator
int Number::denominator(int d)
{
	if (d==0)
	{
		std::cerr << "Divide by 0\n";
		exit(1);	
	}
	int prev = denom;
	int factor = gcdFunction(abs(num),abs(d));
	
	num = num/factor;
	denom = d/factor;
	return prev;
}
// unary negation
Number Number::operator-()
{
	Number newNum(num*-1, denom);
	return newNum;
}
// assignment
Number& Number::operator=(const Number &r)
{	
	num = r.numerator();
	denom = r.denominator();
	 				// update counter
	return *this;
}

////////////////////////////
// COMPARISON FUNCTIONS
////////////////////////////

bool operator==( Number l, Number r ) { 
    return (l.numerator() == r.numerator() && l.denominator() == r.denominator());
}
bool operator!=( Number l, Number r )
{
	return !(l==r);
}
// helper for the operator '<'
bool operatorHelper(int num1, int den1, int num2, int den2)
{
	if (num1==num2 && den1 == den2)
		return false;	
	if (num1/den1 < num2/den2)
		return true;
	if (num1/den1 > num2/den2)
		return false;
	return !operatorHelper(den1, num1%den1, den2, num2%den2);
}
bool operator<(Number l, Number r)
{
	int lnum = l.numerator();
	int lden = l.denominator();
	int rnum = r.numerator();
	int rden = r.denominator();
	if (lnum < 0 && rnum >= 0)
		return true;
	if (rnum <= 0 && lnum >=0)
		return false;
	return operatorHelper(abs(lnum), abs(lden), abs(rnum), abs(rden));
}
bool operator<=(Number l, Number r)
{
	if (l<r)
		return true;
	if (r==l)
	{
		return true;
	}
	return false;
}
bool operator>(Number l, Number r)
{
	return !(l<=r);
}
bool operator>=(Number l, Number r)
{
	return !(l<r);
}

////////////////////////////
// BINARY OPERATORS
////////////////////////////

Number operator+( Number l, Number r)
{
	int lnum = l.num;
	int lden = l.denom;
	int rnum = r.num;
	int rden = r.denom;
	
	// if one is whole number
	if (lden == 1)
	{
		return Number(lnum*rden + rnum, rden);
	}
	if (rden == 1)
	{
		return Number(rnum*lden + lnum, lden);
	}
	// sum of whole numbers
	int sumWhole = lnum/lden + rnum/rden;
	// what is left after subtracting whole number
	lnum = lnum%lden;
	rnum = rnum%rden;	

	// lMaxMul is the largest integer that you can multiply 
	// by lnum to not go over INT_MAX
	int lMaxMul = INT_MAX/abs(lnum);
	int rMaxMul = INT_MAX/abs(rnum);
	int gcdDenom = gcdFunction(lden,rden);

	// if numerators will not go over INT_MAX when
	// multiplied for common denom
	if (lMaxMul >= rden/gcdDenom && rMaxMul >= lden/gcdDenom)
	{
		// if the sum of numerators multiplied by their respective
		// factors will not go over INT_MAX
		if ( INT_MAX- lnum*(rden/gcdDenom) >= rnum*(lden/gcdDenom))
		{
			// numerator (without reducing)
			int OriginalNum = lnum*(rden/gcdDenom) + rnum*(lden/gcdDenom);
			// reduce with both lden and rden
			Number reduce1(OriginalNum, lden);
			Number reduce2(reduce1.numerator(), rden/gcdDenom);	
			return Number(reduce2.numerator() + sumWhole*(reduce1.denominator())*(reduce2.denominator()), (reduce1.denominator())*(reduce2.denominator()));
		} // if 
	} // if
	
	// else, sum of two numbers is over 1 (or under -1)
	l.num = lnum;
	r.num = rnum;
	Number fill = l.fill();
	// fill the first number so it reaches 1 or -1	
	// i.e. if l = 3/5 and r = 4/7, fill = 2/5
	// 3/5 + 4/7 = 3/5 + 2/5 - 2/5 + 4/7
	//           = 1 - 2/5 + 4/7
	if (lnum > 0)
		sumWhole++;
	else
		sumWhole--;	
	Number wholeNumber(sumWhole, 1);	
	Number sumFractions = r - fill;
	return wholeNumber + sumFractions;
}


Number operator-( Number l, Number r)
{
	return l + (-r);
}
Number operator*( Number l, Number r)
{
	int lnum = l.numerator();
	int lden = l.denominator();
	int rnum = r.numerator();
	int rden = r.denominator();
	// cross factor
	int factor1 = gcdFunction(lnum, rden);
	int factor2 = gcdFunction(rnum, lden);

	lnum /= factor1;
	rden /= factor1;
	rnum /= factor2;
	lden /= factor2;

	Number prod(lnum*rnum, rden*lden);
	return prod;
}
Number operator/( Number l, Number r)
{
	Number inverse(r.denominator(), r.numerator());	
	return l*inverse;
}


////////////////////////////
// ISTREAM AND OSTREAM
////////////////////////////

// instream
std::istream &operator>>(std::istream &is, Number &r)
{
	int numerator, denominator;
	is >> numerator;
	is >> denominator;
	if (denominator == 0)
	{
		std::cerr << "Divide by 0\n";
		exit(1);	
	}
	r.numerator(numerator);
	r.denominator(denominator);
	r.reduce();
	return is;
}
// outstream
std::ostream &operator<<(std::ostream &os, Number r)
{
	if (r.denominator() == 1)
	{
		os << r.numerator();
	}
	else
	{
		os << r.numerator() << "/" << r.denominator();
	}
	return os;
}

