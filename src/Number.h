#ifndef __RATIONALNUMBER_H__
#define __RATIONALNUMBER_H__

#include <iostream>
#include <string>

class Number {
    friend bool operator==( Number l, Number r );
    friend bool operator!=( Number l, Number r );
    friend bool operator<( Number l, Number r );
    friend bool operator<=( Number l, Number r );
    friend bool operator>( Number l, Number r );
    friend bool operator>=( Number l, Number r );

    friend Number operator+( Number l, Number r );
    friend Number operator-( Number l, Number r );
    friend Number operator*( Number l, Number r );
    friend Number operator/( Number l, Number r );

    friend std::istream &operator>>( std::istream &is, Number &r );
    friend std::ostream &operator<<( std::ostream &os, Number r );

    int num, denom;                        // implementation

	Number fill();	
	void reduce();
    // YOU MAY ADD OTHER PRIVATE MEMBERS


  public:                                  // interface routines
    Number();
    Number( int n );
    Number( int n, int d );
	Number( std::string stringNum);		   // turn "9/4" into a Number
    Number( const Number &c ); // copy constructor
    ~Number();

    int numerator() const;                 // return numerator
    int numerator( int n );                // set numerator to n; return previous numerator
    int denominator() const;               // return denominator
    int denominator( int d );              // set denominator to d; return previous denominator

    Number operator-();            // unary negation
    Number &operator=( const Number &r ); // assignment

}; // Number

extern bool operator==( Number l, Number r );
extern bool operator!=( Number l, Number r );
extern bool operator<( Number l, Number r );
extern bool operator<=( Number l, Number r );
extern bool operator>( Number l, Number r );
extern bool operator>=( Number l, Number r );

extern Number operator+( Number l, Number r );
extern Number operator-( Number l, Number r );
extern Number operator*( Number l, Number r );
extern Number operator/( Number l, Number r );

extern std::istream &operator>>( std::istream &is, Number &r );
extern std::ostream &operator<<( std::ostream &os, Number r );


#endif // __RATIONALNUMBER_H__
