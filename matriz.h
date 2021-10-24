typedef struct celula Cell;
typedef struct lista ListaEncadeada;
typedef struct matriz MatrizEsparsa;

//Funções da Cell
//static Cell* criar_nova_celula(int item, int linha, int coluna);

//Funções da Matriz Esparsa
MatrizEsparsa* criar_matriz_esparsa(unsigned int numeroLinhas, unsigned int numeroColunas);
//static int validar_posicao_matriz_esparsa(int linha, int coluna, MatrizEsparsa* a);
int buscar_item(int linha, int coluna, MatrizEsparsa *a);
//static Cell* obter_celula_anterior_a_da_linha_na_coluna(int linha, ListaEncadeada *coluna);
bool inserir_remover_trocar_item_na_matriz(int item, int linha, int coluna, MatrizEsparsa *a);
void imprimir_matriz_esparsa(MatrizEsparsa *a);
MatrizEsparsa* converter_bidimensonal_para_esparsa(int **a, int numeroLinhas, int numeroColunas);
void imprimir_esparsa_teste(MatrizEsparsa *a);
bool liberar_matriz_esparsa(MatrizEsparsa *a);

//Funções Matriz bidimensional
int** criar_matriz_bidimensional(int nLinhas, int nColunas);
bool verificar_se_matriz_diagonal(MatrizEsparsa *a);

//Funções do jogo da velha
MatrizEsparsa* criar_campo_jogo_da_velha();
void imprimir_campo(MatrizEsparsa *a);
int inserir_jogada_no_campo(int jogada, int linha, int coluna, MatrizEsparsa *a);
int verificar_diagonais(MatrizEsparsa *a);
int verificar_vitoria(MatrizEsparsa *a);
int verifica_se_campo_cheio(MatrizEsparsa *a);
