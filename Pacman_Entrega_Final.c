#include <stdio.h>
#include <stdlib.h>

#define MAX_DIR 1000

//Modularizações referentes ao mapa
#define VAZIO ' '
#define PAREDE '#'
#define COMIDA '*'
#define COMIDA_ESPECIAL '%'
#define COMIDA_ALTERNATIVA '!'
#define PACMAN '>'
#define PACMAN_ALTERNATIVO '&'
#define TUNEL '@'
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
    char forma;
    tCoordenada coordenada;
    tCoordenada tunel;
    int pontuacao;
    int invencivel;
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
    tCoordenada tunel;
} tMovimento;

typedef struct {
    char letra;
    int quantidade;
    int comidas;
    int colisoes;
} tRanking;


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

//Caso uma fantasma não exista, ele é inicializado com as coordenadas (-1, -1);
int ExisteFantasma(tFantasma fantasma) {
    return !(fantasma.coordenada.x == -1 && fantasma.coordenada.y == -1);
}

//Adiciona um fantasma existente no mapa de fundo para printar;
void PreencheFantasmaNoMapa(int linha, int coluna, char mapaPreenchido[linha][coluna], tFantasma fantasma, tPacman pacman, int x, int y) {
    if(fantasma.coordenada.x == x && fantasma.coordenada.y == y && pacman.invencivel) {
    }
    else {
        mapaPreenchido[fantasma.coordenada.x][fantasma.coordenada.y] = fantasma.letra;
    }
}

//Função destinada a retornar o caractere que o pacman é no momento;
char DizFormaDoPacman(tPacman pacman) {
    return pacman.forma;
}

//Essa função substitui no mapa de fundo a localização do Pacman e dos fantasmas;
void PreencheMapa(int linha, int coluna, char mapaPreenchido[linha][coluna], tJogo jogo) {
    int i, j, xPacman, yPacman;
    char formaDoPacman;
    tCoordenada xyPacman;
    for(i = 0; i < linha; i++) {
        for(j = 0; j < coluna; j++) {
            mapaPreenchido[i][j] = jogo.mapa[i][j];
        }
    }
    xyPacman = GetXYPacman(jogo.pacman);
    xPacman = GetX(xyPacman);
    yPacman = GetY(xyPacman);
    formaDoPacman = DizFormaDoPacman(jogo.pacman);
    if(xPacman != -1 && yPacman != -1) mapaPreenchido[xPacman][yPacman] = formaDoPacman;
    for(i = 0; i < MAX_FANTASMAS; i++) {
        if(ExisteFantasma(jogo.fantasmas[i])) {
            PreencheFantasmaNoMapa(linha, coluna, mapaPreenchido, jogo.fantasmas[i], jogo.pacman, xPacman, yPacman);
        }
    }
}

//Função para imprimir o jogo completo;
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

//Função para encontrar coordenadas de um caractere qualquer no mapa;
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

//Função pra printar a posição inicial do Pacman no arquivo de inicialização;
void PrintaInicioPacmanNoDiretorio(tPacman pacman, FILE * arq) {
    fprintf(arq, "Pac-Man comecara o jogo na linha %d e coluna %d", pacman.coordenada.x+1, pacman.coordenada.y+1);
}

//Varre o mapa pra achar os fantasmas existentes;
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

//Inicializa todos os fantasmas pra não ter problema com segmentation fault e outros;
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

//Inicializa o pacman pra não ter problema com segmentation fault e outros;
tPacman InicializaPacman(int linha, int coluna, char mapaGeral[linha][coluna]) {
    tPacman pacman;
    pacman.coordenada = EncontraCoordenada(linha, coluna, mapaGeral, PACMAN);
    pacman.vida = 1;
    pacman.pontuacao = 0;
    pacman.tunel.x = -1;
    pacman.tunel.y = -1;
    pacman.forma = PACMAN;
    pacman.invencivel = 0;
return pacman;
}

