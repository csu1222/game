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
	// ���� ��ȸ (preorder traverse)
	// ���� ��ȸ (inorder )
	// ���� ��ȸ (postorder)

	//   [��]
	// [��][��]

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
	// ���ڷ� ���� key �� ��� �ִ� ���ο� ��带 ����ϴ�.
	Node* newNode = new Node();
	newNode->key = key;

	// ���� ���� ��Ʈ ��尡 nulltpr �̶�� ���� �߰��� ��尡 ��Ʈ��尡 �˴ϴ�.
	if (_root == nullptr)
	{
		_root = newNode;
		return;
	}

	// Ʈ���� �������� ���������Դϴ�. 
	Node* node = _root;
	Node* parent = nullptr;

	// node �� nullptr�� �ƴҶ� ���� �������ٴ� ���� Ʈ���� ���� �������� ���ٴ� ���Դϴ�. 
	while (node)
	{
		parent = node;
		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}

	// ���� while�� ����ߴٸ� node�� Ʈ���� �����̰� parent �� ���� ����� �θ����Դϴ�.
	newNode->parent = parent;

	// keyNode �� parent �� key ���� ���ؼ� ����, �������� ���մϴ�. 
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
		// ���� ������ ã��
		Node* next = Next(node);
		node->key = next->key;
		Delete(next);
	}
}

// u ����Ʈ���� v ����Ʈ���� ��ü�Ѵ�
// �׸��� delete u
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
