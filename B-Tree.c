#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define ORDER 5

typedef struct BTreeNode {
    int keys[ORDER - 1];
    struct BTreeNode* children[ORDER];
    int numKeys;
    int isLeaf;
} BTreeNode;

BTreeNode* createNode(int isLeaf) {
    int i;
    BTreeNode* newNode = (BTreeNode*)malloc(sizeof(BTreeNode));
    newNode->isLeaf = isLeaf;
    newNode->numKeys = 0;
    for (i = 0; i < ORDER; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

void splitChild(BTreeNode* parent, int index, BTreeNode* child) {
    int i;
    BTreeNode* newChild = createNode(child->isLeaf);
    newChild->numKeys = ORDER / 2 - 1;

    for (i = 0; i < ORDER / 2 - 1; i++) {
        newChild->keys[i] = child->keys[i + ORDER / 2];
    }
    if (!child->isLeaf) {
        for (i = 0; i < ORDER / 2; i++) {
            newChild->children[i] = child->children[i + ORDER / 2];
        }
    }

    child->numKeys = ORDER / 2 - 1;

    for (i = parent->numKeys; i >= index + 1; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = newChild;

    for (i = parent->numKeys - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
    }
    parent->keys[index] = child->keys[ORDER / 2 - 1];
    parent->numKeys++;
}

void insertNonFull(BTreeNode* node, int key) {
    int i = node->numKeys - 1;

    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->numKeys++;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        if (node->children[i]->numKeys == ORDER - 1) {
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

void insert(BTreeNode** root, int key) {
    BTreeNode* r = *root;
    if (r->numKeys == ORDER - 1) {
        BTreeNode* s = createNode(0);
        *root = s;
        s->children[0] = r;
        splitChild(s, 0, r);
        insertNonFull(s, key);
    } else {
        insertNonFull(r, key);
    }
}

int search(BTreeNode* root, int key) {
    int i = 0;
    while (i < root->numKeys && key > root->keys[i]) {
        i++;
    }
    if (i < root->numKeys && root->keys[i] == key) {
        return 1; // Found
    }
    if (root->isLeaf) {
        return 0; // Not found
    }
    return search(root->children[i], key);
}

void removeFromLeaf(BTreeNode* node, int index) {
    int i;
    for (i = index + 1; i < node->numKeys; i++) {
        node->keys[i - 1] = node->keys[i];
    }
    node->numKeys--;
}

void removeFromNonLeaf(BTreeNode* node, int index);

int getPredecessor(BTreeNode* node) {
    while (!node->isLeaf) {
        node = node->children[node->numKeys];
    }
    return node->keys[node->numKeys - 1];
}

int getSuccessor(BTreeNode* node) {
    while (!node->isLeaf) {
        node = node->children[0];
    }
    return node->keys[0];
}

void borrowFromPrev(BTreeNode* node, int index) {
    BTreeNode* child = node->children[index];
    BTreeNode* sibling = node->children[index - 1];

    int i;
    // Shift child's keys and children forward
    for (i = child->numKeys - 1; i >= 0; i--)
        child->keys[i + 1] = child->keys[i];
    if (!child->isLeaf) {
        for (i = child->numKeys; i >= 0; i--)
            child->children[i + 1] = child->children[i];
    }

    // Move key from parent to child
    child->keys[0] = node->keys[index - 1];

    if (!child->isLeaf)
        child->children[0] = sibling->children[sibling->numKeys];

    // Move sibling's last key up to parent
    node->keys[index - 1] = sibling->keys[sibling->numKeys - 1];

    child->numKeys++;
    sibling->numKeys--;
}

void borrowFromNext(BTreeNode* node, int index) {
    BTreeNode* child = node->children[index];
    BTreeNode* sibling = node->children[index + 1];

    // Move key from parent to child
    child->keys[child->numKeys] = node->keys[index];

    if (!child->isLeaf)
        child->children[child->numKeys + 1] = sibling->children[0];

    // Move sibling's first key up to parent
    node->keys[index] = sibling->keys[0];

    // Shift sibling keys and children left by 1
    int i;
    for (i = 1; i < sibling->numKeys; i++)
        sibling->keys[i - 1] = sibling->keys[i];
    if (!sibling->isLeaf) {
        for (i = 1; i <= sibling->numKeys; i++)
            sibling->children[i - 1] = sibling->children[i];
    }

    child->numKeys++;
    sibling->numKeys--;
}

void merge(BTreeNode* node, int index) {
    BTreeNode* left = node->children[index];
    BTreeNode* right = node->children[index + 1];

    int i;
    left->keys[ORDER / 2 - 1] = node->keys[index];

    for (i = 0; i < right->numKeys; i++) {
        left->keys[i + ORDER / 2] = right->keys[i];
    }
    if (!left->isLeaf) {
        for (i = 0; i <= right->numKeys; i++) {
            left->children[i + ORDER / 2] = right->children[i];
        }
    }

    left->numKeys += right->numKeys + 1;

    for (i = index + 1; i < node->numKeys; i++) {
        node->keys[i - 1] = node->keys[i];
    }
    for (i = index + 2; i <= node->numKeys; i++) {
        node->children[i - 1] = node->children[i];
    }
    node->numKeys--;

    free(right);
}

void fill(BTreeNode* node, int index) {
    if (index != 0 && node->children[index - 1]->numKeys >= ORDER / 2) {
        borrowFromPrev(node, index);
    } else if (index != node->numKeys && node->children[index + 1]->numKeys >= ORDER / 2) {
        borrowFromNext(node, index);
    } else {
        if (index != node->numKeys) {
            merge(node, index);
        } else {
            merge(node, index - 1);
        }
    }
}

void deleteKey(BTreeNode* node, int key) {
    int idx = 0;
    while (idx < node->numKeys && node->keys[idx] < key) {
        idx++;
    }

    if (idx < node->numKeys && node->keys[idx] == key) {
        if (node->isLeaf) {
            removeFromLeaf(node, idx);
        } else {
            removeFromNonLeaf(node, idx);
        }
    } else {
        if (node->isLeaf) {
            printf("The key %d is not found in the tree.\n", key);
            return;
        }

        int flag = (idx == node->numKeys) ? 1 : 0;

        if (node->children[idx]->numKeys < ORDER / 2) {
            fill(node, idx);
        }

        if (flag && idx > node->numKeys) {
            deleteKey(node->children[idx - 1], key);
        } else {
            deleteKey(node->children[idx], key);
        }
    }
}

void removeFromNonLeaf(BTreeNode* node, int index) {
    int i;
    int key = node->keys[index];

    if (node->children[index]->numKeys >= ORDER / 2) {
        int pred = getPredecessor(node->children[index]);
        node->keys[index] = pred;
        deleteKey(node->children[index], pred);
    } else if (node->children[index + 1]->numKeys >= ORDER / 2) {
        int succ = getSuccessor(node->children[index + 1]);
        node->keys[index] = succ;
        deleteKey(node->children[index + 1], succ);
    } else {
        merge(node, index);
        deleteKey(node->children[index], key);
    }
}

void printBTree(BTreeNode* root, int level) {
    int i;
    if (root != NULL) {
        printf("Level %d: ", level);
        for (i = 0; i < root->numKeys; i++) {
            printf("%d ", root->keys[i]);
        }
        printf("\n");
        for (i = 0; i <= root->numKeys; i++) {
            printBTree(root->children[i], level + 1);
        }
    }
}

int main() {
    int i;
    srand(time(0));
    BTreeNode* root = createNode(1);
    int arr[100];

    // Generate 100 random elements
    for (i = 0; i < 100; i++) {
        arr[i] = rand() % 1000;
        insert(&root, arr[i]);
    }
    printf("B-Tree structure after inserting 100 random elements:\n");
    printBTree(root, 0);

    // Searching for a random key
    int searchKey = arr[rand() % 100];
    printf("\nSearching for key %d: %s\n", searchKey, search(root, searchKey) ? "Found" : "Not Found");

    // Deleting a key
    int deleteKeyVal = arr[rand() % 100];
    printf("Deleting key %d...\n", deleteKeyVal);
    deleteKey(root, deleteKeyVal);

    printf("B-Tree structure after deleting key %d:\n", deleteKeyVal);
    printBTree(root, 0);
    return 0;
}

