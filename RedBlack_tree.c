#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
struct node
{
    int data;
    struct node *left;
    struct node *right;
    struct node *p;
    bool col;
} *tree_root;
struct node *createnode(int key)
{
    struct node *new = (struct node *)malloc(sizeof(struct node));
    new->data = key;
    new->left = NULL;
    new->right = NULL;
    new->col = 1;
    return new;
}
struct node *rotate_left(struct node *root)
{
    struct node *right = root->right;
    struct node *temp = right->left;
    bool col = root->col;
    root->col = root->right->col;
    root->right->col = col;
    if (root->p != NULL)
    {
        if (root->p->left == root)
        {
            root->p->left = right;
            right->p = root->p;
        }
        else if (root->p->right == root)
        {
            root->p->right = right;
            right->p = root->p;
        }
    }
    else
    {
        tree_root = right;
        tree_root->p = NULL;
        tree_root->col = 0;
    }

    right->left = root;
    root->right = temp;
    root->p = right;
    if (temp != NULL)
        temp->p = root;

    return right;
}
struct node *rotate_right(struct node *root)
{
    struct node *left = root->left;
    struct node *temp = left->right;
    bool col = root->col;
    root->col = root->left->col;
    root->left->col = col;
    if (root->p != NULL)
    {
        if (root->p->left == root)
        {
            root->p->left = left;
            left->p = root->p;
        }
        else if (root->p->right == root)
        {
            root->p->right = left;
            left->p = root->p;
        }
    }
    else
    {
        tree_root = left;
        tree_root->p = NULL;
        tree_root->col = 0;
    }

    left->right = root;
    root->left = temp;
    root->p = left;
    if (temp != NULL)
        temp->p = root;

    return left;
}

struct node *search(struct node *root, int val)
{
    while (root != NULL)
    {
        if (root->data == val)
        {
            return root;
        }
        if (root->data > val)
            root = root->left;
        else if (root->data < val)
            root = root->right;
    }

    return NULL;
}

struct node *successor(struct node *root, int val)
{
    struct node *temp = search(root, val);
    if (temp == NULL)
    {
        printf("Error: %d not found\n", val);
        return NULL;
    }
    else if (temp->right == NULL)
    {
        struct node *min = root;
        while (root->data != val)
        {
            if (root->data > val)
            {
                min = root;
                root = root->left;
            }
            else if (root->data < val)
                root = root->right;
        }
        if (min->data > val)
            return min;
        else
        {
            return NULL;
        }
    }
    else
    {
        temp = temp->right;
        while (temp->left != NULL)
        {
            temp = temp->left;
        }
        return temp;
    }
}

void correct_tree2(struct node *x)
{
    while (x->p != NULL && x->col == 0)
    {
        if (x == x->p->left)
        {
            struct node *w = x->p->right;
            if (w != NULL)
            {
                if (w->col == 1)
                {
                    w->col = 0;
                    x->p->col = 1;
                    rotate_left(x->p);
                    w = x->p->right;
                }
                if (w->left->col == 0 && w->right->col == 0)
                {
                    w->col = 1;
                    x = x->p;
                }
                else if (w->right->col == 0)
                {
                    w->left->col = 0;
                    w->col = 1;
                    rotate_right(w);
                    w = x->p->right;
                }
                w->col = x->p->col;
                x->p->col = 0;
                w->right->col = 0;
                rotate_left(x->p);
                x = tree_root;
            }
        }
        else
        {
            struct node *w = x->p->left;
            if (w != NULL)
            {
                if (w->col == 1)
                {
                    w->col = 0;
                    x->p->col = 1;
                    rotate_right(x->p);
                    w = x->p->left;
                }
                if ((w->right == NULL || w->right->col == 0) && (w->left == NULL || w->left->col == 0))
                {
                    w->col = 1;
                    x = x->p;
                }
                else if (w->left->col == 0)
                {
                    w->right->col = 0;
                    w->col = 1;
                    rotate_left(w);
                    w = x->p->left;
                }
                if (x->p != NULL)
                {
                    w->col = x->p->col;
                    x->p->col = 0;
                    rotate_right(x->p);
                    x = tree_root;
                }
                else
                    break;
            }
        }
    }
    x->col = 0;
}

struct node *delete(struct node *root, int val)
{
    struct node *z = search(root, val);
    if (z == NULL)
    {
        printf("Error:%d not found!Can't be deleted!\n", val);
        return NULL;
    }
    if (z->left == NULL && z->right == NULL && z == tree_root)
    {
        tree_root = NULL;
        return z;
    }
    struct node *y = z;
    struct node *x = NULL;
    bool y_ori_col = y->col;

    if (z->left == NULL && z->right == NULL && z->col == 1)
    {
        if (z == z->p->left)
        {
            z->p->left = NULL;
        }
        else
        {
            z->p->right = NULL;
        } // red leaf node
        return tree_root;
    }

    // Cases for deleting black leaf node