//Formaliza o jogo com o mapa contendo apenas paredes, comida, vazio e tunel, separando os fantasmas e pacman;
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
            if(mapaGeral[i][j] != PAREDE && mapaGeral[i][j] != COMIDA && mapaGeral[i][j] != TUNEL 
            && mapaGeral[i][j] != COMIDA_ESPECIAL && mapaGeral[i][j] != COMIDA_ALTERNATIVA) {
                jogo.mapa[i][j] = VAZIO;
            }
            else {
                jogo.mapa[i][j] = mapaGeral[i][j];
            }
        }
    }
    return jogo;
}

//Função que retorna o jogo lido, inicializa o mapa e gera o arquivo inicializacao.txt;
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

//Muda as coordenadas após cada jogada;
tPacman AtualizaCoordenadaPacman(int x, int y, tPacman pacman) {
    pacman.coordenada.x = x;
    pacman.coordenada.y = y;
return pacman;
}

//Encontra a quantidade de comidas do mapa;
int AchaComidas(int linha, int coluna, tJogo jogo) {
    int i, j, comidas = 0;
    for(i = 0; i < linha; i++) {
        for(j = 0; j < coluna; j++) {
            if(jogo.mapa[i][j] == COMIDA) comidas++;
        }
    }
return comidas;
}

//Troca as coordenadas de um fantasma qualquer durante o turno;
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

//Verifica a existência de um fantasma na coordenada dada ou se o Pacman colidiu diretamente com um deles;
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

//Diminui a vida do Pacman;
tPacman AtualizaVidaPacman(tPacman pacman) {
    pacman.vida--;
return pacman;
}

//Retorna a vida do Pacman;
int GetLifePacman(tPacman pacman) {
return pacman.vida;
}

//Preenche o tipo movimento com as informações relevantes da jogada;
tMovimento PreencheMovimento(char direcao, int comeu, int bateu, tPacman pacman) {
    tMovimento movimento;
    movimento.direcao = direcao;
    movimento.comeu = comeu;
    movimento.bateu = bateu;
    movimento.vida = pacman.vida;
    movimento.coordenada = pacman.coordenada;
    movimento.tunel = pacman.tunel;
return movimento;
}

//Atualiza a vida do Pacman para -1 caso ele tenha vencido o jogo ou atingido o limite de jogadas;
tMovimento MovimentoFinal(tMovimento movimento) {
    movimento.vida = -1;
return movimento;
}

//Envia o Pacman para o outro túnel do mapa;
tPacman EnviaPraOutroTunel(tJogo jogo, tPacman pacman, int x, int y) {
    int i, j;
    for(i = 0; i < jogo.linhas; i++) {
        for(j = 0; j < jogo.colunas; j++) {
            if(jogo.mapa[i][j] == TUNEL && (i != x || j != y)) {
                pacman.coordenada.x = i;
                pacman.coordenada.y = j;
                break;
            }
        }
    }
return pacman;
}

//Adiciona a coordenada de túnel para o movimento;
tPacman AtualizaCoordenadaTunel(tPacman pacman, int x, int y) {
    pacman.tunel.x = x;
    pacman.tunel.y = y;
return pacman;
}

//Função que deixa o Pacman invencível e muda seu caractere de forma;
tPacman TornaInvencivel(tPacman pacman) {
    pacman.forma = PACMAN_ALTERNATIVO;
    pacman.invencivel = 1;
return pacman;
}

//Função que retorna o Pacman ao modo normal;
tPacman TornaNormal(tPacman pacman) {
    pacman.forma = PACMAN;
    pacman.invencivel = 0;
return pacman;
}

//Confere se o Pacman ainda está invencível;
int EstaInvencivel(tPacman pacman) {
return pacman.invencivel;
}

