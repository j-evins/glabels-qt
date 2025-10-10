/*  Variables.cpp
 *
 *  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
 *
 *  This file is part of gLabels-qt.
 *
 *  gLabels-qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gLabels-qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Variables.h"

#include <QtDebug>


namespace glabels
{
	namespace model
	{
		///
		/// Copy constructor
		///
		Variables::Variables( const Variables& other )
			: QMap<QString,Variable>(other)
		{
		}


		///
		/// Copy contents from other
		///
		void Variables::copy( const Variables& other )
		{
			clear();
			for ( const auto& v : other )
			{
				insert( v.name(), v );
			}
		}


		///
		/// Do we have variable?
		///
		bool Variables::hasVariable( const QString& name ) const
		{
			return contains(name);
		}


		///
		/// Add variable ( will replace if name is the same )
		///
		void Variables::addVariable( const Variable& variable )
		{
			insert( variable.name(), variable );
			emit changed();
		}


		///
		/// Delete variable
		///
		void Variables::deleteVariable( const QString& name )
		{
			remove( name );
			emit changed();
		}


		///
		/// Replace variable
		///
		void Variables::replaceVariable( const QString& origName, const Variable& variable )
		{
			remove( origName );
			insert( variable.name(), variable );
			emit changed();
		}


		///
		/// Set initial value of multiple variables
		///
		void Variables::setVariables( const QMap<QString,QString>& definitions )
		{
			for ( auto& name : definitions.keys() )
			{
				if ( hasVariable( name ) )
				{
					(*this)[name].setInitialValue( definitions[name] );
				}
				else
				{
					addVariable( Variable( Variable::Type::STRING,
					                       name,
					                       definitions[name] ) );
				}
			}
		}
		

		///
		/// Reset variables to their initial values
		///
		void Variables::resetVariables()
		{
			for ( auto& v : *this )
			{
				v.resetValue();
			}
		}

		
		///
		/// Reset "on copy" variables to their initial values
		///
		void Variables::resetOnCopyVariables()
		{
			for ( auto& v : *this )
			{
				if ( v.increment() == Variable::Increment::PER_COPY )
				{
					v.resetValue();
				}
			}
		}

		
		///
		/// Increment variables on item
		///
		void Variables::incrementVariablesOnItem()
		{
			for ( auto& v : *this )
			{
				v.incrementValueOnItem();
			}
		}

		
		///
		/// Increment variables on copy
		///
		void Variables::incrementVariablesOnCopy()
		{
			for ( auto& v : *this )
			{
				v.incrementValueOnCopy();
			}
		}

		
		///
		/// Increment variables on page
		///
		void Variables::incrementVariablesOnPage()
		{
			for ( auto& v : *this )
			{
				v.incrementValueOnPage();
			}
		}


	} // namespace model

} // namespace glabels
