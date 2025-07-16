// Change the MAX and MIN to change parameters of the tree
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define btn BTreeNode
#define MAX 7
#define MIN 3
bool flag = true;
struct BTreeNode
{
    int key[MAX + 1], count;
    struct BTreeNode *children[MAX + 1];
} *tree_root;
void insert(struct BTreeNode *root, int val);
void split(struct btn *root, struct btn *, int b);
struct BTreeNode *createnode(int val)
{
    struct BTreeNode *new = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    new->key[0] = val;
    for (int i = 0; i < MAX + 1; i++)
        new->children[i] = NULL;
    new->count = 1;
    return new;
}
struct btn *search(struct btn *root, int val)
{
    if (root == NULL)
    {
        return NULL;
    }
    int i;
    for (i = 0; i < root->count; i++)
    {
        if (val > root->key[i])
            continue;
        else if (val < root->key[i])
        {
            root = root->children[i];
            return search(root, val);
        }
        else
            return root;
    }
    if (i == root->count)
    {
        root = root->children[i];
        return search(root, val);
    }
    return NULL;
}
void insert2(struct btn *root, int val)
{

    int i;
    for (i = 0; i < root->count; i++)
    {
        if (val > root->key[i])
            continue;
        else
        {

            int temp;
            for (int j = i; j <= root->count; j++)
            {
                temp = root->key[j];
                root->key[j] = val;
                val = temp;
            }
            root->count++;

            return;
        }
    }
    if (i == root->count)
    {
        root->key[root->count] = val;
        root->count++;

        return;
    }
}
void split(struct btn *root, struct btn *parent, int b)
{
    int mid = MAX / 2;
    struct btn *new = (struct btn *)malloc(sizeof(struct btn));
    struct btn *new_temp = new;
    for (int i = 0; i < MAX + 1; i++)
        new->children[i] = NULL;
    new->count = 0;
    int i;
    for (i = mid + 1; i < root->count; i++)
    {
        new->key[i - mid - 1] = root->key[i];
        new->children[i - mid - 1] = root->children[i];
        root->children[i] = NULL;

        new->count++;
    }
    new->children[i - mid - 1] = root->children[i];
    root->children[i] = NULL;
    root->count = mid;
    if (b == 1)
    {
        struct btn *new_root = (struct btn *)malloc(sizeof(struct btn));
        for (int i = 0; i < MAX + 1; i++)
            new_root->children[i] = NULL;
        new_root->count = 0;
        new_root->key[0] = root->key[mid];
        new_root->count++;
        new_root->children[1] = new;
        new_root->children[0] = root;

        tree_root = new_root;
    }
    else
    {
        int key = root->key[mid];
        int i;
        insert2(parent, root->key[mid]);
        for (i = 0; i < parent->count; i++)
        {
            if (parent->key[i] == key)
                break;
        }
        parent->children[i] = root;
        for (int j = i + 1; j <= parent->count; j++)
        {
            struct btn *temp2 = parent->children[j];
            parent->children[j] = new;
            new = temp2;
        }
    }
}
void insert(struct BTreeNode *root, int val)
{
    struct btn *sea = search(tree_root, val);
    if (sea != NULL)
    {
        printf("%d is already present. So no need to insert.\n", val);
        flag = false;
        return;
    }
    if (root->count == MAX)
    {
        split(root, NULL, 1);
        insert(tree_root, val);
    }
    else
    {
        int i;
        for (i = 0; i < root->count; i++)
        {
            if (val > root->key[i])
                continue;
            else
            {
                if (root->children[i] == NULL)
                {
                    int temp;
                    for (int j = i; j <= root->count; j++)
                    {
                        temp = root->key[j];
                        root->key[j] = val;
                        val = temp;
                    }
                    root->count++;

                    return;
                }

                else
                {
                    if (root->children[i]->count != MAX)
                    {
                        root = root->children[i];
                        insert(root, val);
                    }
                    else
                    {
                        split(root->children[i], root, 0);
                        insert(tree_root, val);
                    }
                    return;
                }
            }
        }
        if (i == root->count && root->children[i] == NULL)
        {
            root->key[root->count] = val;
            root->count++;

            return;
        }
        else if (i == root->count)
        {
            if (root->children[i]->count != MAX)
            {
                root = root->children[i];
                insert(root, val);
            }
            else
            {
                split(root->children[i], root, 0);
                insert(tree_root, val);
            }
            return;
        }
    }
}

