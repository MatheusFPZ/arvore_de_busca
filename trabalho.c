
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// Estrutura para representar um nó da árvore
typedef struct TreeNode {
    char name[256];
    char location[566];
    unsigned long size;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

int cont=0;
char nome[100];
// Função para criar um novo nó
TreeNode *createNode(const char *name, const char *location, unsigned long size) {
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    strcpy(newNode->name, name);
    strcpy(newNode->location, location);
    newNode->size = size;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Função para inserir um nó na árvore
TreeNode *insert(TreeNode *root, const char *name, const char *location, unsigned long size) {
    if (root == NULL) {
        return createNode(name, location, size);
    }

    int compareResult = strcmp(name, root->name);

    if (compareResult < 0) {
        root->left = insert(root->left, name, location, size);
    } else if (compareResult > 0) {
        root->right = insert(root->right, name, location, size);
    }

    return root;
}

// Função para varrer a pasta e construir a árvore
void scanFolder(const char *path, TreeNode **root) {
    struct dirent *entry;
    struct stat statbuf;
    DIR *dp = opendir(path);
    

    if (dp == NULL) {
        return;
    }

    while ((entry = readdir(dp))) {
        char newpath[1024];
        strcpy(newpath, path);
        strcat(newpath, "/");
        strcat(newpath, entry->d_name);

        if (lstat(newpath, &statbuf) == -1) {
            continue;
            
        }

        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
               
            }

            scanFolder(newpath, root);
        } else {
            cont++;
            *root = insert(*root, entry->d_name, newpath, statbuf.st_size);
            printf("valor inserido:%d", cont);
            
        }
    }

    closedir(dp);
}

// Função para liberar a memória da árvore
void freeTree(TreeNode *root) {


    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void searchAndPrintAll(TreeNode *root, const char *fileName) {
    if (root == NULL) {
        return;
    }

    int compareResult = strcmp(fileName, root->name);

    if (compareResult == 0) {
        // Encontrou uma correspondência, imprime as informações
        printf("Arquivo encontrado:\n");
        printf("Nome: %s\n", root->name);
        printf("Localização: %s\n", root->location);
        printf("Tamanho: %lu bytes\n", root->size);
 }
        
    searchAndPrintAll(root->left, fileName);
    searchAndPrintAll(root->right, fileName);
        
        
   

    // Continue a pesquisa nas subárvores direita e esquerda, pois pode haver mais correspondências
    
}


int main() {
    TreeNode *root = NULL;
    const char *folderPath = "/media/linux/5CF4276DF427491A/Contabil"; // Substitua pelo caminho da pasta pai

    scanFolder(folderPath, &root);
    
    for (;;) {
    printf("\nDigite o nome a ser buscado (ou 'parar' para sair): ");
    scanf("%s", nome);

    if (strcmp(nome, "parar") == 0) {
            printf("Saindo do programa...\n");
            break;
        }
    
    printf("\n//////////buscando///////////");
    searchAndPrintAll(root, nome);
    
   
    }
    freeTree(root);

    return 0;
    
}


