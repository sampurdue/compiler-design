#include <list>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class test
{
	public:
	char* name;
	list<int> *intlist;

	public:
	test(){name = NULL; intlist = new list<int>();}
};

int main()
{
	list<int> mylist;
	mylist.push_back(1);
	mylist.push_back(2);
	for(list<int>::iterator it = mylist.begin(); it != mylist.end();it++)
	{
		printf("%d \n",*it);
	}

	test *mytest = new test();
	mytest->name = "whoa";
	mytest->intlist->push_back(1);
	mytest->intlist->push_back(2);
	for(list<int>::iterator it = mytest->intlist->begin(); it != mytest->intlist->end();it++)
        {
                printf("%d \n",*it);
        }

	return 0;
}
