#include <SWI-cpp.h> 
#include <iostream>  
using namespace std;   
int main(){
	char* argv[] = { "swipl.dll", "-s", "teste.pl", NULL };      
	PlEngine e(3, argv);
	PlTermv av(2);
	av[1] = PlCompound("isaque"); //2 argumento de ancestral(1,2).
	PlQuery q("ancestral", av);     // regra a ser chamada
	while (q.next_solution())  //obter todas as soluções
	{ 
		cout << (char*)av[0] << endl; 
	}    
	cin.get();

	return 1;
}
