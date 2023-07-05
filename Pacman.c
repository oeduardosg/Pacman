#include <stdio.h>
#include <stdlib.h>

#define MAX_DIR 1000

//Modularizações referentes ao mapa
#define VAZIO ' '
#define PAREDE '#'
#define COMIDA '*'
#define PACMAN '>'
#define FANTASMA_B 'B'
#define FANTASMA_P 'P'
#define FANTASMA_I 'I'
#define FANTASMA_C 'C'
#define MAPA_MAX_L 40
#define MAPA_MAX_C 100

//TipoCoordenada
typedef struct {
    int x;
    int y;
} tCoordenada;

//Tipo Pacman
typedef struct {
    tCoordenada coordenada;
    int pontuacao;
} tPacman;

//Tipo Fantasma
typedef struct {
    int existe;
    tCoordenada coordenada;
    char tipo;
} tFantasma;

//Tipo Fantasmas
typedef struct {
    tFantasma B;
    tFantasma P;
    tFantasma I;
    tFantasma C;
} tFantasmas;

//Tipo Jogo
typedef struct {
    int linhas;
    int colunas;
    int limiteDeJogadas;
    tPacman pacman;
    char mapa[MAPA_MAX_L][MAPA_MAX_C];
} tJogo;

//!!! FIM DOS TIPOS !!!

tCoordenada GetXYPacman(tPacman pacman) {
return pacman.coordenada;
}

int GetX(tCoordenada coordenada) {
return coordenada.x;
}

int GetY(tCoordenada coordenada) {
return coordenada.y;
}

//Essa função substitui no mapa a localização do Pacman e dos fantasmas;
void PreencheMapa(int linha, int coluna, char mapaPreenchido[linha][coluna], tJogo jogo) {
    int i, j, xPacman, yPacman;
    tCoordenada xyPacman;
    for(i = 0; i < linha; i++) {
        for(j = 0; j < coluna; j++) {
            mapaPreenchido[i][j] = jogo.mapa[i][j];
        }
    }
    xyPacman = GetXYPacman(jogo.pacman);
    xPacman = GetX(xyPacman);
    yPacman = GetY(xyPacman);
    if(xPacman != -1 && yPacman != -1) mapaPreenchido[xPacman][yPacman] = PACMAN;
}

//Função para imprimir o mapa
void ImprimeJogo(tJogo jogo) {
    int i, j;
    char mapaPreenchido[jogo.linhas][jogo.colunas];
    PreencheMapa(jogo.linhas, jogo.colunas, mapaPreenchido, jogo);      //O mapa recebe aqui as coordenadas do Pacman e dos fantasmas;
    for(i = 0; i < jogo.linhas; i++) {
        for(j = 0; j < jogo.colunas; j++) {
            printf("%c", mapaPreenchido[i][j]);
        }
        printf("\n");
    }
}

//Função para encontrar coordenadas de um caractere qualquer no mapa
tCoordenada EncontraCoordenada(int linha, int coluna, char mapa[linha][coluna], char procurado) {
    tCoordenada coordenada;
    int i, j;
    for(i = 0; i < linha; i++) {
        for(j = 0; j < coluna; j++) {
            if(mapa[i][j] == procurado) {
                coordenada.x = i;
                coordenada.y = j;
                return coordenada;
            }
        }
    }
}

//Função pra printar a posição inicial do Pacman
void PrintaInicioPacman(tPacman pacman) {
    printf("Pac-Man comecara o jogo na linha %d e coluna %d\n", pacman.coordenada.x+1, pacman.coordenada.y+1);
}

tJogo LeJogoCompleto(int linha, int coluna, char mapaGeral[linha][coluna], tJogo jogo) {
    int i, j;

    tPacman pacman;
    pacman.coordenada = EncontraCoordenada(linha, coluna, mapaGeral, PACMAN);
    pacman.pontuacao = 0;
    jogo.pacman = pacman;



    for(i = 0; i < linha; i++) {
        for(j = 0; j < coluna; j++) {
            if(mapaGeral[i][j] != PAREDE && mapaGeral[i][j] != COMIDA) {
                jogo.mapa[i][j] = VAZIO;
            }
            else {
                jogo.mapa[i][j] = mapaGeral[i][j];
            }
        }
    }
    return jogo;
}

