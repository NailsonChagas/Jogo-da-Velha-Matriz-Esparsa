#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "matriz.h"

//Declaração de celula
struct celula{
    int valor;
    int CoordenadaLinha, CoordenadaColuna;
    Cell *next;
    Cell *down;
};

//Declaração de lista encadeada
struct lista{
    Cell *head;
};

//Declaração da matriz
struct matriz{
    int quantidadeDeLinhas;
    int quantidadeDeColunas;
    ListaEncadeada **linhas;
    ListaEncadeada **colunas;
};

//Alocar matriz esparsa
MatrizEsparsa* criar_matriz_esparsa(unsigned int numeroLinhas, unsigned int numeroColunas){
    MatrizEsparsa *a;
    int i;

    a = (MatrizEsparsa*)malloc(sizeof(MatrizEsparsa));

    a->quantidadeDeLinhas = numeroLinhas;
    a->quantidadeDeColunas = numeroColunas;

    a->linhas = (ListaEncadeada**)malloc(sizeof(ListaEncadeada*) * numeroLinhas);
    a->colunas = (ListaEncadeada**)malloc(sizeof(ListaEncadeada*) * numeroColunas);

    if(numeroColunas == numeroLinhas){
        for(i=0; i<numeroLinhas; i++){
            a->linhas[i] = (ListaEncadeada*)malloc(sizeof(ListaEncadeada));
            a->linhas[i]->head = NULL;

            a->colunas[i] = (ListaEncadeada*)malloc(sizeof(ListaEncadeada));
            a->colunas[i]->head = NULL;
        }
    }
    else{
        for(i=0; i<numeroLinhas; i++){
            a->linhas[i] = (ListaEncadeada*)malloc(sizeof(ListaEncadeada));
            a->linhas[i]->head = NULL;
        }

        for(i=0; i<numeroColunas; i++){
            a->colunas[i] = (ListaEncadeada*)malloc(sizeof(ListaEncadeada));
            a->colunas[i]->head = NULL;
        }
    }
    return a;
}

//Alocar um celula
static Cell* criar_nova_celula(int item, int linha, int coluna){
    Cell *a = NULL;

    if(item != 0){
        a = (Cell*)malloc(sizeof(Cell));

        a->CoordenadaColuna = coluna;
        a->CoordenadaLinha = linha;
        a->valor = item;

        a->next = NULL;
        a->down = NULL;
    }
    return a;
}

//Verificar se a posição possui uma celula
static int validar_posicao_matriz_esparsa(int linha, int coluna, MatrizEsparsa* a){
    if((a != NULL) && (linha >= 0) && (coluna >= 0) && (linha < a->quantidadeDeLinhas) && (coluna < a->quantidadeDeColunas)){
        return 1;
    }
    return 0;
}

//Buscar item na matriz
int buscar_item(int linha, int coluna, MatrizEsparsa *a){
    Cell *aux;

    if(validar_posicao_matriz_esparsa(linha, coluna, a) == 1){
        aux = a->linhas[linha]->head;

        while((aux != NULL) && (aux->CoordenadaColuna < coluna)){
            aux = aux->next;
        }

        if((aux != NULL) && (aux->CoordenadaColuna == coluna)){
            return aux->valor;
        }
    }
    return 0;
}

//Obter ultima célula antes da que esta va posição (l, c) -> retorna (l-1, c)
static Cell* obter_celula_anterior_a_da_linha_na_coluna(int linha, ListaEncadeada *coluna){
    Cell *auxA, *auxB = NULL;

    if((linha >= 0) && (coluna != NULL)){
        auxA = coluna->head;

        while((auxA != NULL) && (auxA->CoordenadaLinha < linha)){
            auxB = auxA;
            auxA = auxA->down;
        }
    }
    return auxB;
}

