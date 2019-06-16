#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "colors.h"

typedef unsigned char uchar;

// estrutura do jogador e inimigo 
struct agente		
{
	int linha;
	int coluna;
	char categoria[8];
};   

// funcao que para manipular o menu principal do jogo 
char opcaoMenu(char tecla, uchar op)
{
	switch(tecla)
	{
		case 'W':
			if(op > 1)
				op--;
			break;
		case 'w':
			if(op > 1)
				op--;		
			break;
		case 72:		// Seta para cima ASCII
			if(op > 1)
				op--;		
			break;
		case 'S':
			if(op < 4)
				op++;
			break;
		case 's':
			if(op < 4)
				op++;
			break;
		case 80:		// Seta para baixo ASCII
			if(op < 4)
				op++;		
	}
	return op;		
}

// funcao que mostra o nome do jogo no menu principal, apenas para a estetica do jogo 
void apresentacao()		// somente estetica 
{
	printf(FUNDOBRANCO PRETO 
   "\n\t\t                                         "	
	"\n\t\t %c%c%c%c%c  %c  %c%c     %c   %c%c%c%c%c%c   %c%c%c%c%c%c%c%c%c "
			"\n\t\t %c   %c  %c  %c%c     %c  %c         %c       %c "
			"\n\t\t %c   %c  %c  %c %c    %c %c          %c       %c "
			"\n\t\t %c%c%c%c%c  %c  %c  %c   %c %c    %c%c%c%c%c %c       %c "
			"\n\t\t %c %c    %c  %c   %c  %c %c        %c %c       %c "
			"\n\t\t %c  %c   %c  %c    %c %c  %c      %c  %c       %c "
			"\n\t\t %c   %c  %c  %c     %c%c   %c%c%c%c%c%c   %c%c%c%c%c%c%c%c%c " 
			"\n\t\t                                         " FUNDOPRETO CINZA
			,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219
			,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219
			,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219
			,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219
			,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219
			,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219
			,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219
			,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219
			,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
}

// funcao que escreve una linha no terminal de acordo com o tamanho informado 
void writeLine(uchar tam)		// escreve uma linha
{
	uchar i;
	for(i = 0; i < tam; i++)
		printf("%c",219);
	printf("\n");	
}

// funcao que move o cursor do terminal, equivalente ao gotoxy()
void moverCursor(int linha, int coluna, bool aPartirDaMatriz)	// move o cursor
{
	if(aPartirDaMatriz)
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){coluna * 2 + 9, linha + 4});
	else
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){coluna - 1, linha -1});	
}

// funcao que teleporta o jogador no cenario 
agente teletransporte(agente jogador, char cenario[25][25])
{
	moverCursor(jogador.linha, jogador.coluna, true);
	printf(ROXO "%c%c" CINZA, 178,178);
	if(jogador.linha < 5)			// identificar qual porta de teletransporte
	{
		jogador.linha = 23;
		jogador.coluna = 12;
	}
	else 
	{
		jogador.linha = 0;
		jogador.coluna = 23;
	}
	return jogador;
}

