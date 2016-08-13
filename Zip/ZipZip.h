
#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

using namespace std;
class FileElem
{
protected:
	string name;
public:
	FileElem(string name)
	{
		this -> name = name;
	}
	virtual void getData(ifstream&) = 0;
	void print(ofstream &c)
	{
		c << name << ": " << add() << "\n";
	}
	virtual string add() = 0;
	virtual void binaryPrint(ofstream&) = 0;
	virtual void readTxt(ifstream &f)
	{
		char *b = new char[1024];
		f.get(b, name.length() + 3);
		f.getline(b, 1024);
		toBin(b);
	}
	virtual void toBin(char*) = 0;
};
class BaseDate : public FileElem
{
protected:
	int raw;
public:
	BaseDate(string name) : FileElem(name) {}
	void getData(ifstream& from)
	{
		raw = 0;
		from.read((char*)&raw, 2);
		normalize();
	}
	virtual void normalize() = 0;
	virtual string add() = 0;
	void binaryPrint(ofstream &a)
	{
		a.write((char*)&raw, 2);
	}
	//virtual void toBin(char*) = 0;
};
class Date : public BaseDate
{
	int day, month, year;
public:
	Date() : BaseDate("Last mod file date") {}
	string add() 
	{
		char *a = new char[11];
		sprintf(a, "%d.%d.%d", day, month, year);
		return string(a);
	}
	void normalize()
	{
		day = (raw & 31);
		month = (raw & 480) >> 5;
		year = ((raw & 65024) >> 9) + 1980;
	}
	void toBin(char *a)
	{
		sscanf(a, "%d.%d.%d", &day, &month, &year);
		raw = day + (month << 5) + (year << 9);
	}
};
class Time : public BaseDate
{
	int hour, minute, second;
public:
	Time() : BaseDate("Last mod file time") {}
	string add() 
	{
		char *a = new char[11];
		sprintf(a, "%d:%d:%d", hour, minute, second);
		return string(a);
	}
	void normalize()
	{
		second = (raw & 31) * 2;
		minute = (raw & 2016) >> 5;
		hour = (raw & 63488) >>	11;
	}
	void toBin(char *a)
	{
		sscanf(a, "%d:%d:%d", &hour, &minute, &second);
		raw = second / 2 + (minute << 5) + (hour << 11);
	}
};
class ExtraData : public FileElem
{
	int *value;
	int length;
public:
	ExtraData(string name) : FileElem(name) {}
	void setLength(int length)
	{
		this -> length = length;
	}
	string add()
	{
		string a;
		char *t = new char[9];
		for (int i = 0; i < length / 4 + (length % 4?1:0); i++)
		{
			sprintf(t, "%X", value[i]);
			a += string(t) + " ";
		}
		return a;
	}
	void getData(ifstream& from)
	{
		value = new int[length / 4 + (length % 4?1:0)];
		from.read((char*)value, length);
	}
	void binaryPrint(ofstream &a)
	{
		a.write((char*)value, length);
	}
	void toBin(char *a)
	{
		value = new int[length / 4 + (length % 4?1:0)];
		char t[9];
		int j = 0, k;
		for (int i = 0; i < length / 4 + (length % 4?1:0); i++)
		{
			k = 0;
			while (a[j] && a[j] != ' ')
				t[k++] = a[j++];
			t[k] = '\0';
			sscanf(t, "%X", &value[i]);
		}
	}
};
class CRC32 : public FileElem
{
	int value;
public:
	CRC32() : FileElem("CRC32") {}
	void getData(ifstream& from)
	{
		value = 0;
		from.read((char*)&value, 4);
	}
	string add()
	{
		char *a = new char[11];
		sprintf(a, "%X", value);
		return string(a);
	}
	void binaryPrint(ofstream &a)
	{
		a.write((char*)&value, 4);
	}
	void toBin(char *a)
	{
		sscanf(a, "%X", &value);
	}
};
class TwoBytesField : public FileElem
{
protected:
	int value;
public:
	TwoBytesField(string name) : FileElem(name) {}
	virtual void getData(ifstream& from)
	{
		value = 0;
		from.read((char*)&value, 2);
	}
	int getValue()
	{
		return value;
	}
	virtual string add()
	{
		char *a = new char[11];
		sprintf(a, "%d", value);
		return string(a);
	}
	void binaryPrint(ofstream &a)
	{
		a.write((char*)&value, 2);
	}
	void toBin(char *a)
	{
		sscanf(a, "%d", &value);
	}
};
class VersionField : public TwoBytesField
{
public:
	VersionField(string name) : TwoBytesField(name) {}
	string add()
	{
		char *a = new char[4];
		sprintf(a, "%d.%d", value / 10, value % 10);
		return string(a);
	}
	void toBin(char *a)
	{
		int c1, c2;
		sscanf(a, "%d.%d", &c1, &c2);
		value = c1 * 10 + c2;
	}
};
class FourBytesField : public TwoBytesField
{
public:
	FourBytesField(string name) : TwoBytesField(name) {}
	virtual void getData(ifstream& from)
	{
		value = 0;
		from.read((char*)&value, 4);
	}
	void binaryPrint(ofstream &a)
	{
		a.write((char*)&value, 4);
	}
};
class Long : public FileElem
{
	char *value;
	int length;
public:
	Long(string name) : FileElem(name)
	{
		value = NULL;
	}
	~Long()
	{
		if (value)
			delete [] value;
	}
	char *getValue()
	{
		return value;
	}
	void setLength(int length)
	{
		this -> length = length;
	}
	void getData(ifstream& from)
	{
		value = new char[length + 1];
		from.read(value, length);
		value[length] = '\0';
	}
	string add()
	{
		if (!value)
			return "";
		/*string t(value);
		while (t.find('\r') != t.npos)
			t.erase(t.find('\r'), 1);*/
		
		return string(value);
	}
	void binaryPrint(ofstream &a)
	{
		a.write(value, length);
	}
	void readTxt(ifstream &f)
	{
		char *b = new char[1024];
		f.get(b, name.length() + 3);
		value = new char[length + 1];
		f.read(value, length + 1);
		value[length] = '\0';
	}
	void toBin(char *) {}
};
class Elem
{
protected:
	VersionField *versionNeededToExtract;
	TwoBytesField *generalPurposeBitFlag;
	TwoBytesField *compressionMethod;
	Date *lastModFileDate;
	Time *lastModFileTime;
	CRC32 *crc32;
	FourBytesField *compressedSize;
	FourBytesField *uncompressedSize;
	TwoBytesField *fileNameLength;
	TwoBytesField *extraFieldLength;
	Long *fileName;
	ExtraData *extraField;
public:
	Elem()
	{
		versionNeededToExtract = new VersionField("Version needed to extract");
		generalPurposeBitFlag = new TwoBytesField("General purpose bit flag");
		compressionMethod = new TwoBytesField("Compression method");
		lastModFileDate = new Date();
		lastModFileTime = new Time();
		crc32 = new CRC32();
		compressedSize = new FourBytesField("Compressed size");
		uncompressedSize = new FourBytesField("Uncompressed size");
		fileNameLength = new TwoBytesField("File name length");
		extraFieldLength = new TwoBytesField("Extra field length");
		fileName = new Long("File name");
		extraField = new ExtraData("Extra field");
	}
};
class LocalFileHeader : public Elem
{
	friend class Zip;
	Long *fileData;
public:
	LocalFileHeader()
	{
		fileData = new Long("File data");
	}
};
class CentralDirectoryHeader : public Elem
{
	friend class Zip;
	TwoBytesField *versionMadeBy;
	TwoBytesField *fileCommentLength;
	TwoBytesField *diskNumberStart;
	TwoBytesField *internalFileAttributes;
	FourBytesField *externalFileAttributes;
	FourBytesField *relativeOffsetOfLocalHeader;
	Long *fileComment;
public:
	CentralDirectoryHeader()
	{
		versionMadeBy = new TwoBytesField("Version made by");
		fileCommentLength = new TwoBytesField("File comment length");
		diskNumberStart = new TwoBytesField("Disk number start");
		internalFileAttributes = new TwoBytesField("Internal file attributes");
		externalFileAttributes = new FourBytesField("External file attributes");
		relativeOffsetOfLocalHeader = new FourBytesField("Relative offset of local header");
		fileComment = new Long("File comment");
	}
};
class TwoComponentElem
{
	friend class List;
	friend class Iter;
	TwoComponentElem *next;
public:
	LocalFileHeader *lfh;
	CentralDirectoryHeader *cdh;
	TwoComponentElem()
	{
		next = NULL;
		lfh = new LocalFileHeader;
		cdh = new CentralDirectoryHeader;
	}
};
class Iter
{
	friend class List;
	TwoComponentElem* current_elem;
public:
	Iter() : current_elem(0) {}
	Iter operator++()
	{
		if (current_elem)
			current_elem = current_elem -> next;
		return *this;
	}
	TwoComponentElem& operator*() 
	{
		return *current_elem; 
	}
	TwoComponentElem* operator->()
	{ 
		return current_elem; 
	}
	bool operator!=(Iter v)
	{
		return current_elem != v.current_elem;
	}
};
class List
{
	TwoComponentElem *first;
public:
	List()
	{
		first = NULL;
	}
	~List()
	{
		if (!first)
			return;
		TwoComponentElem *temp;
		do
		{
			temp = first -> next;
			delete first;
			first = temp;
		}
		while (temp);
	}
	void push_back(TwoComponentElem *add)
	{
		TwoComponentElem *temp = NULL;
		for (Iter i = begin(); i != end(); ++i)
			temp = &*i;
		if (temp)
			temp -> next = add;
		else
			first = add;
		add -> next = NULL;
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
class Zip
{
	List files;

	TwoBytesField *numberOfThisDisk;
	TwoBytesField *numberOfTheDiskWithTheStartOfTheCentralDirectory;
	TwoBytesField *totalNumberOfEntriesInTheCentralDirectoryOnThisDisk;
	TwoBytesField *totalNumberOfEntriesInTheCentralDirectory;
	FourBytesField *sizeOfTheCentralDirectory;
	FourBytesField *offsetOfStartOfCentralDirectoryWithRespectToTheStartingDiskNumber;
	TwoBytesField *ZIPFileCommentLength;
	Long *ZIPFileComment;

	ifstream file;
	string fileName;
	enum sign { LFHS, CFHS, ECDS };
public:
	Zip(string fileName, bool bin)
	{
		this -> fileName = fileName;
		if (bin)
			file.open(fileName, ios::binary | ios::in);
		else 
			file.open(fileName);
		numberOfThisDisk = new TwoBytesField("Number of this disk");
		numberOfTheDiskWithTheStartOfTheCentralDirectory = 
			new TwoBytesField("Number of the disk with the start of the central directory");
		totalNumberOfEntriesInTheCentralDirectoryOnThisDisk = 
			new TwoBytesField("Total number of entries in the central directory on this disk");
		totalNumberOfEntriesInTheCentralDirectory = 
			new TwoBytesField("Total number of entries in the central directory");
		sizeOfTheCentralDirectory = new FourBytesField("Size of the central directory");
		offsetOfStartOfCentralDirectoryWithRespectToTheStartingDiskNumber = 
			new FourBytesField("Offset of start of central directory with respect to the starting disk number");
		ZIPFileCommentLength = new TwoBytesField(".ZIP file comment length");
		ZIPFileComment = new Long(".ZIP file comment");
	}
	~Zip()
	{
		file.close();
	}
	void writeHuman()
	{
		string newFileName = fileName;
		newFileName.erase(fileName.find('.') + 1, fileName.npos);
		newFileName += "txt";
		ofstream newFile(newFileName);
		newFile << fileName << endl;
		for (Iter i = files.begin(); i != files.end(); ++i)
		{
			newFile << "====================" << endl;
			i->lfh->compressedSize->print(newFile);
			i->lfh->versionNeededToExtract->print(newFile);
			i->lfh->generalPurposeBitFlag->print(newFile);
			i->lfh->compressionMethod->print(newFile);
			i->lfh->lastModFileTime->print(newFile);
			i->lfh->lastModFileDate->print(newFile);
			i->lfh->crc32->print(newFile);
			i->lfh->compressedSize->print(newFile);
			i->lfh->uncompressedSize->print(newFile);
			i->lfh->fileNameLength->print(newFile);
			i->lfh->extraFieldLength->print(newFile);
			i->lfh->fileName->print(newFile);
			i->lfh->extraField->print(newFile);
			i->lfh->fileData->print(newFile);

			newFile << "----" << endl;
			
			i->cdh->versionMadeBy->print(newFile);
			i->cdh->versionNeededToExtract->print(newFile);
			i->cdh->generalPurposeBitFlag->print(newFile);
			i->cdh->compressionMethod->print(newFile);
			i->cdh->lastModFileTime->print(newFile);
			i->cdh->lastModFileDate->print(newFile);
			i->cdh->crc32->print(newFile);
			i->cdh->compressedSize->print(newFile);
			i->cdh->uncompressedSize->print(newFile);
			i->cdh->fileNameLength->print(newFile);
			i->cdh->extraFieldLength->print(newFile);
			i->cdh->fileCommentLength->print(newFile);
			i->cdh->diskNumberStart->print(newFile);
			i->cdh->internalFileAttributes->print(newFile);
			i->cdh->externalFileAttributes->print(newFile);
			i->cdh->relativeOffsetOfLocalHeader->print(newFile);
			i->cdh->fileName->print(newFile);
			i->cdh->extraField->print(newFile);
			i->cdh->fileComment->print(newFile);
		}
		
		newFile << "+++++++++++++++" << endl;
		numberOfThisDisk->print(newFile);
		numberOfTheDiskWithTheStartOfTheCentralDirectory->print(newFile);
		totalNumberOfEntriesInTheCentralDirectoryOnThisDisk->print(newFile);
		totalNumberOfEntriesInTheCentralDirectory->print(newFile);
		sizeOfTheCentralDirectory->print(newFile);
		offsetOfStartOfCentralDirectoryWithRespectToTheStartingDiskNumber->print(newFile);
		ZIPFileCommentLength->print(newFile);
		ZIPFileComment->print(newFile);

		newFile.close();
	}
	void readZip()
	{
		TwoComponentElem* temp;
		
		while (skip() == LFHS)
		{
			temp = new TwoComponentElem;
			temp->lfh->versionNeededToExtract->getData(file);
			temp->lfh->generalPurposeBitFlag->getData(file);
			temp->lfh->compressionMethod->getData(file);
			temp->lfh->lastModFileTime->getData(file);
			temp->lfh->lastModFileDate->getData(file);
			temp->lfh->crc32->getData(file);
			temp->lfh->compressedSize->getData(file);
			temp->lfh->uncompressedSize->getData(file);
			temp->lfh->fileNameLength->getData(file);
			temp->lfh->extraFieldLength->getData(file);
			temp->lfh->fileName->setLength(temp->lfh->fileNameLength->getValue());
			temp->lfh->fileName->getData(file);
			temp->lfh->extraField->setLength(temp->lfh->extraFieldLength->getValue());
			temp->lfh->extraField->getData(file);
			temp->lfh->fileData->setLength(temp->lfh->compressedSize->getValue());
			temp->lfh->fileData->getData(file);
			files.push_back(temp);
		}
		for (Iter i = files.begin(); i != files.end(); ++i)
		{
			i->cdh->versionMadeBy->getData(file);
			i->cdh->versionNeededToExtract->getData(file);
			i->cdh->generalPurposeBitFlag->getData(file);
			i->cdh->compressionMethod->getData(file);
			i->cdh->lastModFileTime->getData(file);
			i->cdh->lastModFileDate->getData(file);
			i->cdh->crc32->getData(file);
			i->cdh->compressedSize->getData(file);
			i->cdh->uncompressedSize->getData(file);
			i->cdh->fileNameLength->getData(file);
			i->cdh->extraFieldLength->getData(file);
			i->cdh->fileCommentLength->getData(file);
			i->cdh->diskNumberStart->getData(file);
			i->cdh->internalFileAttributes->getData(file);
			i->cdh->externalFileAttributes->getData(file);
			i->cdh->relativeOffsetOfLocalHeader->getData(file);
			i->cdh->fileName->setLength(i->cdh->fileNameLength->getValue());
			i->cdh->fileName->getData(file);
			i->cdh->extraField->setLength(i->cdh->extraFieldLength->getValue());
			i->cdh->extraField->getData(file);
			i->cdh->fileComment->setLength(i->cdh->fileCommentLength->getValue());
			i->cdh->fileComment->getData(file);
			skip();
		}

		numberOfThisDisk->getData(file);
		numberOfTheDiskWithTheStartOfTheCentralDirectory->getData(file);
		totalNumberOfEntriesInTheCentralDirectoryOnThisDisk->getData(file);
		totalNumberOfEntriesInTheCentralDirectory->getData(file);
		sizeOfTheCentralDirectory->getData(file);
		offsetOfStartOfCentralDirectoryWithRespectToTheStartingDiskNumber->getData(file);
		ZIPFileCommentLength->getData(file);
		ZIPFileComment->setLength(ZIPFileCommentLength->getValue());
		ZIPFileComment->getData(file);

	}
	sign skip()
	{
		int byte = 1;
		int OK = 0;
		sign which;
		int PKlfhs[] = { 0x50, 0x4B, 0x3, 0x4};
		int PKcfhs[] = { 0x50, 0x4B, 0x1, 0x2};
		int PKecds[] = { 0x50, 0x4B, 0x5, 0x6};
		while (OK < 4 || byte == EOF)
		{
			file.read((char*)&byte, 1);
			if (byte == PKlfhs[OK])
			{
				OK++;
				which = LFHS;
			}
			else
				if (byte == PKcfhs[OK])
				{
					OK++;
					which = CFHS;
				}
				else
					if (byte == PKecds[OK])
					{
						OK++;
						which = ECDS;
					}
					else 
						OK = 0;
		
		}
		return which;
	}
	void createZip(string zipName)
	{
		ofstream newFile;
		newFile.open(zipName, ios::binary | ios::out);
		int t;
		for (Iter i = files.begin(); i != files.end(); ++i)
		{
			t = 0x04034b50;
			newFile.write((char*)&t, 4);
			i->lfh->versionNeededToExtract->binaryPrint(newFile);
			i->lfh->generalPurposeBitFlag->binaryPrint(newFile);
			i->lfh->compressionMethod->binaryPrint(newFile);
			i->lfh->lastModFileTime->binaryPrint(newFile);
			i->lfh->lastModFileDate->binaryPrint(newFile);
			i->lfh->crc32->binaryPrint(newFile);
			i->lfh->compressedSize->binaryPrint(newFile);
			i->lfh->uncompressedSize->binaryPrint(newFile);
			i->lfh->fileNameLength->binaryPrint(newFile);
			i->lfh->extraFieldLength->binaryPrint(newFile);
			i->lfh->fileName->binaryPrint(newFile);
			i->lfh->extraField->binaryPrint(newFile);
			i->lfh->fileData->binaryPrint(newFile);
		}
			
		for (Iter i = files.begin(); i != files.end(); ++i)
		{
			t = 0x02014b50;
			newFile.write((char*)&t, 4);
			i->cdh->versionMadeBy->binaryPrint(newFile);
			i->cdh->versionNeededToExtract->binaryPrint(newFile);
			i->cdh->generalPurposeBitFlag->binaryPrint(newFile);
			i->cdh->compressionMethod->binaryPrint(newFile);
			i->cdh->lastModFileTime->binaryPrint(newFile);
			i->cdh->lastModFileDate->binaryPrint(newFile);
			i->cdh->crc32->binaryPrint(newFile);
			i->cdh->compressedSize->binaryPrint(newFile);
			i->cdh->uncompressedSize->binaryPrint(newFile);
			i->cdh->fileNameLength->binaryPrint(newFile);
			i->cdh->extraFieldLength->binaryPrint(newFile);
			i->cdh->fileCommentLength->binaryPrint(newFile);
			i->cdh->diskNumberStart->binaryPrint(newFile);
			i->cdh->internalFileAttributes->binaryPrint(newFile);
			i->cdh->externalFileAttributes->binaryPrint(newFile);
			i->cdh->relativeOffsetOfLocalHeader->binaryPrint(newFile);
			i->cdh->fileName->binaryPrint(newFile);
			i->cdh->extraField->binaryPrint(newFile);
			i->cdh->fileComment->binaryPrint(newFile);
		}
		
		t = 0x06054b50;
		newFile.write((char*)&t, 4);
		numberOfThisDisk->binaryPrint(newFile);
		numberOfTheDiskWithTheStartOfTheCentralDirectory->binaryPrint(newFile);
		totalNumberOfEntriesInTheCentralDirectoryOnThisDisk->binaryPrint(newFile);
		totalNumberOfEntriesInTheCentralDirectory->binaryPrint(newFile);
		sizeOfTheCentralDirectory->binaryPrint(newFile);
		offsetOfStartOfCentralDirectoryWithRespectToTheStartingDiskNumber->binaryPrint(newFile);
		ZIPFileCommentLength->binaryPrint(newFile);
		ZIPFileComment->binaryPrint(newFile);

		newFile.close();
	}
	void unzip()
	{
		wchar_t *Dir = new wchar_t[fileName.length()];
		string dir = fileName;
		dir.erase(dir.find('.'), dir.npos);
		int i = 0;
		for (; fileName[i] != '.'; i++)
			Dir[i] = fileName[i];
		Dir[i] = '\0';
		_wmkdir(Dir);
		ofstream f;
		for (Iter i = files.begin(); i != files.end(); ++i)
		{
			f.open(dir + "\\"+ i->lfh->fileName->getValue(), ios::binary | ios::out);
			i->lfh->fileData->binaryPrint(f);
			f.close();
		}
	}
	void readTxt()
	{
		TwoComponentElem *temp;
		char *buf = new char[1024];
		file.getline(buf, 1024);
		while (true)
		{
			file.getline(buf, 1024);
			if (buf[0] == '=')
			{
				temp = new TwoComponentElem;
				temp->lfh->compressedSize->readTxt(file);
				temp->lfh->versionNeededToExtract->readTxt(file);
				temp->lfh->generalPurposeBitFlag->readTxt(file);
				temp->lfh->compressionMethod->readTxt(file);
				temp->lfh->lastModFileTime->readTxt(file);
				temp->lfh->lastModFileDate->readTxt(file);
				temp->lfh->crc32->readTxt(file);
				temp->lfh->compressedSize->readTxt(file);
				temp->lfh->uncompressedSize->readTxt(file);
				temp->lfh->fileNameLength->readTxt(file);
				temp->lfh->extraFieldLength->readTxt(file);
				temp->lfh->fileName->setLength(temp->lfh->fileNameLength->getValue());
				temp->lfh->fileName->readTxt(file);
				temp->lfh->extraField->setLength(temp->lfh->extraFieldLength->getValue());
				temp->lfh->extraField->readTxt(file);
				temp->lfh->fileData->setLength(temp->lfh->compressedSize->getValue());
				temp->lfh->fileData->readTxt(file);
				
				file.getline(buf, 1024);
			
				temp->cdh->versionMadeBy->readTxt(file);
				temp->cdh->versionNeededToExtract->readTxt(file);
				temp->cdh->generalPurposeBitFlag->readTxt(file);
				temp->cdh->compressionMethod->readTxt(file);
				temp->cdh->lastModFileTime->readTxt(file);
				temp->cdh->lastModFileDate->readTxt(file);
				temp->cdh->crc32->readTxt(file);
				temp->cdh->compressedSize->readTxt(file);
				temp->cdh->uncompressedSize->readTxt(file);
				temp->cdh->fileNameLength->readTxt(file);
				temp->cdh->extraFieldLength->readTxt(file);
				temp->cdh->fileCommentLength->readTxt(file);
				temp->cdh->diskNumberStart->readTxt(file);
				temp->cdh->internalFileAttributes->readTxt(file);
				temp->cdh->externalFileAttributes->readTxt(file);
				temp->cdh->relativeOffsetOfLocalHeader->readTxt(file);
				temp->cdh->fileName->setLength(temp->cdh->fileNameLength->getValue());
				temp->cdh->fileName->readTxt(file);
				temp->cdh->extraField->setLength(temp->cdh->extraFieldLength->getValue());
				temp->cdh->extraField->readTxt(file);
				temp->cdh->fileComment->setLength(temp->cdh->fileCommentLength->getValue());
				temp->cdh->fileComment->readTxt(file);

				files.push_back(temp);
				
			}
			else
			{
				numberOfThisDisk->readTxt(file);
				numberOfTheDiskWithTheStartOfTheCentralDirectory->readTxt(file);
				totalNumberOfEntriesInTheCentralDirectoryOnThisDisk->readTxt(file);
				totalNumberOfEntriesInTheCentralDirectory->readTxt(file);
				sizeOfTheCentralDirectory->readTxt(file);
				offsetOfStartOfCentralDirectoryWithRespectToTheStartingDiskNumber->readTxt(file);
				ZIPFileCommentLength->readTxt(file);
				ZIPFileComment->setLength(ZIPFileCommentLength->getValue());
				ZIPFileComment->readTxt(file);
				break;
			}
		}
	}
};
