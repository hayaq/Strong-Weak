#include <stdio.h>
#include <stdlib.h>
#include "Ptr.h"

class Test : public QC::Obj{};

void TestFunc1(QC::Ptr<Test> test);
void TestFunc2();

int main(int argc, const char * argv[])
{
	{
		printf("[Test1]\n");
		QC::Ptr<Test> strong(new Test());
		QC::WeakPtr<Test> weak(strong);
		printf("------- 1 --------\n");
		printf("Strong: %p\n",(Test*)strong);
		printf("Weak:   %p\n",(Test*)weak);
		strong = NULL;
		printf("------- 2 --------\n");
		printf("Strong: %p\n",(Test*)strong);
		printf("Weak:   %p\n",(Test*)weak);
		printf("\n");
	}
	
	{
		printf("[Test2]\n");
		QC::Ptr<Test> strong(new Test());
		TestFunc1(strong);
		strong = NULL;
		TestFunc2();
		printf("\n");
	}
	
	return 0;
}