//Função que retorna o jogo lido, inicializa o mapa e gera o arquivo inicializacao.txt
tJogo InicializaJogo(char * diretorioGeral) {
    tJogo jogo;
    tCoordenada coordenadaPacman;
    char diretorioDoJogo[MAX_DIR];
    FILE * arqJogo;
    int i, j;

    sprintf(diretorioDoJogo, "%s/mapa.txt", diretorioGeral);
    arqJogo = fopen(diretorioDoJogo, "r");

    if(!arqJogo) {
        printf("ERRO: Nao foi possivel localizar o arquivo mapa.txt em %s", diretorioDoJogo);                 //ESSA MENSAGEM NÃO ESTA CORRETA!
        exit(1);
    }
    fscanf(arqJogo, "%d", &jogo.linhas);
    fscanf(arqJogo, "%d", &jogo.colunas);
    fscanf(arqJogo, "%d", &jogo.limiteDeJogadas);
    char mapaGeral[jogo.linhas][jogo.colunas];
    for(i = 0; i < jogo.linhas; i++) {
        fscanf(arqJogo, "\n");
        for(j = 0; j < jogo.colunas; j++) {
            fscanf(arqJogo, "%c", &mapaGeral[i][j]);
        }
    }
    jogo = LeJogoCompleto(jogo.linhas, jogo.colunas, mapaGeral, jogo);        //Aqui eu analiso o que é mapa(PAREDES e COMIDAS) e o que é Pacman/fantasmas;
    ImprimeJogo(jogo);       
    PrintaInicioPacman(jogo.pacman);
    return jogo;
}



//GRUPO DE FUNCOES VOLTADAS PARA AS JOGADAS
tPacman AtualizaCoordenadaPacman(int x, int y, tPacman pacman) {
    pacman.coordenada.x = x;
    pacman.coordenada.y = y;
return pacman;
}

int AchaComidas(int linha, int coluna, char mapa[linha][coluna]) {
    int i, j, comidas = 0;
    for(i = 0; i < linha; i++) {
        for(j = 0; j < coluna; j++) {
            if(mapa[i][j] == COMIDA) comidas++;
        }
    }
return comidas;
}


//Função que deve realizar as jogadas até que todas as comidas sejam consumidas ou o jogador perca
void RealizaJogadas(tJogo jogo) {
    int jogadas = 0, pontuacao = 0, comidas = 0;
    char jogada, lixo;

    int i, j;
    for(i = 0; i < jogo.linhas; i++) {
        for(j = 0; j < jogo.colunas; j++) {
            if(jogo.mapa[i][j] == COMIDA) comidas++;
        }
    }

    tCoordenada xyPacman;
    int xPacman, yPacman;

    while(comidas != pontuacao && jogadas <= jogo.limiteDeJogadas) {
        scanf("%c%*c", &jogada);
        //MOVER FANTASMAS ANTES DO PACMAN
        xyPacman = GetXYPacman(jogo.pacman);
        xPacman = GetX(xyPacman);
        yPacman = GetY(xyPacman);

        switch (jogada) {
            case 'w':
                if(jogo.mapa[xPacman-1][yPacman] != PAREDE) {
                    if(jogo.mapa[xPacman-1][yPacman] == COMIDA) {
                        pontuacao++;
                        jogo.mapa[xPacman-1][yPacman] = VAZIO;
                    }
                    jogo.pacman = AtualizaCoordenadaPacman(xPacman-1, yPacman, jogo.pacman);
                }
                else {
                    jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                    break;
                }
                break;
            
            case 'a':
                if(jogo.mapa[xPacman][yPacman-1] != PAREDE) {
                    if(jogo.mapa[xPacman][yPacman-1] == COMIDA) {
                        pontuacao++;
                        jogo.mapa[xPacman][yPacman-1] = VAZIO;
                    }
                    jogo.pacman = AtualizaCoordenadaPacman(xPacman, yPacman-1, jogo.pacman);
                }
                else {
                    jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                    break;
                }
                break;

            case 's':
                if(jogo.mapa[xPacman+1][yPacman] != PAREDE) {
                    if(jogo.mapa[xPacman+1][yPacman] == COMIDA) {
                        pontuacao++;
                        jogo.mapa[xPacman+1][yPacman] = VAZIO;
                    }
                    jogo.pacman = AtualizaCoordenadaPacman(xPacman+1, yPacman, jogo.pacman);
                }
                else {
                    jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                    break;
                }
                break;

            case 'd':
                if(jogo.mapa[xPacman][yPacman+1] != PAREDE) {
                    if(jogo.mapa[xPacman][yPacman+1] == COMIDA) {
                        pontuacao++;
                        jogo.mapa[xPacman][yPacman+1] = VAZIO;
                    }
                    jogo.pacman = AtualizaCoordenadaPacman(xPacman, yPacman+1, jogo.pacman);
                }
                else {
                    jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                    break;
                }
                break;
        }
        if(comidas == pontuacao) {
            printf("Voce venceu!\nPontuacao final: %d\n", pontuacao);
        }
        else if (jogadas == jogo.limiteDeJogadas) {
            printf("Game over!\nPontuacao final: %d\n", pontuacao);
        }
        else {
            printf("Estado do jogo apos o movimento '%c':\n", jogada);
            ImprimeJogo(jogo);
            printf("Pontuacao: %d\n", pontuacao);
        }
        jogadas++;
    }
}

int main(int argc, char * argv[]) {
    tJogo jogo;
    char diretorioGeral[MAX_DIR];
    if(argc <= 1) {
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        return 1;
    }
    sprintf(diretorioGeral, "%s", argv[1]);
    jogo = InicializaJogo(diretorioGeral);
    RealizaJogadas(jogo);
return 0;
}