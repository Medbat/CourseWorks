#pragma once
#include "digit.h"
#include "digit10.h"
#include <iostream>

using namespace std;

class Long
{
	Digit **a;
	int length;
	bool isNegative;
public:
	Long() : a(NULL), length(0), isNegative(false) {};
	~Long()
	{
		if (a) return;
		for (int i = 0; i<length; i++)
			delete a[i];
		delete [] a;
	}
	void setNum(Digit **, int, bool);
	void outNum();
	Long involution(long);	
	Long& operator=(const Long &);
	friend bool operator==(const Long&, const Long&);
	friend bool operator!=(const Long&, const Long&);
	friend bool operator>(const Long&, const Long&);
	friend bool operator<(const Long&, const Long&);
	friend bool operator<=(const Long&, const Long&);
	friend bool operator>=(const Long&, const Long&);
	friend Long& operator-(const Long&, const Long&);
	friend bool operator>(const Long&, string);
	friend Long& operator*(const Long&, const Long&);
	friend Long& operator*(const Long&, string);
	friend Long& operator+(const Long&, const Long&);
	friend Long& operator-(const Long &a) 
	{
		Long *ex = new Long;
		ex->a = new Digit*[a.length];
		ex->length = a.length;
		for (int i = 0; i < ex->length; i++)
			ex->a[i] = a.a[i]->myCopy();
		ex->isNegative = !a.isNegative;
		return *ex;
	}
	friend Long& operator+(const Long &a) 
	{
		Long *ex = new Long;
		ex->a = new Digit*[a.length];
		ex->length = a.length;
		for (int i = 0; i < ex->length; i++)
			ex->a[i] = a.a[i]->myCopy();
		ex->isNegative = a.isNegative;
		return *ex;
	}
	bool empty();

};

Long& operator+(const Long &a, const Long &b)
{
	Long *ex = new Long;
	if (a.isNegative && !b.isNegative || !a.isNegative && b.isNegative)
	{
		if (a.isNegative) 
			return b-(-a);
		else
			return a-(-b);
	}
	
	if (a.length<b.length)
		return b + a;
	ex->a = new Digit*[b.length + 1];
	Digit *temp;
	Digit *carry = a.a[0]->normalize();// Carry = 0 Т_Т
	int i;
	for (i = 0; i<b.length; i++)
	{
		temp = (a.a[i]->sum(b.a[i]))->sum(carry); // O_O temp = a + b + carry
		carry = temp->normalize();
		ex->a[i] = temp;
	}

	for (;i<a.length; i++)
	{
		temp = a.a[i]->sum(carry);
		carry = temp->normalize();
		ex->a[i] = temp;
	}
	Digit *z = carry->myCopy()->zero();
	if (*carry != *z)
	{
		ex->a[i] = carry;
		ex->length = a.length + 1;
	}
	else
		ex->length = a.length;
	ex->isNegative = a.isNegative;
	return *ex;
}


Long& operator-(const Long &a, const Long &b)
{
	if (a.isNegative && !b.isNegative || !a.isNegative && b.isNegative)
	{
		if (a.isNegative) return -(-a+b);
		else return a+(-b);
	}

	if (a.length < b.length)
		return -(b-a);
	Long *ex = new Long;
	ex->a = new Digit*[b.length + 1];
	Digit *temp;
	Digit *carry = a.a[0]->normalize();// Carry = 0 Т_Т
	int i;
	for (i = 0; i<b.length; i++)
	{
		temp = a.a[i]->sub(b.a[i])->sum(carry);
		carry = temp->normalize();
		ex->a[i] = temp;
	}
	for (; i<a.length; i++)
	{
		temp = a.a[i]->sum(carry);
		carry = temp->normalize();
		ex->a[i] = temp;
	}
	i = a.length - 1;
	Digit *z = carry->myCopy()->zero();
	while ( i>0 && !(*ex->a[i]!=*z)) i--;
	ex->length = i + 1; 
	ex->isNegative = (a>=b)? false : true; 
	return *ex;
}

bool operator!=(const Long &a, const Long &b)
{
	return !(a==b);
}



Long Long::involution(long k)
{
	Long a(*this);
	for (int i = 0; i < k - 1; i++)
		*this = (*this) * a;
	return *this;
}


Long assign(Long &l, string b)
{
	Digit** a = new Digit*[b.length()];
	for (int i = 0; i < b.length(); i++)
		a[i] = new Digit10(b[i]-'0');
	l.setNum(a, b.length(), false);
	return l;
}

bool Long::empty()
{
	Long a;
	return (*this == assign(a, "0"));
}


Long& operator*(const Long& a, string b)
{
	Long t;
	assign(t, b);
	return a*t;
}

void Long::setNum(Digit **num, int length, bool isNegative)
{
	this->isNegative = isNegative;
	this->length = length;
	a = new Digit*[length];
	for (int i = length - 1; i>=0; i--)
		a[i] = num[length - i - 1]->myCopy();
}
void Long::outNum()
{
	if (isNegative) cout<<'-';
	for (int i = length - 1; i>=0; i--)
		a[i]->out();
}
Long& Long::operator=(const Long &sec)
{
	isNegative = sec.isNegative;
	length = sec.length;
	a = new Digit*[length];
	for (int i = 0; i<length; i++)
		a[i] = sec.a[i]->myCopy();
	return *this;
}
bool operator>(const Long& a, string b)
{
	Long c;
	assign(c, b);
	return a > c;
}
bool operator>(const Long &a, const Long &b)
{
	bool flag = false;
	if (a.isNegative && !b.isNegative || a.length<b.length) return false;
	if (!a.isNegative && b.isNegative || a.length>b.length) return true;
	for (int i = 0; i < a.length; i++)
		if (*a.a[i]>*b.a[i])
		{
			flag = true;
			break;
		}
	if (a.isNegative) 
		return !flag;
	else 
		return flag;
}

Long& operator*(const Long &a, const Long &b)
{
	if (a.length<b.length)
		return b*a;
	Long *ex = new Long;
	Digit *temp;
	Digit *carry = a.a[0]->normalize();// Carry = 0 Т_Т
	Digit *zero = carry->myCopy();//Ноль

	ex->a = new Digit*[a.length + b.length];
	for (int i = 0; i<a.length + b.length+1; i++)
		ex->a[i] = zero->myCopy(); // обнуление

	int i,j;
	for (i = 0; i<a.length; i++)
	{
		carry = zero->myCopy();
		for (j = 0; j<b.length; j++)
		{
			temp = ((a.a[i]->mult(b.a[j]))->sum(ex->a[i+j]))->sum(carry);
			carry = temp->normalize();
			ex->a[i+j] = temp;
		}
		ex->a[i+j] = carry;
	}
	i = a.length + b.length - 1;
	if (!(*ex->a[i] != *zero)) i--;
	ex->length = i + 1;
	ex->isNegative = a.isNegative && !b.isNegative || !a.isNegative && b.isNegative;
	return *ex;
}

bool operator==(const Long &a, const Long &b)
{
	bool flag = false;
	if (a.length == b.length && a.isNegative == b.isNegative)
	{
		flag = true;
		for (int i = 0; i<a.length; i++)
			if (*a.a[i] != *b.a[i])
			{
				flag = false;
				break;
			}
	}
	return flag;
}
bool operator<(const Long &a, const Long &b)
{
	if (a>b || a==b) return false;
	else return true;
}
bool operator<=(const Long &a , const Long &b)
{
	return a<b || a==b;
}
bool operator>=(const Long &a , const Long &b)
{
	return a>b || a==b;
}