//Função que deve realizar as jogadas até que todas as comidas sejam consumidas ou o jogador perca;
void RealizaJogadas(tJogo jogo, tMovimento * movimentos) {
    int jogadas = 1, pontuacao = 0, comidas = 0, vida = 1, i, contadorInvencivel = 0;
    char jogada, lixo;

    tCoordenada xyPacman;
    int xPacman, yPacman;

    comidas = AchaComidas(jogo.linhas, jogo.colunas, jogo);
    movimentos[0] = PreencheMovimento('T', 0, 0, jogo.pacman);  //Inicializa a posição inicial para printar na trilha futuramente;

    while(comidas && jogadas <= jogo.limiteDeJogadas) {
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
                    if(TemFantasmaAqui(jogo.fantasmas, xPacman-1, yPacman, jogada) && !EstaInvencivel(jogo.pacman)) {
                        jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                        vida = GetLifePacman(jogo.pacman);
                        if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(CIMA, 0, 0, jogo.pacman);
                    }
                    else if(jogo.mapa[xPacman-1][yPacman] == TUNEL) {
                        jogo.pacman = EnviaPraOutroTunel(jogo, jogo.pacman, xPacman-1, yPacman);
                        jogo.pacman = AtualizaCoordenadaTunel(jogo.pacman, xPacman-1, yPacman);
                        movimentos[jogadas] = PreencheMovimento(CIMA, 0, 0, jogo.pacman);
                    }
                    else {
                        jogo.pacman = AtualizaCoordenadaPacman(xPacman-1, yPacman, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(CIMA, 0, 0, jogo.pacman);
                        if(jogo.mapa[xPacman-1][yPacman] == COMIDA) {
                            pontuacao++;
                            comidas--;
                            jogo.mapa[xPacman-1][yPacman] = VAZIO;
                            movimentos[jogadas] = PreencheMovimento(CIMA, 1, 0, jogo.pacman);
                        }
                        else if(jogo.mapa[xPacman-1][yPacman] == COMIDA_ESPECIAL) {
                            pontuacao = pontuacao + 5;
                            jogo.mapa[xPacman-1][yPacman] = VAZIO;
                            movimentos[jogadas] = PreencheMovimento(CIMA, 0, 0, jogo.pacman);
                        }
                        else if(jogo.mapa[xPacman-1][yPacman] == COMIDA_ALTERNATIVA) {
                            jogo.mapa[xPacman-1][yPacman] = VAZIO;
                            jogo.pacman = TornaInvencivel(jogo.pacman);
                            movimentos[jogadas] = PreencheMovimento(CIMA, 0, 0, jogo.pacman);
                        }
                    }
                }
                else if(TemFantasmaAqui(jogo.fantasmas, xPacman, yPacman, jogada) && !EstaInvencivel(jogo.pacman)) {
                    jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                    vida = GetLifePacman(jogo.pacman);
                    if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                    movimentos[jogadas] = PreencheMovimento(CIMA, 0, 1, jogo.pacman);
                }
                else if(jogo.mapa[xPacman][yPacman] == TUNEL) {
                    jogo.pacman = EnviaPraOutroTunel(jogo, jogo.pacman, xPacman, yPacman);
                    jogo.pacman = AtualizaCoordenadaTunel(jogo.pacman, xPacman, yPacman);
                    movimentos[jogadas] = PreencheMovimento(CIMA, 0, 0, jogo.pacman);
                }
                else {
                    movimentos[jogadas] = PreencheMovimento(CIMA, 0, 1, jogo.pacman);
                }
                break;
            
            case ESQUERDA:
                if(jogo.mapa[xPacman][yPacman-1] != PAREDE) {
                    if(TemFantasmaAqui(jogo.fantasmas, xPacman, yPacman-1, jogada) && !EstaInvencivel(jogo.pacman)) {
                        jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                        vida = GetLifePacman(jogo.pacman);
                        if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(ESQUERDA, 0, 0, jogo.pacman);
                    }
                    else if(jogo.mapa[xPacman][yPacman-1] == TUNEL) {
                        jogo.pacman = EnviaPraOutroTunel(jogo, jogo.pacman, xPacman, yPacman-1);
                        jogo.pacman = AtualizaCoordenadaTunel(jogo.pacman, xPacman, yPacman-1);
                        movimentos[jogadas] = PreencheMovimento(ESQUERDA, 0, 0, jogo.pacman);
                    }
                    else {
                        jogo.pacman = AtualizaCoordenadaPacman(xPacman, yPacman-1, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(ESQUERDA, 0, 0, jogo.pacman);
                        if(jogo.mapa[xPacman][yPacman-1] == COMIDA) {
                            pontuacao++;
                            comidas--;
                            jogo.mapa[xPacman][yPacman-1] = VAZIO;
                            movimentos[jogadas] = PreencheMovimento(ESQUERDA, 1, 0, jogo.pacman);
                        }
                        else if(jogo.mapa[xPacman][yPacman-1] == COMIDA_ESPECIAL) {
                            pontuacao = pontuacao + 5;
                            jogo.mapa[xPacman][yPacman-1] = VAZIO;
                            movimentos[jogadas] = PreencheMovimento(ESQUERDA, 0, 0, jogo.pacman);
                        }
                        else if(jogo.mapa[xPacman][yPacman-1] == COMIDA_ALTERNATIVA) {
                            jogo.mapa[xPacman][yPacman-1] = VAZIO;
                            jogo.pacman = TornaInvencivel(jogo.pacman);
                            movimentos[jogadas] = PreencheMovimento(ESQUERDA, 0, 0, jogo.pacman);
                        }
                    }
                }
                else if(TemFantasmaAqui(jogo.fantasmas, xPacman, yPacman, jogada) && !EstaInvencivel(jogo.pacman)) {
                    jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                    vida = GetLifePacman(jogo.pacman);
                    if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                    movimentos[jogadas] = PreencheMovimento(ESQUERDA, 0, 1, jogo.pacman);
                }
                else if (jogo.mapa[xPacman][yPacman] == TUNEL) {
                    jogo.pacman = EnviaPraOutroTunel(jogo, jogo.pacman, xPacman, yPacman);
                    jogo.pacman = AtualizaCoordenadaTunel(jogo.pacman, xPacman, yPacman);
                    movimentos[jogadas] = PreencheMovimento(ESQUERDA, 0, 0, jogo.pacman);
                }
                else {
                    movimentos[jogadas] = PreencheMovimento(ESQUERDA, 0, 1, jogo.pacman);
                }
                break;

            case BAIXO:
                if(jogo.mapa[xPacman+1][yPacman] != PAREDE) {
                    if(TemFantasmaAqui(jogo.fantasmas, xPacman+1, yPacman, jogada) && !EstaInvencivel(jogo.pacman)) {
                        jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                        vida = GetLifePacman(jogo.pacman);
                        if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(BAIXO, 0, 0, jogo.pacman);
                    }
                    else if (jogo.mapa[xPacman+1][yPacman] == TUNEL) {
                        jogo.pacman = EnviaPraOutroTunel(jogo, jogo.pacman, xPacman+1, yPacman);
                        jogo.pacman = AtualizaCoordenadaTunel(jogo.pacman, xPacman+1, yPacman);
                        movimentos[jogadas] = PreencheMovimento(BAIXO, 0, 0, jogo.pacman);
                    }
                    else {
                        jogo.pacman = AtualizaCoordenadaPacman(xPacman+1, yPacman, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(BAIXO, 0, 0, jogo.pacman);
                        if(jogo.mapa[xPacman+1][yPacman] == COMIDA) {
                            pontuacao++;
                            comidas--;
                            jogo.mapa[xPacman+1][yPacman] = VAZIO;
                            movimentos[jogadas] = PreencheMovimento(BAIXO, 1, 0, jogo.pacman);
                        }
                        else if(jogo.mapa[xPacman+1][yPacman] == COMIDA_ESPECIAL) {
                            pontuacao = pontuacao + 5;
                            jogo.mapa[xPacman+1][yPacman] = VAZIO;
                            movimentos[jogadas] = PreencheMovimento(BAIXO, 0, 0, jogo.pacman);
                        }
                        else if(jogo.mapa[xPacman+1][yPacman] == COMIDA_ALTERNATIVA) {
                            jogo.mapa[xPacman+1][yPacman] = VAZIO;
                            jogo.pacman = TornaInvencivel(jogo.pacman);
                            movimentos[jogadas] = PreencheMovimento(BAIXO, 0, 0, jogo.pacman);
                        }
                    }
                }
                else if(TemFantasmaAqui(jogo.fantasmas, xPacman, yPacman, jogada) && !EstaInvencivel(jogo.pacman)) {
                    jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                    vida = GetLifePacman(jogo.pacman);
                    if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                    movimentos[jogadas] = PreencheMovimento(BAIXO, 0, 1, jogo.pacman);
                }
                else if (jogo.mapa[xPacman][yPacman] == TUNEL) {
                    jogo.pacman = EnviaPraOutroTunel(jogo, jogo.pacman, xPacman, yPacman);
                    jogo.pacman = AtualizaCoordenadaTunel(jogo.pacman, xPacman, yPacman);
                    movimentos[jogadas] = PreencheMovimento(BAIXO, 0, 0, jogo.pacman);
                }
                else {
                    movimentos[jogadas] = PreencheMovimento(BAIXO, 0, 1, jogo.pacman);
                }
                break;

            case DIREITA:
                if(jogo.mapa[xPacman][yPacman+1] != PAREDE) {
                    if(TemFantasmaAqui(jogo.fantasmas, xPacman, yPacman+1, jogada) && !EstaInvencivel(jogo.pacman)) {
                        jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                        vida = GetLifePacman(jogo.pacman);
                        if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(DIREITA, 0, 0, jogo.pacman);
                    }
                    else if (jogo.mapa[xPacman][yPacman+1] == TUNEL) {
                        jogo.pacman = EnviaPraOutroTunel(jogo, jogo.pacman, xPacman, yPacman+1);
                        jogo.pacman = AtualizaCoordenadaTunel(jogo.pacman, xPacman, yPacman+1);
                        movimentos[jogadas] = PreencheMovimento(DIREITA, 0, 0, jogo.pacman);
                    }
                    else {
                        jogo.pacman = AtualizaCoordenadaPacman(xPacman, yPacman+1, jogo.pacman);
                        movimentos[jogadas] = PreencheMovimento(DIREITA, 0, 0, jogo.pacman);
                        if(jogo.mapa[xPacman][yPacman+1] == COMIDA) {
                            pontuacao++;
                            comidas--;
                            jogo.mapa[xPacman][yPacman+1] = VAZIO;
                            movimentos[jogadas] = PreencheMovimento(DIREITA, 1, 0, jogo.pacman);
                        }
                        else if(jogo.mapa[xPacman][yPacman+1] == COMIDA_ESPECIAL) {
                            pontuacao = pontuacao + 5;
                            jogo.mapa[xPacman][yPacman+1] = VAZIO;
                            movimentos[jogadas] = PreencheMovimento(DIREITA, 0, 0, jogo.pacman);
                        }
                        else if(jogo.mapa[xPacman][yPacman+1] == COMIDA_ALTERNATIVA) {
                            jogo.mapa[xPacman][yPacman+1] = VAZIO;
                            jogo.pacman = TornaInvencivel(jogo.pacman);
                            movimentos[jogadas] = PreencheMovimento(DIREITA, 0, 0, jogo.pacman);
                        }
                    }
                }
                else if(TemFantasmaAqui(jogo.fantasmas, xPacman, yPacman, jogada) && !EstaInvencivel(jogo.pacman)) {
                    jogo.pacman = AtualizaVidaPacman(jogo.pacman);
                    vida = GetLifePacman(jogo.pacman);
                    if(!vida) jogo.pacman = AtualizaCoordenadaPacman(-1, -1, jogo.pacman);
                    movimentos[jogadas] = PreencheMovimento(DIREITA, 0, 0, jogo.pacman);
                }
                else if (jogo.mapa[xPacman][yPacman] == TUNEL) {
                    jogo.pacman = EnviaPraOutroTunel(jogo, jogo.pacman, xPacman, yPacman);
                    jogo.pacman = AtualizaCoordenadaTunel(jogo.pacman, xPacman, yPacman);
                    movimentos[jogadas] = PreencheMovimento(DIREITA, 0, 0, jogo.pacman);
                }
                else {
                    movimentos[jogadas] = PreencheMovimento(DIREITA, 0, 1, jogo.pacman);
                }
                break;
 
        }

        if(EstaInvencivel(jogo.pacman)) {
            contadorInvencivel++;
            if(contadorInvencivel == 16) {
                jogo.pacman = TornaNormal(jogo.pacman);
                contadorInvencivel = 0;
            }
        }

        jogo.pacman = AtualizaCoordenadaTunel(jogo.pacman, -1, -1);         //Reseta a coordenada de túnel;

        printf("Estado do jogo apos o movimento '%c':\n", jogada);
        ImprimeJogo(jogo);
        printf("Pontuacao: %d\n\n", pontuacao);
        if(!comidas) {
            printf("Voce venceu!\nPontuacao final: %d\n", pontuacao);
            movimentos[jogadas] = MovimentoFinal(movimentos[jogadas]);
            break;
        }
        else if(jogadas == jogo.limiteDeJogadas || !vida) {
            printf("Game over!\nPontuacao final: %d\n", pontuacao);
            if(jogadas == jogo.limiteDeJogadas) movimentos[jogadas] = MovimentoFinal(movimentos[jogadas]);
            break;
        }
        jogadas++;
    }
}

//Gera o arquivo resumo.txt;
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
        if(movimentos[i].bateu) fprintf(arqResumo, "Movimento %d (%c) colidiu na parede\n", i, movimentos[i].direcao);
        if(movimentos[i].comeu) fprintf(arqResumo, "Movimento %d (%c) pegou comida\n", i, movimentos[i].direcao);
        if(movimentos[i].vida == -1) break;
        i++;
    }

    fclose(arqResumo);
}

