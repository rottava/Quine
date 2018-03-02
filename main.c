#include <stdio.h>
#include <stdlib.h>

struct Data{
    unsigned char *minte;
    unsigned int *cober;
    unsigned int taman;
};

struct Data *matriz;
unsigned int linhas;
unsigned int colunas;

void mintermo(int pos, int linha);
void printMatriz();
void printResult();
void associaMintermos();

int main(){
    int aux, i, j, mask;

    colunas = 1;
    linhas = 0;

    while(colunas != 0){
        printf("Para sair digite 0\nNumero de variaveis: ");
        scanf("%u", &colunas);

        if(colunas != 0){
            printf("Numero de linhas: ");
            scanf("%u", &linhas);
            matriz = calloc(2<<colunas, sizeof(struct Data));
            if(matriz == NULL) {
                printf("Erro, Memoria insuficiente para a tabela verdade!\n");
                return -1;
            }
            for(i = 0; i < 2<<colunas; i++){
                matriz[i].minte = calloc(colunas, sizeof(char));
                if(matriz[i].minte == NULL) {
                    printf("Erro, Memoria insuficiente para a tabela verdade!\n");
                    return -1;
                }
                matriz[i].cober = calloc(linhas, sizeof(int));
                if(matriz[i].cober == NULL) {
                    printf("Erro, Memoria insuficiente para a tabela verdade!\n");
                    return -1;
                }
                matriz[i].taman = 0;
            }
            for(i = 0; i < linhas; i++){
                printf("Linha da tabela verdade: ");
                scanf("%d", &aux);

                for(j = 0; j < colunas; j++){
                    mask = aux >> (colunas-1-j);
                    if(mask & 1){
                       matriz[i].minte[j] = '1';
                    }

                    else{
                        matriz[i].minte[j] = '0';
                    }
                }

                matriz[i].cober[0] = aux;
                matriz[i].taman = 1;
            }
            printf("\nTabela de entrada: \n");
            printMatriz();
            associaMintermos();
            printf("\nTabela de saida: \n");
            printMatriz();
            printf("\nFuncao minimizada: ");
            printResult();
        }
    }

    return 0;
}

void printMatriz(){
    unsigned int i, j;

    j = 'A';

    for(i = 0; i < colunas; i++){
        printf("%c ",j);
        j++;
    }

    printf("\n");

    for(i = 0; i < linhas; i++) {
        for(j = 0; j < colunas; j++) {
            printf("%c ", matriz[i].minte[j]);
        }

        printf("\n");
    }
}

void printResult(){
    unsigned int i, j;

    printf("\n");

    for(i = 0; i < linhas; i++) {
        for(j = 0; j < colunas; j++) {
            if(matriz[i].minte[j] == '0'){
                printf("!%c", 'A'+j);

            }
            if(matriz[i].minte[j] == '1'){
                printf("%c", 'A'+j);
            }
        }

        if(i < linhas-1){
            printf(" + ");
        }
    }
    printf("\n\n");
}

