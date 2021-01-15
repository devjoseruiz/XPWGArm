#include "remover.h"

using namespace std;

remover::remover(){
	pid = 0;
	int *celda = new int;
	for(*celda = 0; *celda <= 3; *celda++){
		disponible[*celda] = TRUE;
	}
	delete celda;
}

void remover::rastrear_proceso(){
	/* 
	 * Un handle es una referencia a un objeto controlado por el sistema operativo.
	 * El sistema sirve como capa de comunicación entre el programa y dicho objeto,
	 * es decir, hace de intermediario.
	 * Hago una captura de todo el árbol de procesos para recorrerla más adelante
	 * en busca del proceso WGA.
	 */
	HANDLE captura = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// Obtengo información acerca de los procesos del árbol.
	PROCESSENTRY32 process = {sizeof(PROCESSENTRY32)};

	// Recorro la captura del árbol en busca del proceso "wgatray.exe".
	// Es decir, del programa WGA en ejecución.
	while(Process32Next(captura, &process)){
		// Compruebo si el proceso sobre el que me encuentro es "wgatray.exe".
		// Si no lo es, sigo recorriendo el árbol hasta el final.
		if(!strcmp(process.szExeFile, "WgaTray.exe")){
			// Si el proceso está en ejecución, la variable guardará su PID.
			// Si no ha sido localizado, la variable contendrá el valor 0 (cero).
			pid = process.th32ProcessID;
		}
	}
	CloseHandle(captura); // Este handle ha cumplido su misión, así que lo cierro.
	matar_proceso();
}

int remover::matar_proceso(){
	HANDLE proceso;
	// Si el atributo "pid" contiene el valor 0 (cero), interpreto que WGA
	// no estaba ejecutándose. Aun así comprobaré si es posible borrar los ejecutables de WGA.
	if(pid == 0) MessageBox(NULL, "El proceso no ha sido localizado: esto significa que WGA no estaba en ejecución. Pulse el botón \"Eliminar WGA\" para continuar.", "¡Atención!", MB_OK | MB_ICONEXCLAMATION);
	else{ // En caso contrario interpreto que el proceso ha sido localizado.
		MessageBox(NULL, "Proceso localizado. A continuación se procederá a matar al proceso.", "Información", MB_OK | MB_ICONINFORMATION);
		// Accedo al proceso para matarlo.
		proceso = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
		// En caso de éxito, se lo notifico al usuario.
		if(TerminateProcess(proceso, 0)) MessageBox(NULL, "Proceso finalizado con éxito. Pulse el botón \"Eliminar WGA\" para continuar.", "Éxito", MB_OK | MB_ICONINFORMATION);
		else{ // En caso contrario interpreto que algo va mal y finalizo el programa devolviendo un código de error.
			CloseHandle(proceso); // Cierro este handle antes de finalizar el programa.
			MessageBox(NULL, "Error: no se ha podido matar al proceso. El programa finalizará...", "Error", MB_OK | MB_ICONSTOP);
			exit(1); // El programa finaliza devolviendo un código de error.
		}
	}
	CloseHandle(proceso); // Este handle también ha cumplido su misión, así que lo cierro.
}