//Inserir, remover ou trocar valor de um item na Matriz
bool inserir_remover_trocar_item_na_matriz(int item, int linha, int coluna, MatrizEsparsa *a){
    Cell *posicaoNaLinhaAnterior, *posicaoNaLinhaAtual, *posicaoAnteriorDaColuna, *nova;

    if((a == NULL) || (coluna < 0) || (linha < 0) || (linha >= a->quantidadeDeLinhas) || (coluna >= a->quantidadeDeColunas)){
        return false;
    }
    else{
        if(validar_posicao_matriz_esparsa(linha, coluna, a) == 1){
            posicaoNaLinhaAnterior = NULL;
            posicaoNaLinhaAtual = a->linhas[linha]->head;

            while((posicaoNaLinhaAtual != NULL) && (posicaoNaLinhaAtual->CoordenadaColuna < coluna)){
                posicaoNaLinhaAnterior = posicaoNaLinhaAtual;
                posicaoNaLinhaAtual = posicaoNaLinhaAtual->next;
            }

            if(item != 0){
                if((posicaoNaLinhaAtual != NULL) && (posicaoNaLinhaAtual->CoordenadaColuna == coluna)){ //Existe uma célula na posição
                    posicaoNaLinhaAtual->valor = item;
                    return true;
                }
                else{ // Não existe a célula na posição, então deve ser alocada
                    nova = criar_nova_celula(item, linha, coluna);
                    posicaoAnteriorDaColuna = obter_celula_anterior_a_da_linha_na_coluna(linha, a->colunas[coluna]);
                    //lidando com a linha
                    if(posicaoNaLinhaAnterior != NULL){
                        nova->next = posicaoNaLinhaAnterior->next;
                        posicaoNaLinhaAnterior->next = nova;
                    }
                    else{ //primero elemnto na linha
                        nova->next = a->linhas[linha]->head;
                        a->linhas[linha]->head = nova;
                    }

                    //linha com a coluna
                    if(posicaoAnteriorDaColuna != NULL){
                        nova->down = posicaoAnteriorDaColuna->down;
                        posicaoAnteriorDaColuna->down = nova;
                    }
                    else{//primeiro elemento na coluna
                        nova->down = a->colunas[coluna]->head;
                        a->colunas[coluna]->head = nova;
                    }
                    return true;
                }
            }
            else if((posicaoNaLinhaAtual != NULL) && (posicaoNaLinhaAtual->CoordenadaColuna == coluna)){ //item == 0 -> remover espaço
                /*posicaoAnteriorDaColuna = obter_celula_anterior_a_da_linha_na_coluna(linha, a->colunas[coluna]);
                posicaoAnteriorDaColuna->down = posicaoNaLinhaAtual->down;
                posicaoNaLinhaAnterior->next = posicaoNaLinhaAtual->next;

                free(posicaoNaLinhaAtual);*/

                posicaoAnteriorDaColuna = obter_celula_anterior_a_da_linha_na_coluna(linha, a->colunas[coluna]);

                if(posicaoAnteriorDaColuna != NULL){
                    posicaoAnteriorDaColuna->down = posicaoNaLinhaAtual->down;
                }
                else{
                    a->colunas[coluna]->head = posicaoNaLinhaAtual->down;
                }

                if(posicaoNaLinhaAnterior != NULL){
                    posicaoNaLinhaAnterior->next = posicaoNaLinhaAtual->next;
                }
                else{
                    a->linhas[linha]->head = posicaoNaLinhaAtual->next;
                }

                free(posicaoNaLinhaAtual);

                return true;
            }
        }
        return false;
    }
}

//Imprimir matriz esparsa
void imprimir_matriz_esparsa(MatrizEsparsa *a){
    int i, j;

    for(i=0; i < a->quantidadeDeLinhas; i++){
        for(j=0; j < a->quantidadeDeColunas; j++){
            printf("%d   ", buscar_item(i, j, a));
        }
        printf("\n\n");
    }
}

//Criar matriz bidemensional
int** criar_matriz_bidimensional(int nLinhas, int nColunas){
    int **a;
    int i;

    a = (int**)malloc(sizeof(int*) * nLinhas);

    for(i=0; i<nLinhas; i++){
        a[i] = (int*)malloc(sizeof(int) * nColunas);
    }

    return a;
}

//Converter matriz bidimensional para esparsa
MatrizEsparsa* converter_bidimensonal_para_esparsa(int **a, int numeroLinhas, int numeroColunas){
    MatrizEsparsa *b;
    int i, j;

    b = criar_matriz_esparsa(numeroLinhas, numeroColunas);

    for(i=0; i<numeroLinhas; i++){
        for(j=0; j<numeroColunas; j++){
            inserir_remover_trocar_item_na_matriz(a[i][j], i, j, b);
        }
    }

    return b;
}

