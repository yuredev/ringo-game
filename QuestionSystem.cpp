#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"
#include "D:\Documentos\libpq\include\pgsql\libpq-fe.h"

struct questao
{
	char texto[300];
	char resposta[300];
	char tema[50];
	char dominio[50];	
	int dificulade;
	int id;
}questoes;

unsigned char menu(char menu[40]);
void gotoxy(int x, int y);			
void menuBusca(unsigned char op);
void menuTemaDominio(unsigned char op);
void menuPrincipal(unsigned char op);
void cadastrarQuestao(PGconn *conn);
void cadastrarDominio(PGconn *conn);
void cadastrarTema(PGconn *conn);
void mostrarOpcoes(unsigned char op);
void telaCadastro(PGconn *conn);
void inputDificuldade(char * string);

static void exit_nicely(PGconn *conn);

int main()
{
	// parte de conexao com banco
	const char *conninfo = "host=localhost dbname=QuestionSystem user=postgres password=linuxyu";
	PGconn *conn;
	PGresult *res;
	int nFields;
    int i;
	
	/* Faz conexao com o banco */
    conn = PQconnectdb(conninfo);
    
    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        exit_nicely(conn);
    }
	    
	setlocale(LC_ALL, "Portuguese");	// deixar com acentuacao eh bom neh
	system("color 0F");
	char varBusca[100];		// variavel a ser usada nas consultas para guardar a palavra, tema, ou seja la o que for
	char queryBusca[400];	// string para busca ira guardar o SELECT FROM BLABLABLA...
	unsigned char op;
	do
	{
		op = menu("principal");	 		// apresenta o menu principal e permite a escolha do usuario  
		switch(op)
		{
			case 1:
				telaCadastro(conn);	           // mostrar a tela de cadastro 
				break;
			case 2:
				while(1)
				{
					// Variavel controladora do switch de busca, (opBusca  ---> opcao do menu busca 
					system("cls");
					op = menu("busca");	// apresenta o menu de busca, e permite a escolha do usuario   
					if(op == 6)	// se a opcao for 6 ja sai do switch e volta ao menu anterior 
					{
						op = 1;
						break;
					}
					system("cls");
					switch(op)
					{
						case 1:
							printf("\tDigite a palavra a ser buscada: ");
							strcpy(queryBusca,"SELECT * FROM QUESTAO WHERE texto LIKE '%");
							break;
						case 2:
							printf("\tDigite o tema a ser buscado: ");
							strcpy(queryBusca,"SELECT * FROM QUESTAO WHERE tema = '");
							break;
						case 3:
							printf("\tDigite o dominio a ser buscado: ");
							strcpy(queryBusca,"SELECT * FROM QUESTAO WHERE dominio = '");
							break;
						case 4:
							printf("\tDigite a dificuldade a ser buscada: ");
							
							// pra nao criar outra variavel usei a posicao 0 do vetor varBusca
							// lembre-se que o tipo char tambem suporta numeros inteiros 
							scanf("%d",&varBusca[0]);							
							sprintf(queryBusca,"SELECT * FROM QUESTAO WHERE dificuldade = %d",varBusca[0]);
							break;
						case 5:
							strcpy(queryBusca,"SELECT * FROM QUESTAO");
						
						//nao precisa de case 6, pois se for 6 ja sai do laco e volta ao menu anterior 
					}
					if(op < 4)	// se não for busca por dificuldade entao coloca-se as aspas simples '' 
					{
						setbuf(stdin,NULL);
						gets(varBusca);					
						strcat(queryBusca, varBusca);
						if(op == 1)
							strcat(queryBusca, "%'");
						else	
							strcat(queryBusca, "'");
					}
					system("cls");
					// parte de busca abaixo, vai funcionar de acordo com o que o usuario escolheu 
					res = PQexec(conn,queryBusca);
					switch (PQresultStatus(res)) 
					{
						case PGRES_COMMAND_OK: printf("ok\n"); break;
						case PGRES_EMPTY_QUERY: printf("empty"); break;
						case PGRES_TUPLES_OK: 
							nFields = PQnfields(res);
							
							switch(op)
							{
								case 4: printf(CIANO"\t%d Questões com dificuldade %d\n\n"CINZA,PQntuples(res),varBusca[0]); break;
								case 5: printf(CIANO"\t%d questões no banco de dados\n\n",PQntuples(res)); break;
								default: printf(CIANO"\t%d Questões encontradas com '%s'\n\n"CINZA,PQntuples(res),varBusca);
							}
						    for (i = 0; i < PQntuples(res); i++)
						    {
						    	// barra de separacao que mostra tbm o id_questao
						    	printf(CIANO"\t=========== Questão %s ===========\n\n"CINZA,PQgetvalue(res, i, 0)); 
						    	
						        printf(AMARELO"\tEnunciado:\n\t"CINZA);
						        printf("%s",PQgetvalue(res, i, 1));
						        
						        printf(AMARELO "\n\tResposta:\n\t" CINZA);
						        printf("%s",PQgetvalue(res, i, 2));
						        
						        printf(AMARELO "\n\tDomínio:\n\t" CINZA);
						        printf("%s",PQgetvalue(res, i, 3));
						        
						        printf(AMARELO"\n\tTema:\n\t"CINZA);
						        printf("%s",PQgetvalue(res, i, 4));
						        
						        printf(AMARELO"\n\tDificuldade: "CINZA);
						        printf("%s\n\n",PQgetvalue(res, i, 5));
						    }	
							break;
						case PGRES_BAD_RESPONSE: printf("error: bad response"); break;
						case PGRES_NONFATAL_ERROR: 
						case PGRES_FATAL_ERROR: printf(PQresultErrorMessage(res)); break;
						default: printf("Algo inesperado");
					}
					printf(CIANO"\t");
					system("pause");		
					printf(CINZA);	
				    PQclear(res);				
				}
				break;
			case 3:
				while(1)
				{
					system("cls");
					op = menu("TemaDominio");	// apresenta o menu de busca, e permite a escolha do usuario   
					if(op == 4)	// se a opcao for 6 ja sai do switch e volta ao menu anterior 
					{
						op = 1;
						break;
					}
					system("cls");
					switch(op)
					{
						case 1:	strcpy(queryBusca, "SELECT * FROM DOMINIO"); break;
						case 2: strcpy(queryBusca, "SELECT * FROM TEMA"); break;
						case 3: 
							strcpy(queryBusca, "SELECT nome_tema FROM TEMA WHERE dominio = '");
							setbuf(stdin,NULL);
							printf("\tInsira o domínio a ser acessado: ");
							gets(varBusca);
							strcat(queryBusca,varBusca);
							strcat(queryBusca,"'");
							break;		
					}
					system("cls");
					res = PQexec(conn,queryBusca);
					switch (PQresultStatus(res)) 
					{
						case PGRES_COMMAND_OK: printf("ok\n"); break;
						case PGRES_EMPTY_QUERY: printf("empty"); break;
						case PGRES_TUPLES_OK: 
							nFields = PQnfields(res);
							switch(op)
							{
								case 1: printf(CIANO"\t%d Domínios cadastrados\n\n"CINZA,PQntuples(res)); break;
								case 2: printf(CIANO"\t%d Temas cadastrados\n\n"CINZA,PQntuples(res)); break;
								case 3: printf(CIANO"\t%d Temas no domínio '%s'\n\n"CINZA,PQntuples(res),varBusca);
							}
						    for (i = 0; i < PQntuples(res); i++)
						        printf("\t%s\n",PQgetvalue(res, i, 0)); 
							break;
						case PGRES_BAD_RESPONSE: printf("error: bad response"); break;
						case PGRES_NONFATAL_ERROR: 
						case PGRES_FATAL_ERROR: printf(PQresultErrorMessage(res)); break;
						default: printf("Algo inesperado");
					}
					printf("\n\t"CIANO);  						      
				    setbuf(stdin,NULL);
				    system("pause");
				    printf(CINZA);
				}
				break;	
			case 4:
				printf("\n\tQuer mesmo apagar todos os registros? (S/N): ");
				do
				{
					varBusca[0] = getch();    // aproveitamento de variaveis 					
					if(varBusca[0] == 'S' || varBusca[0] == 's')
					{
						PQexec(conn, "DELETE FROM QUESTAO");	
						PQexec(conn, "DELETE FROM TEMA");
						PQexec(conn, "DELETE FROM DOMINIO");							
					}		
				}while(varBusca[0] != 'S' && varBusca[0] != 'N' && varBusca[0] != 's' && varBusca[0] != 'n');
				
				break;
			case 5:
				system("cls");
				printf("Programa finalizado");			
		}
	}while(op != 5);
	return 0;
}

