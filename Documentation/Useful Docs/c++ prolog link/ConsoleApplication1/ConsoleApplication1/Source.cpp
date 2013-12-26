#include <SWI-cpp.h> 
#include <iostream>  
using namespace std;   
int main(){
	char* argv[] = { "swipl.dll", "-s", "teste.pl", NULL };      
	PlEngine e(3, argv);
	PlTermv av(2);
	av[1] = PlCompound("isaque");
	PlQuery q("ancestral", av);    
	while (q.next_solution())  
	{ 
		cout << (char*)av[0] << endl; 
	}    
	cin.get();

	return 1;
}
