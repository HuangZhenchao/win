#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include "Tree.h"
#include "GetFilePath.h"
#include "SplitString.h"
using namespace std;
//包含rapidjson必要头文件,rapidjson文件夹拷贝到工程目录，或者设置include路径，或者加入到工程树  
#include "rapidjson/document.h"  
#include "rapidjson/filereadstream.h" 
#include "rapidjson/filewritestream.h"   
#include "rapidjson/encodedstream.h"    // AutoUTFInputStream
#include "rapidjson/prettywriter.h"  
#include "rapidjson/stringbuffer.h" 
#include "rapidjson/encodings.h"
using namespace rapidjson;  //引入rapidjson命名空间
class NODEDATA {
public:
	wstring NodeName;
	wstring NodePath;
	DWORD dIconType=1;	//默认1是文件，文件夹应该为2
	DWORD dButtonType=0;
	DWORD dLinesType=0;
	DWORD dNodeDepth=0;
	BOOL bExpanded=FALSE;
	BOOL bHasChildren= FALSE;
};

class TREEVIEWDATA {
public:
	
	DWORD DataSource;//1:文件夹树 2：文件树 3：json 4：数据库（sqlite3） 
	wstring RootPath;
	wstring DefaultPath;

	
	Tree<NODEDATA> tree;						//这是一个树数据结构
	vector<TreeNode<NODEDATA> *> vTreeNode;		//把树结构转为vector向量，为树形视图的每一项

	

	vector<std::wstring> PathStringToVector(wstring);
	TREEVIEWDATA(DWORD, wstring, wstring);
	void InitTree();
	void SetFirstLevel();
	void ExpandPath(TreeNode<NODEDATA> *,wstring);
	void ExpandDefaultPath(TreeNode<NODEDATA> *, vector<std::wstring>);
	void CollapsePath(wstring);

	void TreeToVector(TreeNode<NODEDATA> *pNode);

	//从文件系统获取文件夹下文件夹
	vector<wstring> EnumerateFolderInDirectory(wstring);
	//从文件系统获取文件夹下文件夹以及文件（所有文件或特定文件）
	vector<wstring> EnumerateFileInDirectory();
	//从json中获取
	vector<wstring> JsonIterator();
	//从数据库中获取
	vector<wstring> GetQueryResult();
};

TREEVIEWDATA::TREEVIEWDATA(DWORD dataSource, wstring rootPath,wstring defaultPath)
{
	DataSource = dataSource;
	RootPath = rootPath;
	DefaultPath = defaultPath;
	InitTree();
	SetFirstLevel();
	//PreLoad();
	//ExpandDefaultPath(tree.pRoot, PathStringToVector(DefaultPath));
	TreeToVector(tree.pRoot);
}
void TREEVIEWDATA::InitTree() 
{
	NODEDATA NodeData;
	NodeData.NodeName = L"ALL";
	NodeData.NodePath = RootPath;

	TreeNode<NODEDATA> *pRoot = new TreeNode<NODEDATA>;
	pRoot->data = NodeData;
	pRoot->pFirstChild = nullptr;
	pRoot->pNextBrother = nullptr;

	tree.SetRoot(pRoot);
}

void TREEVIEWDATA::SetFirstLevel()
{
	vector<wstring> ChildNodeList=EnumerateFolderInDirectory(RootPath);
	for (vector<wstring>::iterator iter=ChildNodeList.begin();iter!=ChildNodeList.end;iter++)
	{
		//获取数据后循环插入树
		NODEDATA NodeData;
		NodeData.NodeName = *iter;
		wstring temStr= RootPath;

		temStr.append(L"\\");
		temStr.append(*iter);
		NodeData.NodePath = temStr;

		TreeNode<NODEDATA> *pNewNode = new TreeNode<NODEDATA>;
		pNewNode->data = NodeData;
		pNewNode->pFirstChild = nullptr;
		pNewNode->pNextBrother = nullptr;

		tree.InsertNode(tree.pRoot, pNewNode);
	}
}
void TREEVIEWDATA::ExpandDefaultPath(TreeNode<NODEDATA> *pNewNode, vector<std::wstring> vPath)
{

}
void TREEVIEWDATA::ExpandPath(TreeNode<NODEDATA> *pNewNode,wstring path)
{

}
vector<wstring> TREEVIEWDATA::EnumerateFolderInDirectory(wstring Dir)
{
	return EnumerateFolder(Dir);
}

vector<std::wstring> TREEVIEWDATA::PathStringToVector(wstring PathString) {
	vector<std::wstring> vChildrenStr = split(PathString, L"\\");
	vector<std::wstring> vPath;
	//WCHAR strTemp[MAX_PATH] = L"";
	std::wstring strTemp = L"";
	//此处考虑为空的问题
	if (vChildrenStr.size() == 0) {
		return;
	}
	for (int i = 0; i < vChildrenStr.size() - 1; i++) {
		//lstrcat(strTemp, vChildrenStr[i].c_str());
		//lstrcat(strTemp, L"\\");
		strTemp.append(vChildrenStr[i]);

		//MessageBox(NULL, strTemp.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		vPath.push_back(strTemp);
		strTemp.append(L"\\");
	}
	return vPath;
}

void TREEVIEWDATA::TreeToVector(TreeNode<NODEDATA> *pNode) {
	if (pNode == nullptr) {
		return;
	}
	vTreeNode.push_back(pNode);

	TreeToVector(pNode->pNextBrother);
	TreeToVector(pNode->pFirstChild);

}