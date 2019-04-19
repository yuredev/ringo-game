#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include "game.h"

/* 
Observações: 
* Somente compatível com Windows 10
* para que o manual de jogo seja apresentado, este jogo deve ser iniciado através 
da execução do programa launchCapman.exe presente na mesma pasta. 
*/

int main()
{
	SetConsoleTitle("*** C A P M A N ***");
	char cenario[25][25];	
	HWND hwnd = GetConsoleWindow();					
	if(hwnd != NULL) 
		MoveWindow(hwnd ,130,50 ,590, 600, TRUE); 
	agente jogador, inimigo, inimigo2;
	uchar espera = 50;
	char direcao;
	uchar qtdMoedas;
	uchar fase = 1;
	bool continuarJogo = true;
	strcpy(jogador.categoria, "jogador");
	strcpy(inimigo.categoria, "inimigo");
	strcpy(inimigo2.categoria, "inimigo2");
	do
	{
		direcao = 'z';				
		jogador = resetarPosicoes(jogador, fase);
		inimigo = resetarPosicoes(inimigo, fase);
		if(fase == 3)
			inimigo2 = resetarPosicoes(inimigo2, fase);
		else
		{
			inimigo2.linha = 99;  // apenas para retirar o inimigo2 de jogo nas rodadas 1 e 2 
			inimigo2.coluna = 99;	
		}	
		montarCenario(fase, cenario, jogador);		
		qtdMoedas = gerarMoedas(cenario, fase);
		do
		{	
			system("cls");
			if(cenario[jogador.linha][jogador.coluna] == 'm')	// Retirar as moedas após captura
			{
				cenario[jogador.linha][jogador.coluna] = '0';
				qtdMoedas--;
			}
			if(cenario[jogador.linha][jogador.coluna] == 't')							// movimentação após teletransporte. válido apenas para fase 3
				(jogador.linha < 5) ? direcao = 'w' : direcao = 'a';						// identificar qual a porta de teletransporte				
			printf("\n\tFase %d", fase);
			printf("\t\t\t\tMoedas restantes: %d",qtdMoedas);
			inimigo = acaoInimigo(inimigo, jogador, cenario);				// atualizar posicao do inimigo
			if(fase == 3)
				inimigo2 = acaoInimigo(inimigo2, jogador, cenario);
			jogador = acaoJogador(direcao, jogador, cenario);		// atualizar posição do jogador 
			mostrarJogo(cenario, jogador, inimigo, inimigo2);
			if(kbhit())												
				direcao = getch();									// pegar tecla digitada do usuário
			if(wasTouched(jogador, inimigo, inimigo2))				// se o inimigo tocar no jogador é Game Over
				break;
			Sleep(espera);
		}while(qtdMoedas > 0 && fase <= 3);
		putchar('\a');
		if(qtdMoedas > 0)
		{
			animacaoDerrota();	
			fase = 1;
		}
		else
		{
			animacaoVitoria(fase);
			fase++;	
			if(fase == 4)
			{
				system("cls");
				printf("Parabens !!!");
				printf("Voce zerou o jogo :)\n");
				fase = 1;
			}
		}
		printf("Deseja continuar jogando? (S/N): ");
		do
		{
			while(!kbhit());									// laço inifinito até usuário digitar alguma tecla
			direcao = getch();   								// aproveitamento de variaveis 	
		}while(direcao != 's' && direcao != 'n' && direcao != 'S' && direcao != 'N');   // só sai do laço se usuário digitar opções validas
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
	Sleep(2000);
	system("taskkill /f /fi \"windowtitle eq Manual\"");		// fechar o manual após termino do jogo
	return 0;
}
