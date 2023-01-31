#include "BinarySearchTree.h"
#include <iostream>
#include <Windows.h>
using namespace std;


void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	::SetConsoleCursorPosition(output, pos);
}


void BinarySearchTree::Print(Node* node, int x, int y)
{
	if (node == nullptr)
		return;

	SetCursorPosition(x, y);
	cout << node->key << endl;
	Print(node->left, x - (5 / (y + 1)), y + 1);
	Print(node->right, x + (5 / (y + 1)), y + 1);
}

void BinarySearchTree::Print_Inorder(Node* node)
{
	// 전위 순회 (preorder traverse)
	// 중위 순회 (inorder )
	// 후위 순회 (postorder)

	//   [중]
	// [좌][우]

	if (node == nullptr)
		return;

	cout << node->key << endl;

	Print_Inorder(node->left);
	Print_Inorder(node->right);
}

Node* BinarySearchTree::Search(Node* node, int key)
{
	if (node == nullptr || key == node->key)
		return node;

	if (key < node->key)
		return Search(node->left, key);
	else
		return Search(node->right, key);
}

Node* BinarySearchTree::Search2(Node* node, int key)
{
	while (node && key != node->key)
	{
		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}
	return node;
}

Node* BinarySearchTree::Min(Node* node)
{
	if (node->left == nullptr)
		return node;
	
	Min(node->left);
}

Node* BinarySearchTree::Max(Node* node)
{
	if (node->right == nullptr)
		return node;

	Max(node->right);
}

Node* BinarySearchTree::Next(Node* node)
{
	if (node->right)
		return Min(node->right);

	Node* parent = node->parent;

	while (parent && node == parent->right)
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

	// 가장 먼저 루트 노드가 nulltpr 이라면 새로 추가된 노드가 루트노드가 됩니다.
	if (_root == nullptr)
	{
		_root = newNode;
		return;
	}

	// 트리의 리프까지 내려갈것입니다. 
	Node* node = _root;
	Node* parent = nullptr;

	// node 가 nullptr이 아닐때 까지 내려간다는 것은 트리의 가장 리프까지 간다는 말입니다. 
	while (node)
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
	if (newNode->key < parent->key)
		parent->left = newNode;
	else
		parent->right = newNode;
}

void BinarySearchTree::Delete(int key)
{
	Node* deleteNode = Search(_root, key);
	Delete(deleteNode);
}

void BinarySearchTree::Delete(Node* node)
{
	if (node == nullptr)
		return;

	if (node->left == nullptr)
		Replace(node, node->right);
	else if (node->right == nullptr)
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
	if (u->parent == nullptr)
		_root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;

	if (v)
		v->parent = u->parent;

	delete u;
}
