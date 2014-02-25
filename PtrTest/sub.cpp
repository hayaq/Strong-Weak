#include <stdio.h>
#include "Ptr.h"

class Test;
static QC::WeakPtr<Test> weak;

void TestFunc1(QC::Ptr<Test> test){
	weak = test;
	printf("TestFunc1(): %p\n",(Test*)weak);
}

void TestFunc2(){
	printf("TestFunc2(): %p\n",(Test*)weak);
}