//Imprimir como esta no exercicio do hackerrank
void imprimir_esparsa_teste(MatrizEsparsa *a){
    int i, j, cont;

    for(i=0; i < a->quantidadeDeLinhas; i++){
        printf("%d: -> ", i);
        cont = 0;
        for(j=0; j < a->quantidadeDeColunas; j++){
            if(buscar_item(i, j, a) != 0){
                if(cont == 0){
                    printf("%d,%d ", j, buscar_item(i, j, a));
                }
                else{
                    printf("-> %d,%d ", j, buscar_item(i, j, a));
                }
                cont++;
            }
        }
        printf("\n");
    }
}

//Desalocar matriz esparsa
bool liberar_matriz_esparsa(MatrizEsparsa *a){
    int i;
    Cell *atualDaLinha;

    if(a != NULL){
        //Desalocando linhas
        for(i=0; i<a->quantidadeDeLinhas; i++){
            atualDaLinha = a->linhas[i]->head;
            while(atualDaLinha != NULL){
                a->linhas[i]->head = atualDaLinha->next;

                free(atualDaLinha);

                atualDaLinha = a->linhas[i]->head;
            }
        }
        free(a->linhas);
        free(a->colunas);
        free(a);
    }
    return false;
}

//Verificar se a matriz é diagonal
bool verificar_se_matriz_diagonal(MatrizEsparsa *a){
    int i, j;

    if(a->quantidadeDeLinhas == a->quantidadeDeColunas){
        for(i=0; i<a->quantidadeDeLinhas; i++){
            for(j=0; j<a->quantidadeDeColunas; j++){
                if(i != j){
                    if(buscar_item(i,j,a) != 0){
                        return false;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

/*JOGO DA VELHA*/

//Criar campo do jogo da velha
MatrizEsparsa* criar_campo_jogo_da_velha(){
    MatrizEsparsa *campo;
    int i;

    campo = (MatrizEsparsa*)malloc(sizeof(MatrizEsparsa));

    campo->quantidadeDeLinhas = 3;
    campo->quantidadeDeColunas = 3;

    campo->linhas = (ListaEncadeada**)malloc(sizeof(ListaEncadeada*) * 3);
    campo->colunas = (ListaEncadeada**)malloc(sizeof(ListaEncadeada*) * 3);

    for(i=0; i<3; i++){
        campo->linhas[i] = (ListaEncadeada*)malloc(sizeof(ListaEncadeada));
        campo->linhas[i]->head = NULL;

        campo->colunas[i] = (ListaEncadeada*)malloc(sizeof(ListaEncadeada));
        campo->colunas[i]->head = NULL;
    }

    return campo;
}

void imprimir_campo(MatrizEsparsa *a){
    Cell *auxL;
    int i, j;

    printf("JOGO DA VELHA:\n");

    printf("      0     1     2     \n\n");
    for(i = 0; i < a->quantidadeDeLinhas; i++){ // percorre as linhas
        auxL = a->linhas[i]->head;
        j = 0;
        printf("%d     ", i);
        while(auxL != NULL){ // enquanto o auxL for diferente de nulo
            while(j < auxL->CoordenadaColuna){ // enquanto a coordenada da coluna do auxL for diferente de j
                printf(".     ");
                j++;
            }

            if(auxL->valor == 1){// quando a coordenada do j e do auxL forem iguais
                printf("X     ");
            }
            else if(auxL->valor == 2){// quando a coordenada do j e do auxL forem iguais
                printf("O     ");
            }
            auxL = auxL->next;
            j++;
        }
        while(j < a->quantidadeDeColunas){ // caso a linha nao tiver nenhum elemento
            printf(".     ");
            j++;
        }

        printf("\n\n");
    }
}

int inserir_jogada_no_campo(int jogada, int linha, int coluna, MatrizEsparsa *a){
    Cell *posicaoNaLinhaAnterior, *posicaoNaLinhaAtual, *posicaoAnteriorDaColuna, *nova;

    if((a == NULL) || (coluna < 0) || (linha < 0) || (linha >= a->quantidadeDeLinhas) || (coluna >= a->quantidadeDeColunas)){
        return -1;
    }
    else{
        if(validar_posicao_matriz_esparsa(linha, coluna, a) == 1){
            posicaoNaLinhaAnterior = NULL;
            posicaoNaLinhaAtual = a->linhas[linha]->head;

            while((posicaoNaLinhaAtual != NULL) && (posicaoNaLinhaAtual->CoordenadaColuna < coluna)){
                posicaoNaLinhaAnterior = posicaoNaLinhaAtual;
                posicaoNaLinhaAtual = posicaoNaLinhaAtual->next;
            }

            if(jogada == 1 || jogada == 2){
                if((posicaoNaLinhaAtual != NULL) && (posicaoNaLinhaAtual->CoordenadaColuna == coluna)){ //Existe uma célula na posição -> Não inserir
                    return 0;
                }
                else{ // Não existe a célula na posição, então deve ser alocada
                    nova = criar_nova_celula(jogada, linha, coluna);
                    posicaoAnteriorDaColuna = obter_celula_anterior_a_da_linha_na_coluna(linha, a->colunas[coluna]);
                    //lidando com a linha
                    if(posicaoNaLinhaAnterior != NULL){
                        nova->next = posicaoNaLinhaAnterior->next;
                        posicaoNaLinhaAnterior->next = nova;
                    }
                    else{ //primero elemnto na linha
                        nova->next = a->linhas[linha]->head;
                        a->linhas[linha]->head = nova;
                    }

                    //linha com a coluna
                    if(posicaoAnteriorDaColuna != NULL){
                        nova->down = posicaoAnteriorDaColuna->down;
                        posicaoAnteriorDaColuna->down = nova;
                    }
                    else{//primeiro elemento na coluna
                        nova->down = a->colunas[coluna]->head;
                        a->colunas[coluna]->head = nova;
                    }
                    return 1;
                }
            }
        }
        return -1;
    }
}

int verificar_diagonais(MatrizEsparsa *a){
    Cell *aux;
    int i, diagonalPrincipalX = 0, diagonalPrincipalO = 0, diagonalSecundariaX = 0, diagonalSecundariaO = 0;

    if(a != NULL){
        for(i=0; i<3; i++){
            aux = a->linhas[i]->head;

            while(aux != NULL){
                if((aux->CoordenadaLinha == aux->CoordenadaColuna) && (aux->valor == 1)){
                    diagonalPrincipalX += 1;
                }
                if((aux->CoordenadaLinha == aux->CoordenadaColuna) && (aux->valor == 2)){
                    diagonalPrincipalO += 1;
                }
                if(((aux->CoordenadaLinha + aux->CoordenadaColuna) == 2) && (aux->valor == 1)){
                    diagonalSecundariaX += 1;
                }
                if(((aux->CoordenadaLinha + aux->CoordenadaColuna) == 2) && (aux->valor == 2)){
                    diagonalSecundariaO += 1;
                }

                aux = aux->next;
            }

            if((diagonalPrincipalX == 3) || (diagonalSecundariaX == 3)){
                return 1; //jogador 1 venceu
            }
            if((diagonalPrincipalO == 3) || (diagonalSecundariaO == 3)){
                return 2; //jogador 2 venceu
            }
        }
        return 0; //ninguem venceu ainda
    }
    return -1;
}

int verificar_vitoria(MatrizEsparsa *a){
    Cell *aux;
    int contColunaX, contColunaO, contLinhaX, contLinhaO, i;

    if(a != NULL){
        if(verificar_diagonais(a)==1){
            return 1;
        }
        else if(verificar_diagonais(a)==2){
            return 2;
        }
        else if(verificar_diagonais(a)==0){
            for(i = 0; i < 3; i++){
                //verificando linhas
                aux = a->linhas[i]->head;

                contLinhaO = 0;
                contLinhaX = 0;

                while(aux != NULL){
                    if(aux->valor == 1){
                        contLinhaX += 1;
                    }
                    else if(aux->valor == 2){
                        contLinhaO += 1;
                    }

                    aux = aux->next;
                }

                //verificando colunas
                aux = a->colunas[i]->head;

                contColunaX = 0;
                contColunaO = 0;

                while(aux != NULL){
                    if(aux->valor == 1){
                        contColunaX += 1;
                    }
                    else if(aux->valor == 2){
                        contColunaO += 1;
                    }

                    aux = aux->down;
                }

                if(contLinhaX == 3 || contColunaX == 3){
                    return 1; // 1 venceu
                }
                else if(contLinhaO == 3 || contColunaO == 3){
                    return 2; // 2 venceu
                }
            }
        }
        return 0;
    }
    return -1;
}

int verifica_se_campo_cheio(MatrizEsparsa *a){
    int i, cont = 0;
    Cell *aux;

    for(i=0; i<3; i++){
        aux = a->linhas[i]->head;

        while(aux != NULL){
            aux = aux->next;
            cont += 1;
        }
    }

    if(cont == 9){
        return 1;
    }
    return 0;
}

