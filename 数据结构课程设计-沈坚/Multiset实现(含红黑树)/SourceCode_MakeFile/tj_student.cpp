#define _CRT_SECURE_NO_WARNINGS
#include"tj_student.h"
#include<cstring>
using namespace std;

student::student(const int n, const char* nam, const char se, const float s, const char* a)
{
	num = n;
	strcpy(name, nam);
	sex = se;
	score = s;
	strcpy(addr, a);
}

student::student(const student& a)
{
	num = a.num;
	strcpy(name, a.name);
	sex = a.sex;
	score = a.score;
	strcpy(addr, a.addr);
}

bool operator ==(const student& a, const student& b)
{
	if (a.num == b.num)
		return true;
	else
		return false;
}

bool operator !=(const student& a, const student& b)
{
	if (a.num != b.num)
		return true;
	else
		return false;
}

bool operator >(const student& a, const student& b)
{
	if (a.num > b.num)
		return true;
	else
		return false;
}

bool operator <(const student& a, const student& b)
{
	if (a.num < b.num)
		return true;
	else
		return false;
}

bool operator >=(const student& a, const student& b)
{
	if (a.num >= b.num)
		return true;
	else
		return false;
}

bool operator <=(const student& a, const student& b)
{
	if (a.num <= b.num)
		return true;
	else
		return false;
}

void student::set(const int n, const char* nam, const char se, const float s, const char* a)
{
	num = n;
	strcpy(name, nam);
	sex = se;
	score = s;
	strcpy(addr, a);
}

ostream& operator<< (ostream& out, const student& a)
{
	out << a.num << " " << a.name << " " << a.sex << " " << a.score << " " << a.addr;
	return out;
}

istream& operator>> (istream& in, student& a)
{
	in >> a.num >> a.name >> a.sex >> a.score >> a.addr;
	return in;
}