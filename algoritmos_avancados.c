#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- 1. Definição da Estrutura do Cômodo (Nó da Árvore Binária) ---

/**
 * @brief Estrutura que representa um cômodo da mansão (um nó na Árvore Binária).
 *
 * Cada cômodo tem um nome e ponteiros para os cômodos adjacentes (esquerda e direita).
 * O ponteiro 'esquerda' representa um caminho para a esquerda, e 'direita' para a direita.
 */
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;


// --- 2. Funções de Criação e Construção da Árvore ---

/**
 * @brief Cria um novo cômodo (nó) para a árvore binária.
 *
 * @param nome O nome do cômodo a ser criado.
 * @return Um ponteiro para o novo nó Sala.
 */
Sala* criarSala(const char *nome) {
    // Aloca memória para a nova sala
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    
    // Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL) {
        perror("Erro ao alocar memória para a sala");
        exit(EXIT_FAILURE);
    }
    
    // Copia o nome para o campo 'nome' da estrutura
    strncpy(novaSala->nome, nome, 49);
    novaSala->nome[49] = '\0'; // Garantir terminação de string
    
    // Inicializa os ponteiros para os filhos como NULL
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * @brief Monta a estrutura da mansão (Árvore Binária).
 *
 * Esta função define estaticamente o mapa do jogo, ligando cada cômodo ao seu caminho esquerdo e direito.
 * @return O ponteiro para o nó raiz da mansão ("Hall de Entrada").
 */
Sala* montarMansao() {
    // Nível 0: Raiz
    Sala *hall = criarSala("Hall de Entrada");

    // Nível 1
    hall->esquerda = criarSala("Sala de Jantar");
    hall->direita = criarSala("Biblioteca");

    // Nível 2
    hall->esquerda->esquerda = criarSala("Cozinha");
    hall->esquerda->direita = criarSala("Quarto Principal");
    hall->direita->esquerda = criarSala("Jardim de Inverno");
    // Nó folha (fim do caminho)
    hall->direita->direita = criarSala("Escritório"); 

    // Nível 3 (Nós folha)
    hall->esquerda->esquerda->esquerda = criarSala("Dispensa");
    hall->esquerda->esquerda->direita = criarSala("Lavandaria");

    // Nó folha
    hall->esquerda->direita->esquerda = criarSala("Banheiro");
    
    // Sub-árvore complexa à direita
    hall->direita->esquerda->direita = criarSala("Sala de Estar");
    hall->direita->esquerda->direita->direita = criarSala("Varanda"); // Nó folha
    
    return hall;
}

// --- 3. Funções de Exploração e Interação ---

/**
 * @brief Permite que o jogador explore a mansão interativamente.
 *
 * O jogador pode escolher 'e' (esquerda), 'd' (direita) ou 's' (sair).
 * A exploração continua até o jogador chegar a um nó folha ou escolher 's'.
 *
 * @param raiz O nó inicial da exploração (normalmente o "Hall de Entrada").
 */
void explorarSalas(Sala *raiz) {
    Sala *atual = raiz;
    char escolha;

    printf("\n--- Início da Exploração Detective Quest ---\n");
    printf("Você está no %s.\n", atual->nome);

    // Continua a exploração enquanto o cômodo atual não for NULL e o jogador não sair
    while (atual != NULL) {
        // Verifica se chegamos a um nó-folha (o fim de um caminho)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\n🎉 Você encontrou o fim do caminho neste percurso: %s.\n", atual->nome);
            break; // Sai do loop
        }

        // Exibe as opções de caminho
        printf("\nOpções de caminho: ");
        if (atual->esquerda) {
            printf("[e] Esquerda ");
        }
        if (atual->direita) {
            printf("[d] Direita ");
        }
        printf("[s] Sair\n");
        
        printf("Sua escolha (e/d/s): ");
        if (scanf(" %c", &escolha) != 1) { // Garante que a entrada é um caractere
            while (getchar() != '\n'); // Limpa o buffer em caso de falha
            continue;
        }

        // Move o jogador com base na escolha
        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda) {
                atual = atual->esquerda;
                printf("Caminhando para: %s\n", atual->nome);
            } else {
                printf("❌ Caminho da esquerda bloqueado ou inexistente aqui.\n");
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita) {
                atual = atual->direita;
                printf("Caminhando para: %s\n", atual->nome);
            } else {
                printf("❌ Caminho da direita bloqueado ou inexistente aqui.\n");
            }
        } else if (escolha == 's' || escolha == 'S') {
            printf("Saindo da exploração. Até a próxima!\n");
            break; // Sai do loop
        } else {
            printf("Opção inválida. Tente 'e', 'd' ou 's'.\n");
        }
    }
}

/**
 * @brief Libera a memória alocada dinamicamente para a árvore.
 *
 * É uma boa prática liberar a memória para evitar memory leaks.
 * Isso é feito usando uma travessia pós-ordem.
 * @param raiz O nó raiz da árvore a ser liberada.
 */
void liberarMansao(Sala *raiz) {
    if (raiz == NULL) {
        return;
    }
    // Libera primeiro