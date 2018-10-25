/**
 * System design: LRU cache (least recently used).
 *
 * cpselvis(cpselvis@gmail.com)
 * Oct 8th, 2016
 */

#include<iostream>
#include<map>
#include <string>

using namespace std;

/**
 * Definition of cachelist node, it's double linked list node.
 */

struct CacheNode {
	std::string key;
	std::string value;
	CacheNode *pre, *next;
	CacheNode(std::string k, std::string v) : key(k), value(v), pre(NULL), next(NULL) {}
};

class LRUCache {
private:
	unsigned int size;                     // Maximum of cachelist size.
	CacheNode *head, *tail;
	map<std::string, CacheNode *> mp;          // Use hashmap to store
public:

	LRUCache(int capacity)
	{
		size = capacity;
		head = NULL;
		tail = NULL;
	}
	   
	std::string get(std::string key)
	{
		map<std::string, CacheNode *>::iterator it = mp.find(key);
		if (it != mp.end())
		{
			CacheNode *node = it->second;
			remove(node);
			setHead(node);
			return node->value;
		}
		else
		{
			return "";
		}
	}

	void set(std::string key, std::string value)
	{
		map<std::string, CacheNode *>::iterator it = mp.find(key);
		if (it != mp.end())
		{
			CacheNode *node = it->second;
			node->value = value;
			remove(node);
			setHead(node);
		}
		else
		{
			CacheNode *newNode = new CacheNode(key, value);
			if (mp.size() >= size)
			{
				map<std::string, CacheNode *>::iterator iter = mp.find(tail->key);
				remove(tail);
				mp.erase(iter);
			}
			setHead(newNode);
			mp[key] = newNode;
		}
	}

	void remove(CacheNode *node)
	{
		if (node->pre != NULL)
		{
			node->pre->next = node->next;
		}
		else
		{
			head = node->next;
		}
		if (node->next != NULL)
		{
			node->next->pre = node->pre;
		}
		else
		{
			tail = node->pre;
		}
	}

	void setHead(CacheNode *node)
	{
		node->next = head;
		node->pre = NULL;
		if (head != NULL)
		{
			head->pre = node;
		}
		head = node;
		if (tail == NULL)
		{
			tail = head;
		}
	}
};
