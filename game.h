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

#define PRETO "\033[30m"
#define AZUL "\033[34m"  
#define VERMELHO "\x1b[31m" //cores em ANSI utilizadas 
#define CINZA     "\e[0;37m"
#define BRANCO "\033[37m"
#define VERDE "\033[32m"
#define BRANCO "\033[37m"
#define FUNDOBRANCO "\033[47m"
#define FUNDOPRETO "\033[0;0m" 
#define FUNDOAZUL "\033[44m"
#define FUNDOVERDE "\033[1;42m"
#define AMARELO "\033[33m"
#define ROXO "\033[1;35m"

typedef unsigned char uchar;

struct agente
{
	int linha;
	int coluna;
	bool congelado;
	char tempoCongelado;
	char categoria[8];
};   

void apresentacao()
{
	system("color 0F");
	printf( BRANCO FUNDOAZUL"\n"
	"  ------     /\\      ----  |\\        /|    /\\    |\\    | "
	"\n |          /  \\    |    | | \\      / |   /  \\   | \\   | "
	"\n |         /----\\   |----  |  \\    /  |  /----\\  |  \\  | "
	"\n |        /      \\  |      |   \\  /   | /      \\ |   \\ | "
	"\n  ------ /        \\ |      |    \\/    |/        \\|    \\| \n");
	
	printf(" A game based in the popular Pacman developed for Atari  \n\n"CINZA FUNDOPRETO);
}
void writeLine(uchar tam)
{
	uchar i;
	for(i = 0; i < tam; i++)
		printf("%c",219);
	printf("\n");	
}

