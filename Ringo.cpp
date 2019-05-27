#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include "game.h"

#define ESPERA 110

/* 
ObservaÃ§Ãµes: 
* Somente compativel com o Windows 10
* para que o manual de jogo seja apresentado, este jogo deve ser iniciado atravÃ©s 
da execuÃ§Ã£o do programa launchCapman.exe presente na mesma pasta. 
*/

void mostrarMenu(char opcao);
void creditos();
void manual();
int jogo(uchar *faseInicial);

int main()
{
	uchar fase = 1;
	uchar opcao = 1;
	char tecla;
	printf("\n\n\n\t   ");
	writeLine(51);
	printf("\n");
	SetConsoleTitle("*** R I N G O ***");
	HWND hwnd = GetConsoleWindow();					
	if(hwnd != NULL) 
		MoveWindow(hwnd ,150,50 ,590, 425, TRUE); 
	do
	{
		if(kbhit())
		{
			tecla = getch();
			if(tecla == 13)								// se o usuário teclar enter 
				switch(opcao)
				{
					case 1: 
						jogo(&fase); 
						if(hwnd != NULL) 
							MoveWindow(hwnd ,150,50 ,590, 425, TRUE); 
						break;
					case 2: 
						manual(); 
						break;
					case 3:
						MoveWindow(hwnd ,150,50 ,590, 300, TRUE); 
						creditos();
						MoveWindow(hwnd ,150,50 ,590, 425, TRUE); 
						break;
					case 4: 
						return 0;	
				}
			else									// se a tecla digitada nao for enter eh pq talvez foi alguma opcao do menu
				opcao = opcaoMenu(tecla, opcao);
		}
		system("cls");
		apresentacao();
		mostrarMenu(opcao);							// apenas mostra o menu de acordo com as opcoes 
		printf(AMARELO"Navegacao: "CINZA);
		printf("\n\t\t ");
		printf(AMARELO"W - cima, S - baixo, Enter - confirmar" CINZA);
		moverCursor(20,1,false);
		if(opcao != 13)
		{
			fflush(stdin);
			while(!kbhit());
		}
	}while(1);
	return 0;
}

int jogo(uchar *faseInicial)
{
	char cenario[25][25];	
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
	HWND hwnd = GetConsoleWindow();					
	if(hwnd != NULL) 
		MoveWindow(hwnd ,150,50 ,590, 600, TRUE); 
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
			
			if(cenario[jogador.linha][jogador.coluna] == 'a')	// Retirar as Aneis apÃ³s captura
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
			
			if(cenario[jogador.linha][jogador.coluna] == 't')			// movimentao apos teletransporte. valido apenas para fase 3
				(jogador.linha < 5) ? direcao = 'w' : direcao = 'a';	// identificar qual a porta de teletransporte				
			
			jogador = acaoJogador(direcao, jogador, cenario, &gelo);			// atualizar posicao do jogador 
			
			if(!gelo)
				inimigo = acaoInimigo(inimigo, jogador, cenario, fase);		// atualizar posicao do inimigo
			if(fase == 3 && !gelo)												
				inimigo2 = acaoInimigo(inimigo2, jogador, cenario, fase);		// atualizar posicao do segundo inimigo
			
			if(kbhit())
			{
				direcao = getch();										// pegar tecla digitada do usuÃ¡rio
				if(direcao == 27)										// se o usuario deigitar ESC o jogo volta ao menu
				{
					moverCursor(1,1,false);							// so pra melhorar a transicao da troca de tela 
					return 0;				
				}
			}												
			if(gelo)			// se o inimigo estiver comgelado, tem que ser contado o tempo de congelamento
			{
				tempo++;
				moverCursor(inimigo.linha,inimigo.coluna,true);
				printf(CIANO "%c%c" CINZA,178,178);
				if(fase == 3)
				{
					moverCursor(inimigo2.linha,inimigo2.coluna,true);
					printf(CIANO "%c%c" CINZA,178,178);
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
			*faseInicial++;
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
			direcao = getch();   								// aproveitamento de variaveis 	
		}while(direcao != 's' && direcao != 'n' && direcao != 'S' && direcao != 'N');   // somente sai do laco se usuario digitar opcoes validas
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
	system("taskkill /f /fi \"windowtitle eq Manual\"");		// fechar o manual apos termino do jogo
	return 0;
}

void creditos()
{
	system("cls");	
	printf("CREDITOS:\n");
	printf("\n Esse jogo foi desenvolvido por Yure Matias de Oliveira" 
	       "\n como trabalho da terceira unidade da disciplina de logica"
	       "\n computacional e algoritmos ofertada pela professora"  
	       "\n Laura Emmanoela Alves dos Santos Santana de Oliveira, no"
	       "\n curso de tecnologo em Analise e Desenvolvimento de Sistemas" 
		   "\n da Universidade Federal do Rio Grande do Norte(UFRN) campus"
	       "\n de Macaiba, onde se situa a Escola Agricola de Jundiai(EAJ).");
	printf("\n\n Acesse: ");
	printf(AZUL "https://github.com/Yurematias/Ringo-Game" CINZA);
	printf("\n\nPressione ESC para voltar");
	
	while(getch() != 27); // 27 eh o numero correspondente ao ESC da tabela ASCII
}	

void manual()
{
	system("cls");
	printf("CONTROLES:\n");
	printf("\n W - Cima\n S - Baixo\n D - Direita\n A - Esquerda");
	printf("\n ESC - Voltar ao menu");
	printf("\n\nOBJETIVO:\n");
	printf("\n Capture todas as moedas\n para ir para o proximo nivel.\n Nao seja pego pelo inimigo.");
	printf("\n\nLEGENDA:\n");
	printf("\n * Jogador: %c",254);
	printf("\n * Inimigo: " VERMELHO);
	printf("%c" CINZA, 254);
	printf("\n * Aneis: " AMARELO);
	printf("o" CINZA);
	printf("\n * Congelar Inimigo : " CIANO);
	printf("#" CINZA);
	printf("\n * Teletransporte: " ROXO);
	printf("%c\n\n" CINZA,178);
	printf(" Pressione ESC para voltar");
	while(getch() != 27); // 27 eh o numero correspondente ao ESC da tabela ASCII
}

void mostrarMenu(char opcao)
{
	printf("\n\n\n\t\t  ");
	(opcao == 1) ? printf(FUNDOBRANCO PRETO " INICIAR JOGO " FUNDOPRETO CINZA " <<=") : printf(" INICIAR JOGO  ");
	printf("\n\n\t\t  ");
	(opcao == 2) ? printf(FUNDOBRANCO PRETO " MANUAL " FUNDOPRETO CINZA " <<=") : printf(" MANUAL  ");
	printf("\n\n\t\t  ");
	(opcao == 3) ? printf(FUNDOBRANCO PRETO " CREDITOS " FUNDOPRETO CINZA " <<=") : printf(" CREDITOS  ");
	printf("\n\n\t\t  ");
	(opcao == 4) ? printf(FUNDOBRANCO PRETO " SAIR " FUNDOPRETO CINZA " <<=") : printf(" SAIR  ");
	printf("\n\n\n\n\t\t ");
}
