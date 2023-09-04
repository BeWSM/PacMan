#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int x; //coluna//
    int y; //linha//

}tPosicao;

typedef struct{
    
    tPosicao p;
    int level;  // level = pontuacao //
    int bateu;  // indicar que colidiu com a parede//
    int morreu; // indicar que colidiu com um fantasma//

}tPacman;

typedef struct{

    int l;
    int c;
    char mapa[40][100];
    char mapacomida[40][100]; //para verificar as comidas do mapa//
    char mapateleporte[40][100]; // para verificar o teleporte //
    int limitemovimentos;

}tMapa;

typedef struct{

    tPosicao inicio;
    tPosicao fim;
    int existe;  //verificar a existencia do teleporte

}tTeleporte;

typedef struct{

    tPosicao posicao;
    char direcao;
    int existe; //para os casos em que nem todos os fantasmas existem//

}tFantasma;

typedef struct{
    
    //referente ao fantasma B//
    tFantasma b;
    //referente ao fantasma P//
    tFantasma p;
    //referente ao fantasma I//
    tFantasma i;
    //referente ao fantasma C//
    tFantasma c;

}tFantasmas;

typedef struct{
    
    int qtdcomeu;   //qtd de vezes q esse movimento pegou comida
    int qtdbateu;   //qtd de vezes q esse movimento bateu na parede
    int qtd;       //qtd de vezes que esse movimento foi usado

}tMovimento;

//funcao princial:
void ExecutaPartida(char arg[], int qtdjogada,  tPacman pacmaninicial, tMapa mapainicial, tFantasmas faninicial);
//funcoes iniciais (inicializar o jogo)//
tMapa LeMapa(char arg[]);
tTeleporte BuscaTeleporte(tMapa m);
tPacman BuscaPacman(tMapa m); 
tFantasmas BuscaFantasma(tMapa m); 
//funcoes de atualizar o mapa ou movimentar os elementos//
tPosicao Movimentacao(char movimento, tPosicao p, tMapa m); 
tPacman AtualizaPacman(char jogada, tPacman p, tMapa mm, tFantasmas faninicial, tFantasmas fanfinal, tTeleporte t); 
tFantasmas MovimentaFantasma(tFantasmas f, tMapa m);
tMapa AtualizaFantasma(tFantasmas fi,tFantasmas ff, tMapa m);
tMapa ExecutaMovimento(char caminho[1050],char jogada, int numjogada, tMapa m, tPacman pacinicial, tPacman pacfinal, tFantasmas faninicial, tFantasmas fanfinal, tTeleporte t);
//funcoes de criar files ou exibir na saida padrao//
void Inicializacao(char caminho[1050], tPacman p, tMapa m, tFantasmas f);
void ResumoDaJogada(char caminho[1050], int comeu, char jogada,int numjogada, int colidiu, int morreu);
void SaidaPadrao(int totalmov, char jogada, tMapa m, tPacman p);
void Trilha(char caminho[1050], int total, tPacman pacman[total], tMapa m);
void Estatisticas(char caminho[1050],int total,char jogadas[total], tPacman pacman[total + 1]);
void Ranking(char caminho[1050], tMovimento w, tMovimento a,tMovimento s, tMovimento d);
//funcoes para auxiliar as demais (retorna verdade ou falso)//
int VenceuJogo(tMapa m);
int ColidiuComFantasma(tPacman p);

