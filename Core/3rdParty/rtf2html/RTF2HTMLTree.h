#ifndef _T_TREE_H_INCLUDED
#define _T_TREE_H_INCLUDED

#pragma warning(disable:4100)
#pragma warning(disable:4018)
#pragma warning(disable:4097)
#pragma warning(disable:4786)
#include "vector"
using namespace std;

template <class Type> class Ref;

template <class Type> class Data {
private:
	typedef unsigned long ULONG;
	Data(Type *p=NULL) { if(!p) p=new Type; pData=p; dwRefs=1;};
	ULONG AddRef() { dwRefs++; return dwRefs;};
 	ULONG Release() { 
		dwRefs--; if(dwRefs==0) {delete this;return 0;} return dwRefs;};

// accessors
	Type *operator->() { return pData;};
	operator Type&() { return *pData;};
	Type &get_Data() { return *pData;};
	~Data() { if(pData) delete pData;};
	// op new
	//
	Type *pData;
	ULONG dwRefs;
	friend class Ref<Type>;
};

template <class Type> class Ref {
	typedef Data<Type> DataT;
	
public:
	typedef unsigned long ULONG;
	typedef unsigned long KEY;
	// ctors
	Ref(const Type &t) { pData=new DataT(new Type(t));};
	Ref(Type *p) { if(p) pData=new DataT(p); else pData=NULL;};
	Ref() { pData=new DataT();};
	//Ref(Data<Type> *p) : pData(p) { if(p) p->AddRef();};
	Ref(const Ref<Type> &rhs) { pData=(rhs.pData); if(pData) pData->AddRef();};
	//dtor
	virtual ~Ref() {if(pData) pData->Release();};

	//operators =
	Ref &operator=(const Ref<Type> &rhs) 
	{ Release(); pData=rhs.pData; if(pData) pData->AddRef(); return *this;};
	Ref &operator=(const Type &t) 
	{ Release(); pData=new DataT(new Type(t)); return *this;};
	Ref &operator=(Type *p) 
	{ Release(); if(p) pData=new DataT(p); else pData=NULL; return *this;};
	// operator ==
	bool operator==(const Ref<Type> &rhs) 
	{ return pData==rhs.pData;};
	
	Ref Clone() { if(!pData) return Ref(); return Ref(pData->get_Data());};
	void Release() { if(pData) pData->Release(); pData=NULL;};
	bool IsNull() { return pData==NULL;};

// accessors	
	Type *operator->() {return pData->operator->();};
	Type &get_Data() { return pData->get_Data();};
	operator Type&() { return pData->get_Data();};
	KEY Key() { return ((KEY)pData)+1;};
protected: 
	ULONG GetRefCount() { if(!pData) return 0; return pData->dwRefs;};
	Ref(KEY key) { try{pData=(DataT*)(key-1);if(pData) pData->AddRef();}catch(...){pData=NULL;}};
private:
	Data<Type> *pData;
	friend class Ref<Type>;
// niama new	
};



template <class Type,class DataType2=NodeData<Type> > class Tree;
template <class Type> class NodeData {
private:	
	typedef Ref<NodeData> NodeRef;
	NodeData(const Type &Data) : tData(Data){};
	NodeData(Type *pData) : tData(pData) {};
	NodeData() : Parent((NodeData*)NULL) {};
	virtual ~NodeData() {};
private:
	Ref<Type> tData;
	std::vector<NodeRef> vChilds;
	Ref<NodeData> Parent;
	friend class Tree<Type>;
	friend class Ref<NodeData>;
	friend Data<NodeData>;
};



template <class Type,class DataType2=NodeData<Type> > class Tree : private Ref<DataType2> {
	typedef Ref<DataType2> NodeBase;
public:
	typedef Tree<Type> Node;
//	Node node;
	Node Parent() { if(!NodeBase::get_Data().IsNull()) return NodeBase::get_Data().Parent; return *this;};
	operator Type&() { return (NodeBase::get_Data().tData.get_Data()); };
	Type *operator->() {return &(NodeBase::get_Data().tData.get_Data());};
	__declspec( property( get=get_Data) ) Ref<Type> Data;
	Ref<Type> get_Data() { return NodeBase::get_Data().tData;};

	bool IsLeaf() { return NodeBase::get_Data().vChilds.size()==0;};
	bool IsNode() { return NodeBase::get_Data()vChilds.size()>0;};
	bool IsRoot() { return NodeBase::get_Data().Parent.IsNull();};
	bool operator ==(const Node &rhs) { return *(NodeBase*)this==(NodeBase &)rhs;};

	__declspec( property( get=get_Nodes) ) Node Nodes[];
	Node get_Nodes(int nIndex) { return (Node)NodeBase::get_Data().vChilds[nIndex]; };
	__declspec( property( get=get_Count) ) int Count;
	int get_Count() { return NodeBase::get_Data().vChilds.size();};
	__declspec( property( get=get_Key) ) unsigned long Key;
	unsigned long get_Key() { return NodeBase::Key();};

	Node AddNode(const Type &t) { Node n(t); ((NodeBase &)n).get_Data().Parent=(*this); NodeBase::get_Data().vChilds.push_back(n); return n;};
	void Delete() {
		if(!NodeBase::get_Data().Parent->IsNull()) NodeBase::get_Data().Parent->DeleteNode(*this);
		//Release();
	};
	void DeleteNode(int nIndex) {
		NodeBase::get_Data().vChilds.erase(NodeBase::get_Data().vChilds.begin()+nIndex);
	};
	void DeleteNode(Node &node) {
		vector<NodeBase>::iterator = find(NodeBase::get_Data().vChilds.begin(),NodeBase::get_Data().vChilds.end(),node);
		if(iterator!=NodeBase::get_Data().vChilds.end()) {
			NodeBase::get_Data().vChilds.erase(iterator);
		}
	};
	Tree(const Type &Data) : NodeBase(Data) {};
	Tree(Type *Data) : NodeBase(Data) {};
	Tree() {};
	Tree(const Tree<Type> &rhs) : NodeBase(rhs) {};
	Tree(const NodeBase &rhs) : NodeBase(rhs) {};
	Tree(KEY key) : NodeBase(key) {};
	virtual ~Tree() 
	{	// if RefCount==ChildsCount+1, now is the time for release this (i.e. remove all refs to parent from childs
		ReleaseNode();
	};

private:
	void ReleaseNode() {
		if(NodeBase::IsNull()) return;
		ULONG nRefs=NodeBase::GetRefCount();
		ULONG nChilds=NodeBase::get_Data().vChilds.size();
		if(nRefs==nChilds+1) {
			std::vector<NodeBase> &vChilds=NodeBase::get_Data().vChilds;
			for(ULONG n=0;n<vChilds.size();n++) {
				vChilds[n]->Parent.Release();
				((Node &)vChilds[n]).ReleaseNode();
			}
		}
		Release();
	}
};


#endif