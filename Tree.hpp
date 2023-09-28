#pragma once

#include <memory>
#include <vector>
#include <algorithm>

template<typename T>
class Tree
{
public:

	// Constructors
	Tree(std::unique_ptr< Tree<T> > left, std::unique_ptr< Tree<T> > right, T key)
		: m_left_tree(std::move(left)), m_right_tree(std::move(right)), m_key(key)
	{

	}

	// Accessors
	T getKey()
	{

	}

	void setKey(const T& new_key)
	{

	}

	// Methods
	static std::unique_ptr< Tree<T> > makeTree(std::vector<T>& keys)
	{
		return makeTreeInternal(keys, 0);
	}

	private std::unique_ptr< Tree<T> > makeTreeInternal(std::vector<T> keys, int axis)
	{
		std::sort(keys.begin(), keys.end(), compare<axis>);

		if (keys.size() == 0)
		{
			throw "Empty key vector";
		}
		else if (keys.size() == 1)
		{
			return std::make_unique< Tree<T> >(nullptr, nullptr, keys[0]);
		}

		axis = ++axis % 3;
		std::size_t middle = keys.size() / 2;

		std::unique_ptr< Tree<T> > left_tree = makeTreeInternal(std::vector<T>(keys.begin(), keys.begin() + middle), axis);
		std::unique_ptr< Tree<T> > right_tree = makeTreeInternal(std::vector<T>(keys.begin() + middle + 1, keys.end()), axis);

		return std::make_unique< Tree<T> >(left_tree, right_tree, keys[middle]);
	}

private:
	// Attributes
	std::unique_ptr<Tree> m_left_tree;
	std::unique_ptr<Tree> m_right_tree;
	T m_key;
};

