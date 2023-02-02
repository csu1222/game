#include "BinarySearchTree.h"
#include <iostream>
#include <Windows.h>
using namespace std;

enum class ConsoleColor
{
	BLACK = 0,
	RED = FOREGROUND_RED,
	GREEN = FOREGROUND_GREEN,
	BLUE = FOREGROUND_BLUE,
	YELLOW = RED | GREEN,
	WHITE = RED | GREEN | BLUE,
};


void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	::SetConsoleCursorPosition(output, pos);
}

void SetCursorColor(ConsoleColor color)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	::SetConsoleTextAttribute(output, static_cast<SHORT>(color));
}

BinarySearchTree::BinarySearchTree()
{
	_nil = new Node();

	_root = _nil;
}

BinarySearchTree::~BinarySearchTree()
{
	delete _nil;
}

void BinarySearchTree::Print(Node* node, int x, int y)
{
	if (node == _nil)
		return;

	SetCursorPosition(x, y);

	if (node->color == Color::Black)
		SetCursorColor(ConsoleColor::BLUE);
	else
		SetCursorColor(ConsoleColor::RED);

	cout << node->key;
	Print(node->left, x - (5 / (y + 1)), y + 1);
	Print(node->right, x + (5 / (y + 1)), y + 1);

	SetCursorColor(ConsoleColor::WHITE);
}

Node* BinarySearchTree::Search(Node* node, int key)
{
	/*
	if (node == nullptr || key == node->key)
		return node;

	if (key < node->key)
		return Search(node->left, key);
	else
		return Search(node->right, key);
	*/
	if (node == _nil || node->key == key)
		return node;
	
	if (key < node->key)
		return Search(node->left, key);
	else
		return Search(node->right, key);

}


Node* BinarySearchTree::Min(Node* node)
{
	while (node->left != _nil)
		node = node->left;

	return node;
}

Node* BinarySearchTree::Max(Node* node)
{
	while (node->right != _nil)
		node = node->right;

	return node;
}

Node* BinarySearchTree::Next(Node* node)
{
	/*if (node->right)
		return Min(node->right);

	Node* parent = node->parent;

	while (parent && node == parent->right)
	{
		node = parent;
		parent = parent->parent;
	}
	
	return parent;*/
	if (node->right != _nil)
		return Min(node->right);

	Node* parent = node->parent;
	while (parent != _nil && node == parent->right)
	{
		node = parent;
		parent = parent->parent;
	}
	return parent;

}

void BinarySearchTree::Insert(int key)
{
	// 인자로 받은 key 를 들고 있는 새로운 노드를 만듭니다.
	Node* newNode = new Node();
	newNode->key = key;
	
	// 트리의 리프까지 내려갈것입니다. 
	Node* node = _root;
	Node* parent = _nil;

	// node 가 nullptr이 아닐때 까지 내려간다는 것은 트리의 가장 리프까지 간다는 말입니다. 
	while (node != _nil)
	{
		parent = node;
		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}

	// 위의 while을 통과했다면 node는 트리의 리프이고 parent 는 리프 노드의 부모노드입니다.
	newNode->parent = parent;

	// keyNode 와 parent 의 key 값을 비교해서 왼쪽, 오른쪽을 정합니다. 
	if (parent == _nil)
		_root = newNode;
	else if (newNode->key < parent->key)
		parent->left = newNode;
	else
		parent->right = newNode;
	
	// 검사
	newNode->left = _nil;
	newNode->right = _nil;
	newNode->color = Color::Red;

	InsertFixup(newNode);
}

void BinarySearchTree::InsertFixup(Node* node)
{
	//- 1) parent = red, uncle = red                
	//- 2) parent = red, uncle = black(triangle)   
	//- 3) parent = red, uncle = black(list) 


	while (node->parent->color == Color::Red)
	{
		if (node->parent == node->parent->parent->left)
		{
			Node* uncle = node->parent->parent->right;
			if (uncle->color == Color::Red)
			{
				node->parent->color = Color::Black;			// p
				uncle->color = Color::Black;				// uncle
				node->parent->parent->color = Color::Red;	// pp
				node = node->parent->parent;
			}
			else
			{
				//		    [pp(B)] 
				//		[p(R)]   [u(B)]
				//		  [n(R)]

				if (node = node->parent->right) // Triangle 타입
				{
					node = node->parent;
					LeftRotate(node);
					//		    [pp(B)] 
					//		[p(R)]   [u(B)]
					//	  [n(R)]
				}

				// List 타입
				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				RightRotate(node->parent->parent);
			}
		}
		else
		{	//		     [pp(B)] 
			//		 [u(R)]   [p(R)]  
			//				[n(R)]
			Node* uncle = node->parent->parent->left;
			if (uncle->color == Color::Red)
			{
				node->parent->color = Color::Black;			// p
				uncle->color = Color::Black;				// uncle
				node->parent->parent->color = Color::Red;	// pp
				node = node->parent->parent;
			}
			else
			{
				//		     [pp(B)] 
				//		 [u(B)]   [p(R)]  
				//				[n(R)]

				if (node = node->parent->left) // Triangle 타입
				{
					node = node->parent;
					RightRotate(node);
					//		     [pp(B)] 
					//		 [u(B)]   [p(R)]  
					//					  [n(R)]
				}

				// List 타입
				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				LeftRotate(node->parent->parent);
			}
		}
	}

	_root->color = Color::Black;
}

void BinarySearchTree::Delete(int key)
{
	Node* deleteNode = Search(_root, key);
	Delete(deleteNode);
}

void BinarySearchTree::Delete(Node* node)
{
	//if (node == nullptr)
	//	return;

	//if (node->left == nullptr)
	//	Replace(node, node->right);
	//else if (node->right == nullptr)
	//	Replace(node, node->left);
	//else
	//{
	//	// 다음 데이터 찾기
	//	Node* next = Next(node);
	//	node->key = next->key;
	//	Delete(next);
	//}
	if (node == _nil)
		return;
	if (node->left == _nil)
		Replace(node, node->right);
	else if (node->right == _nil)
		Replace(node, node->left);
	else
	{
		// 다음 데이터 찾기 
		Node* next = Next(node);
		node->key = next->key;
		Delete(next);
	}
}

// u 서브트리를 v 서브트리로 교체한다
// 그리고 delete u
void BinarySearchTree::Replace(Node* u, Node* v)
{
	/*if (u->parent == nullptr)
		_root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;

	if (v)
		v->parent = u->parent;

	delete u;*/

	if (u->parent == _nil)
		_root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;

	if (v)
		v->parent = u->parent;

	delete u;
}


//			 [y]
//		 [x]     [3]
//     [1] [2]

//		  [x]     
//     [1]   [y]
//          [2] [3]
void BinarySearchTree::LeftRotate(Node* x)
{
	Node* y = x->right;

	x->right = y->left;

	if (y->left != _nil)
		y->left->parent = x;

	y->parent = x->parent;

	if (x->parent == _nil)
		_root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y; 

	y->left = x;
	x->parent = y;
}

//			 [y]
//		 [x]     [3]
//     [1] [2]

//		  [x]     
//     [1]   [y]
//          [2] [3]
void BinarySearchTree::RightRotate(Node* y)
{
	Node* x = y->left;

	y->left = x->right;

	if (y->right != _nil)
		y->right->parent = y;

	x->parent = y->parent;
	
	if (y->parent == _nil)
		_root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	x->right = y;
	y->parent = x;
}
