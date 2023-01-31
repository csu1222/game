#pragma once

struct Node
{
	Node*	parent = nullptr;
	Node*	left = nullptr;
	Node*	right = nullptr;
	int		key = {};
};



class BinarySearchTree
{
public:
	void	Print() { Print(_root, 10, 0); }
	void	Print(Node* node, int x, int y);

	void	Print_Inorder() { Print_Inorder(_root); }
	void	Print_Inorder(Node* node);

	Node*	Search(Node* node, int key);
	Node*	Search2(Node* node, int key);

	Node*	Min(Node* node);
	Node*	Max(Node* node);
	Node*	Next(Node* node);

	void	Insert(int key);

	void	Delete(int key);
	void	Delete(Node* node);

	// u 서브트리를 v 서브트리로 교체한다
	void	Replace(Node* u, Node* v);
private:
	Node*	_root = nullptr;
};