// funcao que realiza as mudancas nas posicoes do jogador
agente acaoJogador(char direcao, agente jogador, char cenario[25][25], bool *gelo)	// mexe o jogador no cenario
{
	// apagar o jogador da tela 
	moverCursor(jogador.linha, jogador.coluna, true);
	printf(" ");	
	if(cenario[jogador.linha][jogador.coluna] == 't')	// teletransporte
		jogador = teletransporte(jogador, cenario);
		
	else			// se nao for teleteransportar a movimentacao ocorre normalmente 
	{
		switch(direcao)	// apenas para aceitar caps lock e as setas 
		{
			case 'W':
				direcao = 'w';
				break;	
			case 'A':
				direcao = 'a';
				break;		
			case 'D':
				direcao = 'd';
				break;	
			case 'S':
				direcao = 's';
				break;			
			case 72:			// SETA PARA CIMA (TABELA ASCII)
				direcao = 'w';
				break;
			case 75:			// SETA PARA ESQUERDA (TABELA ASCII)
				direcao = 'a';
				break;
			case 77:			// SETA PARA DIREITA (TABELA ASCII)
				direcao = 'd';
				break;
			case 80:			// SETA PARA BAIXO (TABELA ASCII)
				direcao = 's';	 			
		}
		switch(direcao)
		{
			case 'w':
				if(jogador.linha > 0 && cenario[jogador.linha - 1][jogador.coluna] != 'p') // o jogador so mexe se a proxima casa nao houver paredes. isso se aplica aos outros cases
					jogador.linha--;
				break;	
			case 'a':
				if(jogador.coluna > 0 && cenario[jogador.linha][jogador.coluna - 1] != 'p')
					jogador.coluna--;
				break;	
			case 'd':
				if(jogador.coluna < 24 && cenario[jogador.linha][jogador.coluna + 1] != 'p')
					jogador.coluna++;
				break;	
			case 's':
				if(jogador.linha < 24 && cenario[jogador.linha + 1][jogador.coluna] != 'p')
					jogador.linha++;
				break;		
		}	
	}
	
	// verifica se o jogador pegou o gelo e se sim ele seta o valor de gelo para verdadeiro
	if(cenario[jogador.linha][jogador.coluna] == 'g')
	{
		*gelo = true;
		cenario[jogador.linha][jogador.coluna] = '0';
	}
	
	// mostrar o jogador ja nas novas posicoes 
	moverCursor(jogador.linha, jogador.coluna, true);
	printf("%c",254);
	moverCursor(25, 0,true);
	return jogador;	
}

// funcao que mexe o inimigo no cenario 
agente acaoInimigo(agente inimigo, agente jogador, char cenario[25][25], uchar fase)
{
	char direcao;
	
	// apagar o inimigo da tela no cenario
	moverCursor(inimigo.linha, inimigo.coluna, true);
	printf("  ");	
	
	// condicao gigante para verificacao para desvio de paredes. (obs: separei o if em 3 partes para melhor compreensao, ou talvez ne...) 
	// a primeira parte eh para limitar para nao passar da parede que preenche a tela inteira na fase 3
	if(!(fase == 3 && (inimigo.linha == 7 || inimigo.linha == 5)) && !strcmp(inimigo.categoria,"inimigo") && 
	((inimigo.linha < jogador.linha && cenario[inimigo.linha + 1][inimigo.coluna] == 'p') || // se tiver uma parede abaixo do inimigo
	(inimigo.linha > jogador.linha && cenario[inimigo.linha - 1][inimigo.coluna] == 'p'))) // se tiver uma parede acima do inimigo
	{
		if(jogador.coluna > 12)	 // se o jogador estiver do lado direito do cenario	
			direcao = 'd';		 // setei ele para desviar da parede pela direita 
		else					 // se estiver do lado esquerdo do cenario 
			direcao = 'a';		 // setei ele para desviar da parede pela esquerda 
	}	
	else 
	if(inimigo.linha == jogador.linha)	// se o inimigo tiver na mesma linha do jogador ele move-se em linha reta 
	{
		if(inimigo.coluna < jogador.coluna)		// se o inimigo estiver a esquerda do jogador 
			direcao = 'd';						// ele deve se movimentar a direita 
		else 									// caso contrario ele esta a direita 
			direcao = 'a';						// entao movimenta-se para esquerda 
	}		 
	else if(inimigo.coluna == jogador.coluna)    // se o inimigo tiver na mesma coluna do jogador ele move-se em linha reta 
	{
		if(inimigo.linha < jogador.linha)		// se o inimigo estiver acima do jogador
			direcao = 's';						// ele deve se movimentar para baixo  
		else									// caso contrario, ele esta abaixo
			direcao = 'w';						// ele deve se movimentar para cima 
	}
	else if(rand() % 2 == 0)                                       // 50% de chance do inimigo mover-se no eixo horizontal 
		direcao = (inimigo.coluna < jogador.coluna)	? 'd' : 'a';   // mexer no eixo horizontal 		
	else					                                       // 50% de chance do inimigo mover-se no eixo vertical 
		direcao = (inimigo.linha < jogador.linha) ? 's' : 'w';	   // mexer no eixo vertical	
	
	switch(direcao)
	{
		case 'w':
			if(cenario[inimigo.linha-1][inimigo.coluna] != 'p')  // limitacao para nao ultrapassar paredes
				inimigo.linha--; 
			break;
		case 's':
			if(cenario[inimigo.linha+1][inimigo.coluna] != 'p') // limitacao para nao ultrapassar paredes
				inimigo.linha++;
			break;
		case 'a':
			if(cenario[inimigo.linha][inimigo.coluna-1] != 'p')  // limitacao para nao ultrapassar paredes
				inimigo.coluna--;
			break;
		case 'd':
			if(cenario[inimigo.linha][inimigo.coluna+1] != 'p') // limitacao para nao ultrapassar paredes
				inimigo.coluna++;
			break;			
	}
	if(cenario[inimigo.linha][inimigo.coluna] == 'g')       // se o inimigo passar por cima do gelo, o gelo desaparece 
		cenario[inimigo.linha][inimigo.coluna] = '0';
	
	// printar o inimigo na nova posicao do cenario
	moverCursor(inimigo.linha, inimigo.coluna, true);
	printf(VERMELHO"%c" CINZA, 254);
	moverCursor(25, 0,true);
	return inimigo;
}

