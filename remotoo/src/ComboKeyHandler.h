#ifndef __DESKTOPSTREAMER_COMBOKEYHANDLER_H__
#define __DESKTOPSTREAMER_COMBOKEYHANDLER_H__

#include <list>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <memory>

namespace desktopstreamer
{
	class ComboNode
	{
		private:
			std::unordered_map<std::string, std::unique_ptr<ComboNode>> tree;
			std::string _key;
		public:
			ComboNode(const std::string & key);
      virtual ~ComboNode() {};
			virtual bool evaluate(std::list<std::string> & chain);
			const std::string & getKey();

			ComboNode & registerNode(const std::string & key);
			ComboNode & registerLeaf(const std::string & key, const std::function<void(void)> & action);
	};

	class ComboLeaf : public ComboNode
	{
		private:
			std::function<void(void)> _action;
		public:
      virtual ~ComboLeaf() {};
			ComboLeaf(const std::string & key, const std::function<void(void)> & action);
			bool evaluate(std::list<std::string> & chain);
	};

	class ComboKeyHandler
	{
		private:
			static ComboKeyHandler INSTANCE;
		public:
			static ComboKeyHandler & getInstance();
		private:
			std::unordered_map<std::string, std::unique_ptr<ComboNode>> root;
			std::list<std::string> comboChain;
			std::unordered_set<std::string> specialKeys;
		private:
			ComboKeyHandler();
		public:
			~ComboKeyHandler();
			ComboNode & registerCombo(const std::string & firstKey);
			bool evaluateKeyPress(const std::string & key);
			bool evaluateKeyUp(const std::string & key);
			void executeCommand(const std::string & xdotoolCommand);
	};
}

#endif
