#include "debugModule.h"
#include <assert.h>
#include <iostream>
using namespace std;
void MyAssert(bool expresion,char *msg)
{
	if(false == expresion)
	{
		return;
	}
	else
	{
		cout<<msg<<endl;
		assert(true);
	}
	
}