// funcao que faz a geracao de aneis na fase 
int gerarAneis(char cenario[25][25], uchar fase)		// gera os Aneis da fase 
{
	uchar qtdAneisFase;
	uchar l,c;
	srand(time(NULL));
	switch(fase)						// definir quantas Aneis serao geradas de acordo com a fase
	{
		case 1:
			qtdAneisFase = 20;
			break;
		case 2:
			qtdAneisFase = 25;
			break;
		case 3:
			qtdAneisFase = 30;
	}
	
	for(int i = 0; i < qtdAneisFase; i++)
	{
		do
		{
			l = rand() % 25;		  
			c = rand() % 25;
			if(cenario[l][c] == '0') // verifica se esta disponivel a posicao para colocar um anel  
			{
				cenario[l][c] = 'a';
				break;
			}
		}while(cenario[l][c] != '0');	// repita ate que as coordenadas geradas para o anel estejam disponiveis 
	}
	return qtdAneisFase;		
}

// funcao que reseta as posicoes do agente, sendo este inimigo ou o jogador. Isto de acordo com a fase 
agente resetarPosicoes(agente x, uchar fase)	
{
	switch(fase)
	{
		case 1:
			if(!strcmp(x.categoria, "inimigo"))	// se o agente for inimigo
			{
				x.linha = 16;
				x.coluna = 10;
			}
			else				// se o agente  for jogador 
			{
				x.linha = 3;
				x.coluna = 9;
			}
			break;
		case 2:
			if(!strcmp(x.categoria, "inimigo"))	// se o agente for inimigo
			{
				x.linha = 20;
				x.coluna = 21;
			}
			else					// se o agente for jogador 
			{
				x.linha = 0;
				x.coluna = 10;
			}
			break;
		case 3:
			if(!strcmp(x.categoria, "inimigo"))	//se o agente for inimigo 
			{
				x.linha = 7;
				x.coluna = 22;
			}
			else if(!strcmp(x.categoria, "jogador"))  // se o agente for jogador 
			{
				x.coluna = 0;
				x.linha = 22;
			}			
			else 			// se o agente for o segundo inimigo da fase 3
			{				
				x.linha = 5;
				x.coluna = 6;
			}
	}	
	return x;			
}

