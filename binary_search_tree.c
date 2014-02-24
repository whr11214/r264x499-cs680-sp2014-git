#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>

typedef struct bst
{
	int key;
	struct bst *right;
	struct bst *left;
	struct bst *parent;
}bst;

struct bst *initial(int k)
{
	struct bst *root = (struct bst*)malloc(sizeof(struct bst));
	root->right   = NULL;
	root->left    = NULL;
	root->parent  = NULL;
	root->key     = k;

	return root;
}

void insert(struct bst *root,int k)
{
	struct bst *p = root;
	struct bst *q = p;

	int flag = -1; //0 means left,1 means right

	if(root == NULL)
	{
		printf("root is NULL\n");
		return;
	}
	while( p != NULL)
	{
		if(k < p->key)
		{
			q = p;
			p = p->left;
			flag = 0;
		}
		else
		{
			q = p;
			p = p->right;
			flag = 1;
		}
	}
	if(flag == 0)
	{
		p = (struct bst *)malloc(sizeof(struct bst));
		q->left    = p;
		p->parent  = q;
		p->right   = NULL;
		p->left    = NULL;
		p->key     = k;
	}
	else if(flag == 1)
	{
		p = (struct bst *)malloc(sizeof(struct bst));
		q->right   = p;
		p->parent  = q;
		p->right   = NULL;
		p->left    = NULL;
		p->key     = k;
	}
}

struct bst *minimum(struct bst *p)
{
	while(p->left != NULL)
		p = p->left;
	return p;
}

struct bst *successor(struct bst *p)
{
	struct bst *q;

	if(p->right != NULL)
		return minimum(p->right);
	q = p->parent;
	while((q != NULL) && (p == q->right))
	{
		p = q;
		q = q->parent;
	}
	return q;
}

void transplant(struct bst **root,struct bst *p,struct bst *q)  //replace p with q
{
	struct bst *b,*c;

	b = p;
	c = q;
	if(b->parent == NULL)
		*root = c;
	else if( b == b->parent->left)
		b->parent->left = c;
	else b->parent->right = c;


	if (c != NULL)
		c->parent = b->parent;
}

struct bst *delete(struct bst *root,struct bst *p)
{
	struct bst *q;
	struct bst *r;

	if(p->left == NULL)
	{
		r = p->right;
		transplant(&root,p,p->right);
		if(r == NULL)
			return root;
		if(r->parent == NULL)
			return r;
		return root;
	}
	else if(p->right == NULL)
	{
		r = p->left;
		transplant(&root,p,p->left);
		if(r == NULL)
			return root;
		if(r->parent == NULL)
			return r;
		return root;
	}
	else
	{
		q = minimum(p->right);
		if( q->parent != p)
		{
			transplant(&root,q,q->right);
			q->right = p->right;
			q->right->parent = q;
			if(q->parent == NULL)
				return q;
		}
		transplant(&root,p,q);
			q->left = p->left;
			q->left->parent = q;
			if(q->parent == NULL)
				return q;
	}
	//if(q->parent == NULL)
	//	root = q;
	free(p);
	return root;
}

struct bst *search(struct bst *p, int k) //find the first match and then return
{
	if((p == NULL) || (k == p->key))
		return p;
	if(k < p->key)
		return search(p->left,k);
	else
		return search(p->right,k);
}


void inorder(struct bst *p)
{
	if(p == NULL)
	{
	//	printf("This is an empty tree\n");	
		return;
	}
	inorder(p->left);

	printf(" %d ",p->key);

	inorder(p->right);
}

void preorder(struct bst *p)
{
	if(p == NULL)
	{
	//	printf("This is an empty tree\n");	
		return;
	}
	printf(" %d ",p->key);

	preorder(p->left);

	preorder(p->right);
}

void postorder(struct bst *p) 
{
	if(p == NULL)
	{
	//	printf("This is an empty tree\n");	
		return;
	}
	postorder(p->left);

	postorder(p->right);

	printf(" %d ",p->key);
}

struct bst *command(struct bst *root)
{
	int choice;
	int key;
	int num = 0, i;
	int arr[1000000];
	struct bst *target = NULL;
	struct bst *p = root;

	long int time_consuming = 0;
	struct timeval commencement,end;

	memset((void *)(&commencement),0,sizeof(struct timeval));
	memset((void *)(&end),0,sizeof(struct timeval));
	
	

	printf("-----------------------------------------\n");
	printf("| 1 : To initialize a binary search tree\n");
	printf("| 2 : To search certain node\n");
	printf("| 3 : To insert a node to this bst tree\n");
	printf("| 4 : To delete a node from this bst tree\n");
	printf("| 5 : To traverse this tree\n");
	printf("| 6 : To create random binary search tree\n");
	printf("| 7 : Exit\n");
	printf("-----------------------------------------\n");
	printf("Please make your choice between 1 to 7 : ");	
	scanf("%d",&choice);

