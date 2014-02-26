#include <stdio.h>
#include <stdlib.h>
#include "Ptr.h"

class Test : public QC::Obj{
public:
	Test(){
		printf("Test::Test()\n");
	}
	~Test(){
		printf("Test::~Test()\n");
	}
};

void TestFunc1(QC::Ptr<Test> test);
void TestFunc2();

int main(int argc, const char * argv[])
{
#if 1
	{
		printf("[Test1]\n");
		QC::Ptr<Test> strong = new Test();
		QC::WeakPtr<Test> weak = strong;
		QC::WeakPtr<Test> weak2 = weak;
		
		printf("------- 1 --------\n");
		printf("Strong: %p\n",(Test*)strong);
		printf("Weak:   %p\n",(Test*)weak);
		printf("Weak2:  %p\n",(Test*)weak2);
		strong = NULL;
		printf("------- 2 --------\n");
		printf("Strong: %p\n",(Test*)strong);
		printf("Weak:   %p\n",(Test*)weak);
		printf("Weak2:  %p\n",(Test*)weak2);
		printf("\n");
	}
	
	{
		printf("[Test2]\n");
		QC::Ptr<Test> strong = new Test();
		TestFunc1(strong);
		strong = NULL;
		TestFunc2();
		printf("\n");
	}
#endif
	
	return 0;
}