void associaMintermos(){
    int aux1, aux2, *cobertura, i, j, k, l, ok, pass, temp, val;
    struct Data *matrizAux;
    char *matrizVal, *nova;

    nova = calloc(colunas, sizeof(char));
    if(nova == NULL){
        printf("Erro, Memoria insuficiente para a tabela verdade!\n");
		exit(-1);
    }

    matrizAux = calloc(2<<colunas, sizeof(struct Data));
    if(matrizAux == NULL){
        printf("Erro, Memoria insuficiente para a tabela verdade!\n");
		exit(-1);
    }
    temp = 0;
    val = 0;
    matrizVal = calloc(2<<colunas, sizeof(int));

    if(matrizVal == NULL){
        printf("Erro, Memoria insuficiente para a tabela verdade!\n");
		exit(-1);
    }

    for(i = 0; i < 2<<colunas; i++){
        matrizAux[i].minte = calloc(colunas, sizeof(char));
        for(j = 0; j < colunas; j++){
            matrizAux[i].minte[j] = '.';
        }
        matrizAux[i].cober = calloc(linhas, sizeof(int));
        if(matrizAux[i].cober == NULL) {
            printf("Erro, Memoria insuficiente para a tabela verdade!\n");
            exit(-1);
        }
        matrizAux[i].taman = 0;
    }

    for(i = 0; i < 2<<colunas; i++){
        matrizVal[i] = '0';
    }

    while(pass != 0){
        pass = 0;
        //Minimiza tabela
        for(i = 0; i < linhas; i++){
            for(j = (i+1); j < linhas; j++){
                aux1 = 0;

                for(k = 0; k < colunas; k++){
                    if(matriz[i].minte[k] == matriz[j].minte[k]){
                        aux1++;
                    }
                    else{
                        aux2 = k;
                    }
                }
                printf("%s | %s\n", matriz[i].minte, matriz[j].minte);
                if(aux1 == colunas-1){
                    matrizVal[i] = '1';
                    matrizVal[j] = '1';
                    ok = 0;
                    for(k = 0; k < colunas; k++){
                        nova[k] = matriz[j].minte[k];
                    }
                    nova[aux2] = '-';
                    //Verifica se ja existe na matriz
                    for(k = 0; k < val; k++){
                        aux2 = 0;
                        for(l = 0; l < colunas; l++){
                            if(nova[l] == matrizAux[k].minte[l]){
                                aux2++;
                            }
                        }
                        if(aux2 == colunas){
                            k = val;
                            ok = 1;
                        }
                    }
                    //Se nao existe
                    if (ok == 0){
                        for(k = 0; k < colunas; k++){
                            matrizAux[val].minte[k] = nova[k];
                        }
                        for(k = 0; k < matriz[i].taman; k++){
                            matrizAux[val].cober[k] = matriz[i].cober[k];
                        }
                        for(k = 0; k < matriz[j].taman; k++){
                            matrizAux[val].cober[matriz[i].taman + k] = matriz[j].cober[k];
                        }
                        matrizAux[val].taman = matriz[i].taman + matriz[j].taman;
                        val++;
                        pass++;
                        printf("Sai : %s | %s\n", matriz[i].minte, matriz[j].minte);
                        printf("Nova: %s\n", nova);
                    }
                }
            }
        }

        //Copia valores não reduzidos para matriz auxiliar
        for(i = 0; i < linhas; i++){
            if(matrizVal[i] == '0'){
                for(j = 0; j < colunas; j++){
                    matrizAux[val].minte[j] = matriz[i].minte[j];
                }
                for(j = 0; j < matriz[i].taman; j++){
                    matrizAux[val].cober[j] = matriz[i].cober[j];
                }
                matrizAux[val].taman = matriz[i].taman;
                val++;
            }
        }

        linhas = val;

        if(temp != val){
            temp = val;
            //Prepara para nova etapa
            for(i = 0; i < linhas; i++){
                //Copia da matriz auxiliar para matriz original as minimizacoes
                for(j = 0; j < colunas; j++){
                    matriz[i].minte[j] = matrizAux[i].minte[j];
                }
                for(j = 0; j < matrizAux[i].taman; j++){
                    matriz[i].cober[j] = matrizAux[i].cober[j];
                }
                matriz[i].taman = matrizAux[i].taman;
                matrizVal[i] = '0';
            }
            val = 0;
            pass++;
        }
    }
    ///HERE

    //Descobre linhas da cobertura e aloca vetor de acordo com tamanho
    aux1 = 0;
    for(i = 0; i < linhas; i++){
        aux1 += matrizAux[i].taman;
    }
    cobertura = calloc(aux1, sizeof(int));
    if(cobertura == NULL){
        printf("Erro, Memoria insuficiente para a tabela verdade!\n");
        exit(-1);
    }
    //Verifica cobertura
    for(i = 0; i < linhas; i++){
        aux1 = 0;
        aux2 = 0;
        //Cria cobertura sem mintermo atual
        for(j = 0; j < linhas; j++){
            if(i != j){
                for(k = 0; k < matrizAux[j].taman; k++){
                    cobertura[aux1] = matrizAux[j].cober[k];
                    aux1++;
                }
            }
        }
        for(j = 0; j < matrizAux[i].taman; j++){
            for(k = 0; k < aux1; k++){
                if(matrizAux[i].cober[j] == cobertura[k]){
                    aux2++;
                    k = aux1;
                }
            }
        }
        //Significa que todos ja estao cobertos por outros
        if(aux2 == matrizAux[i].taman){
            for(j = i; j < linhas-1; j++){
                matrizAux[j].cober = matrizAux[j+1].cober;
                matrizAux[j].minte = matrizAux[j+1].minte;
                matrizAux[j].taman = matrizAux[j+1].taman;
            }
            linhas--;
            i--;
        }
    }

    //Copia para matriz de retorno
    for(i = 0; i < linhas; i++){
        for(j = 0; j < colunas; j++){
            matriz[i].minte[j] = matrizAux[i].minte[j];
        }
        for(j = 0; j < matrizAux[i].taman; j++){
            matriz[i].cober[j] = matrizAux[i].cober[j];
        }
        matriz[i].taman = matrizAux[i].taman;
    }
}
