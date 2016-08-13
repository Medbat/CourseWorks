#include "digit.h"
#include "iostream"

using namespace std;

class Digit10 : public Digit
{
	int a;
public:
	Digit10() : a(0) {};
	Digit10(char x) : a(x) {};
	void out() {cout<<(int)a;}
	Digit * myCopy();
	bool operator!=(Digit&);
	bool operator>(Digit&);
	Digit * sum(Digit*);
	Digit * sub(Digit*);
	Digit * mult(Digit*);
	Digit * normalize();
	Digit * zero() {a = 0; return this;}
};

Digit * Digit10::myCopy()
{
	Digit *ans = new Digit10(a);
	return ans;
}

bool Digit10::operator!=(Digit &b)
{
	Digit10 ex = dynamic_cast<Digit10 &> (b);
	return a!=ex.a;
}

bool Digit10::operator>(Digit &b)
{
	Digit10 ex = dynamic_cast<Digit10 &> (b);
	return a>ex.a;
}

Digit * Digit10::sum(Digit *b)
{
	Digit10 beta = dynamic_cast<Digit10&>(*b);
	Digit *ex = new Digit10(a + beta.a);
	return ex;
}

Digit * Digit10::sub(Digit *b)
{
	Digit10 beta = dynamic_cast<Digit10 &>(*b);
	Digit *ex = new Digit10(a - beta.a);
	return ex;
}

Digit * Digit10::mult(Digit *b)
{
	Digit10 beta = dynamic_cast<Digit10 &>(*b);
	Digit *ex = new Digit10(a * beta.a);
	return ex;
}

Digit * Digit10::normalize()
{
	Digit *ans;
	if (a>=0)
	{
		ans = new Digit10(a/10);
		a%=10;
	}
	else
	{
		ans = new Digit10(-1);
		a+=10;
	}
	return ans;

}
