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
#define MAX_FANTASMAS 4

//Jogadas
#define CIMA 'w'
#define ESQUERDA 'a'
#define BAIXO 's'
#define DIREITA 'd'

//TipoCoordenada
typedef struct {
    int x;
    int y;
} tCoordenada;

//Tipo Pacman
typedef struct {
    int vida;
    tCoordenada coordenada;
    int pontuacao;
} tPacman;

//Tipo Fantasma
typedef struct {
    char letra;
    tCoordenada coordenada;
    tCoordenada coordenadaAnterior;
    int dx;
    int dy;
} tFantasma;

//Tipo Jogo
typedef struct {
    int linhas;
    int colunas;
    int limiteDeJogadas;
    tPacman pacman;
    tFantasma fantasmas[MAX_FANTASMAS];
    char mapa[MAPA_MAX_L][MAPA_MAX_C];
} tJogo;

//Tipo Movimento
typedef struct {
    char direcao;
    int comeu;
    int bateu;
    int vida;
    tCoordenada coordenada;
} tMovimento;


//!!! FIM DOS TIPOS !!!

//FUNÇÕES PARA PEGAR COORDENADAS
tCoordenada GetXYPacman(tPacman pacman) {
return pacman.coordenada;
}

int GetX(tCoordenada coordenada) {
return coordenada.x;
}

int GetY(tCoordenada coordenada) {
return coordenada.y;
}

//Caso uma fantasma não exista, ele é inicializado com as coordenadas (-1, -1)
int ExisteFantasma(tFantasma fantasma) {
    return !(fantasma.coordenada.x == -1 && fantasma.coordenada.y == -1);
}

//Adiciona um fantasma existente no mapa de fundo para printar
void PreencheFantasmaNoMapa(int linha, int coluna, char mapaPreenchido[linha][coluna], tFantasma fantasma) {
    mapaPreenchido[fantasma.coordenada.x][fantasma.coordenada.y] = fantasma.letra;
}

//Essa função substitui no mapa de fundo a localização do Pacman e dos fantasmas;
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
    for(i = 0; i < MAX_FANTASMAS; i++) {
        if(ExisteFantasma(jogo.fantasmas[i])) {
            PreencheFantasmaNoMapa(linha, coluna, mapaPreenchido, jogo.fantasmas[i]);
        }
    }
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
void PrintaInicioPacmanNoDiretorio(tPacman pacman, FILE * arq) {
    fprintf(arq, "Pac-Man comecara o jogo na linha %d e coluna %d", pacman.coordenada.x+1, pacman.coordenada.y+1);
}

//Varre o mapa pra achar os fantasmas existentes
void CacaFantasmas(int linha, int coluna, char mapaGeral[linha][coluna], tFantasma * fantasmas) {
    int i, j, k = 0;
    for(i = 0; i < linha; i++) {
        for(j = 0; j < coluna; j++) {
            if(mapaGeral[i][j] == FANTASMA_B) {
                fantasmas[k].coordenada.x = i;
                fantasmas[k].coordenada.y = j;
                fantasmas[k].dx = 0;
                fantasmas[k].dy = -1;
                fantasmas[k].letra = FANTASMA_B;
                k++;
            }
            else if(mapaGeral[i][j] == FANTASMA_P) {
                fantasmas[k].coordenada.x = i;
                fantasmas[k].coordenada.y = j;
                fantasmas[k].dx = -1;
                fantasmas[k].dy = 0;
                fantasmas[k].letra = FANTASMA_P;
                k++;
            }
            else if(mapaGeral[i][j] == FANTASMA_I) {
                fantasmas[k].coordenada.x = i;
                fantasmas[k].coordenada.y = j;
                fantasmas[k].dx = 1;
                fantasmas[k].dy = 0;
                fantasmas[k].letra = FANTASMA_I;
                k++;
            }
            else if(mapaGeral[i][j] == FANTASMA_C) {
                fantasmas[k].coordenada.x = i;
                fantasmas[k].coordenada.y = j;
                fantasmas[k].dx = 0;
                fantasmas[k].dy = 1;
                fantasmas[k].letra = FANTASMA_C;
                k++;
            }
        }
    }
}

//Inicializa todos os fantasmas pra não ter problema com segmentation fault
void InicializaFantasmas(tFantasma * fantasma) {
    int i;
    for(i = 0; i < MAX_FANTASMAS; i++) {
        fantasma[i].coordenada.x = -1;
        fantasma[i].coordenada.y = -1;
        fantasma[i].dx = 0;
        fantasma[i].dy = 0;
        fantasma[i].letra = 'T';
    }
}

