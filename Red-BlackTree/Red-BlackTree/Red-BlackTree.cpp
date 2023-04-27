#include <iostream>
#include "Red-BlackTree.h"
#include <cmath>
int main(){

	RBtree<int> tree;
	srand(time(NULL));
	double max_time_per_element = 0.0;
	unsigned order = 7;
	unsigned amount = pow(10, order);
	clock_t start, stop;
	double time;
	int quantity = 0;
	start = clock();
	for (int i = 0; i < amount; i++) {
		quantity = ((rand() << 15) + rand()) % 100;
		clock_t t1 = clock();
		tree.add(quantity);
		clock_t t2 = clock();
		double time_per_element = t2 - t1;
		if (time_per_element > max_time_per_element) {
			max_time_per_element = time_per_element;
			std::cout << "Nowy najgorszy czas: " << i << " : " << max_time_per_element / (double)CLOCKS_PER_SEC << " milisekund\n";
		}
	}
	std::cout << "jola\n";
	std::cout << tree.Height() / log2(amount + 1) << "\n";
	
		tree.add(115);
	tree.add(10);
	tree.add(20);
	tree.add(125);
	tree.add(30);
	tree.add(17);
	tree.add(16);
	tree.add(18);
	tree.add(5);
	tree.add(8);
	tree.displayTree(Order::DES);
	if (tree.HasKey(10)) {
		std::cout << "Szukana wartosc znajduje sie w drzewie\n";
	}
	if (tree.isEmpty()) {
		std::cout << "Drzewo jest puste\n";
	}
	else {
		std::cout << "Drzewo nie jest puste\n";
	}
	std::cout << "Wysokosc Drzewa : " << tree.Height() << "\n" << std::endl;
	std::cout << "Usuwanie drzewa...\n";
	tree.destroyTree();
	if (tree.isEmpty()) {
		std::cout << "Drzewo jest puste\n";
	}
	else {
		std::cout << "Drzewo nie jest puste\n";
	}

	//tree.displayTree(Order::DES);
	stop = clock();
	time = (double)(stop - start) / CLOCKS_PER_SEC;
	std::cout << "Czas calkowity: " << time << " sekund\n";
}