//Inicializa uma direção do ranking;
tRanking InicializaDirecao(tRanking direcao, char direcaoLetra) {
    int i;
    direcao.letra = direcaoLetra;
    direcao.quantidade = 0;
    direcao.comidas = 0;
    direcao.colisoes = 0;
return direcao;
}

//Soma informações a cada direção do ranking;
tRanking AtualizaRanking(tRanking direcao, tMovimento movimento) {
    direcao.quantidade++;
    if(movimento.comeu) direcao.comidas++;
    if(movimento.bateu) direcao.colisoes++;
return direcao;
}

//Analisa qual dos caracteres vem primeiro em ordem alfabética;
int MenorLetra(tRanking direcao1, tRanking direcao2) {
    return direcao1.letra < direcao2.letra;
}

//Analisa qual direção foi mais jogada entre duas;
int MaisJogado(tRanking direcao1, tRanking direcao2) {
    if(direcao1.quantidade > direcao2.quantidade) {
        return 1;
    } else if(direcao1.quantidade == direcao2.quantidade) {
        return MenorLetra(direcao1, direcao2);
    }
return 0;
}

//Analisa qual direção, entre duas, gerou menos colisões;
int MenosColisoes(tRanking direcao1, tRanking direcao2) {
    if(direcao1.colisoes < direcao2.colisoes) {
        return 1;
    } else if(direcao1.colisoes == direcao2.colisoes) {
        return MaisJogado(direcao1, direcao2);
    }
return 0;
}

