#pragma once

class Digit
{
public:
	virtual void out() = 0;
	virtual Digit * myCopy() = 0;
	virtual bool operator!=(Digit&) = 0;
	virtual bool operator>(Digit&) = 0;
	virtual Digit *sum(Digit *) = 0;
	virtual Digit *sub(Digit *) = 0;
	virtual Digit *mult(Digit *) = 0;
	virtual Digit *normalize() = 0; //0<digit<base 
	virtual Digit *zero() = 0; //digit = 0, return digit
};
