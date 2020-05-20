#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;
typedef int T;            

#define compLT(a,b) (a < b)
#define compEQ(a,b) (a == b)

typedef struct node 
{
	struct node* left;         
	struct node* right;       
	struct node* parent;      
	bool color;					/* BLACK == 0, RED == 1*/
	T data;                    
} Node;

#define MyNull &leaf           
Node leaf = { MyNull, MyNull, 0, false, 0 };
Node* root = MyNull;               
	
//Поворот налево
void rotateLeft(Node* x) 
{
	Node* y = x->right;

	x->right = y->left;
	if (y->left != MyNull)
	{
		y->left->parent = x;
	}
	if (y != MyNull)
	{
		y->parent = x->parent;
	}

	if (x->parent) 
	{
		if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	}
	else 
	{
		root = y;
	}

	y->left = x;
	if (x != MyNull) x->parent = y;
}

//Поворот направо
void rotateRight(Node* x) 
{
	Node* y = x->left;

	x->left = y->right;
	if (y->right != MyNull)
	{
		y->right->parent = x;
	}

	if (y != MyNull) y->parent = x->parent;
	if (x->parent) 
	{
		if (x == x->parent->right)
		{
			x->parent->right = y;
		}
		else
		{
			x->parent->left = y;
		}
	}
	else 
	{
		root = y;
	}

	y->right = x;
	if (x != MyNull)
	{
		x->parent = y;
	}
}

//Балансировка дерева после вставки
void insertFixup(Node* x) 
{
	while (x != root && x->parent->color) 
	{
		if (x->parent == x->parent->parent->left) 
		{
			Node* y = x->parent->parent->right;
			if (y->color) 
			{
				/* Дядя красный*/
				x->parent->color = false;
				y->color = false;
				x->parent->parent->color = true;
				x = x->parent->parent;
			}
			else 
			{
				/* Дядя черный */
				if (x == x->parent->right) {
					x = x->parent;
					rotateLeft(x);
				}
				x->parent->color = false;
				x->parent->parent->color = true;
				rotateRight(x->parent->parent);
			}
		}
		else 
		{
			Node* y = x->parent->parent->left;
			if (y->color == true) 
			{
				/* Дядя красный*/
				x->parent->color = false;
				y->color = false;
				x->parent->parent->color = true;
				x = x->parent->parent;
			}
			else 
			{
				/* Дядя черный */
				if (x == x->parent->left) 
				{
					x = x->parent;
					rotateRight(x);
				}
				x->parent->color = false;
				x->parent->parent->color = true;
				rotateLeft(x->parent->parent);
			}
		}
	}
	root->color = false;
}


//Вставка
Node* insertNode(T data) 
{
	Node* current, * parent, * x;
	current = root;
	parent = 0;
	while (current != MyNull) 
	{
		if (compEQ(data, current->data))
		{
			return (current);
		}
		parent = current;
		current = compLT(data, current->data) ?
			current->left : current->right;
	}

	x = (Node*)malloc(sizeof(*x));
	x->data = data;
	x->parent = parent;
	x->left = MyNull;
	x->right = MyNull;
	x->color = true;

	if (parent) 
	{
		if (compLT(data, parent->data))
		{
			parent->left = x;
		}
		else
		{
			parent->right = x;
		}
	}
	else
	{
		root = x;
	}

	insertFixup(x);
	return(x);
}


//Балансировка дерева после удаления
void deleteFixup(Node* x) 
{

	while (x != root && !x->color) 
	{
		if (x == x->parent->left) 
		{
			Node* w = x->parent->right;
			if (w->color) 
			{
				w->color = false;
				x->parent->color = true;
				rotateLeft(x->parent);
				w = x->parent->right;
			}
			if (!w->left->color && !w->right->color) 
			{
				w->color = true;
				x = x->parent;
			}
			else 
			{
				if (!w->right->color) 
				{
					w->left->color = false;
					w->color = true;
					rotateRight(w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = false;
				w->right->color = false;
				rotateLeft(x->parent);
				x = root;
			}
		}
		else 
		{
			Node* w = x->parent->left;
			if (w->color) 
			{
				w->color = false;
				x->parent->color = true;
				rotateRight(x->parent);
				w = x->parent->left;
			}
			if (!w->right->color && !w->left->color) 
			{
				w->color = true;
				x = x->parent;
			}
			else 
			{
				if (!w->left->color) 
				{
					w->right->color = false;
					w->color = true;
					rotateLeft(w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = false;
				w->left->color = false;
				rotateRight(x->parent);
				x = root;
			}
		}
	}
	x->color = false;
}


// Удаление
void deleteNode(Node* z) 
{
	Node* x, * y;
	if (!z || z == MyNull)
	{
		return;
	}
	if (z->left == MyNull || z->right == MyNull) {
		y = z;
	}
	else 
	{
		y = z->right;
		while (y->left != MyNull)
		{
			y = y->left;
		}
	}
	if (y->left != MyNull)
	{
		x = y->left;
	}
	else
	{
		x = y->right;
	}

	x->parent = y->parent;
	if (y->parent)
	{
		if (y == y->parent->left)
		{
			y->parent->left = x;
		}
		else
		{
			y->parent->right = x;
		}
	}
	else
	{
		root = x;
	}

	if (y != z)
	{
		z->data = y->data;
	}

	if (!y->color)
	{
		deleteFixup(x);
	}

	free(y);
}

//Поиск
Node* findNode(T data) 
{
	Node* current = root;
	while (current != MyNull)
		if (compEQ(data, current->data))
			return (current);
		else
			current = compLT(data, current->data) ?
			current->left : current->right;
	return(0);
}


//Обход
void inOrder(Node* node)
{
	if (node->right != MyNull)
	{
		inOrder(node->right);
	}
	printf("%d\n", node->data);
	if (node->left != MyNull)
	{
		inOrder(node->left);
	}
}

void deleteData(Node* node, T data)
{
	deleteNode(findNode(data));
}


void main(int argc, char** argv) 
{
	for (int i = 0; i < 1000000; i++)
	{
		insertNode(i);
	}
	insertNode(-47);
	insertNode(47);
	inOrder(root);
	deleteData(root, 3);
	deleteData(root, 2);
	deleteData(root, 3);
	deleteData(root, 4);
	deleteData(root, 5);
	deleteData(root, 6);
	deleteData(root, 7);
	deleteData(root, 8);
	deleteData(root, 9);

	cout << endl;
	inOrder(root);
}