int successor(struct btn *root)
{
    if (root->children[0] == NULL)
        return root->key[0];
    else
        return successor(root->children[0]);
}

void replace(struct btn *root, int a, int b)
{
    int i;
    for (i = 0; i < root->count; i++)
    {
        if (a > root->key[i])
            continue;
        else if (a < root->key[i])
        {
            root = root->children[i];
            replace(root, a, b);
            return;
        }
        else
        {
            root->key[i] = b;
            return;
        }
    }
    if (i == root->count)
    {
        root = root->children[i];
        replace(root, a, b);
        return;
    }
}

void delete(struct btn *root, int val)
{
    int i;
    for (i = 0; i < root->count; i++)
    {
        if (val > root->key[i])
            continue;
        else if (val < root->key[i])
        {
            if (root->children[i] != NULL && root->children[i]->count > MIN)
            {
                root = root->children[i];
                delete (root, val);
                return;
            }
            else if (root->children[i] != NULL && root->children[i]->count == MIN)
            {
                if (i - 1 >= 0 && root->children[i - 1]->count > MIN)
                {
                    int temp = root->key[i - 1];
                    root->key[i - 1] = root->children[i - 1]->key[root->children[i - 1]->count - 1];
                    struct btn *child = root->children[i - 1]->children[root->children[i - 1]->count];
                    for (int j = 0; j <= MIN; j++)
                    {
                        struct btn *child2 = root->children[i]->children[j];
                        int temp2 = root->children[i]->key[j];
                        root->children[i]->key[j] = temp;
                        root->children[i]->children[j] = child;
                        temp = temp2;
                        child = child2;
                    }
                    root->children[i]->children[MIN + 1] = child;
                    root->children[i]->count++;
                    root->children[i - 1]->count--;
                    delete (tree_root, val);
                    return;
                }
                else if (i + 1 <= root->count && root->children[i + 1]->count > MIN)
                {
                    int temp = root->key[i];
                    int j;
                    root->key[i] = root->children[i + 1]->key[0];
                    struct btn *child = root->children[i + 1]->children[0];
                    for (j = 0; j < root->children[i + 1]->count - 1; j++)
                    {
                        root->children[i + 1]->key[j] = root->children[i + 1]->key[j + 1];
                        root->children[i + 1]->children[j] = root->children[i + 1]->children[j + 1];
                    }
                    root->children[i + 1]->children[j] = root->children[i + 1]->children[j + 1];
                    root->children[i + 1]->count--;
                    root->children[i]->key[MIN] = temp;
                    root->children[i]->children[MIN + 1] = child;
                    root->children[i]->count++;
                    delete (tree_root, val);
                    return;
                }
                else
                {
                    root->children[i]->key[MIN] = root->key[i];
                    root->children[i]->count++;
                    for (int j = 0; j < MIN; j++)
                    {
                        root->children[i]->key[MIN + j + 1] = root->children[i + 1]->key[j];
                        root->children[i]->count++;
                    }
                    for (int j = 0; j <= MIN; j++)
                    {
                        root->children[i]->children[MIN + j + 1] = root->children[i + 1]->children[j];
                    }
                    for (int j = i + 1; j < root->count; j++)
                    {
                        root->children[j] = root->children[j + 1];
                    }
                    for (int j = i; j < root->count - 1; j++)
                    {
                        root->key[j] = root->key[j + 1];
                    }
                    root->count--;
                    if (root->count == 0)
                        tree_root = root->children[0];
                    delete (tree_root, val);
                    return;
                }
            }
            else if (root->children[i] == NULL)
            {
                printf("%d not found! Can't be deleted!\n", val);
                flag = false;
                return;
            }
        }
        else
        {
            if (root->children[i] == NULL)
            {
                for (int j = i; j < root->count - 1; j++)
                {
                    root->key[j] = root->key[j + 1];
                }
                root->count--;
                return;
            }
            else
            {
                int succ = successor(root->children[i + 1]);
                delete (tree_root, succ);
                replace(tree_root, val, succ);
                return;
            }
        }
    }
    if (i == root->count)
    {
        if (root->children[root->count] != NULL && root->children[root->count]->count > MIN)
        {
            root = root->children[root->count];
            delete (root, val);
        }
        else if (root->children[root->count] != NULL && root->children[root->count]->count == MIN)
        {
            if (i - 1 >= 0 && root->children[i - 1]->count > MIN)
            {
                int temp = root->key[i - 1];
                root->key[i - 1] = root->children[i - 1]->key[root->children[i - 1]->count - 1];
                struct btn *child = root->children[i - 1]->children[root->children[i - 1]->count];
                for (int j = 0; j <= MIN; j++)
                {
                    struct btn *child2 = root->children[i]->children[j];
                    int temp2 = root->children[i]->key[j];
                    root->children[i]->key[j] = temp;
                    root->children[i]->children[j] = child;
                    temp = temp2;
                    child = child2;
                }
                root->children[i]->children[MIN + 1] = child;
                root->children[i]->count++;
                root->children[i - 1]->count--;
                delete (tree_root, val);
                return;
            }
            else
            {
                root->children[i - 1]->key[MIN] = root->key[i - 1];
                root->children[i - 1]->count++;
                for (int j = 0; j < MIN; j++)
                {
                    root->children[i - 1]->key[MIN + j + 1] = root->children[i]->key[j];
                    root->children[i - 1]->count++;
                }
                for (int j = 0; j <= MIN; j++)
                {
                    root->children[i - 1]->children[MIN + j + 1] = root->children[i]->children[j];
                }
                root->count--;
                if (root->count == 0)
                    tree_root = root->children[0];
                delete (tree_root, val);
                return;
            }
        }
        else
        {
            printf("%d not found! Can't be deleted!\n", val);
            flag = false;
        }
        return;
    }
}
void traversal(struct btn *myNode)
{
    int i;
    if (myNode)
    {
        for (i = 1; i <= myNode->count; i++)
        {
            traversal(myNode->children[i]);
            printf("%d ", myNode->key[i - 1]);
        }
        traversal(myNode->children[i]);
    }
}
struct btn *queue[100];
int head = 0, tail = 0;

