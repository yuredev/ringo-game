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

int jogo(uchar *faseInicial);

int main()
{
	uchar fase = 1;
	uchar opcao = 1;
	char tecla;
	printf("\n\n\n\t   ");
	writeLine(51);
	printf("\n");
	SetConsoleTitle("*** C A P M A N ***");
	do
	{
		system("cls");
		apresentacao();
		if(kbhit())
		{
			tecla = getch();
			opcao = opcaoMenu(tecla, opcao);
		}
		printf("\n\n\t\t  ");
		(opcao == 1) ? printf(FUNDOBRANCO PRETO " INICIAR JOGO " FUNDOPRETO CINZA " <<-") : printf(" INICIAR JOGO  ");
		printf("\n\n\t\t  ");
		(opcao == 2) ? printf(FUNDOBRANCO PRETO " MANUAL " FUNDOPRETO CINZA " <<-") : printf(" MANUAL  ");
		printf("\n\n\t\t  ");
		(opcao == 3) ? printf(FUNDOBRANCO PRETO " CREDITOS " FUNDOPRETO CINZA " <<-") : printf(" CREDITOS  ");
		
		if(opcao != 13)
		{
			fflush(stdin);
			while(!kbhit());
		}
	}while(opcao != 13);
	jogo(&fase);
	return 0;
}


int jogo(uchar *faseInicial)
{
	char cenario[25][25];	
	HWND hwnd = GetConsoleWindow();					
	if(hwnd != NULL) 
		MoveWindow(hwnd ,130,50 ,590, 600, TRUE); 
	agente jogador, inimigo, inimigo2;
	char direcao;
	uchar qtdAneis;
	uchar fase = *faseInicial;
	uchar posAnel[2];
	uchar posAnel2[2];
	uchar tempo = 0;
	bool gelo = false;
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
		qtdAneis = gerarAneis(cenario, fase);
		system("cls");
		printf("\n\tFase %d", fase);
		printf("\t\t\t\t Aneis restantes: %d",qtdAneis);
		mostrarJogo(cenario, jogador, inimigo, inimigo2);
		do
		{	
			if(wasTouched(jogador, inimigo, inimigo2))		// se o inimigo tocar no jogador o jogo termina 
				break;
			
			if(cenario[jogador.linha][jogador.coluna] == 'a')	// Retirar as Aneis após captura
			{
				qtdAneis--;
				cenario[jogador.linha][jogador.coluna] = '0';
				moverCursor(2,59, false);
				printf("%d ",qtdAneis);
			}
			if(flag)										   // mostra denovo a Anel que o inimigo passou por cima
			{
				moverCursor(posAnel[0], posAnel[1], true);
				printf(AMARELO "o" CINZA);
			}
			if(cenario[inimigo.linha][inimigo.coluna] == 'a') // se o inimigo passar por cima da Anel ela nao desaparece
			{
				flag = true;
				posAnel[0] = inimigo.linha;
				posAnel[1] = inimigo.coluna;
			}	
			else 
				flag = false;								
			if(fase == 3)
			{
				if(flag2)										   		// mostra denovo a Anel que o inimigo passou por cima
				{
					moverCursor(posAnel2[0], posAnel2[1], true);
					printf(AMARELO "o" CINZA);
				}
				if(cenario[inimigo2.linha][inimigo2.coluna] == 'a') 	// se o inimigo passar por cima da Anel ela nao desaparece
				{
					flag2 = true;
					posAnel2[0] = inimigo2.linha;
					posAnel2[1] = inimigo2.coluna;
				}	
				else 
					flag2 = false;
			}
			
			if(cenario[jogador.linha][jogador.coluna] == 't')			// movimentação após teletransporte. válido apenas para fase 3
				(jogador.linha < 5) ? direcao = 'w' : direcao = 'a';	// identificar qual a porta de teletransporte				
			
			jogador = acaoJogador(direcao, jogador, cenario, &gelo);			// atualizar posição do jogador 
			
			if(!gelo)
				inimigo = acaoInimigo(inimigo, jogador, cenario, fase);		// atualizar posicao do inimigo
			if(fase == 3 && !gelo)												
				inimigo2 = acaoInimigo(inimigo2, jogador, cenario, fase);		// atualizar posicao do segundo inimigo
			
			if(kbhit())												
				direcao = getch();										// pegar tecla digitada do usuário

			if(gelo)			// se o inimigo estiver comgelado, tem que ser contado o tempo de congelamento
			{
				tempo++;
				moverCursor(inimigo.linha,inimigo.coluna,true);
				printf(CIANO "%c%c" CINZA, 178,178);
				if(fase == 3)
				{
					moverCursor(inimigo2.linha,inimigo2.coluna,true);
					printf(CIANO "%c%c" CINZA, 178,178);
				}
				moverCursor(2, 18, false);
				printf(CIANO "# Inimigo Congelado #" CINZA);
				moverCursor(25, 0,true);
				
			} 
			if(tempo == 25)		// 20 eh o tempo de congelamento, quando chegar nele o inimigo descongela 
			{
				moverCursor(2, 18, false);
				printf("                     ");
				moverCursor(25, 0,true);
				gelo = false;	
				tempo = 0;
			}
		
			Sleep(ESPERA);
		}while(qtdAneis > 0);
		putchar('\a');
		moverCursor(32, 9, false);
		if(qtdAneis > 0)
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
