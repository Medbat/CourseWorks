#pragma once 
  
#include <iostream> 
#include <string> 
#include <stdarg.h>
#include "Long.h"
  
using namespace std; 

struct Attributes
{
	bool invisible;
	bool readOnly;
	Attributes operator=(Attributes a)
	{
		invisible = a.invisible;
		readOnly = a.readOnly;
		return *this;
	}
};

struct Param
{
	Long size; 
	int depth;
	string name;
	Attributes attributes;
	Param() {}
	Param(Long s, string n, bool i, bool r)
	{
		size = s;
		name.assign(n);
		attributes.invisible = i;
		attributes.readOnly = r;
		depth = 0;
	}
	Param operator=(Param p)
	{
		attributes = p.attributes;
		size = p.size;
		name.assign(p.name);
		depth = 0;
		return p;
	}
};
  
class Elem 
{ 
    friend class List; 
	friend class Directory;
protected: 
	Param param;
    Elem* next; 
public: 
	Param getParam()
	{
		return param;
	}
	Long getSize()
	{
		return param.size;
	}
	string getStr()
	{
		return param.name;
	}
	Elem(Param p)
	{
		param = p;
		next = NULL;
	}
	Elem(Long size, string name, bool invisible, bool readOnly)
	{
		param = Param(size, name, invisible, readOnly);
		next = NULL;
	}
    Elem* next_elem() 
    { 
        return next; 
    } 
    virtual string str() = 0; 
}; 
  
class Iter 
{ 
    friend class List; 
    Elem* current_elem; 
public: 
    Iter() : current_elem(0) {} 
    Iter operator++() 
    { 
        if (current_elem) 
            current_elem = current_elem -> next_elem(); 
        return *this; 
    } 
    Elem& operator*()  
    { 
        return *current_elem;  
    } 
    Elem* operator->() 
    {  
        return current_elem;  
    } 
	
    bool operator!=(Iter v) 
    { 
        return current_elem != v.current_elem; 
    } 

	Elem* operator+()
	{
		return current_elem -> next_elem();
	}
}; 
  
class List 
{ 
protected: 
    friend class Iter; 
    Elem* first; 
public: 
    List() : first(NULL) {}; 
    ~List() { } 
	Elem* getFirst()
	{
		return first;
	}
    int length() 
    { 
        int ret = 0; 
        Elem* temp = first; 
        while (temp) 
        { 
            ret++; 
            temp = temp -> next; 
        } 
        return ret; 
    }
    void push_back(Elem* p) 
    { 
        Elem *e = NULL; 
        for (Iter i = begin(); i != end(); ++i) 
            e = &*i; 
        if (e)   
            e -> next = p; 
        else
            first = p; 
        p -> next = NULL; 
    } 
    Elem* pop() 
    { 
        Elem *p = first; 
        if (p) 
            first = p->next; 
        return p; 
    }
    Iter begin() 
    { 
        Iter tmp; 
        tmp.current_elem = first; 
        return tmp; 
    } 
    Iter end() 
    { 
        Iter tmp; 
        tmp.current_elem = NULL; 
        return tmp; 
    } 
}; 
  
  
class File : public Elem 
{ 
public: 
	File(Param p) : Elem(p) {}
	File(Long size, string name, bool invisible, bool readOnly) : 
		Elem(size, name, invisible, readOnly) {}
	File(File *f) : Elem(f->param) {}
	

    string str() 
    { 
		string s;
		for (int i = 0; i < param.depth; i++)
			s += "  ";
		s += param.name;
		return s; 
    } 
}; 
  
class Directory : public Elem 
{ 
    List content; 
	friend class DVDDirectoryCheck;
public: 
	Directory(Param p) : Elem(p) {}
	Directory(Long size, string name, bool invisible, bool readOnly) : 
		Elem(size, name, invisible, readOnly) {}
	Directory(Directory *f) : Elem(f->param) {}
	List getContent()
	{
		return content;
	}
    string content_str() 
    { 
        string s;
        for (Iter i = content.begin(); i != content.end(); ++i)
			s += i -> str() + ((+i)!=NULL?'\n':' ');
        return s; 
    } 
    string str() 
    { 
		string s; 
		for (int i = 0; i < param.depth; i++)
			s += "  ";
		return s + param.name + "/\n"  + content_str(); 
    } 
    Directory& push_back(int n, ...) 
    { 
		va_list elements;
		va_start(elements, n);
		for (int i = 0; i < n; i++)
			content.push_back(va_arg(elements, Elem*));
		va_end(elements);
        return *this; 
    } 
	Directory& push_back(Elem* a)
	{
		content.push_back(a);
		return *this;
	}

	void deepness(Directory* a, File* el)
	{
		if (el == NULL)
			if (a->getContent().getFirst())
			{
				a->getContent().getFirst()->param.depth=a->param.depth+1;
				if ((a->getContent().getFirst())->getSize().empty())
					deepness(dynamic_cast<Directory*>(a->getContent().getFirst()), NULL);
				else
					deepness(NULL, dynamic_cast<File*>(a->getContent().getFirst()));
			}

		if (el && el->next_elem())	
		{
			el->next_elem()->param.depth = el->param.depth;
			if (!((el->next_elem()->getSize().empty())))
				deepness(NULL, dynamic_cast<File*>(el->next_elem()));
			else
				deepness(dynamic_cast<Directory*>(el->next_elem()), NULL);
		}

		if (a && a -> next_elem())
		{
			a->next_elem()->param.depth = a -> param.depth;
			if (!((a->next_elem()->getSize().empty())))
				deepness(NULL, dynamic_cast<File*>(a->next_elem()));
			else
				deepness(dynamic_cast<Directory*>(a->next_elem()), NULL);
		}
	}
	void prepare()
	{
		deepness(this, NULL);
	}
}; 

ostream &operator<<(ostream &out, Elem &e) 
{ 
    out << e.str(); 
    return out; 
} 

