#include "game.h" 
#include "windows.h" 

int main()
{
	SetConsoleTitle("LaunchCapman");
	HWND hwnd = GetConsoleWindow();
	MoveWindow(hwnd , 450, 80, 500, 500, TRUE); 
	apresentacao();
	printf("Pronto para jogar?\n");
	system("pause");
	if(hwnd != NULL) 
		MoveWindow(hwnd , 710, 50, 500, 600, TRUE); 
	system("start capman.exe");
	system("cls");
	apresentacao();
	printf(" CONTROLES:\n");
	printf("\n W - Cima\n S - Baixo\n D - Direita\n A - Esquerda");
	printf("\n\n INSTRUCOES:\n");
	printf("\n * Fuja do inimigo " VERMELHO);
	printf("%c" CINZA, 254);
	printf("\n * Capture as moedas " AMARELO);
	printf("*" CINZA);
	getch();
	return 0;
}
