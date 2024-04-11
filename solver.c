#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct TreeNode {
    int key;
    int previous;
    int remaining[10];
    int childSize;
    int depth;
    int solution;
    struct TreeNode **child;
};

struct TreeNode* createRoot(int value, int* exo){
    struct TreeNode* temp = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    temp->key = value;
    temp->previous = 0;
    for(int i = 1; i < 10; i++) temp->remaining[i] = 0;
    for(int i = 0; i < 8; i++ ) temp->remaining[exo[i]]++;
    temp->childSize = 0;
    temp->depth = 0;
    temp->solution = value;
    temp->child = NULL;
    return temp;
}

struct TreeNode* createNode(int value, struct TreeNode* parent){
    struct TreeNode* temp = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    temp->key = value;
    temp->previous = parent->key;
    for(int i = 1; i < 10; i++) temp->remaining[i] = parent->remaining[i];
    temp->remaining[value]--;
    temp->childSize = 0;
    temp->depth = parent->depth + 1;
    temp->solution = parent->solution + value * pow(10, temp->depth);
    temp->child = NULL;
    return temp;
}

int calculable(int valA, int valB, int res){
    if( ((valA + valB) % 10 == res) ||
        ((valA - valB)      == res) ||
        (((float)valA / (float)valB) == (float)res) ||
        ((valA * valB) % 10 == res)) return 1;

    return 0;
}

void firstLeaves(struct TreeNode* root, int *exo, int size){
    int aux[10][2];

    int aux_pos = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            //printf("%d%d : %d%d", j, i, exo[j], exo[i]);
            if((i == j) && (root->remaining[exo[i]] < 2)) continue;

            else if(i == j) continue;

            else if(calculable(exo[j], exo[i], root->key)){
                int repeated = 0;
                for(int k = 0; k < 10; k++){
                    if((exo[j] == aux[k][0]) && (exo[i] == aux[k][1])){
                        repeated = 1;
                        break;
                    }
                }
                if(repeated) continue;
                aux[aux_pos][0] = exo[j];
                aux[aux_pos][1] = exo[i];
                aux_pos++;
                continue;
            }
        }
    }
    
    for(int i = 0; i < aux_pos; i++){
        root->childSize++;
        root->child = (struct TreeNode**)realloc(root->child, root->childSize * sizeof(struct TreeNode*));
        struct TreeNode* child1 = createNode(aux[i][1], root);
        root->child[root->childSize - 1] = child1;
        child1->childSize++;
        child1->child = (struct TreeNode**)realloc(child1->child, child1->childSize * sizeof(struct TreeNode*));
        struct TreeNode* child2 = createNode(aux[i][0], child1);
        child1->child[child1->childSize - 1] = child2;
    }
}

void dfs(struct TreeNode *node){
    if(node->depth > 2) printf("%d\n", node->solution);
    if((node->depth > 1) && (node->depth < 8)){
        for(int j = 1; j < 10; j++){
            if((node->remaining[j] > 0) && calculable(j, node->key, node->previous)){
                int count = 1;
                node->childSize++;
                node->child = (struct TreeNode**)realloc(node->child, node->childSize * sizeof(struct TreeNode*));
                struct TreeNode* child1 = createNode(j, node);
                node->child[node->childSize - 1] = child1;
            }
        }
    }

    if(node->childSize > 0) for(int i = 0; i < node->childSize; i++) dfs(node->child[i]);

    free(node->child);
    free(node);
}

int main(int argc, char *argv[]){
    int exo[strlen(argv[1])];

    for(int i = 0; i < strlen(argv[1]); i++) exo[i] = argv[1][i] - '0';

    struct TreeNode* root = createRoot(argv[2][0] - '0', exo);

    int size = sizeof(exo) / sizeof(exo[0]);
    firstLeaves(root, exo, size);
    dfs(root);
}
