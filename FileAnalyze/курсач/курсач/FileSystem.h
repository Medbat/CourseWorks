
#pragma once 
#include <algorithm>
#include "Catalogue.h"

class FileCheck
{
protected:
	File* toCheck;
public:
	void operator=(File* f)
	{
		toCheck = (new File(f));
	}
	FileCheck(File* toCheck) : toCheck(toCheck) {};
	virtual bool summarize() = 0;
	bool invisible()
	{
		Param a = toCheck -> getParam();
		if (a.attributes.invisible)
		{
			cout << "File is invisible\n";
			return true;
		}
		return false;
	}
	bool readOnly()
	{
		Param a = toCheck -> getParam();
		if (a.attributes.readOnly)
		{
			cout << "File is only for reading"; 
			return true;
		}
		return false;
	}
};

class WindowsFileCheck : public FileCheck
{
	static string restrictedNames[4];
	static char restrictedSymbols[10];
public:
	WindowsFileCheck(File *toCheck) : FileCheck(toCheck) {}
	bool summarize()
	{
		cout << "\nChecking file " << toCheck -> getStr() << "...\n";
		if (!(restrictedNameCheck() || fileNameCheck() || fileSizeCheck()))
		{
			cout << toCheck -> getStr() << " has no errors\n";
			return true;
		}
		return false;
	}
	bool restrictedNameCheck()
	{
		string s = toCheck -> getStr().substr(0, 3);
		string ss = toCheck -> getStr();
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		bool bad = false;
		for (int i = 0; i < 4; i++)
			if (s == restrictedNames[i])
				bad = true;
		if (ss.length() > 3 && ((int)ss[3]) <= 58 &&
			((int)ss[3]) >= 48 && (s == "com" || s == "lpt"))
			bad = true;
		if (bad)
			cout << "Restricted in NTFS/FAT32 name\n";
		return bad;
	}
	bool fileNameCheck()
	{
		string badS("");
		string ss = toCheck -> getStr();
		bool bad = false;
		for (int i = 1; i < 10; i++)
			if (ss.find(restrictedSymbols[i]) != ss.npos)
			{
				bad = true;
				badS += restrictedSymbols[i];
			}
		for (int i = 0; i < ss.length(); i++)
			if (ss[i] == '\0')
				badS += "NULL ";
		if (bad)
			cout << "Filename contains bad NTFS/FAT32 symbols: " << badS << "\n";
		if (ss.length() > 255)
		{
			bad = true;
			cout << "Filename contains more than 255 symbols\n";
		}
		if (ss[0] == ' ' || ss[ss.length() - 1] == ' ')
		{
			cout << "Filename contains spaces at the beginning/end\n";
			bad = true;
		}
		return bad;
	}
	bool fileSizeCheck()
	{
		bool bad = false;
		Long NTFS, FAT32, size;
		assign(NTFS, "2");
		NTFS.involution(64);
		assign(FAT32, "2");
		FAT32.involution(10);
		NTFS = NTFS - FAT32;
		assign(FAT32, "2");
		FAT32.involution(32);
		size = toCheck -> getSize();
		if (size > FAT32)
		{
			bad = true;
			cout << "FAT32 cannot contain this file due to its size\n";
		}
		if (size > NTFS)
		{
			bad = true;
			cout << "NTFS cannot contain this file due to its size\n";
		}
		return bad;
	}
};

class UnixFileCheck : public FileCheck
{
public:
	UnixFileCheck(File *toCheck) : FileCheck(toCheck) {}
	bool summarize()
	{
		cout << '\n' << toCheck -> getStr() << '\n';
		if (!(fileNameCheck() || fileSizeCheck()))
		{
			cout << "Has no errors\n";
			return true;
		}
		return false;
	}
	bool fileNameCheck()
	{
		string badS("");
		string ss = toCheck -> getStr();
		bool bad = false;
		if (ss.find('/') != ss.npos)
		{
			bad = true;
			badS += '/';
		}
		for (int i = 0; i < ss.length(); i++)
			if (ss[i] == '\0')
				badS += "NULL ";
		if (bad)
			cout << "Filename contains bad EXT3/EXT4 symbols: " << badS << "\n";
		if (ss.length() > 256)
		{
			bad = true;
			cout << "Filename contains more than 256 symbols\n";
		}
		return bad;
	}
	bool fileSizeCheck()
	{
		bool bad = false;
		Long EXT3, EXT4, size;
		assign(EXT3, "2");
		EXT3.involution(40);
		assign(EXT4, "2");
		EXT4.involution(44);
		size = toCheck -> getSize();
		if (size > EXT3)
		{
			bad = true;
			cout << "EXT3 cannot contain this file due to its size\n";
		}
		if (size > EXT4)
		{
			bad = true;
			cout << "EXT4 cannot contain this file due to its size\n";
		}
		return bad;
	}
};

