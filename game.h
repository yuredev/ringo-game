#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#endif

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

//cores em ANSI utilizadas 
#define CIANO "\033[36m"
#define PRETO "\033[30m"
#define AZUL "\033[34m"  
#define VERMELHO "\x1b[31m" 
#define CINZA     "\e[0;37m"
#define BRANCO "\033[37m"
#define VERDE "\033[32m"
#define BRANCO "\033[37m"
#define FUNDOPRETO "\033[0;0m" 
#define FUNDOAZUL "\033[44m"
#define AMARELO "\033[33m"
#define ROXO "\033[1;35m"
#define FUNDOBRANCO "\033[47m"

typedef unsigned char uchar;

struct agente		// estrutura do jogador e inimigo 
{
	int linha;
	int coluna;
	char categoria[8];
};   

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
		case 'S':
			if(op < 3)
				op++;
			break;
		case 's':
			if(op < 3)
				op++;
	}
	return op;		
}

void apresentacao()		// sÃ³ estÃ©tica
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
void writeLine(uchar tam)		// escreve uma linha
{
	uchar i;
	for(i = 0; i < tam; i++)
		printf("%c",219);
	printf("\n");	
}
void moverCursor(int linha, int coluna, bool aPartirDaMatriz)	// move o cursor
{
	if(aPartirDaMatriz)
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){coluna * 2 + 9, linha + 4});
	else
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){coluna - 1, linha -1});	
}

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