// funcao que escreve na tela o cenario do jogo
void mostrarJogo(char cenario[25][25], agente jogador, agente inimigo, agente inimigo2)		// mostra a tela de jogo 
{
	uchar i,j;
	printf("\n\n\t");
	writeLine(52);											// parede superior do cenario 
	for(i = 0; i < 25; i++)
	{
		printf("\t%c",219);									// limite do cenario, parede vertical do inicio 
		for(j = 0; j < 25; j++)
		{
			if(cenario[i][j] == 't')					//mostrar teletransporte
				printf(ROXO "%c%c" CINZA,178,178);	
			else if(cenario[i][j] == 'g')				// mostrar gelo 
				printf(CIANO "# " CINZA);	
			else if(i == inimigo2.linha && j == inimigo2.coluna)	// mostrar segundo inimigo (fase 2)
				printf(VERMELHO "%c " CINZA,254);	
			else if(i == inimigo.linha && j == inimigo.coluna)	// mostrar inimigo 
				printf(VERMELHO "%c " CINZA,254);
			else if(i == jogador.linha && j == jogador.coluna)	// mostrar jogador 	
				printf("%c ",254);							
			else if(cenario[i][j] == '0')					// mostrar espaÃ§o vazio
				printf("  ");
			else if(cenario[i][j] == 'p')					// mostrar paredes
				printf("%c%c", 178,178);		
			else if(cenario[i][j] == 'a')					//mostrar aneis
				printf(AMARELO "o " CINZA);	
		}
		printf("%c",219);							// limite do cenario, parede vertical do fim
		printf("\n");
	}
	printf("\t");
	writeLine(52);									// parede inferior do cenario 
	printf("\n\t");						
}

// funcao que verifica se o inimigo tocou no jogador 
bool wasTouched(agente jogador, agente inimigo, agente inimigo2)    
{
	bool touch = false; 
	// verifica se o inimigo 1 ou 2 esta na mesma cordenada do jogador 
	if((jogador.linha == inimigo.linha && jogador.coluna == inimigo.coluna) || (jogador.linha == inimigo2.linha && jogador.coluna == inimigo2.coluna))
		touch = true;
	// verifica se o inimigo 1 ou 2 esta a um bloco a direita do jogador
	else if((jogador.linha-1 == inimigo.linha && jogador.coluna == inimigo.coluna) || (jogador.linha-1 == inimigo2.linha && jogador.coluna == inimigo2.coluna))
		touch = true;
	// verifica se o inimigo 1 ou 2 esta a um bloco a esquerda do jogador 	
	else if((jogador.linha+1 == inimigo.linha && jogador.coluna == inimigo.coluna) || (jogador.linha+1 == inimigo2.linha && jogador.coluna == inimigo2.coluna))
		touch = true;
	// verifica se o inimigo 1 ou 2 esta um bloco acima do jogador	
	else if((jogador.linha == inimigo.linha && jogador.coluna-1 == inimigo.coluna) || (jogador.linha == inimigo2.linha && jogador.coluna-1 == inimigo2.coluna))
		touch = true;
	// verifica se o inimigo 1 ou 2 esta a um bloco abaixo do jogador 	
	else if((jogador.linha == inimigo.linha && jogador.coluna+1 == inimigo.coluna) || (jogador.linha == inimigo2.linha && jogador.coluna+1 == inimigo2.coluna))	
		touch = true;
	return touch;
}

// funcao que constroi uma parede horizontal no cenario do jogo 
void fazerParede(uchar linha, uchar inicioParede, uchar fimParede, char cenario[25][25])	
{
	for(; inicioParede <= fimParede; inicioParede++)
		cenario[linha][inicioParede] = 'p';
}

