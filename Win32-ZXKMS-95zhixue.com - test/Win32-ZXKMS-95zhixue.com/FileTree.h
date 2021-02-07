#pragma once
#include <vector>
#include <string>
using namespace std;
#include "GetFilePath.h"
#include "SplitString.h"
#include "Tree.h"
/*
*功能：实现一个文件树类
*
*
*/
class NODEDATA {
public:
	int depth;
	BOOL isExpand;
	WIN32_FIND_DATA FileData;
};

class FileTree
{
public:
	Tree<NODEDATA> tree;	
	vector<WIN32_FIND_DATA> vFileData;
	vector<TreeNode<NODEDATA> *> vTreeNode;
	vector<std::wstring> vPath;
	LOGFONT lfList;
	DWORD MaxItemWidth=0;
	DWORD ItemHeight = 0;
	DWORD dIndent=20;

	FileTree();
	void SetLOGFONT(LOGFONT lfList);
	void Set_vPath(wstring szProjectPath);
	void InitFileTree(wstring szProjectPath, wstring szTempStr);
	void BuildFileTree(wstring wsProjectPath, TreeNode<NODEDATA> *pNode);
	void TreeToVector(TreeNode<NODEDATA> *pNode);
	void ClearVTreeNode();
};

FileTree::FileTree() {
	//Tree<NODEDATA> tree();
	
}
void FileTree::SetLOGFONT(LOGFONT lf) {
	lfList = lf;
}

void FileTree::InitFileTree(wstring szProjectPath, wstring szTempStr) {
	//Tree<NODEDATA> tree();

	NODEDATA NodeData;
	NodeData.depth = 0;
	NodeData.isExpand = TRUE;
	NodeData.FileData = getFileDirectory(szProjectPath)[0];
	
	TreeNode<NODEDATA> *pRoot = new TreeNode<NODEDATA>;
	pRoot->data = NodeData;
	pRoot->pFirstChild = nullptr;
	pRoot->pNextBrother = nullptr;

	tree.SetRoot(pRoot);
	Set_vPath(szTempStr);
	
	BuildFileTree(szProjectPath, tree.pRoot);
}
void FileTree::Set_vPath(wstring szProjectPath)
{
	vector<std::wstring> vChildrenStr=split(szProjectPath, L"\\");
	
	//WCHAR strTemp[MAX_PATH] = L"";
	std::wstring strTemp = L"";
	//此处考虑为空的问题
	if (vChildrenStr.size()==0) {
		return;
	}
	for (int i = 0; i < vChildrenStr.size()-1;i++) {
		//lstrcat(strTemp, vChildrenStr[i].c_str());
		//lstrcat(strTemp, L"\\");
		strTemp.append(vChildrenStr[i]);
		
		//MessageBox(NULL, strTemp.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		vPath.push_back(strTemp);
		strTemp.append(L"\\");
	}
	//MessageBox(NULL, szProjectPath.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
}

void FileTree::BuildFileTree(wstring wsProjectPath, TreeNode<NODEDATA> *pParentNode) {
	
	wstring wsFileDirectory;
	// 构造代表子目录和文件夹路径的字符串，使用通配符"*" 
	wsFileDirectory.assign(wsProjectPath);
	wsFileDirectory.append(L"\\*");

	//wchar_t tempStr1[10];
	//_itow_s(NodeData.id, tempStr1, 10);
	//MessageBox(NULL, szFileDirectory, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);

	vector<WIN32_FIND_DATA> FindFileData = getFileDirectory(wsFileDirectory);

	for (int i = 2; i < FindFileData.size(); i++) {
		NODEDATA NodeData;
		NodeData.depth = pParentNode->data.depth + 1;
		NodeData.isExpand = FALSE;
		NodeData.FileData = FindFileData[i];
		std::wstring wsChildrenPath;
		wsChildrenPath.append(wsProjectPath);
		wsChildrenPath.append(L"\\");
		wsChildrenPath.append(FindFileData[i].cFileName);
		
		for (int i = 0; i < vPath.size(); i++) {		
			//MessageBox(NULL, wsChildrenPath.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
			//MessageBox(NULL, vPath[i].c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
			if (wsChildrenPath.compare(vPath[i]) == 0) {
				
				NodeData.isExpand = TRUE;
			}
		}

		TreeNode<NODEDATA> *pNewNode = new TreeNode<NODEDATA>;
		pNewNode->data = NodeData;
		pNewNode->pFirstChild = nullptr;
		pNewNode->pNextBrother = nullptr;

		tree.InsertNode(pParentNode, pNewNode);
		
		if (NodeData.isExpand) {
			BuildFileTree(wsChildrenPath, pNewNode);
		}
	}
}


void FileTree::TreeToVector(TreeNode<NODEDATA> *pNode) {
	if (pNode == nullptr) {
		return;
	}
	vTreeNode.push_back(pNode);
	
	TreeToVector(pNode->pNextBrother);
	TreeToVector(pNode->pFirstChild);
		
}

void FileTree::ClearVTreeNode() {
	vector<TreeNode<NODEDATA> *>().swap(vTreeNode);
}