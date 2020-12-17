/***************************************************************************
    copyright            : (C) 2020 Felix Salfelder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*--------------------------------------------------------------------------*/
#include "globals.h"
#include "symbol.h"
#include "viewpainter.h"
/*--------------------------------------------------------------------------*/
namespace {
/*--------------------------------------------------------------------------*/
class NodeLabel : public Symbol {
public:
	explicit NodeLabel() : Symbol(), _port(0,0) {}
	~NodeLabel(){}
private:
	NodeLabel(NodeLabel const& p)
     : Symbol(p), _port(p._port),
		 _highlight(p._highlight) {}
	Element* clone()const override{
		return new NodeLabel(*this);
	}

	void paint(ViewPainter*) const override;
	std::string getParameter(std::string const& n) const override;
	void setParameter(std::string const& n, std::string const& v) override;
	virtual unsigned numPorts() const {return 1;}
	QRectF boundingRect() const override;
	Port& port(unsigned i);
private:
	Port _port;
	bool _highlight;
}d;
static Dispatcher<Symbol>::INSTALL p(&symbol_dispatcher, "NodeLabel", &d);
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
std::string NodeLabel::getParameter(std::string const& n) const
{
	return Symbol::getParameter(n);
}
/*--------------------------------------------------------------------------*/
void NodeLabel::setParameter(std::string const& n, std::string const& v)
{
	if(n == "netname"){
		incomplete();
	}else{
		Symbol::setParameter(n, v);
	}
}
/*--------------------------------------------------------------------------*/
Port& NodeLabel::port(unsigned n)
{
	assert(n==0);
	return _port;
}
/*--------------------------------------------------------------------------*/
QRectF NodeLabel::boundingRect() const
{
    return QRectF(-5, -5, 5, 5);
}
/*--------------------------------------------------------------------------*/
void NodeLabel::paint(ViewPainter* p) const
{
	int w=5;

	int px = 10;
	int py = 10;

	p->drawLine(0, 0, px, py);
	p->drawLine(px, py, px+w, py);
	p->drawLine(px, py, px, py+w);
}
/*--------------------------------------------------------------------------*/

} // namespace
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