void enqueue(struct btn *a[], struct btn *c)
{
    a[tail++] = c;
}
struct btn *dequeue(struct btn *a[])
{
    if (head == tail)
    {
        printf("Error:queue underflow!\n");
        exit(0);
    }

    return a[head++];
}
struct btn *front(struct btn *queue[])
{
    return queue[head];
}
int size()
{
    return tail - head;
}
bool check_empty(struct btn *a[])
{
    if (head == tail)
        return true;
    else
        return false;
}
void clear_queue()
{
    head = 0;
    tail = 0;
}
void print_level(struct btn *root)
{
    if (root == NULL)
        return;
    enqueue(queue, root);
    int lvl = 0;
    while (!check_empty(queue))
    {
        int cnt = size();
        printf("Level %d : ", lvl);
        while (cnt > 0)
        {
            struct btn *temp = front(queue);
            for (int i = 0; i < temp->count; i++)
                printf("%d ", temp->key[i]);
            printf("  ");
            dequeue(queue);
            for (int i = 0; i <= temp->count; i++)
            {
                if (temp->children[i] != NULL)
                    enqueue(queue, temp->children[i]);
            }
            cnt--;
        }
        lvl++;
        printf("\n");
    }
    clear_queue();
}

int main()
{
    int n;
    printf("Enter the number of nodes in the tree: ");
    scanf("%d", &n);
    int d;
    printf("Enter the numbers in the tree: ");
    scanf("%d", &d);
    tree_root = createnode(d);
    for (int i = 1; i < n; i++)
    {
        scanf("%d", &d);
        insert(tree_root, d);
    }
    while (1)
    {
        printf("Enter the query number: ");
        int e;
        scanf("%d", &d);
        // if(d==1||d==2||d==3)scanf("%d",&e);
        if (d == 1)
        {
            printf("Enter the number to be searched: ");
            scanf("%d", &e);
            struct btn *found = search(tree_root, e);
            if (found == NULL)
                printf("%d not found\n", e);
            else
                printf("%d found\n", e);
        }
        else if (d == 2)
        {
            printf("Enter the number to be inserted: ");
            scanf("%d", &e);
            insert(tree_root, e);
            if (flag)
                printf("%d inserted\n", e);
            flag = true;
        }
        else if (d == 3)
        {
            printf("Enter the number to be deleted: ");
            scanf("%d", &e);
            delete (tree_root, e);
            if (flag)
                printf("%d deleted!\n", e);
            flag = true;
        }
        else if (d == 4)
        {
            print_level(tree_root);
        }
        else
        {
            printf("Invalid query!\n");
        }
    }
}