// funcao que mostra as opcoes do menu principal
void menuPrincipal(unsigned char op)
{
	printf(CIANO"\t========= SISTEMA DE QUESTÕES ========="CINZA);
	printf("%s",(op == 1) ? PRETO FUNDOBRANCO "\n\n\t Cadastrar Questões "CINZA FUNDOPRETO" <==" : "\n\n\t Cadastrar Questões");
	printf("%s",(op == 2) ? PRETO FUNDOBRANCO "\n\n\t Buscar Questões "CINZA FUNDOPRETO" <==" : "\n\n\t Buscar Questões");
	printf("%s",(op == 3) ? PRETO FUNDOBRANCO "\n\n\t Domínios e Temas "CINZA FUNDOPRETO" <==" : "\n\n\t Domínios e Temas");
	printf("%s",(op == 4) ? PRETO FUNDOBRANCO "\n\n\t Apagar Todos os Registros "CINZA FUNDOPRETO" <==" : "\n\n\t Apagar Todos os Registros");
	printf("%s",(op == 5) ? PRETO FUNDOBRANCO "\n\n\t Sair "CINZA FUNDOPRETO " <==\n\n" : "\n\n\t Sair\n\n");
	printf(CIANO"\t=======================================\n"CINZA);
}
// funcao que mostra as opcoes do menu de busca
void menuBusca(unsigned char op)
{
	printf(CIANO"\t=========== BUSCA DE QUESTÕES ==========="CINZA);
	printf("%s",(op == 1) ? PRETO FUNDOBRANCO"\n\n\t Buscar por palavra "CINZA FUNDOPRETO" <==" : "\n\n\t Buscar por palavra");
	printf("%s",(op == 2) ? PRETO FUNDOBRANCO"\n\n\t Buscar por tema "CINZA FUNDOPRETO" <==" : "\n\n\t Buscar por tema");
	printf("%s",(op == 3) ? PRETO FUNDOBRANCO"\n\n\t Buscar por domínio "CINZA FUNDOPRETO " <==" : "\n\n\t Buscar por domínio");
	printf("%s",(op == 4) ? PRETO FUNDOBRANCO"\n\n\t Buscar por dificuldade "CINZA FUNDOPRETO" <==" : "\n\n\t Buscar por dificuldade");
	printf("%s",(op == 5) ? PRETO FUNDOBRANCO"\n\n\t Mostrar todas as questões "CINZA FUNDOPRETO" <==" : "\n\n\t Mostrar todas as questões");
	printf("%s",(op == 6) ? PRETO FUNDOBRANCO"\n\n\t Voltar "CINZA FUNDOPRETO" <==" : "\n\n\t Voltar");
 	printf(CIANO"\n\n\t========================================\n"CINZA);
}

