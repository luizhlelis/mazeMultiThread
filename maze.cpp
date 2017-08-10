// TP 1 ATR (Maze)- Luiz Henrique Silva Lelis 2014034847

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <thread>
#include <vector>

using namespace std;

#define upperLimit 0
#define leftLimit 0

int row = 0;
int col = 0;
bool exitFound = false;
vector<vector<char>> mazeMatrix;

void walkInsideMaze(int y, int x);
void printMaze();
int iterateMaze(int* beginRow, int* beginCol);
void wayAhead(bool* currentThread, int* nextY, int* nextX, vector<vector<char>> mazeMatrix, int newWayY, int newWayX);

int main(){
    int beginRow,beginCol;

    // Preencher o labirinto(mazeMatrix)
    if(!iterateMaze(&beginRow,&beginCol)){
        // Arquivo não foi lido
        return 0;
    }
    // Thread que imprime o labirinto(mazeMatrix)
    std::thread updateScreen(printMaze);
    updateScreen.detach();
    // Thread para caminhar pelo labirinto(mazeMatrix)
    std::thread walk(walkInsideMaze,beginRow,beginCol);
    walk.detach();
    // Enquanto a saida do labirinto não for encontrada, o programa não termina.
	while(!exitFound);
	// Aguarda antes de encerrar
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
	
	return 0;
}

void walkInsideMaze(int y, int x){
    
    /* O labirinto esta' sendo representado no quarto quadrante do palno cartesiano, dessa forma
    as colunas sao o x do plano e as linhas o y */

    // Caso a saida seja encontrada
	if(mazeMatrix[y][x] == 's'){
        exitFound = true;
    }

    // 'o' representa a posição atual
	mazeMatrix[y][x] =  'o';

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Variaveis para fazer com que a thread atual continue em seu caminho
	bool currentThread = false;
    int nextY, nextX;

    // Testa as possiveis rotas
    if(y > upperLimit){
        // Para cima
        wayAhead(&currentThread, &nextY, &nextX, mazeMatrix, y-1, x);
    }
	if((y+1) < row){
        // Para baixo
        wayAhead(&currentThread, &nextY, &nextX, mazeMatrix, y+1, x);
    }
    if(x > leftLimit){
        // Para a esquerda
        wayAhead(&currentThread, &nextY, &nextX, mazeMatrix, y, x-1);
    }
	if((x+1) < col){
	    // Para direita
        wayAhead(&currentThread, &nextY, &nextX, mazeMatrix, y, x+1);
    }
    if(currentThread){
        // Continua no mesmo caminho, sem criar uma nova thread.
        walkInsideMaze(nextY,nextX);
    }

    // Atribui posição percorrida
	mazeMatrix[y][x] = '-';
}

void wayAhead(bool* currentThread, int* nextY, int* nextX, vector<vector<char>> mazeMatrix, int newWayY, int newWayX){
    
    // Testa se ha' caminho na direcao desejada
    if(mazeMatrix[newWayY][newWayX] == 'x' || mazeMatrix[newWayY][newWayX] == 's'){
        if(!currentThread){
            // Thread atual continua seu caminho, sem criar nova thread
            *currentThread = true;
            *nextY = newWayY;
            *nextX = newWayX;
        }else{
            // Cria nova thread na nova direcao
            std::thread newWayAhead(walkInsideMaze,newWayY,newWayX);
            newWayAhead.detach();
        }
    }    
}

void printMaze(){
    // Imprime a matriz

    int i,j;
    while(!exitFound){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
        for(i=0; i < row; i++){
            for(j=0; j< col; j++){
                std::cout << mazeMatrix[i][j];
            }
            std::cout << "\n";
        }
        std::cout << "\n\n";
    }
}

int iterateMaze(int* beginRow, int* beginCol){
    // Preenche a matriz lendo o arquivo de entrada

    ifstream file;
    int i,j;

    // Abre o arquivo 'maze.txt'
    file.open("maze.txt");
    if(!file.good()){
        // Arquivo não foi aberto corretamente
        std::cout << "Nao foi possivel abrir arquivo 'maze.txt'";
        return 0;
    }

    /*  Copia a primeira linha do arquivo que contém o número de linha
        e colunas da matriz para as respectivas variaveis globais */
    file >> row >> col;

    // Aloca uma matriz com a dimensão lida no arquivo
    mazeMatrix.resize(row);
    for (i = 0; i < row; i++){
        mazeMatrix[i].resize(col);
    }

    // Copia a matriz do arquivo para a matriz alocada
    for(i=0; i < row; i++){
        for(j=0; j< col; j++){
            file >> mazeMatrix[i][j];

            // Entrada da matriz
            if(mazeMatrix[i][j] == 'e'){
                *beginRow = i;
                *beginCol = j;
            }
        }
    }

    file.close();
    return 1;
}