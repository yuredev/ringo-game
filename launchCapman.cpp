#include "game.h" 
#include "windows.h" 
// este programa tem como objetivo iniciar o jogo Capman e mostrar o manual, controle e instruções.
int main()
{
	SetConsoleTitle("LaunchCapman");
	HWND hwnd = GetConsoleWindow();
	MoveWindow(hwnd , 450, 80, 500, 500, TRUE); 
	apresentacao();
	printf("Pronto para jogar?\n");
	system("pause");
	SetConsoleTitle("MANUAL");
	if(hwnd != NULL) 
		MoveWindow(hwnd , 710, 50, 500, 600, TRUE); 
	system("start capman.exe");
	system("cls");
	apresentacao();
	printf("CONTROLES:\n");
	printf("\n W - Cima\n S - Baixo\n D - Direita\n A - Esquerda");
	printf("\n\nOBJETIVO:\n");
	printf("\n Capture todas as moedas\n para ir pra o proximo nivel.\n Nao deixe o inimigo te pegar.");
	printf("\n\nLEGENDA:\n");
	printf("\n * Jogador: %c",254);
	printf("\n * Inimigo: " VERMELHO);
	printf("%c" CINZA, 254);
	printf("\n * Moedas: " AMARELO);
	printf("*" CINZA);
	printf("\n * Teletransporte: " ROXO);
	printf("%c\n\n" CINZA,178);
	getch();
	return 0;
}