// funcao que monta o cenario do jogo de acrodo com a fase 
void montarCenario(uchar fase, char cenario[25][25], agente jogador)
{
	uchar i,j;
	uchar limite;					// usada para construir o cenario da fase 2
	for(i = 0; i < 25; i++)			// zerar cenario para construir o novo 
		for(j = 0; j < 25; j++)
			cenario[i][j] = '0';
	switch(fase)					// monta as paredes do cenario de acordo com a fase 
	{
		case 1:
			
			cenario[10][1] = 'g';
			cenario[23][20] = 'g';
			
			for(j = 2; j < 23; j+=2)
				cenario[4][j] = 'p';
					
			fazerParede(7,2,12,cenario);	
			fazerParede(10, 14, 23, cenario);
			fazerParede(13, 2, 12, cenario);		
			
			for(j = 4; j < 21; j+=4)
				cenario[15][j] = 'p';
			for(j = 1; j < 11; j+=1)
				cenario[18][j] = 'p';	
			for(j = 13; j < 24; j+=1)
				cenario[18][j] = 'p';		
			for(j = 2; j < 23; j+=3)
				cenario[21][j] = 'p';				
			break;	
		case 2:
			cenario[8][3] = 'g';
			cenario[23][20] = 'g';
			
			
			for(j = 1; j < 24; j++)   
			{
				if((j+2) % 2 == 1)   // j + 2 para nao dar erro na divisÃ£o por 2. numero par + 2 continua par. impar + 2 continua impar
				{							
					cenario[10][j] = 'p';
					cenario[14][j] = 'p';
				}
				else 
					cenario[12][j] = 'p';					
			}
			fazerParede(3, 2, 12, cenario);
			fazerParede(5, 14, 23, cenario);
			fazerParede(7, 2, 12, cenario);
			fazerParede(18, 14, 23, cenario);	
			fazerParede(20, 2, 12, cenario);	
			fazerParede(22, 14, 23, cenario);	
			break;	
		case 3:
			cenario[1][5] = 'g';
			cenario[0][24] = 't';			// teletransportes
			cenario[1][24] = 't';
			fazerParede(24, 0, 24, cenario);
			cenario[24][11] = 't';
			cenario[24][12] = 't';	
			cenario[24][13] = 't';
			j = 2; 
			limite = 3;
			for(i = 0; i < 8; i++)
			{
				fazerParede(2, j, limite, cenario);
				j = limite+2;
				limite += 3;
			}
			j = 1; 
			limite = 2;
			for(i = 0; i < 8; i++)
			{
				fazerParede(4, j, limite, cenario);
				j = limite+2;
				limite += 3;
			}
			fazerParede(6, 0, 24, cenario);
			for(j = 1; j < 24; j++)   
			{
				if((j+2) % 2 == 1)		// j + 2 para nao dar erro na divisÃ£o por 2. numero par + 2 continua par. impar + 2 continua impar
				{							
					cenario[9][j] = 'p';
				}
				else 
					cenario[11][j] = 'p';					
			}
			fazerParede(14, 2, 22, cenario);
			for(j = 1; j < 24; j++)   
			{
				if((j+2) % 2 == 1)		// j + 2 para nao dar erro na divisÃ£o por 2. numero par + 2 continua par. impar + 2 continua impar
				{							
					cenario[19][j] = 'p';
				}
				else 
					cenario[17][j] = 'p';					
			}
	}		
}

// funcao que faz a animacao de derrota do jogo
void animacaoDerrota()	
{	
	const uchar tempo = 100;    
	printf("G");
	Sleep(tempo);		 
	printf("A");
	Sleep(tempo);		 
	printf("M"); 
	Sleep(tempo);		
	printf("E");	
	Sleep(tempo);	
	printf(" ");	
	Sleep(tempo);	
	printf("O"); 
	Sleep(tempo);		
	printf("V");
	Sleep(tempo);		
	printf("E");
	Sleep(tempo);	
	printf("R");
	Sleep(tempo);		
	printf(" !");
	Sleep(tempo);	
	printf("!");
	Sleep(tempo);	
	printf("!\n\n\t");	
}

// funcao que faz a animacao de vitoria do jogo
void animacaoVitoria(uchar fase)	
{
	const uchar tempo = 100;
	printf("F");
	Sleep(tempo);		 
	printf("A");
	Sleep(tempo);		 
	printf("S"); 
	Sleep(tempo);		
	printf("E");	
	Sleep(tempo);		
	printf(" %d ", fase);
	Sleep(tempo);		
	printf("C");
	Sleep(tempo);	
	printf("O");
	Sleep(tempo);		
	printf("M");
	Sleep(tempo);	
	printf("P");
	Sleep(tempo);	
	printf("L");	
	Sleep(tempo);
	printf("E");	
	Sleep(tempo);
	printf("T");	
	Sleep(tempo);
	printf("A");	
	Sleep(tempo);
	printf(" !");	
	Sleep(tempo);
	printf("!\n\n\t");	
	Sleep(tempo);	
}