// funcao que mostra as opcoes do menu de dominio
void menuTemaDominio(unsigned char op)
{
	printf(CIANO"\t=========== DOMÍNIOS E TEMAS =========="CINZA);
	printf("%s",(op == 1) ? PRETO FUNDOBRANCO "\n\n\t Listar todos os domínios "CINZA FUNDOPRETO" <==" : "\n\n\t Listar todos os domínios");
	printf("%s",(op == 2) ? PRETO FUNDOBRANCO "\n\n\t Listar todos os temas "CINZA FUNDOPRETO" <==" : "\n\n\t Listar todos os temas");
	printf("%s",(op == 3) ? PRETO FUNDOBRANCO "\n\n\t Listar temas de um dominio "CINZA FUNDOPRETO" <==" : "\n\n\t Listar temas de um dominio");
	printf("%s",(op == 4) ? PRETO FUNDOBRANCO "\n\n\t Voltar "CINZA FUNDOPRETO " <==\n\n" : "\n\n\t Voltar\n\n");
	printf(CIANO"\t=======================================\n"CINZA);
}

// funcao que faz o controle do menu com as setas 
unsigned char menu(char* menu)
{
	unsigned char limite;
	unsigned char op = 1;
	char tecla;
	do
	{
		system("cls");
		if(strcmp("principal",menu) == 0)
		{
			menuPrincipal(op);
			limite = 5;
		}
		else if(strcmp("busca",menu) == 0)
		{
			menuBusca(op);
			limite = 6;
		}
		else	// menu TemaDominio
		{
			menuTemaDominio(op);
			limite = 4;
		}
		while(1)
			if(kbhit())
			{
				tecla = getch();
				switch(tecla)
				{
					case 'w':
						if(op > 1)
							op--;					
						break;
					case 72:			// valor ASCII da seta para cima
						if(op > 1)
							op--;					
						break;	
					case 'W':			// aceita CAPS LOCK tambem 
						if(op > 1)
							op--;					
						break;	
					case 's':
						if(op < limite)
							op++;
						break;	
					case 80:			// valor ASCII da seta para baixo
						if(op < limite)
							op++;	
						break;	
					case 'S':			// aceita CAPS LOCK tambem 
						if(op < limite)
							op++;			
				}
				break;	// sai do laço se o usuario teclar algo, isto para mostrar o menu com as opcoes ja alteradas
			}
	}while(tecla != 13); // 13 eh o enter da tabela ascii
	return op;
}

