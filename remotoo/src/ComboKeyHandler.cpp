#include "ComboKeyHandler.h"

#include <unistd.h>
#include <algorithm>
#include <sys/wait.h>
#include <cctype>
#include <iostream>

namespace desktopstreamer
{
	ComboNode::ComboNode(const std::string & key)
		: _key(key)
	{
	}

	bool ComboNode::evaluate(std::list<std::string> & chain)
	{
		std::string st = chain.front();
		chain.pop_front();

		auto it = tree.find(st);
		if(it != tree.end())
		{
			return it->second.get()->evaluate(chain);
		}

		return false;
	}

	const std::string & ComboNode::getKey()
	{
		return _key;
	}

	ComboNode & ComboNode::registerNode(const std::string & key)
	{
		std::string lower = key;
		std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c){ return std::tolower(c);});

		auto it = tree.find(lower);
		if(it != tree.end())
		{
			return *(it->second.get());
		}

		std::unique_ptr<ComboNode> newNode = std::make_unique<ComboNode>(lower);
		ComboNode * result = newNode.get();
		tree[lower] = std::move(newNode);
		return *result;
	}

	ComboNode & ComboNode::registerLeaf(const std::string & key, const std::function<void(void)> & action)
	{
		std::string lower = key;
		std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c){ return std::tolower(c);});

		auto it = tree.find(lower);
		if(it != tree.end())
		{
			return *(it->second.get());
		}

		std::unique_ptr<ComboNode> newNode = std::make_unique<ComboLeaf>(lower, action);
		ComboNode * result = newNode.get();
		tree[lower] = std::move(newNode);
		return *result;
	}

	ComboLeaf::ComboLeaf(const std::string & key, const std::function<void(void)> & action)
		: ComboNode(key)
		,	_action(action)
	{
	}
	
	bool ComboLeaf::evaluate(std::list<std::string> & chain)
	{
    (void)chain;
		_action();
		return true;
	}

	ComboKeyHandler ComboKeyHandler::INSTANCE;

	ComboKeyHandler & ComboKeyHandler::getInstance()
	{
		return INSTANCE;
	}

	ComboKeyHandler::ComboKeyHandler()
	{
		specialKeys.insert("shift");
		specialKeys.insert("control");
		specialKeys.insert("alt");

		registerCombo("control").registerLeaf("v", []() 
		{
			// Do nothing, let the clipboard packet handle it 
		});

		registerCombo("control").registerLeaf("c", []() 
		{
			// Same 
		});

		// Terminal copy/paste
		registerCombo("shift").registerNode("control").registerLeaf("v", []() { ComboKeyHandler::getInstance().executeCommand("shift+Control+v"); });
		registerCombo("shift").registerNode("control").registerLeaf("c", []() { ComboKeyHandler::getInstance().executeCommand("shift+Control+c"); });

	}

	ComboKeyHandler::~ComboKeyHandler()
	{
	}

	ComboNode & ComboKeyHandler::registerCombo(const std::string & firstKey)
	{
		std::string lower = firstKey;
		std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c){ return std::tolower(c);});

		auto it = root.find(lower);
		if(it != root.end())
		{
			return *(it->second.get());
		}

		std::unique_ptr<ComboNode> newNode = std::make_unique<ComboNode>(lower);
		ComboNode * result = newNode.get(); 
		root[lower] = std::move(newNode);
		return *result;
	}

	bool ComboKeyHandler::evaluateKeyPress(const std::string & rawKey)
	{
		std::string key = rawKey; 
		std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){ return std::tolower(c);});

		auto it = specialKeys.find(key);
		if(it != specialKeys.end())
		{
			comboChain.push_back(key);
			return false;
		}
		else if(comboChain.size() > 0)
		{
			comboChain.push_back(key);

			const std::string first = comboChain.front();
			comboChain.pop_front();

			auto comboIt = root.find(first);
			bool result = false;

			if(comboIt != root.end())
			{
				result = comboIt->second.get()->evaluate(comboChain);
			}
			
			comboChain.clear();
			return result;
		}

		return false;
	}

	bool ComboKeyHandler::evaluateKeyUp(const std::string & rawKey)
	{
		if(comboChain.size() > 0)
		{
			std::string key = rawKey; 
			std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){ return std::tolower(c);});
			
			auto it = comboChain.begin();
			for(; it != comboChain.end(); it++)
			{
				if(*it == key)
				{
					comboChain.erase(it);
					break;
				}
			}
		}
		return false;
	}

	void ComboKeyHandler::executeCommand(const std::string & xdotoolCommand)
	{
		int child = fork();
		if(child == 0)
		{
			execl("/usr/bin/xdotool", "xdotool", "key", xdotoolCommand.c_str(), (char*)0);
		}
		else if (child > 0)
		{
			int status;
			waitpid(child, &status, 0);
		}
	}
}