class DVDFileCheck : public FileCheck
{
public:
	DVDFileCheck(File *toCheck) : FileCheck(toCheck) {}
	bool summarize(bool dir)
	{
	cout << "\nChecking " << (dir?"directory ":"file ") << toCheck -> getStr() << "...\n";
		if ((!size() && !symbols() && !dotStartEnd() && !dotCount(dir)) &&
			!length(dir))
		{
			cout << toCheck -> getStr() << " can be written on DVD\n";
			return true;
		}
		else
		{
			cout << toCheck -> getStr() << " cannot be written on DVD\n";
			return false;
		}
	}
	bool summarize()
	{
		return summarize(false);
	}
	bool size()
	{
		Long size;
		Long DVD;
		assign(DVD, "10");
		DVD.involution(8);
		DVD = DVD * assign(size, "47");
		size = toCheck->getSize();
		if (size> DVD)
			cout << "My file is too big\n";
		return size > DVD;
	}
	bool length(bool dir)
	{
		bool bad = dir?toCheck ->getStr().length() > 8:toCheck ->getStr().length() > 11;
		if (bad)
			cout << "Filename is too long\n";
		return bad;
	}
	bool symbols()
	{
		char buf[1024];
		strcpy(buf, toCheck->getStr().c_str());
		bool bad = false;
		for (int i = 0; i < strlen(buf); i++)
			if (!((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= '0' && buf[i] <= '9') ||
				 buf[i] == '.' || buf[i] == ' '))
				bad = true;
		if (bad)
			cout << "Filename contains bad ISO 9660 symbols\n";
		return bad;
	}
	bool dotCount(bool dir)
	{
		string ss = toCheck -> getStr();
		int c = 0;
		for (int i = 0; i < ss.length(); i++)
			if (ss[i] == '.')
				c++;
		if (dir)
			return c > 0;
		else 
			return c > 1;
	}
	bool dotStartEnd()
	{
		bool a = toCheck -> getStr()[0] == '.' || 
			toCheck -> getStr()[toCheck -> getStr().length() - 1] == '.';
		if (a)
			cout << "Filename has dot at the end/beginning\n";
		return a;
	}

};

class DirectoryCheck
{
protected:
	Directory* toCheck;
public:
	DirectoryCheck(Directory* toCheck) : toCheck(toCheck) {};
	void operator=(Directory* d)
	{
		toCheck = (new Directory(d));
	}
	virtual void summarize() = 0;
};

class AttributeCheck : public DirectoryCheck
{
public:
	AttributeCheck(Directory *toCheck) : DirectoryCheck(toCheck) {}
	void summarize()
	{
		int i = 0, r = 0, ir = 0;
		cout << "Searching for invisible/RO files in " << toCheck->getParam().name << "...\n";
		deepness(toCheck, i, r, ir, NULL);
		cout << "Invisible files: " << i << "\nRO files: " << r << "\nInvisible/RO files: "
			<< ir << endl;
	}
	void deepness(Directory* a, int &i, int &r, int &ir, File* el)
	{
		if (el == NULL)
			if (a->getContent().getFirst())
				if ((a->getContent().getFirst())->getSize().empty())
					deepness(dynamic_cast<Directory*>(a->getContent().getFirst()), i, r, ir, NULL);
				else
					deepness(NULL, i, r, ir, dynamic_cast<File*>(a->getContent().getFirst()));

		if (el && el->next_elem())	
			if (!((el->next_elem()->getSize().empty())))
				deepness(NULL, i, r, ir, dynamic_cast<File*>(el->next_elem()));
			else
				deepness(dynamic_cast<Directory*>(el->next_elem()), i, r, ir, NULL);

		if (a && a -> next_elem())
			if (!((a->next_elem()->getSize().empty())))
				deepness(NULL, i, r, ir, dynamic_cast<File*>(a->next_elem()));
			else
				deepness(dynamic_cast<Directory*>(a->next_elem()), i, r, ir, NULL);

		if (el == NULL)
		{
			if (a->getParam().attributes.invisible && a->getParam().attributes.readOnly)
				ir++;
			else 
				if (a->getParam().attributes.invisible)
					i++;
				else
					if (a->getParam().attributes.readOnly)
						r++;
		}

		if (a == NULL)
		{
			if (el->getParam().attributes.invisible && el->getParam().attributes.readOnly)
				ir++;
			else 
				if (el->getParam().attributes.invisible)
					i++;
				else
					if (el->getParam().attributes.readOnly)
						r++;
		}
	}
};

class DVDDirectoryCheck : public DirectoryCheck
{
public:
	DVDDirectoryCheck(Directory *toCheck) : DirectoryCheck(toCheck) {}
	void summarize()
	{
		bool deep = true;
		cout << "\nChecking direcory " << toCheck->getStr() << "...";
		deepness(toCheck, deep, NULL);
		if (deep)
			cout << "\nCatalogue " << toCheck->getStr() << " can be written on DVD\n";
		else
			cout << "\nCatalogue " << toCheck->getStr() << " cannot be written on DVD\n";
	}
	void deepness(Directory* a, bool &b, File* el)
	{
		if (el == NULL)
			if (a->getContent().getFirst())
				if ((a->getContent().getFirst())->getSize().empty())
					deepness(dynamic_cast<Directory*>(a->getContent().getFirst()), b, NULL);
				else
					deepness(NULL, b, dynamic_cast<File*>(a->getContent().getFirst()));

		if (el && el->next_elem())	
			if (!((el->next_elem()->getSize().empty())))
				deepness(NULL, b, dynamic_cast<File*>(el->next_elem()));
			else
				deepness(dynamic_cast<Directory*>(el->next_elem()), b, NULL);

		if (a && a -> next_elem())
			if (!((a->next_elem()->getSize().empty())))
				deepness(NULL, b, dynamic_cast<File*>(a->next_elem()));
			else
				deepness(dynamic_cast<Directory*>(a->next_elem()), b, NULL);

		if (el == NULL)
		{
			if (a->getParam().depth > toCheck->getParam().depth + 8)
				b = false;
			if (DVDFileCheck(new File(a->getParam())).summarize(true))
				b = false;
		}
		else
		{
			if (el->getParam().depth > toCheck->getParam().depth + 8)
				b = false;
			if (DVDFileCheck(new File(el->getParam())).summarize())
				b = false;
		}
	}
};

string WindowsFileCheck::restrictedNames[4] = {"con", "nul", "prn", "aux"};
char WindowsFileCheck::restrictedSymbols[10] = {'\0', '/', '\\',
	':', '*', '?', '"', '<' , '>', '|'};