// funcao da tela de cadastro, onde o usuario digita todas as entradas 
void telaCadastro(PGconn *conn)
{	
	system("cls");
	gotoxy(5,22);
	printf(CIANO"\t==============================="CINZA);
	gotoxy(5,1);
	printf(CIANO"\t===== CADASTRO DE QUESTÕES ====="CINZA);
	printf(AMARELO"\n\n\tEscreva a questão:\n\n\t"CINZA);
	setbuf(stdin,NULL);
	gets(questoes.texto);
	printf(AMARELO"\n\tEscreva a resposta da questão:\n\n\t"CINZA);
	gets(questoes.resposta);
	printf(AMARELO"\n\tInsira o dominio da questão:\n\n\t"CINZA);
	gets(questoes.dominio);
	printf(AMARELO"\n\tInsira o tema da questão:\n\n\t"CINZA);
	gets(questoes.tema);
	inputDificuldade(AMARELO"\n\tInsira a dificuldade da questão (de 1 a 3):\n\n\t"CINZA);
	cadastrarDominio(conn);
	cadastrarTema(conn);
	cadastrarQuestao(conn);
    printf("\n\t");
	Sleep(1700);	
}

// prepara a query e insire o tema anteriormente digitado no banco de dados, 
// esta funcao eh chamada dentro da funcao telaCadastro.
void cadastrarTema(PGconn *conn)
{
	char query[500];
	sprintf(query, "INSERT INTO TEMA VALUES('%s','%s');",questoes.tema,questoes.dominio);
	PGresult *res = PQexec(conn, query);
	/* handle the response */	
	switch (PQresultStatus(res)) 
	{
		case PGRES_EMPTY_QUERY: 
		case PGRES_COMMAND_OK: break;
		case PGRES_TUPLES_OK:  break;
		case PGRES_BAD_RESPONSE: printf(VERMELHO"\t[ERROR] Bad Response\n"CINZA); system("pause"); break;
		case PGRES_NONFATAL_ERROR: 
		case PGRES_FATAL_ERROR: break;
		default: printf(VERMELHO"\tUnexpected Response\n"CINZA); system("pause");
	}	
    PQclear(res);
}

// prepara a query e insire o dominio anteriormente digitado no banco de dados, 
// esta funcao eh chamada dentro da funcao telaCadastro.
void cadastrarDominio(PGconn *conn)
{
	char query[500];
	sprintf(query, "INSERT INTO DOMINIO VALUES('%s');",questoes.dominio);
	PGresult *res = PQexec(conn, query);
	/* handle the response */	
	switch (PQresultStatus(res)) 
	{
		case PGRES_EMPTY_QUERY: 
		case PGRES_COMMAND_OK: break;
		case PGRES_TUPLES_OK:  break;
		case PGRES_BAD_RESPONSE: printf(VERMELHO"\t[ERROR] Bad Response\n"CINZA); system("pause"); break;
		case PGRES_NONFATAL_ERROR: 
		case PGRES_FATAL_ERROR: break;
		default: printf(VERMELHO"\tUnexpected Response\n"CINZA); system("pause");
	}	
    PQclear(res);
}

// prepara a query e insire a questao no banco de dados, 
// esta funcao eh chamada dentro da funcao telaCadastro.
void cadastrarQuestao(PGconn *conn)
{ 
	char query[500];
	
	sprintf(query,"INSERT INTO QUESTAO VALUES(DEFAULT,'%s','%s','%s','%s',%d);",
	questoes.texto, questoes.resposta, questoes.dominio, questoes.tema, questoes.dificulade);
	PGresult *res = PQexec(conn, query);
		/* handle the response */
	switch (PQresultStatus(res)) 
	{
		case PGRES_EMPTY_QUERY: 
		case PGRES_COMMAND_OK: printf(CIANO"\n\n\tCadastro realizado com sucesso...\n"CINZA); break;
		case PGRES_TUPLES_OK:  break;
		case PGRES_BAD_RESPONSE: printf(VERMELHO"\t[ERROR] Bad Response\n"CINZA); system("pause"); break;
		case PGRES_NONFATAL_ERROR: 
		case PGRES_FATAL_ERROR: printf(PQresultErrorMessage(res)); break;
		default: printf(VERMELHO"\tUnexpected Response\n"CINZA); system("pause");
	}	
    PQclear(res);
}

//	funcao que mexe o cursor do terminal para as cordenadas x,y  
void gotoxy(int x, int y)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x-1,y-1});
}

static void exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    system("pause");
    exit(1);
}

void inputDificuldade(char * string)
{
	do
	{
		setbuf(stdin,NULL);
		printf("%s",string);
		scanf("%d",&questoes.dificulade);
		gotoxy(8,23);
		printf("                                                      ");
		if(!(questoes.dificulade >= 1 && questoes.dificulade <= 3))
		{
			gotoxy(2,21);
			printf("                                                  ");
			gotoxy(9,23);
			printf("Entrada invalida!!");
			gotoxy(8,18);
		}
	}while(!(questoes.dificulade >= 1 && questoes.dificulade <= 3));
}