int main(int argc, char * argv[]){
    
    if(argc == 1){
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado");
    }else{
        tMapa mapainicial;
        tPacman pacmaninicial;
        tFantasmas faninicial;
        //inicializa as variaveis principais:
        mapainicial = LeMapa(argv[1]);
        faninicial = BuscaFantasma(mapainicial);
        pacmaninicial = BuscaPacman(mapainicial);
        Inicializacao(argv[1], pacmaninicial, mapainicial, faninicial);
        //executa as jogadas:
        ExecutaPartida(argv[1], mapainicial.limitemovimentos, pacmaninicial,mapainicial,faninicial);
    }

    return 0;
}
tPacman BuscaPacman(tMapa m){
    tPacman pacman;
    pacman.bateu = 0;
    pacman.level = 0;
    pacman.morreu = 0;
    int i, j;
        for( i=0; i < m.l; i++){
            for( j=0; j < m.c; j++){
                    if(m.mapa[i][j]=='>'){
                        pacman.p.x=j;
                        pacman.p.y=i;
                    //Busca a posição do pacman no mapa//
                    }
            }
    }
    return pacman;
}  
tFantasmas BuscaFantasma(tMapa m){
    tFantasmas fantasmas;
    int i, j, countb=0, countp=0, counti=0, countc=0;
    for( i=0; i < m.l; i++){
            for( j=0; j < m.c; j++){
                    if(m.mapa[i][j]=='B'){
                        fantasmas.b.posicao.x=j;
                        fantasmas.b.posicao.y=i;
                        fantasmas.b.direcao='a';
                        //Busca a posição do fantasma b no mapa//
                        countb++;
                         fantasmas.b.existe=1;
                        //verifica a existencia do fantasma b no mapa//
                    }else if(m.mapa[i][j]=='P'){
                        fantasmas.p.posicao.x=j;
                        fantasmas.p.posicao.y=i;
                        fantasmas.p.direcao='w';
                        //Busca a posição do fantasma p no mapa//
                        countp++;
                         fantasmas.p.existe=1;
                         //verifica a existencia do fantasma p no mapa//
                    }else if(m.mapa[i][j]=='I'){
                        fantasmas.i.posicao.x=j;
                        fantasmas.i.posicao.y=i;
                        fantasmas.i.direcao='s';
                        //Busca a posição do fantasma i no mapa//
                        counti++;
                         fantasmas.i.existe=1;
                         //verifica a existencia do fantasma i no mapa//
                    }else if(m.mapa[i][j]=='C'){
                        fantasmas.c.posicao.x=j;
                        fantasmas.c.posicao.y=i;
                        fantasmas.c.direcao='d';
                        //Busca a posição do fantasma c no mapa//
                        countc++;
                        fantasmas.c.existe=1;
                         //verifica a existencia do fantasma c no mapa//
                    }
            }
    }
    //caso nem todos os fantasmas estiverem presentes://
    if(countb==0){
        fantasmas.b.existe=0;
    }
    if(countp==0){
        fantasmas.p.existe=0;
    }
    if(counti==0){
       fantasmas.i.existe=0;

    }if(countc==0){
        fantasmas.c.existe=0;
    }
    return fantasmas;

}
tTeleporte BuscaTeleporte(tMapa m){
    int i=0, j=0, count1=0, count2=0;
    tTeleporte t;
    for(i=0; i< m.l; i++){
        for(j=0; j <m.c; j++){
            //localiza os teleportes no mapa
            if(((m.mapa[i][j]=='@') && (t.inicio.x > j) && (t.inicio.y > i))||((m.mapa[i][j]=='@') && count1==0)){
                t.inicio.x = j;
                t.inicio.y = i;
                count1++;
            }else if(((m.mapa[i][j]=='@') && (t.fim.x < j) && (t.fim.y < i))||((m.mapa[i][j]=='@') && count2==0)){
                t.fim.x = j;
                t.fim.y = i;
                count2++;
            }
        }
    }
    if(count1==0 && count2==0){
        t.existe = 0;
    }else{
        t.existe = 1;
    }
    return t;
}
int VenceuJogo(tMapa m){
    int i=0, j=0, count=0;
        for( i=0; i < m.l; i++){
            for( j=0; j < m.c; j++){
                    if(m.mapacomida[i][j]=='*'){
                        count++;
                        //verifico no mapa da comida pois pode ter um fantasma em cima dela no normal e contabilizar como se ele ja tivesse comido//
                    }
            }
        }
    if(count==0){
        return 1;
    }else{
        return 0;
    }
}
int ColidiuComFantasma(tPacman p){ //verifica se o pacman morreu na rodada//
    if(p.morreu==1){
        return 1;
    }else{
        return 0;
    }
}

