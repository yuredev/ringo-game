#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include "game.h"

#define ESPERA 110

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
	char direcao;
	uchar qtdMoedas;
	uchar fase = 3;
	uchar posMoeda[2];
	uchar posMoeda2[2];
	bool flag = false;
	bool flag2 = false;
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
		system("cls");
		printf("\n\tFase %d", fase);
		printf("\t\t\t\tMoedas restantes: %d",qtdMoedas);
		mostrarJogo(cenario, jogador, inimigo, inimigo2);
		do
		{	
			if(cenario[jogador.linha][jogador.coluna] == 'm')	// Retirar as moedas após captura
			{
				qtdMoedas--;
				cenario[jogador.linha][jogador.coluna] = '0';
				moverCursor(2,59, false);
				printf("%d ",qtdMoedas);
			}
			if(flag)										   // mostra denovo a moeda que o inimigo passou por cima
			{
				moverCursor(posMoeda[0], posMoeda[1], true);
				printf(AMARELO "*" CINZA);
			}
			if(cenario[inimigo.linha][inimigo.coluna] == 'm') // se o inimigo passar por cima da moeda ela nao desaparece
			{
				flag = true;
				posMoeda[0] = inimigo.linha;
				posMoeda[1] = inimigo.coluna;
			}	
			else 
				flag = false;
			if(fase == 3)
			{
				if(flag2)										   // mostra denovo a moeda que o inimigo passou por cima
				{
					moverCursor(posMoeda2[0], posMoeda2[1], true);
					printf(AMARELO "*" CINZA);
				}
				if(cenario[inimigo2.linha][inimigo2.coluna] == 'm') // se o inimigo passar por cima da moeda ela nao desaparece
				{
					flag2 = true;
					posMoeda2[0] = inimigo2.linha;
					posMoeda2[1] = inimigo2.coluna;
				}	
				else 
					flag2 = false;
			}
			
			if(cenario[jogador.linha][jogador.coluna] == 't')							// movimentação após teletransporte. válido apenas para fase 3
				(jogador.linha < 5) ? direcao = 'w' : direcao = 'a';						// identificar qual a porta de teletransporte				
			
			jogador = acaoJogador(direcao, jogador, cenario);		// atualizar posição do jogador 
			inimigo = acaoInimigo(inimigo, jogador, cenario);		// atualizar posicao do inimigo

			if(fase == 3)												
				inimigo2 = acaoInimigo(inimigo2, jogador, cenario);	
			
			if(kbhit())												
				direcao = getch();									// pegar tecla digitada do usuário

			Sleep(ESPERA);
		}while(qtdMoedas > 0 && !wasTouched(jogador, inimigo, inimigo2));
		putchar('\a');
		moverCursor(32, 9, false);
		if(qtdMoedas > 0)
			animacaoDerrota();	
		
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
