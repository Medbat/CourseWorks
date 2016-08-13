
#include "FileSystem.h"

int main()
{
	setlocale(LC_ALL, "rus");

	Long aa;
	Directory a(assign(aa, "0"), "ROOT", 0, 1);
	Directory* push;
	a.push_back(push = new Directory(assign(aa, "0"),"D1",0,0));
	Directory* save;
	File* saveF1, *saveF2;
	push -> push_back(save = new Directory(assign(aa, "0"), "D2", 0, 0));
	save -> push_back(new File(assign(aa, "1231"), "F1", 0, 1))
		.push_back(new File(assign(aa, "11"), "F2", 0, 0));
	push -> push_back(4, new File(assign(aa, "12"), "F2", 1, 0), 
		saveF1 = new File(assign(aa, "2").involution(2),"F3", 1, 1), 
		saveF2 = new File(assign(aa, "129"),"F4", 0, 0),
		new File(assign(aa, "12"),"F5", 0, 0));
	a.push_back(new Directory(assign(aa, "0"),"D3",0,0));
	a.prepare();
	cout << a.str();

	WindowsFileCheck(saveF1).summarize();

	DVDFileCheck dvd(saveF1);
	dvd.summarize();
	dvd = saveF2;
	dvd.summarize();

	DVDDirectoryCheck dvdD(&a);
	dvdD.summarize();
	
	AttributeCheck attrCheck(&a);
	attrCheck.summarize();
	cout << endl;
	system("pause");
}
