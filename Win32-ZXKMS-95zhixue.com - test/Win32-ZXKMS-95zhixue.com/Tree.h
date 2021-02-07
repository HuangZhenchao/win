#pragma once

/*
*功能：实现一个树类
*
*
*/
class TVITEM {
public:
	int id;
	int depth;
	BOOL isExp;
	HWND hTreeViewItem;
};



template <class T>
class TreeNode
{
public:
	T data;
	TreeNode *pFirstChild= nullptr;
	TreeNode *pNextBrother= nullptr;
};

typedef void(*lpFunc)(TreeNode<TVITEM> *pNode);

template <class T>
class Tree
{
public:
	TreeNode<T> *pRoot= nullptr;
	Tree();
	void SetRoot(TreeNode<T> *pNode);
	void FindBrotherNode(TreeNode<T> *pNode, TreeNode<T> *pNewNode);
	void InsertNode(TreeNode<T> *pNode, TreeNode<T> *pNewNode);
	void Preorder(TreeNode<T> *pNode, lpFunc fCallback);
};

template <class T>
Tree<T>::Tree() {

}

template <class T>
void Tree<T>::SetRoot(TreeNode<T> *pNode) {
	pRoot = pNode;
}

template <class T>
void Tree<T>::FindBrotherNode(TreeNode<T> *pNode, TreeNode<T> *pNewNode) {
	if (pNode->pNextBrother == nullptr) {
		pNode->pNextBrother = pNewNode;
		return;
	}
	FindBrotherNode(pNode->pNextBrother, pNewNode);
}

template <class T>
void Tree<T>::InsertNode(TreeNode<T> *pNode, TreeNode<T> *pNewNode) {
	if (pNode->pFirstChild == nullptr) {
		pNode->pFirstChild = pNewNode;
	}
	else {
		FindBrotherNode(pNode->pFirstChild, pNewNode);
	}
}

template <class T>
void Tree<T>::Preorder(TreeNode<T> *pNode, lpFunc fCallback) {
	
	if (pNode==nullptr) {
		return;
	}
	
	fCallback(pNode);
	
	Preorder(pNode->pNextBrother, fCallback);
	Preorder(pNode->pFirstChild, fCallback);
	
}