#pragma once

using namespace rapidjson;  //引入rapidjson命名空间
class NODEDATA {
public:
	wstring wsNodeName;
	wstring wsNodePath;
	DWORD dwNodeType=2;
	DWORD dwIconType=2;	//默认1是文件夹，文件应该为2
	DWORD dwButtonType=0;
	DWORD dwLinesType=0;
	DWORD dwNodeDepth=0;
	DWORD dwExpanded=0;
	DWORD dwHasChildren= 0;
	DWORD dwLoadChildrenDone=0;
	HWND hWndItem;
};

class TREEVIEWDATA {
public:
	
	DWORD DataSource;//1:文件夹树 2：文件树 3：json 4：数据库（sqlite3） 
	DWORD DataLoadWay=2;//树的数据加载方式，1：初始化时全部加载，2：动态加载，初始化或打开一个节点时，加载显示节点的下一层
	wstring RootPath;
	wstring DefaultPath;

	
	Tree<NODEDATA> tree;						//这是一个树数据结构
	vector<TreeNode<NODEDATA> *> vTreeNode;		//把树结构转为vector向量，为树形视图的每一项
	map<HWND, TreeNode<NODEDATA> *> m_HWND_TreeNode;
	HWND hWndControl;
	HINSTANCE hInstance;
	vector<std::wstring> PathStringToVector(wstring);
	TREEVIEWDATA(DWORD, DWORD,wstring, wstring);
	void InitTree();
	void ExpandCollapsePath(TreeNode<NODEDATA> *);
	void ExpandDefaultPath(TreeNode<NODEDATA> *, vector<std::wstring>);
	//void CollapsePath(wstring);

	void TreeToVector(TreeNode<NODEDATA> *pNode);
	static void fun1(WIN32_FIND_DATA);
	DWORD SetData(TreeNode<NODEDATA> *pNode);
	void RemoveData(TreeNode<NODEDATA> *pNode);

	DWORD PreLoad(TreeNode<NODEDATA> *);
	//从文件系统获取文件夹下文件夹
	//vector<wstring> EnumerateFolderInDirectory(wstring);
	//从文件系统获取文件夹下文件夹以及文件（所有文件或特定文件）
	//vector<wstring> EnumerateFileInDirectory();
	//从json中获取
	//vector<wstring> JsonIterator();
	//从数据库中获取
	//vector<wstring> GetQueryResult();
};

TREEVIEWDATA::TREEVIEWDATA(DWORD dataSource, DWORD dataLoadWay, wstring rootPath,wstring defaultPath)
{
	DataSource = dataSource;
	RootPath = rootPath;
	DefaultPath = defaultPath;
	DataLoadWay = dataLoadWay;
	
	
	//ExpandDefaultPath(tree.pRoot, PathStringToVector(DefaultPath));
	
}
void TREEVIEWDATA::InitTree()
{
	NODEDATA NodeData;
	NodeData.wsNodeName = L"ALL";
	NodeData.wsNodePath = RootPath;
	NodeData.dwNodeDepth = 0;
	NodeData.dwExpanded = 0;
	
	NodeData.dwNodeType = 1;
	NodeData.dwHasChildren = 0;
	NodeData.dwLoadChildrenDone = 1;

	TreeNode<NODEDATA> *pRoot = new TreeNode<NODEDATA>;
	pRoot->data = NodeData;
	pRoot->pFirstChild = nullptr;
	pRoot->pNextBrother = nullptr;

	tree.SetRoot(pRoot);
	NodeData.dwHasChildren = PreLoad(tree.pRoot);
	NodeData.dwLoadChildrenDone = 1;

	ExpandCollapsePath(tree.pRoot);
}

DWORD TREEVIEWDATA::PreLoad(TreeNode<NODEDATA> *pNode)
{
	vector<WIN32_FIND_DATA> ChildNodeList;
	FINDFILEPARAM *FindFileParam = new FINDFILEPARAM();
	FindFileParam->dwFileFloderFlag = 0x00000011;
	FindFileParam->szFileDirectory = pNode->data.wsNodePath;

	ChildNodeList = FileSystem::FindFile(FindFileParam);
	for (vector<WIN32_FIND_DATA>::iterator iter = ChildNodeList.begin(); iter != ChildNodeList.end(); iter++)
	{
		NODEDATA NodeData;
		NodeData.wsNodeName = iter->cFileName;
		//获取数据后循环插入树
		//再获取子节点是否有子项
		wstring temStr = pNode->data.wsNodePath;
		temStr.append(L"\\");
		temStr.append(iter->cFileName);

		if (iter->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) {
			NodeData.dwNodeType = 1;
			NodeData.dwIconType = 1;
			NodeData.dwHasChildren = 0;

		}

		NodeData.wsNodePath = temStr;
		NodeData.dwNodeDepth = pNode->data.dwNodeDepth + 1;

		TreeNode<NODEDATA> *pNewNode = new TreeNode<NODEDATA>;
		pNewNode->data = NodeData;
		pNewNode->pFirstChild = nullptr;
		pNewNode->pNextBrother = nullptr;

		tree.InsertNode(pNode, pNewNode);
		DWORD dItemID = m_HWND_TreeNode.size() + 1;
		HWND hWndItem = CreateWindowEx(NULL, L"MyTreeViewItem", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			0, 0, 0, 0, hWndControl, (HMENU)dItemID, hInstance, NULL);
		m_HWND_TreeNode.insert(map<HWND, TreeNode<NODEDATA> * >::value_type(hWndItem, pNewNode));
		pNewNode->data.hWndItem = hWndItem;
	}
	return ChildNodeList.size();
}



