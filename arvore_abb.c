#include <stdio.h>
#include <stdlib.h>

// Estrutura de um nó de árvore
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

// Função para criar um novo nó
struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Função para inserir um nó em uma árvore ABB
struct Node* insertABB(struct Node* root, int value, int* comparisons) {
    if (root == NULL) {
        return createNode(value);
    }

    (*comparisons)++;
    if (value < root->data) {
        root->left = insertABB(root->left, value, comparisons);
    } else if (value > root->data) {
        root->right = insertABB(root->right, value, comparisons);
    }

    return root;
}

// Função para buscar um valor em uma árvore ABB
int searchABB(struct Node* root, int value, int* comparisons) {
    if (root == NULL || root->data == value) {
        (*comparisons)++;
        return *comparisons;
    }

    (*comparisons)++;
    if (value < root->data) {
        return searchABB(root->left, value, comparisons);
    } else {
        return searchABB(root->right, value, comparisons);
    }
}

// Função para calcular a altura de uma árvore
int height(struct Node* node) {
    if (node == NULL) {
        return 0;
    } else {
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
    }
}

// Função de rotação direita para balancear uma árvore AVL
struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    return x;
}

// Função de rotação esquerda para balancear uma árvore AVL
struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    return y;
}

// Função para inserir um nó em uma árvore AVL
struct Node* insertAVL(struct Node* root, int value, int* comparisons) {
    if (root == NULL) {
        return createNode(value);
    }

    (*comparisons)++;
    if (value < root->data) {
        root->left = insertAVL(root->left, value, comparisons);
    } else if (value > root->data) {
        root->right = insertAVL(root->right, value, comparisons);
    } else {
        return root; // Não permite valores duplicados
    }

    int balance = height(root->left) - height(root->right);

    // Rotação direita
    if (balance > 1 && value < root->left->data) {
        return rightRotate(root);
    }

    // Rotação esquerda
    if (balance < -1 && value > root->right->data) {
        return leftRotate(root);
    }

    // Rotação esquerda-direita
    if (balance > 1 && value > root->left->data) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Rotação direita-esquerda
    if (balance < -1 && value < root->right->data) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Função para buscar um valor em uma árvore AVL
int searchAVL(struct Node* root, int value, int* comparisons) {
    if (root == NULL || root->data == value) {
        (*comparisons)++;
        return *comparisons;
    }

    (*comparisons)++;
    if (value < root->data) {
        return searchAVL(root->left, value, comparisons);
    } else {
        return searchAVL(root->right, value, comparisons);
    }
}

// Função para liberar a memória da árvore
void freeTree(struct Node* root) {
    if (root == NULL) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    FILE* file = fopen("/home/linux/Downloads/numeros_aleatorios.txt", "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    struct Node* rootABB = NULL;
    struct Node* rootAVL = NULL;
    int value;
    int comparisonsABB = 0;
    int comparisonsAVL = 0;

    // Lê os valores do arquivo e insere nas árvores
    while (fscanf(file, "%d", &value) == 1) {
        rootABB = insertABB(rootABB, value, &comparisonsABB);
        rootAVL = insertAVL(rootAVL, value, &comparisonsAVL);
        
    }
    printf("lendo e inserindo");
    fclose(file);

    printf("procurando");

    // Realiza a busca do valor 1.000.000 nas árvores
    int resultABB = searchABB(rootABB, 1000000, &comparisonsABB);
    int resultAVL = searchAVL(rootAVL, 1000000, &comparisonsAVL);

    if (resultABB == -1) {
        printf("Valor 1.000.000 não encontrado na ABB.\n");
    } else {
        printf("\nNúmero de comparações na ABB: %d\n", comparisonsABB);
    }

    if (resultAVL == -1) {
        printf("Valor 1.000.000 não encontrado na AVL.\n");
    } else {
        printf("Número de comparações na AVL: %d\n", comparisonsAVL);
    }

    // Libera a memória das árvores
    freeTree(rootABB);
    freeTree(rootAVL);

    return 0;
}
