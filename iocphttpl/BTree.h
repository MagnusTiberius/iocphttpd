#pragma once
#include <vector>
#include <list>
#include "Entity.h"

namespace resource{

	template <class T>
	class BTree
	{
	public:
		BTree(void);
		~BTree(void);

	public:
		void add(T* node);
		std::list<std::wstring> toArray();
		void preorder(T* node);

	private:
		void add(T* currNode, T* node);
		static T* root;
		std::list<std::wstring> mArray;

	protected:
	};

}


namespace resource{

	template <class T>
	T* BTree<T>::root = NULL;

	template <class T>
	BTree<T>::BTree(void)
	{
		BTree::root = NULL;
	}

	template <class T>
	BTree<T>::~BTree(void)
	{
	}

	template <class T>
	void BTree<T>::add(T* node)
	{
		if (BTree<T>::root == NULL)
		{
			BTree<T>::root = node;
			return;
		}
		add(BTree<T>::root, node);
	}

	template <class T>
	void BTree<T>::add(T* currNode, T* node)
	{
		if (currNode->compare(*node) <= 0)
		{
			if (currNode->getRight() == NULL)
			{
				currNode->addRight(node);
			}
			else
			{
				add(currNode->getRight(), node);
			}
		}
		if (currNode->compare(*node) > 0)
		{
			if (currNode->getLeft() == NULL)
			{
				currNode->addLeft(node);
			}
			else
			{
				add(currNode->getLeft(), node);
			}
		}
	}

	template <class T>
	std::list<std::wstring> BTree<T>::toArray()
	{
		preorder(root);
		return mArray;
	}

	template <class T>
	void BTree<T>::preorder(T* node)
	{
		if (node->getLeft() != NULL)
		{
			preorder(node->getLeft());
		}
		mArray.push_back(node->getName());
		if (node->getRight() != NULL)
		{
			preorder(node->getRight());
		}
	}

}