agente acaoJogador(char direcao, agente jogador, char cenario[25][25])	// mexe o jogador no cenario
{
	if(cenario[jogador.linha][jogador.coluna] == 't')	// teletransporte
	{
		if(jogador.linha < 5)
		{
			jogador.linha = 23;
			jogador.coluna = 12;
		}
		else 
		{
			jogador.linha = 0;
			jogador.coluna = 23;
		}
	}
	else
	{
		switch(direcao)
		{
			case 'W':
				direcao = 'w';
				break;	
			case 'w':
				direcao = 'w';
				break;	
			case 'A':
				direcao = 'a';
				break;		
			case 'a':
				direcao = 'a';
				break;	
			case 'D':
				direcao = 'd';
				break;	
			case 'd':
				direcao = 'd';
				break;	
			case 'S':
				direcao = 's';
				break;		
			case 's':
				direcao = 's';
				break;		
		}
		switch(direcao)
		{
			case 'w':
				if(jogador.linha > 0 && cenario[jogador.linha - 1][jogador.coluna] != 'p')		// o jogador só mexe se a próxima casa não houver paredes
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
	return jogador;	
}

agente resetarPosicoes(agente x, uchar rodada)		// metodo que reseta posicoes do jogador ou do inimigo
{
	switch(rodada)
	{
		case 1:
			if(!strcmp(x.categoria, "inimigo"))
			{
				x.linha = 16;
				x.coluna = 10;
			}
			else				// se for jogador 
			{
				x.linha = 3;
				x.coluna = 9;
			}
			break;
		case 2:
			if(!strcmp(x.categoria, "inimigo"))
			{
				x.linha = 20;
				x.coluna = 21;
			}
			else					// se for jogador 
			{
				x.linha = 0;
				x.coluna = 10;
			}
			break;
		case 3:
			if(!strcmp(x.categoria, "inimigo"))
			{
				x.linha = 11;
				x.coluna = 12;
			}
			else
			{
				x.coluna = 0;
				x.linha = 22;
			}
			break;
		case 4:
			//if(!strcmp(x.categoria, "inimigo"))
			//{
				
			//}
			//else
			break;
		case 5:
			//if(!strcmp(x.categoria, "inimigo"))
			//{
				
			//}
			//else
			break;	
	}	
	return x;
}

void mostrarJogo(char cenario[25][25], agente jogador, agente inimigo)		// mostra a tela de jogo 
{
	uchar i,j;
	printf("\n\n\t");
	writeLine(52);
	for(i = 0; i < 25; i++)
	{
		printf("\t%c",219);									// limite do cenário, parede do ínicio
		for(j = 0; j < 25; j++)
		{
			if(i == inimigo.linha && j == inimigo.coluna)	// mostrar inimigo 
				printf(VERMELHO "%c " CINZA,254);
			else if(i == jogador.linha && j == jogador.coluna)	// mostrar jogador 	
				printf("%c ",254);							
			else if(cenario[i][j] == '0')					// mostrar espaço vazio
				printf("  ");
			else if(cenario[i][j] == 'p')					// mostrar paredes
				printf("%c%c", 178,178);		
			else if(cenario[i][j] == 'c')					//mostrar pontos
				printf(AMARELO "* " CINZA);	
			else if(cenario[i][j] == 't')					//mostrar teletransporte
				printf(ROXO "%c%c" CINZA,178,178);	
		}
		printf("%c",219);							// limite do cenário, parede do fim
		printf("\n");
	}
	printf("\t");
	writeLine(52);
	printf("\n\t");						
}

bool wasTouched(agente jogador, agente inimigo)	// verifica se o jogador tocou no inimigo 
{
	bool touch = false; 
	if(jogador.linha == inimigo.linha && jogador.coluna == inimigo.coluna)
		touch = true;
	return touch;
}

void fazerParede(uchar linha, uchar inicioParede, uchar fimParede, char cenario[25][25])
{
	for(; inicioParede <= fimParede; inicioParede++)
		cenario[linha][inicioParede] = 'p';
}

void montarCenario(uchar rodada, char cenario[25][25], agente jogador)	// monta o cenario de jogo de acordo com a rodada  
{
	uchar i,j;
	uchar limite;					// usada para construir o cenario da rodada 2
	for(i = 0; i < 25; i++)			// zerar cenario para construir o novo 
		for(j = 0; j < 25; j++)
			cenario[i][j] = '0';
	switch(rodada)					// Definine localização das paredes
	{
		case 1:
			for(j = 2; j < 23; j+=2)
				cenario[4][j] = 'p';
					
			fazerParede(7,2,12,cenario);	
			fazerParede(10, 14, 23, cenario);
			fazerParede(13, 2, 12, cenario);		
			
			for(j = 4; j < 19; j+=4)
				cenario[15][j] = 'p';
			for(j = 1; j < 11; j+=1)
				cenario[18][j] = 'p';	
			for(j = 13; j < 24; j+=1)
				cenario[18][j] = 'p';		
			for(j = 2; j < 23; j+=3)
				cenario[21][j] = 'p';				
			break;	
		case 2:
			for(j = 1; j < 24; j++)   
			{
				if((j+2) % 2 == 1)		// j + 2 para nao dar erro na divisão por 2. numero par + 2 continua par. impar + 2 continua impar
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
			cenario[0][24] = 't';
			cenario[1][24] = 't';
			fazerParede(24, 0, 24, cenario);
			cenario[24][11] = 't';
			cenario[24][12] = 't';	
			cenario[24][13] = 't';
			j = 2; 
			limite = 4;
			for(i = 0; i < 8; i++)
			{
				for(; j < limite; j++)
					cenario[2][j] = 'p';
				j++;
				limite+=3;
			}
			j = 1; 
			limite = 3;
			for(i = 0; i < 8; i++)
			{
				for(; j < limite; j++)
					cenario[4][j] = 'p';
				j++;
				limite+=3;
			}		
			fazerParede(6, 0, 24, cenario);

			for(j = 1; j < 24; j++)   
			{
				if((j+2) % 2 == 1)		// j + 2 para nao dar erro na divisão por 2. numero par + 2 continua par. impar + 2 continua impar
				{							
					cenario[16][j] = 'p';
					cenario[20][j] = 'p';
				}
				else 
					cenario[18][j] = 'p';					
			}
			fazerParede(14, 2, 23, cenario);
			for(j = 1; j < 24; j++)   
			{
				if((j+2) % 2 == 1)		// j + 2 para nao dar erro na divisão por 2. numero par + 2 continua par. impar + 2 continua impar
				{							
					cenario[8][j] = 'p';
				}
				else 
					cenario[10][j] = 'p';					
			}
			break;	
		case 4:
		
			break;	
		case 5:
		
			break;	
		default:
		
			break;
	}		
}

int gerarPontos(char cenario[25][25], uchar rodada)		// gera os pontos da rodada 
{
	uchar qtdPontosRodada;
	uchar i,j;
	uchar qtdPontos = 0;
	srand(time(NULL));
	switch(rodada)
	{
		case 1:
			qtdPontosRodada = 20;
			break;
		case 2:
			qtdPontosRodada = 25;
			break;
		case 3:
			qtdPontosRodada = 30;
			break;
		case 4:
			qtdPontosRodada = 35;
			break;
		case 5:
			qtdPontosRodada = 40;
			break;	
	}
	
	while(qtdPontos != qtdPontosRodada) // se a quantidade de pontos geradas nao for igual a quantidade da rodada, os pontos tem que ser gerados novamente. 
	{
		qtdPontos = 0;
		for(i = 0; i < 25; i++)
			for(j = 0; j < 25; j++)
			{
				if(cenario[i][j] == 'c')
					cenario[i][j] = '0';
				if(rand() % 25 == 0 && cenario[i][j] == '0')
				{	
					cenario[i][j] = 'c';
					qtdPontos++;
				}		
			}
				
	}	
	return qtdPontos;		
}

void animacaoDerrota()	
{
	uchar tempo = 100;
	printf("\a");    
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

void animacaoVitoria(uchar rodada)
{
	uchar tempo = 100;
	printf("\a");    
	printf("R");
	Sleep(tempo);		 
	printf("O");
	Sleep(tempo);		 
	printf("D"); 
	Sleep(tempo);		
	printf("A");	
	Sleep(tempo);	
	printf("D");	
	Sleep(tempo);	
	printf("A"); 
	Sleep(tempo);		
	printf(" %d ", rodada);
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