//Analisa qual é o melhor ranking, começando pelas comidas;
int MelhorRanking(tRanking direcao1, tRanking direcao2) {
    if(direcao1.comidas > direcao2.comidas) {
        return 1;
    } else if(direcao1.comidas == direcao2.comidas) {
        return MenosColisoes(direcao1, direcao2);
    }
return 0;
}

//Ordena os vetores do ranking;
void OrganizaRanking(tRanking * direcao) {
    int i, j;
    tRanking aux;
    for(i = 0; i < 3; i++) {
        for(j = i + 1; j < 4; j++) {
            if(MelhorRanking(direcao[i], direcao[j])) {
                aux = direcao[j];
                direcao[j] = direcao[i];
                direcao[i] = aux;
            }
        }
    }  
} 

//Escreve no arquivo desejado as informações das direções;
void PrintaNoRanking(tRanking * direcao, FILE * arqRanking) {
    int i;
    for(i = 3; i >= 0; i--) {
        fprintf(arqRanking, "%c,%d,%d,%d\n", direcao[i].letra, direcao[i].comidas, direcao[i].colisoes, direcao[i].quantidade);
    }
}

//Gera o arquivo ranking.txt;
void GeraRanking(tMovimento * movimentos, char * diretorioGeral) {
    FILE * arqRanking = NULL;
    char diretorioDoRanking[MAX_DIR];
    sprintf(diretorioDoRanking, "%s/saida/ranking.txt", diretorioGeral);
    arqRanking = fopen(diretorioDoRanking, "w");

    if(!arqRanking) {
        printf("ERRO: Nao foi possivel criar o arquivo ranking.txt em %s", diretorioDoRanking);        
        exit(1);
    }

    int i = 1, j;
    tRanking direcao[4];
    direcao[0] = InicializaDirecao(direcao[0], CIMA);
    direcao[1] = InicializaDirecao(direcao[1], ESQUERDA);
    direcao[2] = InicializaDirecao(direcao[2], BAIXO);
    direcao[3] = InicializaDirecao(direcao[3], DIREITA);
    //Inicialmente: 0 == w; 1 == a; 2 == s; 3 == d;
    while(1) {
        if(movimentos[i].direcao == CIMA) {
            direcao[0] = AtualizaRanking(direcao[0], movimentos[i]);
        }
        else if(movimentos[i].direcao == ESQUERDA) {
            direcao[1] = AtualizaRanking(direcao[1], movimentos[i]);
        }
        else if(movimentos[i].direcao == BAIXO) {
            direcao[2] = AtualizaRanking(direcao[2], movimentos[i]);
        }
        else if(movimentos[i].direcao == DIREITA) {
            direcao[3] = AtualizaRanking(direcao[3], movimentos[i]);
        }
        if(movimentos[i].vida == 0 || movimentos[i].vida == -1) break;
        i++;
    }
    OrganizaRanking(direcao);
    PrintaNoRanking(direcao, arqRanking);
    fclose(arqRanking);
}

