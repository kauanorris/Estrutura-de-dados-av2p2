#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Começamos criando a struct de usuarios onde armazenara os dados do usuarios
typedef struct usuarios{
    char Nome[100];
    int id;
    char email[100];
} usuarios;

// Criamos a struct do No onde armazena um usuarios, ponteiros da direita e da esquerda e a informa~ção da altura
typedef struct No{
    usuarios usuarios;
    struct No *direita;
    struct No *esquerda;
    int altura;
} No; 

usuarios criarusuarios(const char *Nome, int id, const char *email) {
    usuarios Novo;
    strncpy(Novo.Nome, Nome, sizeof(Novo.Nome) - 1);
    Novo.Nome[sizeof(Novo.Nome) - 1] = '\0';
    Novo.id = id;
    strncpy(Novo.email, email, sizeof(Novo.email) - 1);
    Novo.email[sizeof(Novo.email) - 1] = '\0';
    return Novo;
}

// Criamos a função para auxiliar na parte de obter a altura de um nó
int altura(No *no) {
    if (no == NULL)
        return 0;
    return no->altura;
}

// Função auxiliar para calcular o máximo entre dois números
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função para criar um Novo nó 
No* NovoNo(usuarios usuario) {  
    No* novoNo = (No*)malloc(sizeof(No));  
    if (novoNo == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
    novoNo->usuarios = usuario;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->altura = 1;
    return novoNo;
}

// Função para realizar rotação à direita
No* rotacaodireita(No *y) {
    No *x = y->esquerda;
    No *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;

    return x;
}

// Função para realizar rotação à esquerda
No* rotacaoesquerda(No *x) {
    No *y = x->direita;
    No *T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;

    return y;
}

// Função para obter o fator de balanceamento de um nó
int fatorBalanceamento(No *no) {
    if (no == NULL)
        return 0;
    return altura(no->esquerda) - altura(no->direita);
}

// Função para inserir um usuário na árvore AVL
No* inserirusuarios(No* No, usuarios usuarios) {
    if (No == NULL)
        return NovoNo(usuarios);

    int cmp = strcasecmp(usuarios.Nome, No->usuarios.Nome);
    
    if (cmp < 0)
        No->esquerda = inserirusuarios(No->esquerda, usuarios);
    else if (cmp > 0)
        No->direita = inserirusuarios(No->direita, usuarios);
    else // Nomes iguais não são permitidos
        return No;

    No->altura = 1 + max(altura(No->esquerda), altura(No->direita));

    int balance = fatorBalanceamento(No);

    // Caso esquerda-esquerda
    if (balance > 1 && strcasecmp(usuarios.Nome, No->esquerda->usuarios.Nome) < 0)
        return rotacaodireita(No);

    // Caso direita-direita
    if (balance < -1 && strcasecmp(usuarios.Nome, No->direita->usuarios.Nome) > 0)
        return rotacaoesquerda(No);

    // Caso esquerda-direita
    if (balance > 1 && strcasecmp(usuarios.Nome, No->esquerda->usuarios.Nome) > 0) {
        No->esquerda = rotacaoesquerda(No->esquerda);
        return rotacaodireita(No);
    }

    // Caso direita-esquerda
    if (balance < -1 && strcasecmp(usuarios.Nome, No->direita->usuarios.Nome) < 0) {
        No->direita = rotacaodireita(No->direita);
        return rotacaoesquerda(No);
    }

    return No;
}

// Função para encontrar o nó com o valor mínimo 
No* minValorNo(No* no) {  
    if (no == NULL) return NULL;
    No* atual = no;
    while (atual->esquerda != NULL)
        atual = atual->esquerda;
    return atual;
}

// Função para remover um usuário da árvore 
No* removerusuarios(No* raiz, const char* Nome) {
    if (raiz == NULL)
        return raiz;

    int cmp = strcasecmp(Nome, raiz->usuarios.Nome);
    
    if (cmp < 0)
        raiz->esquerda = removerusuarios(raiz->esquerda, Nome);
    else if (cmp > 0)
        raiz->direita = removerusuarios(raiz->direita, Nome);
    else {
        // Nó com apenas um filho ou sem filhos
        if ((raiz->esquerda == NULL) || (raiz->direita == NULL)) {
            No* temp = raiz->esquerda ? raiz->esquerda : raiz->direita;

            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else 
                *raiz = *temp; 
            
            free(temp);
        } else {
            // Nó com dois filhos: pega o sucessor 
            No* temp = minValorNo(raiz->direita);

            // Copia os dados do sucessor para este nó
            strcpy(raiz->usuarios.Nome, temp->usuarios.Nome);
            raiz->usuarios.id = temp->usuarios.id;
            strcpy(raiz->usuarios.email, temp->usuarios.email);

            raiz->direita = removerusuarios(raiz->direita, temp->usuarios.Nome);
        }
    }

    // Se a árvore tinha apenas um nó, retorna
    if (raiz == NULL)
        return raiz;

    // Atualiza altura do nó atual
    raiz->altura = 1 + max(altura(raiz->esquerda), altura(raiz->direita));

    // Verifica o balanceamento
    int balance = fatorBalanceamento(raiz);

    // Caso esquerda-esquerda
    if (balance > 1 && fatorBalanceamento(raiz->esquerda) >= 0)
        return rotacaodireita(raiz);

    // Caso esquerda-direita
    if (balance > 1 && fatorBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoesquerda(raiz->esquerda);
        return rotacaodireita(raiz);
    }

    // Caso direita-direita
    if (balance < -1 && fatorBalanceamento(raiz->direita) <= 0)
        return rotacaoesquerda(raiz);

    // Caso direita-esquerda
    if (balance < -1 && fatorBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaodireita(raiz->direita);
        return rotacaoesquerda(raiz);
    }

    return raiz;
}

// Função para buscar um usuário pelo Nome
No* buscarusuarios(No* raiz, const char* Nome) {
    if (raiz == NULL)
        return NULL;

    int cmp = strcasecmp(Nome, raiz->usuarios.Nome);
    
    if (cmp < 0)
        return buscarusuarios(raiz->esquerda, Nome);
    else if (cmp > 0)
        return buscarusuarios(raiz->direita, Nome);
    else
        return raiz;
}

// Função para listar usuários em ordem alfabética (in-order traversal)
void listarusuarios(No* raiz) {
    if (raiz != NULL) {
        listarusuarios(raiz->esquerda);  
        printf("Nome: %s\nID: %d\nE-mail: %s\n\n", 
              raiz->usuarios.Nome, raiz->usuarios.id, raiz->usuarios.email);
        listarusuarios(raiz->direita); 
    }
}

// Função para liberar a memória da árvore
void liberarArvore(No* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

// Função para exibir o menu
void exibirMenu() {
    printf("\nSistema de Gerenciamento de Usuarios\n");
    printf("1. Cadastrar usuario\n");
    printf("2. Remover usuario\n");
    printf("3. Buscar usuario\n");
    printf("4. Listar todos os usuarios (ordem alfabetica)\n");
    printf("5. Sair\n");
    printf("Escolha uma opcao: ");
}

// Função principal
int main() {
    No* raiz = NULL;
    int opcao, id;
    char Nome[100], email[100];
    No* resultadoBusca;

    do {
        exibirMenu();
        scanf("%d", &opcao);
        while (getchar() != '\n'); // Limpa o buffer

        switch (opcao) {
            case 1: // Cadastrar usuário
                printf("Nome: ");
                fgets(Nome, sizeof(Nome), stdin);
                Nome[strcspn(Nome, "\n")] = '\0'; // Remove a quebra de linha
                
                printf("ID: ");
                scanf("%d", &id);
                while (getchar() != '\n'); // Limpa o buffer
                
                printf("E-mail: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = '\0'; // Remove a quebra de linha
                
                raiz = inserirusuarios(raiz, criarusuarios(Nome, id, email));
                printf("Usuario cadastrado com sucesso!\n");
                break;
                
            case 2: // Remover usuário
                printf("Nome do usuario a ser removido: ");
                fgets(Nome, sizeof(Nome), stdin);
                Nome[strcspn(Nome, "\n")] = '\0';
                
                resultadoBusca = buscarusuarios(raiz, Nome);
                if (resultadoBusca != NULL) {
                    raiz = removerusuarios(raiz, Nome);
                    printf("Usuario removido com sucesso!\n");
                } else {
                    printf("Usuario nao encontrado.\n");
                }
                break;
                
            case 3: // Buscar usuário
                printf("Nome do usuario a ser buscado: ");
                fgets(Nome, sizeof(Nome), stdin);
                Nome[strcspn(Nome, "\n")] = '\0';
                
                resultadoBusca = buscarusuarios(raiz, Nome);
                if (resultadoBusca != NULL) {
                    printf("\nUsuario encontrado:\n");
                    printf("Nome: %s\nID: %d\nE-mail: %s\n", 
                           resultadoBusca->usuarios.Nome, 
                           resultadoBusca->usuarios.id, 
                           resultadoBusca->usuarios.email);
                } else {
                    printf("Usuario nao encontrado.\n");
                }
                break;
                
            case 4: // Listar usuários
                printf("\nLista de Usuarios (ordem alfabetica):\n");
                if (raiz == NULL) {
                    printf("Nenhum usuario cadastrado.\n");
                } else {
                    listarusuarios(raiz);
                }
                break;
                
            case 5: // Sair
                printf("Encerrando o sistema...\n");
                break;
                
            default:
                printf("Opção invalida. Tente Novamente.\n");
        }
    } while (opcao != 5);

    liberarArvore(raiz);
    return 0;
}