void remover::suprimir_wga(){
	// Obtengo la variable de entorno %windir% para suprimir WGA desde una ruta relativa.
	char *ruta = getenv("windir");
	// Ahora cambio el directorio actual por el contenido de "ruta" (UNIDAD:\[...]\Windows).
	chdir(ruta);

	existencia();
	
	char log[300] = "";
	strcat(log, ruta);
	strcat(log, "\\system32\\wgatray.exe\n");

	// Ahora desde la ruta relativa puedo eliminar los ejecutables de WGA:
	if(disponible[0] == FALSE) strcat(log, "Error: el fichero no existe.\n\n");
	else{
		// En caso de éxito, se lo notifico al usuario.
	    if(!remove(".\\system32\\wgatray.exe")) strcat(log, "Hecho.\n\n");
		// Si por algún motivo el archivo no pudiera ser borrado
		// (no existe, o este programa no tiene privilegios, o WGA está siendo usado, etc.),
		// se lo notifico al usuario con un mensaje de error.
	    else strcat(log, "Error: fichero en uso.\n\n");
	}
	strcat(log, ruta);
	strcat(log, "\\system32\\dllcache\\wgatray.exe\n");
	if(disponible[1] == FALSE) strcat(log, "Error: el fichero no existe.\n\n");
	else{
		if(!remove(".\\system32\\dllcache\\wgatray.exe")) strcat(log, "Hecho.\n\n"); // Idem.
		else strcat(log, "Error: fichero en uso.\n\n"); // Idem.
	}

	// Así como también elimino sus librerías auxiliares:
	strcat(log, ruta);
	strcat(log, "\\system32\\wgalogon.dll\n");
    if(disponible[2] == FALSE) strcat(log, "Error: el fichero no existe.\n\n");
	else{
		// En caso de éxito, se lo notifico al usuario.
		if(!remove(".\\system32\\wgalogon.dll")) strcat(log, "Hecho.\n\n");
		// Si por algún motivo el archivo no pudiera ser borrado, se lo notifico al usuario.
		else strcat(log, "Error: fichero en uso.\n\n");
	}
	strcat(log, ruta);
	strcat(log, "\\system32\\dllcache\\wgalogon.dll\n");
	if(disponible[3] == FALSE) strcat(log, "Error: el fichero no existe.\n\n");
	else{
		if(!remove(".\\system32\\dllcache\\wgalogon.dll")) strcat(log, "Hecho.\n\n"); // Idem.
		else strcat(log, "Error: fichero en uso.\n\n"); // Idem.
	}
	
	MessageBox(NULL, log, "Información", MB_OK | MB_ICONINFORMATION);

	reiniciar();
}

void remover::existencia(){
	// Hago una comprobación previa de la existencia de los archivos a borrar.
	// Esto me servirá para ofrecerle al usuario información más detallada sobre
	// algún posible error durante la eliminación de los ficheros.
	ifstream traydll, traydllcache, logondll, logondllcache;
	// Dejo el modo de apertura por defecto (ios::in).
	traydll.open(".\\system32\\wgatray.exe");
	if(!traydll) disponible[0] = FALSE;
	traydll.close();

	traydllcache.open(".\\system32\\dllcache\\wgatray.exe");
	if(!traydllcache) disponible[1] = FALSE;
	traydllcache.close();

	logondll.open(".\\system32\\wgalogon.dll");
	if(!logondll) disponible[2] = FALSE;
	logondll.close();

	logondllcache.open(".\\system32\\dllcache\\wgalogon.dll");
	if(!logondllcache) disponible[3] = FALSE;
	logondllcache.close();
}

void remover::reiniciar(){
	if(disponible[0] == FALSE && disponible[1] == FALSE && disponible[2] == FALSE && disponible[3] == FALSE){
		MessageBox(NULL, "Error: no se ha encontrado ningún fichero. Pulse el botón \"Aceptar\" para finalizar el programa.", "Error", MB_OK | MB_ICONSTOP);
		exit(0);
    }

    else{
		if(MessageBox(NULL, "El sistema debe ser reiniciado para aplicar los cambios.\n¿Desea reiniciar ahora?.", "Información", MB_YESNO | MB_ICONQUESTION) == IDYES){
			MessageBox(NULL, "Cierre todos los programas abiertos y pulse el botón \"Aceptar\".\nSu sistema se reiniciará ahora.", "¡Atención!", MB_OK | MB_ICONEXCLAMATION);
			HANDLE privilegio;
			TOKEN_PRIVILEGES tkprivilegio;

			// Solicito los permisos necesarios para reiniciar el sistema.
			OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &privilegio);
			LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkprivilegio.Privileges[0].Luid);

			tkprivilegio.PrivilegeCount = 1;
			tkprivilegio.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			AdjustTokenPrivileges(privilegio, FALSE, &tkprivilegio, 0, (PTOKEN_PRIVILEGES)NULL, 0);

			// Reinicio el sistema forzando el cierre de todos los programas.
			ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
    	}

        else{
			MessageBox(NULL, "Pulse el botón \"Aceptar\" para finalizar el programa.", "Información", MB_OK | MB_ICONINFORMATION);
			exit(0);
		}
	}
}

remover::~remover(){}