//Adiciona a jogada na trilha previamente composta por -1;
void AlteraTrilha(int linha, int coluna, int trilha[linha][coluna], tCoordenada coordenada, tCoordenada tunel, int movimento) {
    trilha[coordenada.x][coordenada.y] = movimento;
    if(tunel.x != -1 && tunel.y != -1) trilha[tunel.x][tunel.y] = movimento;
}

//Gera o arquivo trilha.txt;
void GeraTrilha(tMovimento * movimentos, char * diretorioGeral, tJogo jogo) {
    FILE * arqTrilha = NULL;
    char diretorioDaTrilha[MAX_DIR];
    sprintf(diretorioDaTrilha, "%s/saida/trilha.txt", diretorioGeral);
    arqTrilha = fopen(diretorioDaTrilha, "w");

    if(!arqTrilha) {
        printf("ERRO: Nao foi possivel criar o arquivo trilha.txt em %s", diretorioDaTrilha);        
        exit(1);
    }

    int trilha[jogo.linhas][jogo.colunas], i, j;
    for(i = 0; i < jogo.linhas; i++) {
        for(j = 0; j < jogo.colunas; j++) {
            trilha[i][j] = -1;
        }
    }

    i = 0;

    while(1) {
        AlteraTrilha(jogo.linhas, jogo.colunas, trilha, movimentos[i].coordenada, movimentos[i].tunel, i);
        if(movimentos[i].vida == 0 || movimentos[i].vida == -1) break;
        i++;
    }

    for(i = 0; i < jogo.linhas; i++) {
        for(j = 0; j < jogo.colunas; j++) {
            if(trilha[i][j] == -1) {
                fprintf(arqTrilha, "#");
            }
            else {
                fprintf(arqTrilha, "%d", trilha[i][j]);
            }
            if(j != jogo.colunas - 1) fprintf(arqTrilha, " ");
        }
        fprintf(arqTrilha, "\n");
    }
    fclose(arqTrilha);
}