DWORD TREEVIEWDATA::SetData(TreeNode<NODEDATA> *pNode)
{
	vector<WIN32_FIND_DATA> ChildNodeList;
	switch (DataSource)
	{
	case 1://1:文件夹树
	{		
			FINDFILEPARAM *FindFileParam = new FINDFILEPARAM();
			FindFileParam->dwFileFloderFlag = 0x00000011;
			FindFileParam->szFileDirectory = pNode->data.wsNodePath;

			ChildNodeList = FileSystem::FindFile(FindFileParam);
			
			//再预加载一层数据
			for (vector<WIN32_FIND_DATA>::iterator iter = ChildNodeList.begin(); iter != ChildNodeList.end(); iter++)
			{
				NODEDATA NodeData;
				NodeData.wsNodeName = iter->cFileName;
				//获取数据后循环插入树
				//再获取子节点是否有子项
				wstring temStr = pNode->data.wsNodePath;
				temStr.append(L"\\");
				temStr.append(iter->cFileName);

				FindFileParam->szFileDirectory = temStr;
				if(iter->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
					NodeData.dwNodeType = 1;
					NodeData.dwIconType = 1;
					NodeData.dwHasChildren = FileSystem::FindFile(FindFileParam).size();
					
				}
				
				NodeData.wsNodePath = temStr;
				NodeData.dwNodeDepth = pNode->data.dwNodeDepth + 1;
				TreeNode<NODEDATA> *pNewNode = new TreeNode<NODEDATA>;
				pNewNode->data = NodeData;
				pNewNode->pFirstChild = nullptr;
				pNewNode->pNextBrother = nullptr;

				tree.InsertNode(pNode, pNewNode);
			}
	}
	break;
	case 2: //2：文件树 
		//ChildNodeList = EnumerateFolderInDirectory(RootPath);
		break;
	case 3://3：json 
		//ChildNodeList = EnumerateFolderInDirectory(RootPath);
		break;
	case 4://4：数据库（sqlite3）
		//ChildNodeList = EnumerateFolderInDirectory(RootPath);
		break;
	default:
		break;
	}
	return ChildNodeList.size();
	
}

void TREEVIEWDATA::ExpandDefaultPath(TreeNode<NODEDATA> *pNewNode, vector<std::wstring> vPath)
{

}

void TREEVIEWDATA::ExpandCollapsePath(TreeNode<NODEDATA> *pNode)
{
	pNode->data.dwExpanded = (~pNode->data.dwExpanded)<<31>>31;
	if ((pNode->data.dwExpanded==1) && (pNode->data.dwHasChildren>0) &&(pNode->data.dwLoadChildrenDone == 0)) {
		//MessageBox(NULL, L"打开一个节点", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		//SetData(pNode);
		if (pNode->pFirstChild != nullptr) {		
			pNode->pFirstChild->data.dwHasChildren = PreLoad(pNode->pFirstChild);
			pNode->pFirstChild->data.dwLoadChildrenDone = 1;
			TreeNode<NODEDATA> *pTempNode;
			pTempNode = pNode->pFirstChild->pNextBrother;
			while (pTempNode != nullptr) {
				pTempNode->data.dwHasChildren = PreLoad(pTempNode);
				pTempNode->data.dwLoadChildrenDone = 1;
				pTempNode = pTempNode->pNextBrother;
			}
		}
		
	}
	else {
		//wchar_t tempStr1[32];
		//_itow_s(pNode->data.dwExpanded, tempStr1, 32);
		//MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		//RemoveData(pNode);
	}
	//vector<TreeNode<NODEDATA> *>().swap(vTreeNode);
	//TreeToVector(tree.pRoot);
	TraverseTVItem();
} 

vector<std::wstring> TREEVIEWDATA::PathStringToVector(wstring PathString) {
	vector<std::wstring> vChildrenStr = split(PathString, L"\\");
	vector<std::wstring> vPath;
	//WCHAR strTemp[MAX_PATH] = L"";
	std::wstring strTemp = L"";
	//此处考虑为空的问题
	if (vChildrenStr.size() == 0) {
		return vPath;
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
	//MessageBox(NULL, pNode->data.NodePath.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
	if (pNode->data.dwExpanded) {
		TreeToVector(pNode->pFirstChild);
	}
	TreeToVector(pNode->pNextBrother);
}

void TREEVIEWDATA::fun1(WIN32_FIND_DATA FindFileData) {
	MessageBox(NULL, FindFileData.cFileName, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
}

void TREEVIEWDATA::RemoveData(TreeNode<NODEDATA> *pNode) {
	if (pNode->pFirstChild != nullptr) {
		
		TreeNode<NODEDATA> *pTempNode;
		pTempNode = pNode->pFirstChild->pNextBrother;
		while (pTempNode != nullptr) {		
			TreeNode<NODEDATA> *pDelNode= pTempNode;
			
			pTempNode = pTempNode->pNextBrother;
			delete pDelNode;
		}
		delete pNode->pFirstChild;
		pNode->pFirstChild = nullptr;
	}
}
/*
if (pNode->pFirstChild != nullptr) {
pNode->pFirstChild->data.bHasChildren = PreLoad(pNode->pFirstChild);
TreeNode<NODEDATA> *pTempNode;
pTempNode = pNode->pFirstChild->pNextBrother;
while (pTempNode!= nullptr) {
pTempNode->data.bHasChildren=PreLoad(pTempNode);
pTempNode = pTempNode->pNextBrother;
}
}
*/