	if((choice > 7) || (choice < 1))
	{
		printf("please input your choice between 1 to 7\n");
		return p;
	}

	switch(choice)
	{
		case 1:		//initialize
			{
				if(p != NULL)
				{
					printf("This bst tree has already initialized\n");
					return p;
				}

				printf("please input the key value of the root of the tree :");
				scanf("%d",&key);
				
				p = initial(key);

				return p;
			}
		case 2:		//search
			{
				if(p == NULL)
				{
					printf("This bst tree is empty, please initialize it before searching\n");
					return p;
				}
				
				printf("please input the key value you want to find : ");
				scanf("%d",&key);
				
				gettimeofday(&commencement,NULL);

				target = search(p,key);
				
				gettimeofday(&end,NULL);

				time_consuming = (end.tv_sec * 1000000 + end.tv_usec) - (commencement.tv_sec *1000000 + commencement.tv_usec);

				if(target == NULL)
					printf("%d is not in this bst tree\n",key);
				else
				{
					if(target->parent != NULL)
						printf("%d's parent is %d\n",key,target->parent->key);
					else
						printf("%d's parent is NULL\n",key);

					if(target->left != NULL)
						printf("%d's leftchild is %d\n",key,target->left->key);
					else
						printf("%d's leftchild is NULL\n",key);
					
					if(target->right != NULL)
						printf("%d's rightchild is %d\n",key,target->right->key);
					else
						printf("%d's rightchild is NULL\n",key);
				}
				
				printf("It takes %ld ms to find %d\n",time_consuming,key);
				
				return p;
			}
		case 3:		//insert
			{
				if(p == NULL)
				{
					printf("This bst tree is empty, please initialize it before insertion\n");
					return p;
				}
				printf("please input the key value you want : ");
				scanf("%d",&key);
				
				gettimeofday(&commencement,NULL);

				insert(p,key);
				
				gettimeofday(&end,NULL);

				time_consuming = (end.tv_sec * 1000000 + end.tv_usec) - (commencement.tv_sec *1000000 + commencement.tv_usec);
			
				printf("It takes %ld ms to insert %d into the tree\n",time_consuming,key);
				
				return p;
			}
		case 4:		//delete
			{
				if(p == NULL)
				{
					printf("This bst tree is empty, please initialize it before delete\n");
					return p;
				}
				
				printf("please input the key value you want to delete : ");
				scanf("%d",&key);
				if((target = search(p,key)) == NULL)
				{
					printf("%d is not in this tree\n",key);
					return p;
				}
				
				gettimeofday(&commencement,NULL);

				target = search(p,key);

				p = delete(p,target);
				
				gettimeofday(&end,NULL);

				time_consuming = (end.tv_sec * 1000000 + end.tv_usec) - (commencement.tv_sec *1000000 + commencement.tv_usec);
				
				printf("It takes %ld ms to delete %d from the tree\n",time_consuming,key);
				
				return p;
			}
		case 5:		//traverse
			{
				if(p == NULL)
				{
					printf("This bst tree is empty, please initialize it before traverse\n");
					return p;
				}
				printf("preorder : ");
				preorder(p);
				printf("\n");

				printf("inorder : ");
				inorder(p);
				printf("\n");
				
				printf("postorder : ");
				postorder(p);
				printf("\n");

				return p;
			}
		case 6:
			{
				printf("Please input the number of the array : ");
				scanf("%d",&num);
				if((num < 0) || (num > 1000000))
					printf("This number should less or equal than 1000000 and greater than 0\n");
				
				
				gettimeofday(&commencement,NULL);
				srand(commencement.tv_sec);

				printf("num = %d\n",num);
				
				for(i = 0;i < num ; i++)
				{
					arr[i] = rand();
					printf("arr[%d] = %d\n",i,arr[i]);
					if(i == 0)
					{
						p = initial(arr[i]);
					}
					else
						insert(p,arr[i]);
				}

				gettimeofday(&end,NULL);
				time_consuming = (end.tv_sec * 1000000 + end.tv_usec) - (commencement.tv_sec *1000000 + commencement.tv_usec);
				
				printf("Random binary search tree creation succeeded\n");

				printf("It takes %ld ms to create this tree\n",time_consuming);
				
				return p;
			}
		case 7:
			{
				exit(0);
			}
	}
}

int main(int argc,char *argv[])
{

	struct bst *root = NULL;
	
	while(1)
	{
		root = command(root);
		
		if(root != NULL)
			printf("root->key = %d\n",root->key);
	}
	
	return 0;
}