    if (z->left == NULL && z->right == NULL)
    {
        struct node *s;
        if (z == z->p->left)
        {
            s = z->p->right;
            z->p->left = NULL;
        }
        else
        {
            s = z->p->left;
            z->p->right = NULL;
        }
        if (s == NULL)
            x = z->p;
        else
        {

            if (s->col == 1 && s == z->p->left)
            {

                rotate_right(z->p);
                z->p->col = 0;
                if (z->p->left != NULL)
                    z->p->left->col = 0;
            }
            else if (s->col == 1 && s == z->p->right)
            {
                rotate_left(z->p);
                z->p->col = 0;
                if (z->p->right != NULL)
                    z->p->right->col = 0;
            }
            else if (s->col == 0)
            {
                if ((s->left == NULL || s->left->col == 0) && (s->right == NULL || s->right->col == 0))
                {
                    bool c = z->p->col;
                    z->p->col = 0;
                    s->col = 1;
                    if (c == 0)
                        correct_tree2(z->p);
                }
                else
                {
                    if (s == z->p->left && s->left != NULL && s->left->col == 1)
                    {
                        s->left->col = 0;
                        rotate_right(z->p);
                    }
                    else if (s == z->p->left && s->right != NULL && s->right->col == 1)
                    {
                        s->right->col = 0;
                        rotate_left(s);
                        rotate_right(z->p);
                    }
                    else if (s == z->p->right && s->right != NULL && s->right->col == 1)
                    {
                        s->right->col = 0;
                        rotate_left(z->p);
                    }
                    else if (s == z->p->right && s->left != NULL && s->left->col == 1)
                    {
                        s->left->col = 0;
                        rotate_right(s);
                        rotate_left(z->p);
                    }
                }
            }
        }
    }

    else if (z->left == NULL)
    {
        x = z->right;
        if (z->p != NULL)
        {
            if (z->p->right == z)
            {
                z->p->right = x;
            }
            else
            {
                z->p->left = x;
                if (x != NULL)
                    x->p = z->p;
            }
        }
        else
        {
            tree_root = x;
        }
        if (x != NULL)
            x->p = z->p; // One child case
    }
    else if (z->right == NULL)
    {
        x = z->left;
        if (z->p != NULL)
        {
            if (z->p->right == z)
                z->p->right = x;
            else
                z->p->left = x;
        }
        else
            tree_root = x;
        x->p = z->p;
    }

    else
    {
        y = successor(tree_root, val);
        // Two children case
        delete (tree_root, y->data);
        z->data = y->data;
    }

    if (y_ori_col == 0 && x != NULL)
        correct_tree2(x);
    return tree_root;
}

void correct_tree(struct node *root)
{
    if (root == NULL)
        return;
    if (root->col == 0)
        return;
    if (root->p == NULL)
    {
        root->col = 0;
        return;
    }
    else if (root->p->p == NULL)
        return;
    else if (root->p != NULL && root->p->col != 0)
    {
        struct node *u;
        if (root->p->p->left == root->p)
        {
            u = root->p->p->right;
        }
        else
            u = root->p->p->left;

        if (u != NULL && u->col == 1)
        {
            root->p->col = 0;
            u->col = 0;
            root->p->p->col = 1;
            root = root->p->p;
        }
        else
        {
            if (root->data <= root->p->data && root->p->data <= root->p->p->data)
                rotate_right(root->p->p);
            else if (root->data > root->p->data && root->p->data > root->p->p->data)
                rotate_left(root->p->p);
            else if (root->data <= root->p->data && root->p->data > root->p->p->data)
            {
                root->p->p->right = rotate_right(root->p);
                rotate_left(root->p);
            }
            else if (root->data > root->p->data && root->p->data <= root->p->p->data)
            {
                root->p->p->left = rotate_left(root->p);
                rotate_right(root->p);
            }
            root = root->p;
        }
        correct_tree(root);
    }
}

struct node *insert(struct node *root, int key)
{
    if (root == NULL)
    {
        tree_root = createnode(key);
        tree_root->col = 0;
        return tree_root;
    }

    struct node *temp = search(tree_root, key);
    if (temp != NULL)
    {
        printf("%d is already present.So no need to insert.\n", key);
        return NULL;
    }
    struct node *new = createnode(key);
    while (root != NULL)
    {
        if (key <= root->data && root->left != NULL)
            root = root->left;
        else if (key > root->data && root->right != NULL)
            root = root->right;
        else if (key <= root->data)
        {
            root->left = new;
            new->p = root;
            break;
        }
        else if (key > root->data)
        {
            root->right = new;
            new->p = root;
            break;
        }
    }
    correct_tree(new);
    return new;
}

struct node *queue[100];
int head = 0, tail = 0;
void clear_queue()
{
    head = 0;
    tail = 0;
}
void enqueue(struct node *a[], struct node *c)
{
    if (tail > 99)
    {
        printf("Queue overflow!\n");
        exit(-1);
    }
    a[tail] = c;
    tail++;
}
struct node *dequeue(struct node *a[])
{
    if (head == tail)
    {
        printf("Error:queue underflow!\n");
        exit(0);
    }

    return a[head++];
}
struct node *front(struct node *queue[])
{
    return queue[head];
}
int size()
{
    return tail - head;
}
bool check_empty(struct node *a[])
{
    if (head == tail)
        return true;
    else
        return false;
}
void print_level(struct node *root)
{
    if (root == NULL)
    {
        printf("The tree is empty!\n");
        return;
    }
    enqueue(queue, root);
    while (!check_empty(queue))
    {
        int cnt = size();
        while (cnt > 0)
        {
            struct node *temp = front(queue);
            char c = temp->col == 0 ? 'b' : 'r';
            printf("%d%c ", temp->data, c);
            dequeue(queue);
            if (temp->left != NULL)
                enqueue(queue, temp->left);
            if (temp->right != NULL)
                enqueue(queue, temp->right);
            cnt--;
        }
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
    tree_root->col = 0;
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
            struct node *found = search(tree_root, e);
            if (found == NULL)
                printf("%d not found\n", e);
            else
                printf("%d found\n", e);
        }
        else if (d == 2)
        {
            printf("Enter the number to be inserted: ");
            scanf("%d", &e);
            struct node *ins = insert(tree_root, e);
            if (ins != NULL)
                printf("%d inserted\n", e);
        }
        else if (d == 3)
        {
            printf("Enter the number to be deleted: ");
            scanf("%d", &e);
            struct node *del = delete (tree_root, e);
            if (del != NULL)
                printf("%d deleted!\n", e);
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
