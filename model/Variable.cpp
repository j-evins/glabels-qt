//  Variable.cpp
//
//  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
//
//  This file is part of gLabels-qt.
//
//  gLabels-qt is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  gLabels-qt is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
//


#include "Variable.hpp"


namespace glabels::model
{

        Variable::Variable()
                : mType(Type::STRING),
                  mIncrement(Increment::NEVER),
                  mStepSize("0"),
                  mIntegerValue(0),
                  mIntegerStep(0),
                  mFloatingPointValue(0),
                  mFloatingPointStep(0)
        {
                // empty
        }


        Variable::Variable( Variable::Type      type,
                            const QString&      name,
                            const QString&      initialValue,
                            Variable::Increment increment,
                            const QString&      stepSize )
                : mType(type),
                  mName(name),
                  mInitialValue(initialValue),
                  mIncrement(increment),
                  mStepSize(stepSize),
                  mIntegerValue(0),
                  mIntegerStep(0),
                  mFloatingPointValue(0),
                  mFloatingPointStep(0)
        {
                resetValue();
        }


        Variable::Type Variable::type() const
        {
                return mType;
        }


        QString Variable::name() const
        {
                return mName;
        }


        QString Variable::initialValue() const
        {
                return mInitialValue;
        }


        Variable::Increment Variable::increment() const
        {
                return mIncrement;
        }


        QString Variable::stepSize() const
        {
                return mStepSize;
        }


        void Variable::setInitialValue( const QString& value )
        {
                mInitialValue = value;
        }


        void    Variable::resetValue()
        {
                switch (mType)
                {
                case Type::STRING:
                        // do nothing
                        break;
                case Type::INTEGER:
                        mIntegerValue = mInitialValue.toLongLong();
                        mIntegerStep  = mStepSize.toLongLong();
                        break;
                case Type::FLOATING_POINT:
                        mFloatingPointValue = mInitialValue.toDouble();
                        mFloatingPointStep  = mStepSize.toDouble();
                        break;
                case Type::COLOR:
                        // do nothing
                        break;
                }
        }


        void    Variable::incrementValueOnItem()
        {
                if ( mIncrement == Increment::PER_ITEM )
                {
                        switch (mType)
                        {
                        case Type::STRING:
                                // do nothing
                                break;
                        case Type::INTEGER:
                                mIntegerValue += mIntegerStep;
                                break;
                        case Type::FLOATING_POINT:
                                mFloatingPointValue += mFloatingPointStep;
                                break;
                        case Type::COLOR:
                                // do nothing
                                break;
                        }
                }
        }


        void    Variable::incrementValueOnCopy()
        {
                if ( mIncrement == Increment::PER_COPY )
                {
                        switch (mType)
                        {
                        case Type::STRING:
                                // do nothing
                                break;
                        case Type::INTEGER:
                                mIntegerValue += mIntegerStep;
                                break;
                        case Type::FLOATING_POINT:
                                mFloatingPointValue += mFloatingPointStep;
                                break;
                        case Type::COLOR:
                                // do nothing
                                break;
                        }
                }
        }


        void    Variable::incrementValueOnPage()
        {
                if ( mIncrement == Increment::PER_PAGE )
                {
                        switch (mType)
                        {
                        case Type::STRING:
                                // do nothing
                                break;
                        case Type::INTEGER:
                                mIntegerValue += mIntegerStep;
                                break;
                        case Type::FLOATING_POINT:
                                mFloatingPointValue += mFloatingPointStep;
                                break;
                        case Type::COLOR:
                                // do nothing
                                break;
                        }
                }
        }


        QString Variable::value() const
        {
                switch (mType)
                {
                case Type::STRING:
                        return mInitialValue;
                case Type::INTEGER:
                        return QString::number( mIntegerValue );
                case Type::FLOATING_POINT:
                        return QString::number( mFloatingPointValue, 'g', 15 );
                case Type::COLOR:
                        return mInitialValue;
                default:
                        return mInitialValue;
                }
        }


        QString Variable::typeToI18nString( Type type )
        {
                switch (type)
                {
                case Type::STRING:
                        return tr("String");
                case Type::INTEGER:
                        return tr("Integer");
                case Type::FLOATING_POINT:
                        return tr("Floating Point");
                case Type::COLOR:
                        return tr("Color");
                default:
                        return tr("String");
                }
        }


        QString Variable::typeToIdString( Type type )
        {
                switch (type)
                {
                case Type::STRING:
                        return "string";
                case Type::INTEGER:
                        return "integer";
                case Type::FLOATING_POINT:
                        return "float";
                case Type::COLOR:
                        return "color";
                default:
                        return "string";
                }
        }


        Variable::Type Variable::idStringToType( const QString& id )
        {
                if ( id == "string" )
                {
                        return Type::STRING;
                }
                else if ( id == "integer" )
                {
                        return Type::INTEGER;
                }
                else if ( id == "float" )
                {
                        return Type::FLOATING_POINT;
                }
                if ( id == "color" )
                {
                        return Type::COLOR;
                }
                else
                {
                        return Type::STRING; // Default
                }
        }


        QString Variable::incrementToI18nString( Increment increment )
        {
                switch (increment)
                {
                case Increment::NEVER:
                        return tr("Never");
                case Increment::PER_ITEM:
                        return tr("Per item");
                case Increment::PER_COPY:
                        return tr("Per copy");
                case Increment::PER_PAGE:
                        return tr("Per page");
                default:
                        return tr("Never");
                }
        }


        QString Variable::incrementToIdString( Increment increment )
        {
                switch (increment)
                {
                case Increment::NEVER:
                        return "never";
                case Increment::PER_ITEM:
                        return "per_item";
                case Increment::PER_COPY:
                        return "per_copy";
                case Increment::PER_PAGE:
                        return "per_page";
                default:
                        return "never";
                }
        }


        Variable::Increment Variable::idStringToIncrement( const QString& id )
        {
                if ( id == "never" )
                {
                        return Increment::NEVER;
                }
                else if ( id == "per_item" )
                {
                        return Increment::PER_ITEM;
                }
                else if ( id == "per_copy" )
                {
                        return Increment::PER_COPY;
                }
                else if ( id == "per_page" )
                {
                        return Increment::PER_PAGE;
                }
                else
                {
                        return Increment::NEVER; // Default
                }
        }


}
