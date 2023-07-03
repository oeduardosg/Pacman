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

//Tipo Jogo
typedef struct {
    int linhas;
    int colunas;
    int limiteDeJogadas;
    char mapa[MAPA_MAX_L][MAPA_MAX_C];
} tJogo;

//Função para imprimir o mapa
void ImprimeMapa(tJogo jogo) {
    int i, j;
    for(i = 0; i < jogo.linhas; i++) {
        for(j = 0; j < jogo.colunas; j++) {
            printf("%c", jogo.mapa[i][j]);
        }
        printf("\n");
    }
}

//Função para encontrar coordenadas de um caractere qualquer no mapa
tCoordenada EncontraCoordenada(tJogo jogo, char procurado) {
    tCoordenada coordenada;
    int i, j;
    for(i = 0; i < jogo.linhas; i++) {
        for(j = 0; j < jogo.linhas; j++) {
            if(jogo.mapa[i][j] == procurado) {
                coordenada.x = i;
                coordenada.y = j;
                return coordenada;
            }
        }
    }
}

//Função pra printar a posição inicial do Pacman
void PrintaInicioPacman(tCoordenada coordenada) {
    printf("Pac-Man comecara o jogo na linha %d e coluna %d\n", coordenada.x+1, coordenada.y+1);
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
    for(i = 0; i < jogo.linhas; i++) {
        fscanf(arqJogo, "\n");
        for(j = 0; j < jogo.colunas; j++) {
            fscanf(arqJogo, "%c", &jogo.mapa[i][j]);
        }
    }
    ImprimeMapa(jogo);
    coordenadaPacman = EncontraCoordenada(jogo, PACMAN);
    PrintaInicioPacman(coordenadaPacman);
    return jogo;
}

//Função que tualiza as coordenadas do Pacman no mapa       REPENSAR NESSAS FUNCOES
tPacman EncontraCoordenadaDoPacman(tJogo jogo, tPacman pacman) {
    pacman.coordenada = EncontraCoordenada(jogo, PACMAN);
    return pacman;
}

//Função que deve realizar as jogadas até que todas as comidas sejam consumidas ou o jogador perca
void RealizaJogadas(tJogo jogo) {
    int jogadas = 0, pontuacao = 0, comidas;
    char jogada;
    tPacman pacman;
    while(comidas && jogadas < jogo.limiteDeJogadas) {
        scanf("%c", &jogada);
        switch (jogada) {
            case 'w':
            //COMPLETAR A PARTIR DAQUI!!!!!!!
        }
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