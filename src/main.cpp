#include "menu.h"
#include "remover.h"

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil){
	HWND hwnd;
	MSG mensaje;
	WNDCLASSEX wincl;

	wincl.hInstance = hInstance;
	wincl.lpszClassName = "WINCLASS";
	wincl.lpfnWndProc = WindowProcedure;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof (WNDCLASSEX);

	wincl.hIcon = LoadIcon(hInstance, "MAIN_ICON");
	wincl.hIconSm = LoadIcon(hInstance, "MAIN_ICON");
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;

	wincl.hbrBackground = (HBRUSH)COLOR_WINDOW;

	if(!RegisterClassEx(&wincl)) return 0;

	hwnd = CreateWindowEx(
		0,
		"WINCLASS",
		"XPWGArm v0.1.2",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		250,
		200,
		HWND_DESKTOP,
		LoadMenu(hInstance, "Menu"),
		hInstance,
		NULL
		);

    HWND button_wga = CreateWindowEx(0, "BUTTON", "1. Matar WGA", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 55, 25, 130, 45, hwnd, (HMENU)ID_BUTTON_WGA, hInstance, NULL);
    HWND button_rem = CreateWindowEx(0, "BUTTON", "2. Eliminar WGA", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 55, 80, 130, 45, hwnd, (HMENU)ID_BUTTON_REM, hInstance, NULL);

	ShowWindow(hwnd, nFunsterStil);

	while(TRUE == GetMessage(&mensaje, NULL, 0, 0)){
		TranslateMessage(&mensaje);
		DispatchMessage(&mensaje);
	}

	return mensaje.wParam;
}

remover rm;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
        case WM_COMMAND:
            switch(LOWORD(wParam)){
				case CM_CREDITOS:
					if(MessageBox(hwnd, "Programa desarrollado por @joseruizdev.\n¿Deseas visitar mi perfil en GitLab para ver más proyectos?.", "Créditos", MB_YESNO) == IDYES) ShellExecute(NULL, "open", "https://gitlab.com/joseruizdev", NULL, NULL, SW_SHOWNORMAL);
					break;

				case CM_LICENCIA:
					MessageBox(hwnd, "XPWGArm versión 0.1.2.\n\nEste software es suministrado por el desarrollador \"como está\" y cualquier garantía expresa o implícita, incluyendo, pero no limitado a, las garantías  implícitas de comercialización y aptitud para un propósito determinado son rechazadas.\n\nEn ningún caso el desarrollador será responsable por ningún daño directo, indirecto, incidental, especial, ejemplar o consecuente (incluyendo, pero no limitado a, la adquisición de bienes o servicios; la pérdida de uso, de datos o de beneficios; o interrupción de la actividad empresarial) o por cualquier teoría de responsabilidad, ya sea por contrato, responsabilidad estricta o agravio (incluyendo negligencia o cualquier otra causa) que surja de cualquier manera del uso de este software, incluso si se ha advertido de la posibilidad de tales daños.", "Licencia", MB_OK);
					break;
                   
				case CM_SALIR:
					PostQuitMessage(0);
					break;
				
				case ID_BUTTON_WGA:
					rm.rastrear_proceso();
					break;

                case ID_BUTTON_REM:
					rm.suprimir_wga();
					break;
			}
			break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
