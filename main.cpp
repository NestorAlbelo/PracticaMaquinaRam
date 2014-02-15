#include "RamMachine.h"

int main() {
	maquinaRam M1;

	if (M1.leerTexto("test2.ram"))
		M1.mostrarLineas();
		
	system("pause");
	return 0;
}
