#ifndef __REMOTOO_COMBOKEYHANDLER_H__
#define __REMOTOO_COMBOKEYHANDLER_H__

#include <list>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <memory>

namespace remotoo
{
	class ComboNode
	{
    public:
			ComboNode ( const std::string & key_ );
			virtual ~ComboNode ( );
			virtual bool evaluate ( std::list<std::string> & chain_ );
			const std::string & getKey();

			ComboNode & registerNode ( const std::string & key_ );
			ComboNode & registerLeaf ( const std::string & key_, const std::function<void(void)> & action_ );

		private:
			std::unordered_map < std::string, std::unique_ptr < ComboNode > > _tree;
			std::string _key;
	};

	class ComboLeaf : public ComboNode
	{
		public:
			ComboLeaf ( const std::string & key_, const std::function < void ( void ) > & action_ );
			bool evaluate( std::list<std::string> & chain_ );

    private:
			std::function < void ( void ) > _action;
	};

	class ComboKeyHandler
	{
		public:
			static ComboKeyHandler & getInstance ( );
      ~ComboKeyHandler ( );
			ComboNode & registerCombo ( const std::string & firstKey_ );
			bool evaluateKeyPress ( const std::string & key_ );
			bool evaluateKeyUp ( const std::string & key_ );
			void executeCommand ( const std::string & xdotoolCommand_ );

		private:
      ComboKeyHandler ( );
      static ComboKeyHandler _INSTANCE;
			std::unordered_map < std::string, std::unique_ptr < ComboNode > > _root;
			std::list < std::string > _comboChain;
			std::unordered_set < std::string > _specialKeys;
	};
}

#endif
