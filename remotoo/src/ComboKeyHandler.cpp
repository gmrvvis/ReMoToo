#include "ComboKeyHandler.h"

#include <unistd.h>
#include <algorithm>
#include <sys/wait.h>
#include <cctype>
#include <iostream>
#include <cassert>

#include "XdoWrapper.h"

namespace remotoo
{
	ComboNode::ComboNode ( const std::string & key_ )
		: _key ( key_ )
	{
	}

	ComboNode::~ComboNode ( )
	{

	}

	bool ComboNode::evaluate ( std::list<std::string> & chain_ )
	{
		std::string st = chain_.front ( );
		chain_.pop_front ( );

		auto it = _tree.find ( st );
		if( it != _tree.end ( ) )
		{
			return it->second.get ( )->evaluate ( chain_ );
		}

		return false;
	}

	const std::string & ComboNode::getKey ( )
	{
		return _key;
	}

	ComboNode & ComboNode::registerNode ( const std::string & key_ )
	{
		std::string lower = key_;
		std::transform ( lower.begin( ),
                     lower.end ( ), 
                     lower.begin ( ), 
                     [] ( unsigned char c ) { return std::tolower ( c ); } );

		auto it = _tree.find ( lower );
		if( it != _tree.end ( ) )
		{
			return *( it->second.get ( ) );
		}

		std::unique_ptr < ComboNode > newNode ( new ComboNode ( lower ) );
		ComboNode * result = newNode.get ( );
		_tree[ lower ] = std::move ( newNode );
		return *result;
	}

	ComboNode & ComboNode::registerLeaf ( const std::string & key_, 
                                        const std::function < void ( void ) > & action_ )
	{
		std::string lower = key_;
		std::transform ( lower.begin( ),
                     lower.end ( ), 
                     lower.begin ( ), 
                     [] ( unsigned char c ) { return std::tolower ( c ); } );

		auto it = _tree.find ( lower );
		if( it != _tree.end ( ) )
		{
			return *( it->second.get ( ) );
		}

		std::unique_ptr < ComboNode > newNode ( new ComboLeaf ( lower, action_ ) );
		ComboNode * result = newNode.get ( );
		_tree[ lower ] = std::move ( newNode );
		return *result;
	}

	ComboLeaf::ComboLeaf ( const std::string & key_, 
                         const std::function < void ( void ) > & action_ )
		: ComboNode ( key_ )
		,	_action ( action_ )
	{
	}
	
	bool ComboLeaf::evaluate ( std::list < std::string > & chain_ )
	{
		(void) chain_;
		assert ( chain_.size ( ) < 2 );
		_action ( );
		return true;
	}

	ComboKeyHandler ComboKeyHandler::_INSTANCE;

	ComboKeyHandler & ComboKeyHandler::getInstance ( )
	{
		return _INSTANCE;
	}

	ComboKeyHandler::ComboKeyHandler ( )
	{
		_specialKeys.insert ( "shift" );
		_specialKeys.insert ( "control" );
		_specialKeys.insert ( "alt" );

    // CNTRL + V
		registerCombo ( "control" )
      .registerLeaf ( "v", [] ( ) 
      {
        // Do nothing, let the clipboard packet handle it 
      } );

    // CNTRL + C
		registerCombo ( "control" )
      .registerLeaf( "c", [] ( ) 
      {
        // Same 
      } );

		// Terminal copy/paste
    // SHIFT + CNTRL + V
		registerCombo ( "shift" )
      .registerNode ( "control" )
      .registerLeaf ( "v", [] ( ) 
      { 
        ComboKeyHandler::getInstance ( )
          .executeCommand ( "shift+Control+v" ); 
      } );

    // SHIFT + CNTROL + C
		registerCombo ( "shift" )
      .registerNode ( "control" )
      .registerLeaf ( "c", [] ( ) 
      { 
        ComboKeyHandler::getInstance ( )
          .executeCommand ( "shift+Control+c" ); 
      } );

	}

	ComboKeyHandler::~ComboKeyHandler ( )
	{
	}

	ComboNode & ComboKeyHandler::registerCombo ( const std::string & firstKey_ )
	{
		std::string lower = firstKey_;

		std::transform ( lower.begin( ),
                     lower.end ( ), 
                     lower.begin ( ), 
                     [] ( unsigned char c ) { return std::tolower ( c ); } );

		auto it = _root.find ( lower );
		if ( it != _root.end ( ) )
		{
			return *( it->second.get ( ) );
		}

		std::unique_ptr < ComboNode > newNode ( new ComboNode ( lower ) );

		ComboNode * result = newNode.get ( ); 
		_root[ lower ] = std::move ( newNode );
		return *result;
	}

	bool ComboKeyHandler::evaluateKeyPress ( const std::string & rawKey_ )
	{
		std::string key = rawKey_; 
		std::transform ( key.begin( ),
                     key.end ( ), 
                     key.begin ( ), 
                     [] ( unsigned char c ) { return std::tolower ( c ); } );

		auto it = _specialKeys.find ( key );
		if( it != _specialKeys.end ( ) )
		{
			_comboChain.push_back ( key );
			return false;
		}
		else if ( _comboChain.size ( ) > 0 )
		{
			_comboChain.push_back ( key );

			const std::string first = _comboChain.front ( );
			_comboChain.pop_front ( );

			auto comboIt = _root.find ( first );
			bool result = false;

			if( comboIt != _root.end ( ) )
			{
				result = comboIt->second.get ( )->evaluate ( _comboChain );
			}
			
			_comboChain.clear ( );
			return result;
		}

		return false;
	}

	bool ComboKeyHandler::evaluateKeyUp ( const std::string & rawKey_ )
	{
		if ( _comboChain.size ( ) > 0 )
		{
			std::string key = rawKey_; 
			std::transform ( key.begin( ),
                     key.end ( ), 
                     key.begin ( ), 
                     [] ( unsigned char c ) { return std::tolower ( c ); } );
			
			auto it = _comboChain.begin ( );
			for( ; it != _comboChain.end ( ); it++ )
			{
				if( *it == key )
				{
					_comboChain.erase ( it );
					break;
				}
			}
		}
		return false;
	}

	void ComboKeyHandler::executeCommand ( const std::string & xdotoolCommand_ )
	{
		XdoWrapper::getInstance ( ).key ( xdotoolCommand_ );
	}
}
