#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include "game.h"

int main()
{
	SetConsoleTitle("*** C A P M A N ***");
	char cenario[25][25];	
	HWND hwnd = GetConsoleWindow();
	if(hwnd != NULL) 
		MoveWindow(hwnd ,130,50 ,590, 600, TRUE); 
	agente jogador, inimigo, inimigo2;
	uchar espera = 70;
	char direcao;
	uchar qtdPontos = 50;
	uchar rodada = 2;
	bool pararJogo = false;
	strcpy(jogador.categoria, "jogador");
	strcpy(inimigo.categoria, "inimigo");
	strcpy(inimigo2.categoria, "inimigo2");
	do
	{
		direcao = 'z';				
		jogador = resetarPosicoes(jogador, rodada);
		inimigo = resetarPosicoes(inimigo, rodada);
		if(rodada == 3)
			inimigo2 = resetarPosicoes(inimigo2, rodada);
		montarCenario(rodada, cenario, jogador);
		qtdPontos = gerarPontos(cenario, rodada);
		do
		{	
			system("cls");
			if(cenario[jogador.linha][jogador.coluna] == 'c')
			{
				cenario[jogador.linha][jogador.coluna] = '0';
				qtdPontos--;
			}
			printf("\n\tPontos restantes: %d",qtdPontos);
			printf("\t\t\t    Rodada %d",rodada);
			
			mostrarJogo(cenario, jogador, inimigo, inimigo2);
					
			if(kbhit())
				direcao = getch();			
			jogador = acaoJogador(direcao, jogador, cenario);
			Sleep(espera);
		}while(!wasTouched(jogador, inimigo) && qtdPontos > 0);
		if(qtdPontos > 0)
		{
			animacaoDerrota();
			rodada = 1;	
		}
		else
		{
			animacaoVitoria(rodada);
			rodada++;	
			espera -= 10; 
		}
		printf("Deseja continuar jogando? (S/N): ");
		do
		{
			while(!kbhit());							// laço inifinito até usuário digitar alguma tecla
			direcao = getch();   						// aproveitamento de variaveis 	
		}while(direcao != 's' && direcao != 'n' && direcao != 'S' && direcao != 'N');	
		switch(direcao)
		{
			case 'S':
				pararJogo = false;
				break;
			case 's':
				pararJogo = false;
				break;
			case 'N':
				pararJogo = true;
				break;
			case 'n':
				pararJogo = true;
		}
	}while(!pararJogo);
	system("cls");
	printf("Obigado por jogar :)\nDesenvolvido por Yure Matias\n\n");
	Sleep(700);
	system("taskkill /f /fi \"windowtitle eq LaunchCapman\"");
	return 0;
}