//Gera o arquivo estatisticas.txt;
void GeraEstatisticas(tMovimento * movimentos, char * diretorioGeral, tJogo jogo) {
    FILE * arqEstatistica = NULL;
    char diretorioDaEstatistica[MAX_DIR];
    sprintf(diretorioDaEstatistica, "%s/saida/estatisticas.txt", diretorioGeral);
    arqEstatistica = fopen(diretorioDaEstatistica, "w");

    if(!arqEstatistica) {
        printf("ERRO: Nao foi possivel criar o arquivo estatistica.txt em %s", diretorioDaEstatistica);        
        exit(1);
    }

    int i, nMovimentos = 0, nMovimentosSemPont = 0, nColisoes = 0, nCima = 0, nEsquerda = 0, nDireita = 0, nBaixo = 0;
    for(i = 1; i <= jogo.limiteDeJogadas; i++) {
        if(movimentos[i].direcao == CIMA) nCima++;
        if(movimentos[i].direcao == ESQUERDA) nEsquerda++;
        if(movimentos[i].direcao == DIREITA) nDireita++;
        if(movimentos[i].direcao == BAIXO) nBaixo++;
        if(!movimentos[i].comeu) nMovimentosSemPont++;
        if(movimentos[i].bateu) nColisoes++;
        nMovimentos++;
        if(!movimentos[i].vida || movimentos[i].vida == -1) break; 
    }

    fprintf(arqEstatistica, "Numero de movimentos: %d\n", nMovimentos);
    fprintf(arqEstatistica, "Numero de movimentos sem pontuar: %d\n", nMovimentosSemPont);
    fprintf(arqEstatistica, "Numero de colisoes com parede: %d\n", nColisoes);
    fprintf(arqEstatistica, "Numero de movimentos para baixo: %d\n", nBaixo);
    fprintf(arqEstatistica, "Numero de movimentos para cima: %d\n", nCima);
    fprintf(arqEstatistica, "Numero de movimentos para esquerda: %d\n", nEsquerda);
    fprintf(arqEstatistica, "Numero de movimentos para direita: %d\n", nDireita);

    fclose(arqEstatistica);
}

//Retorna o limite de jogadas;
int GetLimit(tJogo jogo) {
return jogo.limiteDeJogadas;
}

//Principal;
int main(int argc, char * argv[]) {
    tJogo jogo;
    int limite;
    char diretorioGeral[MAX_DIR];

    if(argc <= 1) {
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        return 1;
    }
    sprintf(diretorioGeral, "%s", argv[1]);

    jogo = InicializaJogo(diretorioGeral);

    limite = GetLimit(jogo);
    tMovimento movimentos[limite + 1];

    RealizaJogadas(jogo, movimentos);
    GeraResumo(movimentos, diretorioGeral);
    GeraRanking(movimentos, diretorioGeral);
    GeraTrilha(movimentos, diretorioGeral, jogo);
    GeraEstatisticas(movimentos, diretorioGeral, jogo);
return 0;
}