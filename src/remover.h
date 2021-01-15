#include <iostream>
#include <fstream>
#include <windows.h>
#include <dir.h>
// Incluyo la librería que me permitirá capturar el árbol de procesos
// y obtener información acerca de los mismos.
#include <tlhelp32.h>

class remover{
	private:
		int pid; // Atributo que contendrá el Process ID de WGA.
		bool disponible[4];
		int matar_proceso();
		void existencia(); // Método que comprueba la existencia de los ficheros a borrar.
		void reiniciar();

	public:
		remover(); // Constructor.
		void rastrear_proceso();
		void suprimir_wga();
		~remover(); // Destructor.
};
