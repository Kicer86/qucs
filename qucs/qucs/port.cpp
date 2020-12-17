/***************************************************************************
    copyright            : (C) 2020 Felix Salfelder / QUCS team
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "port.h"
#include "node.h"
/*--------------------------------------------------------------------------*/
Port::Port(Port const& p)
	: _p(p._p), _node(nullptr)
{
}
/*--------------------------------------------------------------------------*/
QString const& Port::netLabel() const
{
	assert(_node);
	return _node->netLabel();
}
/*--------------------------------------------------------------------------*/
void Port::connect(Node* n)
{
	assert(!_node);
	_node = n;
	_node->inc_ports();
}
/*--------------------------------------------------------------------------*/
void Port::disconnect(Node* n)
{
	assert(_node);
	n->dec_ports();

	_node = nullptr;
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
