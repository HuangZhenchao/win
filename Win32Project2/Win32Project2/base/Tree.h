#pragma once

/*
*功能：实现一个树类
*
*
*/


template <class T>
class TreeNode
{
public:
	T data;
	TreeNode *pFirstChild= nullptr;
	TreeNode *pNextBrother= nullptr;
	TreeNode *pLastBrother = nullptr;
};

typedef BOOL(*pcbTreeTraversalHandler)(void *,void *);

template <class T>
class Tree
{
public:
	TreeNode<T> *pRoot= nullptr;
	Tree();
	typedef void(*lpFunc)(TreeNode<T> *pNode);
	void SetRoot(TreeNode<T> *pNode);
	void FindBrotherNode(TreeNode<T> *pNode, TreeNode<T> *pNewNode);
	void InsertNode(TreeNode<T> *pNode, TreeNode<T> *pNewNode);
	void Preorder(TreeNode<T> *pNode, lpFunc fCallback);
	void TreePreorderTraversal(TreeNode<T> *pNode,pcbTreeTraversalHandler pcbTTHandler,void *pOtherData);
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
		if (pNode->pFirstChild->pNextBrother==nullptr) {
			pNode->pFirstChild->pNextBrother = pNewNode;
			pNode->pFirstChild->pLastBrother = pNewNode;
		}
		else {
			pNode->pFirstChild->pLastBrother->pNextBrother = pNewNode;
			pNode->pFirstChild->pLastBrother = pNewNode;
		}
		//FindBrotherNode(pNode->pFirstChild, pNewNode);
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

template <class T>
void Tree<T>::TreePreorderTraversal(TreeNode<T> *pNode,pcbTreeTraversalHandler pcbTTHandler, void *pOtherData) {
	//TreeNode<T> *pTempNode;
	stack<TreeNode<T> *> s;
	s.push(pNode);
	while (!s.empty())
	{
		TreeNode<T> *pTmp = (TreeNode<T> *)s.top();
		s.pop();
		
		if (pTmp->pNextBrother != NULL) s.push(pTmp->pNextBrother);
		if(pcbTTHandler(pTmp, pOtherData)){
			if (pTmp->pFirstChild != NULL) s.push(pTmp->pFirstChild);
		}
	}	
}