tPacman InicializaPacman(int linha, int coluna, char mapaGeral[linha][coluna]) {
    tPacman pacman;
    pacman.coordenada = EncontraCoordenada(linha, coluna, mapaGeral, PACMAN);
    pacman.vida = 1;
    pacman.pontuacao = 0;
return pacman;
}

//Adiciona as informações que faltam no jogo (fantasmas e pacman)
tJogo LeJogoCompleto(int linha, int coluna, char mapaGeral[linha][coluna], tJogo jogo) {
    int i, j;

    tPacman pacman;
    pacman = InicializaPacman(linha, coluna, mapaGeral);
    jogo.pacman = pacman;

    tFantasma fantasmas[MAX_FANTASMAS];
    InicializaFantasmas(fantasmas);
    CacaFantasmas(linha, coluna, mapaGeral, fantasmas);
    for(i = 0; i < MAX_FANTASMAS; i++) {
        jogo.fantasmas[i] = fantasmas[i];
    }

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
    FILE * arqJogo = NULL;
    int i, j;

    sprintf(diretorioDoJogo, "%s/mapa.txt", diretorioGeral);
    arqJogo = fopen(diretorioDoJogo, "r");

    if(!arqJogo) {
        printf("ERRO: Nao foi possivel localizar o arquivo mapa.txt em %s", diretorioGeral);                 
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

    FILE * arqInicializacao = NULL;
    char diretorioDaInicializacao[MAX_DIR];
    sprintf(diretorioDaInicializacao, "%s/saida/inicializacao.txt", diretorioGeral);
    arqInicializacao = fopen(diretorioDaInicializacao, "w");

    if(!arqInicializacao) {
        printf("ERRO: Nao foi possivel criar o arquivo inicializacao.txt em %s", diretorioDaInicializacao);        
        exit(1);
    }
    for(i = 0; i < jogo.linhas; i++) {
        for(j = 0; j < jogo.colunas; j++) {
            fprintf(arqInicializacao, "%c", mapaGeral[i][j]);
        }
        fprintf(arqInicializacao, "\n");
    }
    PrintaInicioPacmanNoDiretorio(jogo.pacman, arqInicializacao);
    fclose(arqInicializacao);

    return jogo;
}



//GRUPO DE FUNCOES VOLTADAS PARA AS JOGADAS
tPacman AtualizaCoordenadaPacman(int x, int y, tPacman pacman) {
    pacman.coordenada.x = x;
    pacman.coordenada.y = y;
return pacman;
}

int AchaComidas(int linha, int coluna, tJogo jogo) {
    int i, j, comidas = 0;
    for(i = 0; i < linha; i++) {
        for(j = 0; j < coluna; j++) {
            if(jogo.mapa[i][j] == COMIDA) comidas++;
        }
    }
return comidas;
}

//Troca as coordenadas de um fantasma qualquer durante o turno
tFantasma AtualizaCoordenadaFantasma(tFantasma fantasma, tJogo jogo) {
    int i, j;
    fantasma.coordenadaAnterior.x = fantasma.coordenada.x;
    fantasma.coordenadaAnterior.y = fantasma.coordenada.y;
    if(fantasma.dx != 0) {
        if(jogo.mapa[fantasma.coordenada.x+fantasma.dx][fantasma.coordenada.y] == PAREDE) {
            fantasma.dx *= -1;
            fantasma.coordenada.x += fantasma.dx;
        }
        else {
            fantasma.coordenada.x += fantasma.dx;
        }
    }
    else if(fantasma.dy != 0) {
        if(jogo.mapa[fantasma.coordenada.x][fantasma.coordenada.y+fantasma.dy] == PAREDE) {
            fantasma.dy *= -1;
            fantasma.coordenada.y += fantasma.dy;
        }
        else {
            fantasma.coordenada.y += fantasma.dy;
        }
    }
return fantasma;
}

//Verifica a existência de um fantasma na coordenada
int TemFantasmaAqui(tFantasma * fantasmas, int x, int y, char jogada) {
    int i;
    for(i = 0; i < MAX_FANTASMAS; i++) {
        if(fantasmas[i].coordenada.x == x && fantasmas[i].coordenada.y == y) return 1;
        if(fantasmas[i].dx == 1 && fantasmas[i].coordenadaAnterior.x == x && fantasmas[i].coordenadaAnterior.y == y && jogada == CIMA) return 1;
        if(fantasmas[i].dx == -1 && fantasmas[i].coordenadaAnterior.x == x && fantasmas[i].coordenadaAnterior.y == y && jogada == BAIXO) return 1;
        if(fantasmas[i].dy == 1 && fantasmas[i].coordenadaAnterior.x == x && fantasmas[i].coordenadaAnterior.y == y && jogada == ESQUERDA) return 1;
        if(fantasmas[i].dy == -1 && fantasmas[i].coordenadaAnterior.x == x && fantasmas[i].coordenadaAnterior.y == y && jogada == DIREITA) return 1;
    }
return 0;
}

tPacman AtualizaVidaPacman(tPacman pacman) {
    pacman.vida--;
return pacman;
}

int GetLifePacman(tPacman pacman) {
return pacman.vida;
}

tMovimento PreencheMovimento(char direcao, int comeu, int bateu, tPacman pacman) {
    tMovimento movimento;
    movimento.direcao = direcao;
    movimento.comeu = comeu;
    movimento.bateu = bateu;
    movimento.vida = pacman.vida;
    movimento.coordenada = pacman.coordenada;
return movimento;
}

//Função que deve realizar as jogadas até que todas as comidas sejam consumidas ou o jogador perca
void RealizaJogadas(tJogo jogo, tMovimento * movimentos) {
    int jogadas = 1, pontuacao = 0, comidas = 0, vida = 1, i;
    char jogada, lixo;

    tCoordenada xyPacman;
    int xPacman, yPacman;

    comidas = AchaComidas(jogo.linhas, jogo.colunas, jogo);

    while(comidas != pontuacao && jogadas <= jogo.limiteDeJogadas) {
        scanf("%c%*c", &jogada);
        
        for(i = 0; i < MAX_FANTASMAS; i++) {
            if(ExisteFantasma(jogo.fantasmas[i])) {
                jogo.fantasmas[i] = AtualizaCoordenadaFantasma(jogo.fantasmas[i], jogo);
            }
        }

        xyPacman = GetXYPacman(jogo.pacman);
        xPacman = GetX(xyPacman);
        yPacman = GetY(xyPacman);

        switch (jogada) {
            case CIMA:
                if(jogo.mapa[xPacman-1][yPacman] != PAREDE) {
                    if(TemFantasmaAqui(jogo.fantasmas, xPacman-1, yPacman, jogada)) {
                        jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                        vida = GetLifePacman(jogo.pacman);
                        if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(CIMA, 0, 0, jogo.pacman);
                    }
                    else {
                        jogo.pacman = AtualizaCoordenadaPacman(xPacman-1, yPacman, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(CIMA, 0, 0, jogo.pacman);
                        if(jogo.mapa[xPacman-1][yPacman] == COMIDA) {
                            pontuacao++;
                            jogo.mapa[xPacman-1][yPacman] = VAZIO;
                            movimentos[jogadas] = PreencheMovimento(CIMA, 1, 0, jogo.pacman);
                        }
                    }
                }
                else if(TemFantasmaAqui(jogo.fantasmas, xPacman, yPacman, jogada)) {
                    jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                    vida = GetLifePacman(jogo.pacman);
                    if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                    movimentos[jogadas] = PreencheMovimento(CIMA, 0, 0, jogo.pacman);
                }
                else {
                    movimentos[jogadas] = PreencheMovimento(CIMA, 0, 1, jogo.pacman);
                }
                break;
            
            case ESQUERDA:
                if(jogo.mapa[xPacman][yPacman-1] != PAREDE) {
                    if(TemFantasmaAqui(jogo.fantasmas, xPacman, yPacman-1, jogada)) {
                        jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                        vida = GetLifePacman(jogo.pacman);
                        if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(ESQUERDA, 0, 0, jogo.pacman);
                    }
                    else {
                        jogo.pacman = AtualizaCoordenadaPacman(xPacman, yPacman-1, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(ESQUERDA, 0, 0, jogo.pacman);
                        if(jogo.mapa[xPacman][yPacman-1] == COMIDA) {
                            pontuacao++;
                            jogo.mapa[xPacman][yPacman-1] = VAZIO;
                            movimentos[jogadas] = PreencheMovimento(ESQUERDA, 1, 0, jogo.pacman);
                        }
                    }
                }
                else if(TemFantasmaAqui(jogo.fantasmas, xPacman, yPacman, jogada)) {
                    jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                    vida = GetLifePacman(jogo.pacman);
                    if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                    movimentos[jogadas] = PreencheMovimento(ESQUERDA, 0, 0, jogo.pacman);
                }
                else {
                    movimentos[jogadas] = PreencheMovimento(ESQUERDA, 0, 1, jogo.pacman);
                }
                break;

            case BAIXO:
                if(jogo.mapa[xPacman+1][yPacman] != PAREDE) {
                    if(TemFantasmaAqui(jogo.fantasmas, xPacman+1, yPacman, jogada)) {
                        jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                        vida = GetLifePacman(jogo.pacman);
                        if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(BAIXO, 0, 0, jogo.pacman);
                    }
                    else {
                        jogo.pacman = AtualizaCoordenadaPacman(xPacman+1, yPacman, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(BAIXO, 0, 0, jogo.pacman);
                        if(jogo.mapa[xPacman+1][yPacman] == COMIDA) {
                            pontuacao++;
                            jogo.mapa[xPacman+1][yPacman] = VAZIO;
                            movimentos[jogadas] = PreencheMovimento(BAIXO, 1, 0, jogo.pacman);
                        }
                    }
                }
                else if(TemFantasmaAqui(jogo.fantasmas, xPacman, yPacman, jogada)) {
                    jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                    vida = GetLifePacman(jogo.pacman);
                    if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                    movimentos[jogadas] = PreencheMovimento(BAIXO, 0, 0, jogo.pacman);
                }
                else {
                    movimentos[jogadas] = PreencheMovimento(BAIXO, 0, 1, jogo.pacman);
                }
                break;

            case DIREITA:
                if(jogo.mapa[xPacman][yPacman+1] != PAREDE) {
                    if(TemFantasmaAqui(jogo.fantasmas, xPacman, yPacman+1, jogada)) {
                        jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                        vida = GetLifePacman(jogo.pacman);
                        if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(DIREITA, 0, 0, jogo.pacman);
                    }
                    else {
                        jogo.pacman = AtualizaCoordenadaPacman(xPacman, yPacman+1, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(DIREITA, 0, 0, jogo.pacman);
                        if(jogo.mapa[xPacman][yPacman+1] == COMIDA) {
                            pontuacao++;
                            jogo.mapa[xPacman][yPacman+1] = VAZIO;
                            movimentos[jogadas] = PreencheMovimento(DIREITA, 1, 0, jogo.pacman);
                        }
                    }
                }
                else if(TemFantasmaAqui(jogo.fantasmas, xPacman, yPacman, jogada)) {
                    jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                    vida = GetLifePacman(jogo.pacman);
                    if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                    movimentos[jogadas] = PreencheMovimento(DIREITA, 0, 0, jogo.pacman);
                }
                else {
                    movimentos[jogadas] = PreencheMovimento(DIREITA, 0, 1, jogo.pacman);
                }
                break;
 
        }
        printf("Estado do jogo apos o movimento '%c':\n", jogada);
        ImprimeJogo(jogo);
        printf("Pontuacao: %d\n\n", pontuacao);
        if(comidas == pontuacao) {
            printf("Voce venceu!\nPontuacao final: %d\n", pontuacao);
            break;
        }
        else if(jogadas == jogo.limiteDeJogadas || !vida) {
            printf("Game over!\nPontuacao final: %d\n", pontuacao);
            break;
        }
        jogadas++;
    }
}

void GeraResumo(tMovimento * movimentos, char * diretorioGeral) {
    FILE * arqResumo = NULL;
    char diretorioDoResumo[MAX_DIR];
    sprintf(diretorioDoResumo, "%s/saida/resumo.txt", diretorioGeral);
    arqResumo = fopen(diretorioDoResumo, "w");

    if(!arqResumo) {
        printf("ERRO: Nao foi possivel criar o arquivo resumo.txt em %s", diretorioDoResumo);        
        exit(1);
    }

    int i = 1, flagVida = 1;
    while(flagVida) {
        if(!movimentos[i].vida) {
            fprintf(arqResumo, "Movimento %d (%c) fim de jogo por encostar em um fantasma\n", i, movimentos[i].direcao);
            flagVida = 0;
        }
        if(movimentos[i].bateu) fprintf(arqResumo, "Movimento %d (%c) colidiu com a parede\n", i, movimentos[i].direcao);
        if(movimentos[i].comeu) fprintf(arqResumo, "Movimento %d (%c) pegou comida\n", i, movimentos[i].direcao);
        i++;
    }

    fclose(arqResumo);
}

int main(int argc, char * argv[]) {
    tJogo jogo;
    int i, j;
    char diretorioGeral[MAX_DIR];

    if(argc <= 1) {
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        return 1;
    }
    sprintf(diretorioGeral, "%s", argv[1]);

    jogo = InicializaJogo(diretorioGeral);
    tMovimento movimentos[jogo.limiteDeJogadas];
    RealizaJogadas(jogo, movimentos);
    GeraResumo(movimentos, diretorioGeral);
return 0;
}