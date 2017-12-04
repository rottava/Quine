#include <stdio.h>
#include <stdlib.h>

unsigned short int init();
char **criaMatriz(unsigned short int colunas, unsigned int linhas);
char *mintermo(unsigned short int linha, unsigned short int colunas, unsigned int linhas);
void printMatriz(unsigned short int colunas, unsigned int linhas, char** matriz);
void associaMintermos(unsigned short int colunas, unsigned int *linhas, char **matriz);
void printResult(unsigned short int colunas, unsigned int linhas, char **matriz);
void copiaMatriz(unsigned short int colunas, unsigned short int linhasA, unsigned short int linhasB, char **matriz, char **MatrizAux);

int main(){
    unsigned short int colunas;
    unsigned int aux1, aux2, linhas, sdp;
    char **matriz, **matrizAux, *mintermos;

    colunas = 1;

    while(colunas > 0){
        colunas = init();                                               //A,B,C,D...
        aux2 = 0;
        linhas = 2 << (colunas - 1);                                    //2^Numero de variaveis
        sdp = 0;
        matriz = criaMatriz(colunas, linhas);
        printf("Insira as linhas com valor 1 da tabela verdade.\nInsira 0 para encerrar.\n");

        while (aux2 == 0){                                              //Le numero da linha soma de produto
            scanf("%d", &aux1);

            if(aux1 == 0){
                aux2++;
            }

            else if(aux1 > linhas){
                printf("Essa linha nao existe na tabela!\n");
            }

            else{
                mintermos = mintermo(aux1, colunas, linhas);            //Gera mintermo apartir da tabela verdade
                for(aux1 = 0; aux1 < colunas; aux1++){
                    matriz[sdp][aux1] = mintermos[aux1];                //Salva mintermo na matriz
                }
                free(mintermos);
                sdp++;
            }
        }

        if(sdp < linhas){                                               //Realoca o tamanho da matriz caso use menos espaço que o esperado
            matrizAux = criaMatriz(colunas, sdp);
            copiaMatriz(colunas, sdp, linhas, matriz, matrizAux);
        }

        printf("\nMatriz de entrada\n");
        printMatriz(colunas, sdp, matriz);                              //Imprime tabela de entrada
        associaMintermos(colunas, &sdp, matriz);
        printf("\nMatriz de saida\n");
        printMatriz(colunas, sdp, matriz);                              //Imprime tabela de entrada
        printResult(colunas, sdp, matriz);
    }

    return 0;
}

unsigned short int init(){
    unsigned int colunas;

    colunas = 27;

    while(colunas > 26){
        printf("Insira o numero de variaveis.\nInsira 0 para encerrar.\n");
        scanf("%d", &colunas);

        if(colunas > 26){
            printf("Nao pode-se calcular com mais de 26 variaveis, tente novamente.\n");
        }

        else if(colunas == 0){
            printf("Saindo do programa...");
            exit(0);
        }

    }

    return colunas;
}

char **criaMatriz(unsigned short int colunas, unsigned int linhas){
    char **matriz;
    unsigned int i, j;

	matriz = calloc(linhas, sizeof(char*));               //Aloca uma matriz na memoria

	if(matriz == NULL){
		printf("Erro, Memoria insuficiente para a tabela verdade!\n");
		exit(-1);
	}

	else{
		for(i = 0; i < linhas; i++){
			matriz[i] = calloc(colunas, sizeof(char));      //Aloca um vetor de caracteres para cada posição do vetor de ponteiros.

			if(matriz[i] == NULL){
				printf("Erro, Memoria insuficiente para tabela verdade!\n");
				exit(-1);
			}

			else{
				for (j = 0; j < colunas; j++){
					matriz[i][j] = 0; 							    //Inicializa cada posição do vetor de caracteres com 0.
                }
			}
		}
	}

	return matriz;
}

char *mintermo(unsigned short int linha, unsigned short int colunas, unsigned int linhas){
    char *mint;
    int i, mask;

    mint = calloc(colunas, sizeof(char));

    for(i = 0; i < colunas; i++){
        mask = linhas >> (i+1);
        if((linha-1) & mask){
           mint[i] = '1';
        }

        else{
            mint[i] = '0';
        }
    }

    return mint;
}

