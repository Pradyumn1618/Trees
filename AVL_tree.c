#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
struct node{
    int data;
    struct node* left;
    struct node* right;
    int height;
}*tree_root;
bool flag=true;
struct node* queue[100];
int head=0,tail=0;
void clear_queue(){
    head=0;tail=0;
}
void enqueue(struct node* a[],struct node* c){
    if(tail>99){printf("Queue overflow!");exit(-1);}
    a[tail]=c;
    tail++;
    
}
struct node* dequeue(struct node* a[]){
    if(head==tail){printf("Error:queue underflow!\n");exit(0);}

    return a[head++];
}
struct node* front(struct node* queue[]){
    return queue[head];
}
int size(){
    return tail-head;
}
bool check_empty(struct node* a[]){
   if(head==tail)return true;
   else return false;
}
void print_level(struct node* root){
    if(root==NULL)return;
    enqueue(queue,root);
    while(!check_empty(queue)){
        int cnt=size();
        while(cnt>0){
             struct node* temp=front(queue);
            printf("%d ",temp->data);
            dequeue(queue);
            if(temp->left!=NULL)enqueue(queue,temp->left);
            if(temp->right!=NULL)enqueue(queue,temp->right);
            cnt--;
        }
        printf("\n");
    }
    clear_queue();
}
struct node* createnode(int data){
  struct node* new=(struct node*)malloc(sizeof(struct node));
  new->data=data;
  new->left=NULL;
  new->right=NULL;
  new->height=1;
  return new;
}
int get_height(struct node* node){
    if(node==NULL)return 0;
    else return node->height;
}
void update_height(struct node* root){
     root->height= get_height(root->left)>=get_height(root->right)?get_height(root->left)+1:get_height(root->right)+1;
}
int get_balance(struct node* root){
  return get_height(root->right)-get_height(root->left);
}
struct node* rotate_left(struct node* root){
    struct node* right=root->right;
    struct node* temp=right->left;
    
       right->left=root;
        root->right=temp;
        
    update_height(root);
    update_height(right);
    return right;
}
struct node* rotate_right(struct node* root){
    struct node* left=root->left;
    struct node* temp=left->right;
    
       left->right=root;
        root->left=temp;
        
        
    update_height(root);
    update_height(left);
    return left;
}
struct node* check_balance(struct node* root,int key){
    
     update_height(root);
     if(get_balance(root)>1&&key>root->right->data){return rotate_left(root);}
     else if(get_balance(root)<-1&&key<=root->left->data){return rotate_right(root);}
     else if(get_balance(root)>1&&key<=root->right->data){root->right=rotate_right(root->right);return rotate_left(root);}
     else if(get_balance(root)<-1&&key>root->left->data){root->left=rotate_left(root->left);return rotate_right(root);
     }
     else return root;
    
}
struct node* check_balance2(struct node* root){
    // if(root==NULL)return;
    
        update_height(root);
     if(get_balance(root)>1&&get_balance(root->right)>=0){return rotate_left(root);}
     else if(get_balance(root)<-1&&get_balance(root->left)<=0){return rotate_right(root);}
      else if(get_balance(root)>1&&get_balance(root->right)<0){root->right=rotate_right(root->right);return rotate_left(root);}
     else if(get_balance(root)<-1&&get_balance(root->left)>0){root->left=rotate_left(root->left);return rotate_right(root);}
     else return root;
     
}
struct node* insert(struct node* root,int key){
    
    if (root == NULL)
        return(createnode(key));
 
    if (key < root->data)
        root->left  = insert(root->left, key);
    else if (key > root->data)
        root->right = insert(root->right, key);
    else {
        // Equal keys not allowed
        printf("%d is already present. So no need to insert.\n",key);
        flag=false;
        return root;
    }
    return check_balance(root,key);
    
    return root;
}
struct node* search(struct node* root,int val){
   while(root!=NULL){
    if(root->data==val){return root;}
    if(root->data>val)root=root->left;
    else if(root->data<val)root=root->right;
   }
   
   return NULL;
   
}
struct node* successor(struct node* root){
        root=root->right;
        while(root->left!=NULL){
            root=root->left;
        }
        return root;  
}
struct node* delete(struct node* root,int key){
    struct node* del=search(tree_root,key);
    if(del==NULL){
        printf("%d not found! Can't be deleted!\n",key);
        flag=false;
        return root;
    }
     if (root == NULL)
        return root;
    if ( key < root->data )
        root->left = delete(root->left, key);
    else if( key > root->data )
        root->right = delete(root->right, key);
    else
    {
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct node *temp = root->left ? root->left :root->right;
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else 
             *root = *temp; 
            free(temp);
        }
        else
        {
            struct node* temp = successor(root);
            root->data = temp->data;
            root->right = delete(root->right, temp->data);
        }
    }
 
    if (root == NULL)
      return root;
   return check_balance2(root);
   return root;
  
}

void print(struct node* root){
    if(root==NULL)return;
    printf("%d ",root->data);
    print(root->left);
    print(root->right);
}
int main(){
    int n;
    printf("Enter the number of nodes in the tree: ");
    scanf("%d",&n);
    int d;
    printf("Enter the numbers in the tree: ");
    scanf("%d",&d);
    tree_root=createnode(d);
    for(int i=1;i<n;i++){
        scanf("%d",&d);
        tree_root=insert(tree_root,d);
    }
    while(1){
            printf("Enter the query number: ");
            int e;
        scanf("%d",&d);
        // if(d==1||d==2||d==3)scanf("%d",&e);
           if(d==1){
            printf("Enter the number to be searched: ");
            scanf("%d",&e);
            struct node* found=search(tree_root,e);
            if(found==NULL)printf("%d not found\n",e);
            else printf("%d found\n",e);
           }
           else if(d==2){
            printf("Enter the number to be inserted: ");
            scanf("%d",&e);
              struct node* ins=insert(tree_root,e);
              if(flag)printf("%d inserted\n",e);
              flag=true;
           }
           else if(d==3){
            printf("Enter the number to be deleted: ");
            scanf("%d",&e);
              struct node* del=delete(tree_root,e);
              if(flag)printf("%d deleted!\n",e);
              flag=true;
           }
           else if(d==4){
            print_level(tree_root);
           }
           else{
            printf("Invalid query!\n");
           }
        }
    
}
