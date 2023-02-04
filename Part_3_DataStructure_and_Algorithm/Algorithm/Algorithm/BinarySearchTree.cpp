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

void ShowConsoleCursor(bool flag)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	::GetConsoleCursorInfo(output, &cursorInfo);
	cursorInfo.bVisible = flag;
	::SetConsoleCursorInfo(output, &cursorInfo);
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

void BinarySearchTree::Print()
{
	::system("cls");
	ShowConsoleCursor(false);
	Print(_root, 10, 0);
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

				if (node == node->parent->right) // Triangle 타입
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

				if (node == node->parent->left) // Triangle 타입
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


// 먼저 BST 삭제 실행
// 
void BinarySearchTree::Delete(Node* node)
{
	if (node == _nil)
		return;
	if (node->left == _nil)
	{
		Color color = node->color;
		Node* right = node->right;

		Replace(node, node->right);

		if (color == Color::Black)
			DeleteFixup(right);
	}
	else if (node->right == _nil)
	{
		Color color = node->color;
		Node* left = node->left;

		Replace(node, node->left);
		if (color == Color::Black)
			DeleteFixup(left);
	}
	else
	{
		// 다음 데이터 찾기 
		Node* next = Next(node);
		node->key = next->key;
		Delete(next);
	}
}

// 먼저 BST 삭제 실행 ...
// - Case 1) 삭제할 노드가 Red -> 그냥 삭제! 끝!
// - Case 2) root가 DB -> 그냥 추가 Black 삭제! 끝!
// - Case 3) DB의 sibling 노드가 Red
// -- s = black, p = red (s <-> p 색상교환)
// -- DB 방향으로 rotate(p)
// -- goto other case
// - Case 4) DB 의 sibling 노드가 Black && sibling의 양쪽 자식도 Black
// -- 추가 Black을 Parent 에 이전 
// -- p가 Red 였으면 Black 이 됨.
// -- p가 Black 이었으면 DB가 됩.
// -- s = red
// -- p를 대상으로 알고리즘 이어서 실행 (p 가 DB가 되었을 경우)
// - Case 5) DB의 silbling 노드가 Black && sibling의 near child =red , far child = black
// -- s <-> near 색상교환
// -- far 방향으로 rotate(s)
// -- goto case 6
// - Case 6) DB의 sibling 노드가 Black && sibling의 for child = red
// -- p <-> s 색상교환 (이때 p가 red 였었다면 s -> far 가 연속으로 red 가됨)
// -- far = black
// -- rotate(p) DB 방향으로 
// -- 추가 Black 제거 
//
void BinarySearchTree::DeleteFixup(Node* node)
{
	Node* x = node;
	
	// 인자 node 가 black 이었다면 DB인 상황
	// [Case 1][Case 2] Fixup 할 노드가 red 라면? && Fixup 할 노드가 root 라면?
	while (x != _root && x->color == Color::Black)
	{
		// 지금 Fixup 을 하고 있는 대상이 그 부모의 왼쪽 자식인지 오른쪽 자식인지 대칭적으로 체크
		//       [p]
		// [x(B)]    [s]
		if (x == x->parent->left)
		{
			// [Case 3] sibling 의 색상이 Red라면?
			Node* s = x->parent->right;
			if (s->color == Color::Red)
			{
				s->color = Color::Black;
				x->parent->color = Color::Red;

				LeftRotate(x->parent);
				s = x->parent->right;
			}

			// [Case 4] DB 의 sibling 노드가 Black && sibling의 양쪽 자식도 Black 이라면?
			if (s->left->color == Color::Black && s->right->color == Color::Black)
			{
				s->color = Color::Red;
				x = x->parent;
			}
			else
			{ 
				// Case 5) DB의 silbling 노드가 Black && sibling의 near child =red , far child = black
				if (s->right->color == Color::Black)
				{
					s->left->color = Color::Black;
					s->color = Color::Red;
					RightRotate(s);
					s = x->parent->right;
				}
				// Case 6) DB의 sibling 노드가 Black && sibling의 for child = red
				s->color = x->parent->color;
				x->parent->color = Color::Black;
				s->right->color = Color::Black;
				LeftRotate(x->parent);
				x = _root;
			}

		}
		else
		{
			Node* s = x->parent->left;
			if (s->color == Color::Red)
			{
				s->color = Color::Black;
				x->parent->color = Color::Red;

				RightRotate(x->parent);
				s = x->parent->left;
			}

			if (s->right->color == Color::Black && s->left->color == Color::Black)
			{
				s->color = Color::Red;
				x = x->parent;
			}
			else
			{
				if (s->left->color == Color::Black)
				{
					s->right->color = Color::Black;
					s->color = Color::Red;
					LeftRotate(s);
					s = x->parent->left;
				}
				s->color = x->parent->color;
				x->parent->color = Color::Black;
				s->left->color = Color::Black;
				RightRotate(x->parent);
				x = _root;
			}
		}

	}
	 
	// 인자 node 가 red 였다면 black으로 색칠 
	x->color = Color::Black;
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