void Inicializacao(char caminho[1050],tPacman p, tMapa m, tFantasmas f){
    char caminhoinicializacao[1050];
    sprintf(caminhoinicializacao,"%s/saida/inicializacao.txt",caminho);
    FILE *inicializa = fopen(caminhoinicializacao, "w");
    int i=0, j=0;
    if (inicializa == NULL) {
        printf("Erro ao criar o arquivo de inicializacao\n");
        return;
    }
    //cria o arquivo de inicializacao//
    for(i=0; i < m.l; i++){
        for(j=0; j < m.c; j++){
            fprintf(inicializa,"%c",m.mapa[i][j]);
        }
        fprintf(inicializa,"\n");
    }
    fprintf(inicializa, "Pac-Man comecara o jogo na linha %d e coluna %d\n",p.p.y+1, p.p.x+1); //somar +1 pq a matriz começa em 0
    fclose(inicializa);
}
tMapa LeMapa(char arg[]){
    int i, j, l;
    char caminho[1050];
    sprintf(caminho,"%s/mapa.txt",arg);
    FILE *board;
    tMapa m;
    board = fopen(caminho, "r");
    if(board == NULL){
        printf("erro ao ler o arquivo do mapa");
    }else{
        fscanf(board, "%d %d %d\n",&m.l, &m.c, &m.limitemovimentos);
        m.mapa[m.l][m.c];
        for( i=0; i < m.l; i++){
            for( j=0; j < m.c; j++){
                    fscanf(board, "%c",&m.mapa[i][j]);
                    m.mapacomida[i][j]=m.mapa[i][j];
                    //le o arquivo mapa.txt e salva ele como uma matriz//
            }
            fscanf(board, "%*c");
            //tira os \n que atrapalham na leitura do mapa//
        }
    }
    fclose(board);
    return m;
}
void SaidaPadrao(int totalmov, char jogada,tMapa m, tPacman p){
    int i=0, j=0;
    //printa as coisas da saida padrao
    printf("Estado do jogo apos o movimento '%c':\n",jogada);
    for(i=0; i < m.l; i++){
        for(j=0; j < m.c; j++){
            printf("%c",m.mapa[i][j]);
        }
        printf("\n");
    }
    printf("Pontuacao: %d\n",p.level);
    printf("\n");
     if(VenceuJogo(m)){
            printf("Voce venceu!\n");
            printf("Pontuacao final: %d\n",p.level);   
    }else if(ColidiuComFantasma(p)){
            printf("Game over!\n");
            printf("Pontuacao final: %d\n",p.level);
        
    }else if(totalmov >= m.limitemovimentos){
            printf("Game over!\n");
            printf("Pontuacao final: %d\n",p.level);
    }
}
tPosicao Movimentacao(char movimento, tPosicao p, tMapa m){
    tPosicao posicaofinal = p;
    //funcao que serve para movimentar TUDO
    if(movimento == 'w'){
        posicaofinal.y = p.y - 1;
    }else if(movimento == 's'){
        posicaofinal.y = p.y + 1;
    }else if(movimento == 'a'){
        posicaofinal.x = p.x - 1;
    }else if(movimento == 'd'){
        posicaofinal.x = p.x + 1;
    }
    if(m.mapa[posicaofinal.y][posicaofinal.x]=='#'){
        posicaofinal = p;
        //bateu na parede a posicao se mantem igual a inicial
    }
    return posicaofinal;

}
tMapa AtualizaFantasma(tFantasmas faninicial,tFantasmas fanfinal, tMapa mapainicial){
    tMapa mapafinal = mapainicial;
    //fantasma tem prioridade sob a comida MAS TEM Q PRINTAR ELA DEPOISS!!!//
    if((faninicial.b.existe)){
        if(mapafinal.mapacomida[faninicial.b.posicao.y][faninicial.b.posicao.x]=='*'){
            mapafinal.mapa[faninicial.b.posicao.y][faninicial.b.posicao.x]='*';
            mapafinal.mapa[fanfinal.b.posicao.y][fanfinal.b.posicao.x]='B';
        }else if(mapafinal.mapateleporte[faninicial.b.posicao.y][faninicial.b.posicao.x]=='@'){
            mapafinal.mapa[faninicial.b.posicao.y][faninicial.b.posicao.x]='@';
            //para que o fantasma nao suma com o portal
            mapafinal.mapa[fanfinal.b.posicao.y][fanfinal.b.posicao.x]='B';
        }else{
            mapafinal.mapa[faninicial.b.posicao.y][faninicial.b.posicao.x]=' ';
            mapafinal.mapa[fanfinal.b.posicao.y][fanfinal.b.posicao.x]='B';
        }
    }
    if((faninicial.p.existe)){
        if(mapafinal.mapacomida[faninicial.p.posicao.y][faninicial.p.posicao.x]=='*'){
            mapafinal.mapa[faninicial.p.posicao.y][faninicial.p.posicao.x]='*';
            mapafinal.mapa[fanfinal.p.posicao.y][fanfinal.p.posicao.x]='P';
        }else if(mapafinal.mapateleporte[faninicial.p.posicao.y][faninicial.p.posicao.x]=='@'){
            mapafinal.mapa[faninicial.p.posicao.y][faninicial.p.posicao.x]='@';
             //para que o fantasma nao suma com o portal
            mapafinal.mapa[fanfinal.p.posicao.y][fanfinal.p.posicao.x]='P';
        }else{
            mapafinal.mapa[faninicial.p.posicao.y][faninicial.p.posicao.x]=' ';
            mapafinal.mapa[fanfinal.p.posicao.y][fanfinal.p.posicao.x]='P';
        }

    }
    if((faninicial.i.existe)){
        if(mapafinal.mapacomida[faninicial.i.posicao.y][faninicial.i.posicao.x]=='*'){
            mapafinal.mapa[faninicial.i.posicao.y][faninicial.i.posicao.x]='*';
            mapafinal.mapa[fanfinal.i.posicao.y][fanfinal.i.posicao.x]='I';
        }else if(mapafinal.mapateleporte[faninicial.i.posicao.y][faninicial.i.posicao.x]=='@'){
            mapafinal.mapa[faninicial.i.posicao.y][faninicial.i.posicao.x]='@';
             //para que o fantasma nao suma com o portal
            mapafinal.mapa[fanfinal.i.posicao.y][fanfinal.i.posicao.x]='I';

        }else{
            mapafinal.mapa[faninicial.i.posicao.y][faninicial.i.posicao.x]=' ';
            mapafinal.mapa[fanfinal.i.posicao.y][fanfinal.i.posicao.x]='I';
        }
    }
    if((faninicial.c.existe)){
        if(mapafinal.mapacomida[faninicial.c.posicao.y][faninicial.c.posicao.x]=='*'){
            mapafinal.mapa[faninicial.c.posicao.y][faninicial.c.posicao.x]='*';
            mapafinal.mapa[fanfinal.c.posicao.y][fanfinal.c.posicao.x]='C';
        }else if(mapafinal.mapateleporte[faninicial.c.posicao.y][faninicial.c.posicao.x]=='@'){
            mapafinal.mapa[faninicial.c.posicao.y][faninicial.c.posicao.x]='@';
             //para que o fantasma nao suma com o portal
            mapafinal.mapa[fanfinal.c.posicao.y][fanfinal.c.posicao.x]='C';
        }else{
            mapafinal.mapa[faninicial.c.posicao.y][faninicial.c.posicao.x]=' ';
            mapafinal.mapa[fanfinal.c.posicao.y][fanfinal.c.posicao.x]='C';
        }

    }
    return mapafinal;
}
tFantasmas MovimentaFantasma(tFantasmas faninicial, tMapa m) {
    tFantasmas ff = faninicial;
 // quando ele bate na parede ja automaticamente muda de direcao na MESMA rodada!!!//
    if (faninicial.b.existe) {
        if (faninicial.b.direcao == 'a') {
            ff.b.posicao = Movimentacao(faninicial.b.direcao, faninicial.b.posicao, m);
                if((ff.b.posicao.x == faninicial.b.posicao.x) && (ff.b.posicao.y==faninicial.b.posicao.y)){
                    ff.b.direcao = 'd'; //bateu na parede e mudou de direcao//
                    ff.b.posicao= Movimentacao(ff.b.direcao, faninicial.b.posicao, m);
                }
        } else if (faninicial.b.direcao == 'd') {
             ff.b.posicao = Movimentacao(faninicial.b.direcao, faninicial.b.posicao, m);
             if((ff.b.posicao.x == faninicial.b.posicao.x) && (ff.b.posicao.y==faninicial.b.posicao.y)){
                ff.b.direcao = 'a'; //bateu na parede e mudou de direcao//
                ff.b.posicao = Movimentacao(ff.b.direcao, faninicial.b.posicao, m);
            }
        }
    }
    if (faninicial.p.existe) {
        if (faninicial.p.direcao == 'w') {
            ff.p.posicao = Movimentacao(faninicial.p.direcao, faninicial.p.posicao, m);
            if((ff.p.posicao.x == faninicial.p.posicao.x) && (ff.p.posicao.y==faninicial.p.posicao.y)){
                ff.p.direcao = 's'; //bateu na parede e mudou de direcao//
                ff.p.posicao = Movimentacao(ff.p.direcao, faninicial.p.posicao, m);
            }
        } else if (faninicial.p.direcao == 's') {
           ff.p.posicao = Movimentacao(faninicial.p.direcao, faninicial.p.posicao, m);
            if((ff.p.posicao.x == faninicial.p.posicao.x) && (ff.p.posicao.y==faninicial.p.posicao.y)){
                ff.p.direcao = 'w'; //bateu na parede e mudou de direcao//
                ff.p.posicao = Movimentacao(ff.p.direcao, faninicial.p.posicao, m);
            }
        }
    }
    if (faninicial.i.existe) {
        if (faninicial.i.direcao == 's') {
           ff.i.posicao = Movimentacao(faninicial.i.direcao, faninicial.i.posicao, m);
           if((ff.i.posicao.x == faninicial.i.posicao.x) && (ff.i.posicao.y==faninicial.i.posicao.y)){
            ff.i.direcao = 'w'; //bateu na parede e mudou de direcao//
            ff.i.posicao = Movimentacao(ff.i.direcao, faninicial.i.posicao, m);
           }
        } else if (faninicial.i.direcao == 'w') {
            ff.i.posicao = Movimentacao(faninicial.i.direcao, faninicial.i.posicao, m);
           if((ff.i.posicao.x == faninicial.i.posicao.x) && (ff.i.posicao.y==faninicial.i.posicao.y)){
            ff.i.direcao = 's'; //bateu na parede e mudou de direcao//
            ff.i.posicao = Movimentacao(ff.i.direcao, faninicial.i.posicao, m);
           }
        }
    }

    if (faninicial.c.existe) {
        if (faninicial.c.direcao == 'd') {
           ff.c.posicao = Movimentacao(faninicial.c.direcao, faninicial.c.posicao, m);
           if((ff.c.posicao.x == faninicial.c.posicao.x) && (ff.c.posicao.y==faninicial.c.posicao.y)){
            ff.c.direcao = 'a'; //bateu na parede e mudou de direcao//
            ff.c.posicao = Movimentacao(ff.c.direcao, faninicial.c.posicao, m);
           }
        } else if (faninicial.c.direcao == 'a') {
             ff.c.posicao = Movimentacao(faninicial.c.direcao, faninicial.c.posicao, m);
           if((ff.c.posicao.x == faninicial.c.posicao.x) && (ff.c.posicao.y==faninicial.c.posicao.y)){
            ff.c.direcao = 'd'; //bateu na parede e mudou de direcao//
            ff.c.posicao = Movimentacao(ff.c.direcao, faninicial.c.posicao, m);
           }
        }
    }
    return ff;
}
tPacman AtualizaPacman(char jogada, tPacman pacinicial, tMapa mapa, tFantasmas faninicial, tFantasmas fanfinal, tTeleporte t){
    tPacman pacfinal = pacinicial;
    pacfinal.p = Movimentacao(jogada, pacinicial.p, mapa);
    
    if((pacfinal.p.x == pacinicial.p.x) && (pacfinal.p.y == pacinicial.p.y)){
        pacfinal.bateu = 1;
        //retorna que o pacman bateu na parede
    }
    if(mapa.mapa[pacfinal.p.y][pacfinal.p.x]=='@'){
        if((pacfinal.p.x == t.fim.x) && (pacfinal.p.y==t.fim.y)){
            pacfinal.p.x = t.inicio.x;
            pacfinal.p.y = t.inicio.y;
            //teleporta o pacman//
        }else if((pacfinal.p.x == t.inicio.x)&&(pacfinal.p.y==t.inicio.y)){
            pacfinal.p.x = t.fim.x;
            pacfinal.p.y = t.fim.y;
            //teleporta o pacman//
        }
    }
    if (fanfinal.b.existe){
        if((pacfinal.p.x== fanfinal.b.posicao.x)&&(pacfinal.p.y == fanfinal.b.posicao.y)){
            pacfinal.morreu= 1;
        }else if((pacfinal.p.x == faninicial.b.posicao.x) && (pacfinal.p.y == faninicial.b.posicao.y) && (pacinicial.p.x == fanfinal.b.posicao.x) && (pacinicial.p.y == fanfinal.b.posicao.y)){
            pacfinal.morreu= 1; //colidiu com o fantasma indiretamente, durante a movimentacao!!//
        }
    }
    if (fanfinal.p.existe){
        if((pacfinal.p.x== fanfinal.p.posicao.x)&&(pacfinal.p.y == fanfinal.p.posicao.y)){
            pacfinal.morreu = 1;
        }else if((pacfinal.p.x == faninicial.p.posicao.x) && (pacfinal.p.y == faninicial.p.posicao.y) && (pacinicial.p.x == fanfinal.p.posicao.x) && (pacinicial.p.y == fanfinal.p.posicao.y)){
            pacfinal.morreu = 1;//colidiu com o fantasma indiretamente, durante a movimentacao!!//
        }

    }if (fanfinal.i.existe){
        if((pacfinal.p.x== fanfinal.i.posicao.x)&&(pacfinal.p.y == fanfinal.i.posicao.y)){
            pacfinal.morreu = 1;
        }else if((pacfinal.p.x == faninicial.i.posicao.x) && (pacfinal.p.y == faninicial.i.posicao.y) && (pacinicial.p.x == fanfinal.i.posicao.x) && (pacinicial.p.y == fanfinal.i.posicao.y)){
            pacfinal.morreu = 1;//colidiu com o fantasma indiretamente, durante a movimentacao!!//
        }

    }if (fanfinal.c.existe){
        if((pacfinal.p.x== fanfinal.c.posicao.x)&&(pacfinal.p.y == fanfinal.c.posicao.y)){
            pacfinal.morreu = 1;
        }else if((pacfinal.p.x == faninicial.c.posicao.x) && (pacfinal.p.y == faninicial.c.posicao.y) && (pacinicial.p.x == fanfinal.c.posicao.x) && (pacinicial.p.y == fanfinal.c.posicao.y)){
            pacfinal.morreu = 1;//colidiu com o fantasma indiretamente, durante a movimentacao!!//
        }
    }
    if((mapa.mapacomida[pacfinal.p.y][pacfinal.p.x]=='*') && (pacfinal.morreu==0)){
        pacfinal.level++; //comeu comida e subiu a pontuacao//
    }
    return pacfinal;
}
tMapa ExecutaMovimento(char caminho[1050],char jogada, int numjogada, tMapa m, tPacman pacinicial, tPacman pacfinal, tFantasmas faninicial, tFantasmas fanfinal, tTeleporte t){
   tMapa mapafinal = m;
    int comeu = 0, colidiu = 0, morreu = 0;
    mapafinal = AtualizaFantasma(faninicial, fanfinal, m);
        if(pacfinal.level > pacinicial.level){
            comeu = 1;
        }
        if(pacfinal.bateu){  //pacman bateu na parede, logo nao tem motivo de atualizar a posicao//
            colidiu = 1;
        }
        if(pacfinal.morreu){ //quando pacman morre, só é printado o fantasma!!//
            if(mapafinal.mapa[pacfinal.p.y][pacfinal.p.x]=='B'){
                if(mapafinal.mapacomida[pacinicial.p.y][pacinicial.p.x]=='*'){
                    mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = '*';
                     //para nao limpar a comida sem querer//    
                }else{
                mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = ' ';
                }
                mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = 'B';
            }else if(mapafinal.mapa[pacfinal.p.y][pacfinal.p.x]=='C'){
                if(mapafinal.mapacomida[pacinicial.p.y][pacinicial.p.x]=='*'){
                    mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = '*'; 
                     //para nao limpar a comida sem querer//   
                }else{
                mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = ' ';
                }
                mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = 'C';
            }else if(mapafinal.mapa[pacfinal.p.y][pacfinal.p.x]=='P'){
                if(mapafinal.mapacomida[pacinicial.p.y][pacinicial.p.x]=='*'){
                    mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = '*';  
                     //para nao limpar a comida sem querer//  
                }else{
                mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = ' ';
                }
                mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = 'P';
            }else if(mapafinal.mapa[pacfinal.p.y][pacfinal.p.x]=='I'){
               if(mapafinal.mapacomida[pacinicial.p.y][pacinicial.p.x]=='*'){
                    mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = '*';  
                     //para nao limpar a comida sem querer//  
                }else{
                mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = ' ';
                }
                mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = 'I';
            } else if(mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] == 'B'){
                if((pacfinal.p.x == faninicial.b.posicao.x) && (pacfinal.p.y == faninicial.b.posicao.y)){
                    if( mapafinal.mapacomida[pacfinal.p.y][pacfinal.p.x] == '*'){
                        mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = '*';
                         //para nao limpar a comida sem querer//
                    }else{
                    mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = ' ';
                    }
                    mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = 'B';
                }
            }else if(mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] == 'P'){
                if((pacfinal.p.x == faninicial.p.posicao.x) && (pacfinal.p.y == faninicial.p.posicao.y)){
                    if(mapafinal.mapacomida[pacfinal.p.y][pacfinal.p.x] == '*'){
                        mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = '*';
                         //para nao limpar a comida sem querer//
                    }else{
                    mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = ' ';
                    }
                    mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = 'P';
                }
                

            }else if(mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] == 'I'){
                if((pacfinal.p.x == faninicial.i.posicao.x) && (pacfinal.p.y == faninicial.i.posicao.y)){
                    if(mapafinal.mapacomida[pacfinal.p.y][pacfinal.p.x] == '*'){
                        mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = '*';
                        //para nao limpar a comida sem querer//
                    }else{
                    mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = ' ';
                    }
                    mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = 'I';
                }

            }else if(mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] == 'C'){
                if((pacfinal.p.x == faninicial.c.posicao.x) && (pacfinal.p.y == faninicial.c.posicao.y)){
                     if( mapafinal.mapacomida[pacfinal.p.y][pacfinal.p.x] == '*'){
                        mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = '*';
                        //para nao limpar a comida sem querer//
                    }else{
                    mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = ' ';
                    }
                    mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = 'C';
                }
            }
        }else{
            if(t.existe){
                //verifica se o pacman passou no teleporte//
               if(pacinicial.p.y==t.inicio.y && pacinicial.p.x==t.inicio.x){
                    mapafinal.mapa[t.inicio.y][t.inicio.x]='@';
                    mapafinal.mapa[pacfinal.p.y][pacfinal.p.x]='>';
                }else if(pacinicial.p.y==t.fim.y && pacinicial.p.x==t.fim.x){
                    mapafinal.mapa[t.fim.y][t.fim.x]='@';
                    mapafinal.mapa[pacfinal.p.y][pacfinal.p.x]='>';
                }
                
            }
            if(mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] =='*' && (mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] !='B') && (mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] !='P')&&(mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] !='I')&&(mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] !='C') && (mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] !='@')){
                //para caso tenha um fantasma perto e um portal, ele nao desapareca//
                mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = '>';
                mapafinal.mapacomida[pacfinal.p.y][pacfinal.p.x] = ' ';
                mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = ' ';
            }else if(mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] ==' ' && (mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] !='B') && (mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] !='P')&&(mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] !='I')&&(mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] !='C') && (mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] !='@')){
                //para caso tenha um fantasma perto ou um portal, ele nao desapareca//
                mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = '>';
                mapafinal.mapacomida[pacfinal.p.y][pacfinal.p.x] = ' ';
                mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = ' ';
            }else if((pacinicial.p.x == fanfinal.c.posicao.x) && (pacinicial.p.y == fanfinal.c.posicao.y)){
                //quando o fantasma e o pacman estao colados, mas eles nao se encostaram ainda//
                mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = '>';
                mapafinal.mapacomida[pacfinal.p.y][pacfinal.p.x] = ' ';
                mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = 'C';

            }else if((pacinicial.p.x == fanfinal.p.posicao.x) && (pacinicial.p.y == fanfinal.p.posicao.y)){
                //quando o fantasma e o pacman estao colados, mas eles nao se encostaram ainda//
                mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = '>';
                mapafinal.mapacomida[pacfinal.p.y][pacfinal.p.x] = ' ';
                mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = 'P';

            }else if((pacinicial.p.x == fanfinal.i.posicao.x) && (pacinicial.p.y == fanfinal.i.posicao.y)){
                //quando o fantasma e o pacman estao colados, mas eles nao se encostaram ainda//
                mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = '>';
                mapafinal.mapacomida[pacfinal.p.y][pacfinal.p.x] = ' ';
                mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = 'I';

            }else if((pacinicial.p.x == fanfinal.b.posicao.x) && (pacinicial.p.y == fanfinal.b.posicao.y)){
                //quando o fantasma e o pacman estao colados, mas eles nao se encostaram ainda//
                mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = '>';
                mapafinal.mapacomida[pacfinal.p.y][pacfinal.p.x] = ' ';
                mapafinal.mapa[pacinicial.p.y][pacinicial.p.x] = 'B';

            }else if(mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] =='*'){
                mapafinal.mapa[pacfinal.p.y][pacfinal.p.x] = '>';
                mapafinal.mapacomida[pacfinal.p.y][pacfinal.p.x] = ' ';
                //sempre lembrar de atualizar o mapa da comida!!//
            }
        }
        
        ResumoDaJogada(caminho,comeu, jogada, numjogada, colidiu, pacfinal.morreu);
    return mapafinal;
}
void ResumoDaJogada(char caminho[1050], int comeu, char jogada,int numjogada, int colidiu, int morreu){
    char caminhoresumo[1050];
    sprintf(caminhoresumo,"%s/saida/resumo.txt",caminho);
    FILE *resumo = fopen(caminhoresumo, "a");
        if(morreu==1){
            fprintf(resumo, "Movimento %d (%c) fim de jogo por encostar em um fantasma\n",numjogada,jogada);
        }if(colidiu){
            fprintf(resumo,"Movimento %d (%c) colidiu na parede\n",numjogada,jogada);
        }if(comeu){
            fprintf(resumo,"Movimento %d (%c) pegou comida\n",numjogada, jogada);
        }
    fclose(resumo);
}
void Trilha(char caminho[1050], int total, tPacman pacman[total], tMapa m){
    char caminhotrilha[1050];
    sprintf(caminhotrilha,"%s/saida/trilha.txt",caminho);
    FILE *trilha = fopen(caminhotrilha,"w");
    int i=0, j=0, k=0;
    int mm[m.l][m.c];
    for(i=0; i<m.l; i++){
        for(j=0; j<m.c; j++){
            mm[i][j] = -1;    //inicializa o mapa da trilha como negativo para ser mais facil de printar os '#' depois
        }
    }
    for(i=0; i<m.l; i++){
        for(j=0; j<m.c; j++){
            mm[i][j] = -1;
            if(pacman[total].morreu==1){
                for(k=0; k<total; k++){ // tem que ser menor que o total pois a posicao final em que o pacman morre nao é mostrada na trilha//
                    if((j==pacman[k].p.x) && (i == pacman[k].p.y)){
                        mm[i][j] = k;
                    }
                }
            }else if(total > m.limitemovimentos){
                for(k=0; k<total; k++){ // tem que ser menor que o total pois ele ultrapassou o limite de movimentos//
                    if((j==pacman[k].p.x) && (i == pacman[k].p.y)){
                        mm[i][j] = k;
                    }
                }   
            } else {
                for(k=0; k<=total; k++){ // tem que ser igual ao total pois todas as jogadas sao válidas
                    if((j==pacman[k].p.x) && (i == pacman[k].p.y)){
                        mm[i][j] = k;
                    }
                }  
            }
        }
    }
    if(trilha == NULL){
        printf("Erro ao criar o arquivo da trilha\n");
        return;
    }
    for(i=0; i < m.l; i++){
        for(j=0; j < m.c; j++){
            if(mm[i][j]==-1){
                fprintf(trilha,"# "); 
            }else{
                fprintf(trilha,"%d ",mm[i][j]);    
            }   
        }
        fprintf(trilha,"\n");
    }
        fclose(trilha);
}
void ExecutaPartida(char arg[],int qtdjogada, tPacman pacmaninicial, tMapa mapainicial, tFantasmas faninicial){
    int i=1;
    char caminho[1050];
    sprintf(caminho,"%s",arg);
    char jogadas[qtdjogada+1];
    tPacman pacman[qtdjogada+1];
    tFantasmas fanfinal, fanantes;
    tTeleporte teleporte = BuscaTeleporte(mapainicial);
    tMapa mapafinal, mapaantes;
    pacman[0] = pacmaninicial;
    mapaantes = mapainicial;
    fanantes = faninicial;

    while(scanf("%c",&jogadas[i])==1){
            scanf("%*c");
            fanfinal=MovimentaFantasma(fanantes, mapaantes);
            pacman[i]=AtualizaPacman(jogadas[i],pacman[i-1], mapaantes, fanantes, fanfinal, teleporte);
            mapafinal = ExecutaMovimento(caminho, jogadas[i], i ,mapaantes,pacman[i-1],pacman[i], fanantes, fanfinal,teleporte);
            SaidaPadrao(i, jogadas[i], mapafinal, pacman[i]);
            //jogadafinal = AnalisaJogada(jogadaant, jogadas[i],pacman[i-1],pacman[i],mapafinal);
            if(VenceuJogo(mapafinal)){
                break;
            }else if(ColidiuComFantasma(pacman[i])){
                break;
            }else if(i > mapainicial.limitemovimentos){
                break;
            }
            pacman[i].bateu=0;
            //reseta para o proximo pacman//
            i++;
            mapaantes = mapafinal;
            fanantes = fanfinal;
            //jogadaant = jogadafinal;
        }
        Trilha(caminho, i, pacman, mapainicial);
        Estatisticas(caminho, i, jogadas, pacman);

}

