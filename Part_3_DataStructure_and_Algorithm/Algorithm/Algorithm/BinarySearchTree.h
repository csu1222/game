#pragma once
#include <iostream>

enum class Color
{
	Red = 0,
	Black = 1,
};
struct Node
{
	Node*	parent = nullptr;
	Node*	left = nullptr;
	Node*	right = nullptr;
	int		key = {};
	Color	color = Color::Black;
};


// Red-Black Tree
// 1) 모든 노드는 Red or Black
// 2) Root는 Black
// 3) Leaf(NIL)은 Black
// 4) Red노드의 자식은 Black(즉, 연속해서 Red는 안됨)
// 5) 각 노드 부터 Leaf 까지의 경로에서는 모두 같은 수의 Black 노드를 만난다.

class BinarySearchTree
{
public:
	BinarySearchTree();
	~BinarySearchTree();

	void	Print() { Print(_root, 10, 0); }
	void	Print(Node* node, int x, int y);

	Node*	Search(Node* node, int key);

	Node*	Min(Node* node);
	Node*	Max(Node* node);
	Node*	Next(Node* node);

	void	Insert(int key);
	void	InsertFixup(Node* node);

	void	Delete(int key);
	void	Delete(Node* node);

	// u 서브트리를 v 서브트리로 교체한다
	void	Replace(Node* u, Node* v);

	// Red-Black Tree
	void	LeftRotate(Node* node);
	void	RightRotate(Node* node);
private:
	Node*	_root = nullptr;
	Node*	_nil = nullptr;
};

