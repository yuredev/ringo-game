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
	agente jogador, inimigo;
	uchar espera = 40;
	char direcao;
	uchar qtdPontos = 50;
	uchar rodada = 3;
	bool continuarJogo = true;
	strcpy(jogador.categoria, "jogador");
	strcpy(inimigo.categoria, "inimigo");
	
	do
	{
		direcao = 'z';				
		jogador = resetarPosicoes(jogador, rodada);
		inimigo = resetarPosicoes(inimigo, rodada);
		montarCenario(rodada, cenario, jogador);
		qtdPontos = gerarPontos(cenario, rodada);
		do
		{	
			system("cls");
			if(cenario[jogador.linha][jogador.coluna] == 'c')	// Retirar os pontos após captura
			{
				cenario[jogador.linha][jogador.coluna] = '0';
				qtdPontos--;
			}
			if(cenario[jogador.linha][jogador.coluna] == 't')	// movimentação após teletransporte
			{
				if(jogador.linha < 5)
					direcao = 'w';
				else
					direcao = 'a';	
			}
			printf("\n\tPontos restantes: %d",qtdPontos);
			printf("\t\t\t    Rodada %d",rodada);
			mostrarJogo(cenario, jogador, inimigo);
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
				continuarJogo = true;
				break;
			case 's':
				continuarJogo = true;
				break;
			case 'N':
				continuarJogo = false;
				break;
			case 'n':
				continuarJogo = false;
		}
	}while(continuarJogo);
	system("cls");
	printf("Obigado por jogar :)\nDesenvolvido por Yure Matias\n\n");
	Sleep(500);
	system("taskkill /f /fi \"windowtitle eq Manual\"");
	system("pause");
	return 0;
}