void Estatisticas(char caminho[1050],int total,char jogadas[total], tPacman pacman[total+1]){
    char caminhoesta[1050];
    int i=0,totalbateu=0, totalcomeu=0, semponto=0, qtdw=0, qtda=0, qtds=0, qtdd=0, comeuw=0, comeua=0, comeud=0, comeus=0;
    int bateuw=0, bateua=0, bateus=0, bateud=0;
    tMovimento w, a, s ,d;
    for(i=1; i<=total; i++){    //verifica cada jogada e as contabilizam para montar as estatisticas
        if(jogadas[i]=='w'){
            qtdw++;
            if(pacman[i-1].level < pacman[i].level){
                comeuw++;
            }
            if((pacman[i-1].p.x==pacman[i].p.x) && (pacman[i-1].p.y==pacman[i].p.y)){
                bateuw++;
            }
        }else if(jogadas[i]=='a'){
            qtda++;
            if(pacman[i-1].level < pacman[i].level){
                comeua++;
            }
            if((pacman[i-1].p.x==pacman[i].p.x) && (pacman[i-1].p.y==pacman[i].p.y)){
                bateua++;
            }
        }else if(jogadas[i]=='s'){
            qtds++;
            if(pacman[i-1].level < pacman[i].level){
                comeus++;
            }
            if((pacman[i-1].p.x==pacman[i].p.x) && (pacman[i-1].p.y==pacman[i].p.y)){
                bateus++;
            }
        }else if(jogadas[i]=='d'){
            qtdd++;
            if(pacman[i-1].level < pacman[i].level){
                comeud++;
            }
            if((pacman[i-1].p.x==pacman[i].p.x) && (pacman[i-1].p.y==pacman[i].p.y)){
                bateud++;
            }
        }
    }
    w.qtd = qtdw;            //atribui as jogadas contadas ao struct movimento para ser mais dinamico na passagem desses valores para o ranking
    w.qtdbateu = bateuw;
    w.qtdcomeu = comeuw;     
    a.qtd = qtda;
    a.qtdbateu = bateua;
    a.qtdcomeu = comeua;
    s.qtd = qtds;
    s.qtdbateu = bateus;
    s.qtdcomeu = comeus;
    d.qtd = qtdd;
    d.qtdbateu = bateud;
    d.qtdcomeu = comeud;
    total = a.qtd + d.qtd + s.qtd + w.qtd;
    totalcomeu = comeua + comeud + comeus + comeuw;
    totalbateu = bateua + bateud + bateus + bateuw;
    semponto = total - totalcomeu;
    sprintf(caminhoesta,"%s/saida/estatisticas.txt",caminho);
    FILE*estatistica = fopen(caminhoesta, "w");

    fprintf(estatistica,"Numero de movimentos: %d\n",total);
    fprintf(estatistica,"Numero de movimentos sem pontuar: %d\n",semponto);
    fprintf(estatistica,"Numero de colisoes com parede: %d\n",totalbateu);
    fprintf(estatistica,"Numero de movimentos para baixo: %d\n",qtds);
    fprintf(estatistica,"Numero de movimentos para cima: %d\n",qtdw);
    fprintf(estatistica,"Numero de movimentos para esquerda: %d\n",qtda);
    fprintf(estatistica,"Numero de movimentos para direita: %d\n",qtdd);

    fclose(estatistica);
    Ranking(caminho, w,a,s,d);

}
void Ranking(char caminho[1050], tMovimento w, tMovimento a,tMovimento s, tMovimento d){
    char caminhorank[1050];
    char movimentos[] = {'w','a','s','d'};
    int qtdcomeu[] = {w.qtdcomeu, a.qtdcomeu, s.qtdcomeu, d.qtdcomeu};
    int qtdbateu[] = {w.qtdbateu, a.qtdbateu, s.qtdbateu, d.qtdbateu};
    int totalmov[] = {w.qtd, a.qtd, s.qtd, d.qtd};
    int i, j;
    for(i=0; i<4; i++){
        for(j=i+1; j<4; j++){
                if((qtdcomeu[j] > qtdcomeu[i]) || ((qtdcomeu[j] == qtdcomeu[i]) && (qtdbateu[j] < qtdbateu[i])) || 
                ((qtdcomeu[j] == qtdcomeu[i]) && (qtdbateu[j] == qtdbateu[i]) && (totalmov[j] > totalmov[i])) || 
                ((qtdcomeu[j] == qtdcomeu[i]) && (qtdbateu[j] == qtdbateu[i]) && (totalmov[j] == totalmov[i]) && (movimentos[j] < movimentos[i]))){
                    
                    char auxmov = movimentos[i];   
                    movimentos[i] = movimentos[j];
                    movimentos[j] = auxmov;    //troca a posicao do movimento

                    int auxcomeu = qtdcomeu[i];
                    qtdcomeu[i] = qtdcomeu[j];
                    qtdcomeu[j] = auxcomeu;   //acerta a posicao da qtd que comeu do movimento

                    int auxbateu = qtdbateu[i];
                    qtdbateu[i] = qtdbateu[j];
                    qtdbateu[j] = auxbateu;  //acerta a posicao da qtd que bateu do movimento

                    int auxqtd = totalmov[i];
                    totalmov[i] = totalmov[j];
                    totalmov[j] = auxqtd; // acerta a posicao da qtd total do movimento
            }
        }
    }
   sprintf(caminhorank,"%s/saida/ranking.txt",caminho);
    FILE*ranking = fopen(caminhorank, "w");
    for(i=0; i<4; i++){
        fprintf(ranking,"%c,%d,%d,%d\n",movimentos[i],qtdcomeu[i],qtdbateu[i],totalmov[i]); //printa o ranking
    }
    fclose(ranking);
}