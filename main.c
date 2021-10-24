#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "matriz.h"

int main()
{
    MatrizEsparsa *teste;
    int linha, coluna, jogador, continuar;

    srand(time(NULL));

    while(1){
        teste = criar_campo_jogo_da_velha();

        jogador = rand()%2 + 1;

        printf("Primeiro Turno da Jogada: %d\n\n", jogador);

        while(1){
            if(verifica_se_campo_cheio(teste)==1){
                imprimir_campo(teste);
                printf("\nVELHA!!!\n");
                break;
            }

            printf("Jogador %d\n\n", jogador);

            imprimir_campo(teste);

            if(verificar_vitoria(teste) == 1){
                printf("\nJogador 1 venceu !!!");
                system("clear||cls");
                break;
            }
            else if(verificar_vitoria(teste) == 2){
                printf("\nJogador 2 venceu !!!");
                system("clear||cls");
                break;
            }

            printf("Jogada: (X, Y) -> valor\n");
            printf("Coordenada: ");
            scanf("%d %d", &linha, &coluna);

            inserir_jogada_no_campo(jogador,linha,coluna,teste);

            if(jogador == 2){
                jogador -= 1;
            }
            else{
                jogador += 1;
            }

            system("clear||cls");
        }

        printf("\nDeseja jogar mais uma partida? (1 == Sim / 0 == Nao): ");
        scanf("%d", &continuar);

        if(continuar == 0){
            liberar_matriz_esparsa(teste);
            break;
        }
        else{
            liberar_matriz_esparsa(teste);
            system("clear||cls");
        }
    }

    return 0;
}
