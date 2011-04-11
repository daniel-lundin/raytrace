#include <iostream>
#include <time.h>
using namespace std;

int main()
{
	/* initialize random seed: */
	srand ( time(NULL) );

	int foo = rand() % 10;
	for (int i = 0; i < 10; i++) {
		int foo = rand() % 10;
		cout << foo << endl;
	}
    return 0;
}