void printMatriz(unsigned short int colunas, unsigned int linhas, char** matriz){
    unsigned int i, j;

    j = 'A';

    for(i = 0; i < colunas; i++){
        printf("%c ",j);
        j++;
    }

    printf("|SDP\n");

    for(i = 0; i < linhas; i++) {
        for(j = 0; j < colunas; j++) {
            printf("%c ", matriz[i][j]);
        }

        printf("| 1\n");
    }
}

void associaMintermos(unsigned short int colunas, unsigned int *linha, char **matriz){
    int aux1, aux2, i, j, k, linhas, linhasRe, pass;
    char **matrizAux;

    linhas = *linha;
    matrizAux = criaMatriz(colunas, linhas);
    linhasRe = 0;

    while(pass != 0){
        pass = 0;

        for(i = 0; i < linhas; i++){                                //Copia a matriz
            for(j = 0; j < colunas; j++){
                matrizAux[i][j] = matriz[i][j];
            }
        }

        for(i = 0; i < linhas; i++){
            for(j = (i+1); j < linhas; j++){
                aux1 = 0;

                for(k = 0; k < colunas; k++){
                    if(matrizAux[i][k] == matrizAux[j][k]){
                        aux1++;
                    }

                    else{
                        aux2 = k;
                    }
                }

                if(aux1 == colunas-1 && matriz[i][0] != '.'){
                    for(aux1 = 0; aux1 < colunas; aux1++){
                        matriz[i][aux1] = '.';
                    }
                    matriz[j][aux2] = '-';
                    linhasRe++;
                    pass++;
                }

                else if(aux1 == colunas && matrizAux[i][0] != '.'){
                    for(aux1 = 0; aux1 < colunas; aux1++){
                        matriz[i][aux1] = '.';
                    }
                    linhasRe++;
                }
            }
        }
    }

    for(i = 0; i < linhas; i++){
        free(matrizAux[i]);
    }

    free(matrizAux);
    linhasRe = linhas - linhasRe;
    matrizAux = criaMatriz(colunas, linhasRe);
    aux1 = 0;

    for(i = 0; i < linhas; i++){
        if(matriz[i][0] != '.'){
            for(j = 0; j < colunas; j++){
                matrizAux[aux1][j] = matriz[i][j];
            }
            aux1++;
        }

        free(matriz[i]);
    }

    free(matriz);
    matriz = criaMatriz(colunas, linhasRe);

    for(i = 0; i < linhasRe; i++){
        for(j = 0; j < colunas; j++){
            matriz[i][j] = matrizAux[i][j];
        }

        free(matrizAux[i]);
    }

    free(matrizAux);
    *linha = linhasRe;
}

void printResult(unsigned short int colunas, unsigned int linhas, char** matriz){
    unsigned int i, j;

    j = 'A';
    printf("\n");

    for(i = 0; i < linhas; i++) {
        for(j = 0; j < colunas; j++) {
            if(matriz[i][j] == '0'){
                printf("%c'", 'A'+j);
            }
            else if(matriz[i][j] == '1'){
                printf("%c", 'A'+j);
            }
        }

        if(i < linhas-1){
            printf(" + ");
        }
    }
    printf("\n\n");
}

void copiaMatriz(unsigned short int colunas, unsigned short int linhasA, unsigned short int linhasB, char **matriz, char **matrizAux){
    int aux, i, j;

    aux = 0;

    for(i = 0; i < linhasA; i++){
        if(matriz[i][0] != '.'){
            for(j = 0; j < colunas; j++){
                matrizAux[aux][j] = matriz[i][j];
            }
            aux++;
        }

        free(matriz[i]);
    }

    for(i = linhasA; i < linhasB; i++){
        free(matriz[i]);
    }

    free(matriz);
    matriz = criaMatriz(colunas, linhasA);

    for(i = 0; i < linhasA; i++){
        for(j = 0; j < colunas; j++){
            matriz[i][j] = matrizAux[i][j];
        }

        free(matrizAux[i]);
    }

    free(matrizAux);
}