agente acaoJogador(char direcao, agente jogador, char cenario[25][25], bool *gelo)	// mexe o jogador no cenario
{
	moverCursor(jogador.linha, jogador.coluna, true);
	printf(" ");	
	if(cenario[jogador.linha][jogador.coluna] == 't')	// teletransporte
		jogador = teletransporte(jogador, cenario);
		
	else			// se não for teleteransportar a movimentação ocorre normalmente 
	{
		switch(direcao)	// apenas para aceitar caps lock
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
		}
		switch(direcao)
		{
			case 'w':
				if(jogador.linha > 0 && cenario[jogador.linha - 1][jogador.coluna] != 'p') // o jogador sÃ³ mexe se a prÃ³xima casa nÃ£o houver paredes. isso se aplica aos outros cases
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
	
	moverCursor(jogador.linha, jogador.coluna, true);
	printf("%c",254);
	moverCursor(25, 0,true);
	return jogador;	
}

// função que mexe o inimigo no cenario 
agente acaoInimigo(agente inimigo, agente jogador, char cenario[25][25], uchar fase)
{
	char direcao;
	
	moverCursor(inimigo.linha, inimigo.coluna, true);
	printf("  ");	
	
	// verificação parar desvio de paredes, a primeira parte é apenas para na fase 3 o jogador não desviar da parede que preenche a tela inteira.
	if(!(fase == 3 && (inimigo.linha == 6 || inimigo.linha)) && !strcmp(inimigo.categoria,"inimigo") && ((inimigo.linha < jogador.linha && cenario[inimigo.linha + 1][inimigo.coluna] == 'p') || (inimigo.linha > jogador.linha && cenario[inimigo.linha - 1][inimigo.coluna] == 'p')))
	{
		if(jogador.coluna > 12)
			direcao = 'd';
		else
			direcao = 'a';
	}	
	else 
	if(inimigo.linha == jogador.linha)	// se o inimigo tiver na mesma linha do jogador ele move-se em linha reta 
	{
		if(inimigo.coluna < jogador.coluna)
			direcao = 'd';
		else 
			direcao = 'a';	
	}		 
	else if(inimigo.coluna == jogador.coluna)    // se o inimigo tiver na mesma coluna do jogador ele move-se em linha reta 
	{
		if(inimigo.linha < jogador.linha)
			direcao = 's';
		else
			direcao = 'w';	
	}
	else if(rand() % 2 == 0)      // 50% de chance do inimigo mover-se no eixo horizontal 
	{		
		if(inimigo.coluna < jogador.coluna)
			direcao = 'd';
		else 
			direcao = 'a';	
	}
	else					// 50% de chance do inimigo mover-se no eixo vertical 
	{
		if(inimigo.linha < jogador.linha)
			direcao = 's';
		else
			direcao = 'w';	
	}
		
	switch(direcao)
	{
		case 'w':
			if(cenario[inimigo.linha-1][inimigo.coluna] != 'p')  // não ultrapassar paredes
				inimigo.linha--; 
			break;
		case 's':
			if(cenario[inimigo.linha+1][inimigo.coluna] != 'p') // " "
				inimigo.linha++;
			break;
		case 'a':
			if(cenario[inimigo.linha][inimigo.coluna-1] != 'p')  // " "
				inimigo.coluna--;
			break;
		case 'd':
			if(cenario[inimigo.linha][inimigo.coluna+1] != 'p') // " "
				inimigo.coluna++;
			break;			
	}
	if(cenario[inimigo.linha][inimigo.coluna] == 'g')       // se o inimigo passar por cima do gelo, o gelo desaparece 
		cenario[inimigo.linha][inimigo.coluna] = '0';
	
	moverCursor(inimigo.linha, inimigo.coluna, true);
	printf(VERMELHO"%c" CINZA, 254);
	moverCursor(25, 0,true);
	return inimigo;
}

int gerarAneis(char cenario[25][25], uchar fase)		// gera os Aneis da fase 
{
	uchar qtdAneisFase;
	uchar l,c;
	srand(time(NULL));
	switch(fase)						// definir quantas Aneis serÃ£o geradas de acordo com a fase
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
	
	for(int i = 0; i <= qtdAneisFase; i++)
	{
		do
		{
			l = rand() % 25;
			c = rand() % 25;
			if(cenario[l][c] == '0')
			{
				cenario[l][c] = 'a';
				break;
			}
		}while(cenario[l][c] != '0');
	}
	return qtdAneisFase;		
}

agente resetarPosicoes(agente x, uchar fase)	// função que reseta as posicoes do agente de acordo com a fase 
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

void mostrarJogo(char cenario[25][25], agente jogador, agente inimigo, agente inimigo2)		// mostra a tela de jogo 
{
	uchar i,j;
	printf("\n\n\t");
	writeLine(52);
	for(i = 0; i < 25; i++)
	{
		printf("\t%c",219);									// limite do cenÃ¡rio, parede do Ã­nicio
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
		printf("%c",219);							// limite do cenÃ¡rio, parede do fim
		printf("\n");
	}
	printf("\t");
	writeLine(52);
	printf("\n\t");						
}

bool wasTouched(agente jogador, agente inimigo, agente inimigo2)    // verifica se o jogador tocou no inimigo 
{
	bool touch = false; 
	if((jogador.linha == inimigo.linha && jogador.coluna == inimigo.coluna) || (jogador.linha == inimigo2.linha && jogador.coluna == inimigo2.coluna))
		touch = true;
	else if((jogador.linha-1 == inimigo.linha && jogador.coluna == inimigo.coluna) || (jogador.linha-1 == inimigo2.linha && jogador.coluna == inimigo2.coluna))
		touch = true;
	else if((jogador.linha+1 == inimigo.linha && jogador.coluna == inimigo.coluna) || (jogador.linha+1 == inimigo2.linha && jogador.coluna == inimigo2.coluna))
		touch = true;
	else if((jogador.linha == inimigo.linha && jogador.coluna-1 == inimigo.coluna) || (jogador.linha == inimigo2.linha && jogador.coluna-1 == inimigo2.coluna))
		touch = true;
	else if((jogador.linha == inimigo.linha && jogador.coluna+1 == inimigo.coluna) || (jogador.linha == inimigo2.linha && jogador.coluna+1 == inimigo2.coluna))	
		touch = true;
	return touch;
}

void fazerParede(uchar linha, uchar inicioParede, uchar fimParede, char cenario[25][25])	// constroi uma parede horizontal no cenario do jogo 
{
	for(; inicioParede <= fimParede; inicioParede++)
		cenario[linha][inicioParede] = 'p';
}

void montarCenario(uchar fase, char cenario[25][25], agente jogador)	// monta o cenario de jogo de acordo com a fase  
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

void mostrarMenu()
{
	uchar i;
	for(i = 10; i <= 40; i++)
	{
		moverCursor(25,i,false);	
		printf("%c",219);
	}
}

void animacaoDerrota()	// apenas para estÃ©tica
{	
	uchar tempo = 100;    
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

void animacaoVitoria(uchar fase)	// apenas para estÃ©tica
{
	uchar tempo = 100;
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
