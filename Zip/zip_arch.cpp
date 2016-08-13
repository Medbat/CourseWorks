
#define _CRT_SECURE_NO_WARNINGS
#include "ZipZip.h"

int main()
{
	setlocale(LC_ALL, "rus");
	Zip a("b.zip", 1);
	a.readZip();
	a.writeHuman();
	a.createZip("bb.zip");

	Zip b("bb.zip", 1);
	b.readZip();
	b.writeHuman();

	Zip c("b.txt", 0);
	c.readTxt();
	c.createZip("bbb.zip");

	return 0;
}

