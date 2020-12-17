/***************************************************************************
    copyright            : (C) 2003 by Michael Margraf
                               2020 Felix Salfelder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// TODO: move
#include <stdlib.h>
#include <cmath>

#include "components/componentdialog.h"
#include "components/component.h"
#include "node.h"
#include "qucs.h"
#include "schematic_doc.h"
#include "viewpainter.h"
#include "module.h"
#include "misc.h"

#include <QPen>
#include <QString>
#include <QMessageBox>
#include <QPainter>
#include <QDebug>

CmdElement::CmdElement(CmdElement const& p)
  : Element(),
    mirroredX(p.mirroredX),
    rotated(p.rotated),
    //cx(p.cx),
    //cy(p.cy),
    tx(p.tx),
    ty(p.ty),
    showName(p.showName),
	 Name(p.Name),
    containingSchematic(p.containingSchematic)
{
  qDebug() << "component copy";

  assert(!Props.count());
  for(auto i : p.Props){
    Props.append(new Property(*i));
  }
}

/*!
 * \class Component
 * \brief The CmdElement class constitutes a baseclass for commands.
 *        this one is a copy of the legacy Component class, the former
 *        baseclass for CmdElements. As such, it contains unneeded stuff.
 */
CmdElement::CmdElement() : Element()
{
  mirroredX = false;
  rotated = 0;
  isActive = COMP_IS_ACTIVE;
  showName = true;

  _cx = 0;
  _cy = 0;
  tx = 0;
  ty = 0;

  Props.setAutoDelete(true);

  containingSchematic = NULL;
}
/*--------------------------------------------------------------------------*/
SchematicModel* CmdElement::scope()
{
	if(auto o=dynamic_cast<Symbol*>(owner())){
		return o->subckt();
	}else{ untested();
		return nullptr;
	}
}
/*--------------------------------------------------------------------------*/
// Size of component text.
int CmdElement::textSize(int& _dx, int& _dy)
{
  // get size of text using the screen-compatible metric
  QFontMetrics metrics(QucsSettings.font, 0);

  int tmp, count=0;
  _dx = _dy = 0;
  if(showName) {
    _dx = metrics.width(Name);
    _dy = metrics.height();
    count++;
  }else{
  }
  for(Property *pp = Props.first(); pp != 0; pp = Props.next()){
    if(pp->display) {
      // get width of text
      tmp = metrics.width(pp->Name+"="+pp->Value);
      if(tmp > _dx) {
	_dx = tmp;
      }else{
      }
      _dy += metrics.height();
      count++;
    }
  }
  return count;
}

// -------------------------------------------------------
// Boundings including the component text.
void CmdElement::entireBounds(int&, int&, int&, int&, float)
{
  // qt does that.
  assert(false);
}

// -------------------------------------------------------
// obsolete?
void CmdElement::setCenter(int x, int y, bool relative)
{
  if(relative) {
    _cx += x;  _cy += y;
  } else {
    _cx = x;  _cy = y;
  }
}

// -------------------------------------------------------
void CmdElement::getCenter(int& x, int& y)
{
  unreachable();
  x = cx();
  y = cy();
}

// -------------------------------------------------------
int CmdElement::getTextSelected(int x_, int y_, float Corr)
{
  x_ -= cx();
  y_ -= cy();
  if(x_ < tx) return -1;
  if(y_ < ty) return -1;

  x_ -= tx;
  y_ -= ty;
  int w, dy = int(float(y_) * Corr);  // correction for font scaling
  // use the screen-compatible metric
  QFontMetrics  metrics(QucsSettings.font, 0);
  if(showName) {
    w  = metrics.width(Name);
    if(dy < 1) {
      if(x_ < w) return 0;
      return -1;
    }
    dy--;
  }

  Property *pp;
  for(pp = Props.first(); pp != 0; pp = Props.next())
    if(pp->display)
      if((dy--) < 1) break;
  if(!pp) return -1;

  // get width of text
  w = metrics.width(pp->Name+"="+pp->Value);
  if(x_ > w) return -1; // clicked past the property text end - selection invalid
  return Props.at()+1;  // number the property
}

// -------------------------------------------------------
#if 0
bool CmdElement::getSelected(int x_, int y_)
{
  x_ -= cx();
  y_ -= cy();
  if(x_ >= x1) if(x_ <= x2) if(y_ >= y1) if(y_ <= y2)
    return true;

  return false;
}
#endif

// -------------------------------------------------------
void CmdElement::paint(ViewPainter *p) const
{
  int cx=_cx;
  int cy=_cy;
  int x, y, a, b, xb, yb;
  QFont f = p->Painter->font();   // save current font
  QFont newFont = f;
  {   // is simulation component (dc, ac, ...)
    unsigned size;
    if(!Texts.isEmpty()){
      size = Texts.first()->Size;
    }else{
      // possibly incomplete.
      size = 1;
    }
    newFont.setPointSizeF(p->Scale * size);
    newFont.setWeight(QFont::DemiBold);
    p->Painter->setFont(newFont);
    p->map(cx, cy, x, y);

    p->Painter->setPen(QPen(Qt::darkBlue,2));
    a = b = 0;
    QRect r, t;
    foreach(Text *pt, Texts) {
      t.setRect(x, y+b, 0, 0);
      p->Painter->drawText(t, Qt::AlignLeft|Qt::TextDontClip, pt->s, &r);
      b += r.height();
      if(a < r.width())  a = r.width();
    }
    xb = a + int(12.0*p->Scale);
    yb = b + int(10.0*p->Scale);
    x2 = x1+25 + int(float(a) / p->Scale);
    y2 = y1+23 + int(float(b) / p->Scale);
    if(ty < y2+1) if(ty > y1-r.height())  ty = y2 + 1;

    p->map(-1, 0, x, y);
    p->map(-6, -5, a, b);
    p->Painter->drawRect(a, b, xb, yb);
    p->Painter->drawLine(x,      y+yb, a,      b+yb);
    p->Painter->drawLine(x+xb-1, y+yb, x,      y+yb);
    p->Painter->drawLine(x+xb-1, y+yb, a+xb,   b+yb);
    p->Painter->drawLine(x+xb-1, y+yb, x+xb-1, y);
    p->Painter->drawLine(x+xb-1, y,    a+xb,   b);
  }

  // restore old font
  p->Painter->setFont(f);

  p->Painter->setPen(QPen(Qt::black,1));
  p->map(tx, ty, x, y);
  if(showName) {
    p->Painter->drawText(x, y, 0, 0, Qt::TextDontClip, Name);
    y += p->LineSpacing;
  }
  // write all properties
  for(Property *p4 = Props.first(); p4 != 0; p4 = Props.next())
    if(p4->display) {
      p->Painter->drawText(x, y, 0, 0, Qt::TextDontClip, p4->Name+"="+p4->Value);
      y += p->LineSpacing;
    }

  if(isActive == COMP_IS_OPEN)
    p->Painter->setPen(QPen(Qt::red,0));
  else if(isActive & COMP_IS_SHORTEN)
    p->Painter->setPen(QPen(Qt::darkGreen,0));
  if(isActive != COMP_IS_ACTIVE) {
    p->drawRect(+x1, +y1, x2-x1+1, y2-y1+1);
    p->drawLine(+x1, +y1, +x2, +y2);
    p->drawLine(+x1, +y2, +x2, +y1);
  }

#if 0
  // draw component bounding box
  if(isSelected()) {
    p->Painter->setPen(QPen(Qt::darkGray,3));
    p->drawRoundRect(cx+x1, cy+y1, x2-x1, y2-y1);
  }
#endif
} // CmdElement::paint

// -------------------------------------------------------
// For output on a printer device.
void CmdElement::print(ViewPainter *p, float FontScale)
{
  foreach(Text *pt, Texts)
    pt->Size *= FontScale;

  paint(p);

 foreach(Text *pt, Texts)
    pt->Size /= FontScale;
}
// -------------------------------------------------------
QString CmdElement::netlist()
{
  incomplete(); // remove.
  return "obsolete";
}
// -------------------------------------------------------
QString CmdElement::getNetlist()
{
  unreachable(); // obsolete
  return QString("");
}
// -------------------------------------------------------

// *******************************************************************
// ***  The following functions are used to load the schematic symbol
// ***  from file. (e.g. subcircuit, library component)

int CmdElement::analyseLine(const QString& Row, int numProps)
{
	unreachable();
	return 0;
}

// ---------------------------------------------------------------------
// not sure what this is. parse schematic?
bool CmdElement::getIntegers(const QString& s, int *i1, int *i2, int *i3,
			     int *i4, int *i5, int *i6)
{
	assert(false);
	bool ok;
	QString n;

	if(!i1) return true;
	n  = s.section(' ',1,1);
	*i1 = n.toInt(&ok);
	if(!ok) return false;

	if(!i2) return true;
	n  = s.section(' ',2,2);
	*i2 = n.toInt(&ok);
	if(!ok) return false;

	if(!i3) return true;
	n  = s.section(' ',3,3);
	*i3 = n.toInt(&ok);
	if(!ok) return false;

	if(i4) {
		n  = s.section(' ',4,4);
		*i4 = n.toInt(&ok);
		if(!ok) return false;
	}

	if(!i5) return true;
	n  = s.section(' ',5,5);
	*i5 = n.toInt(&ok);
	if(!ok) return false;

	if(!i6) return true;
	n  = s.section(' ',6,6);
	*i6 = n.toInt(&ok);
	if(!ok) return false;

	return true;
}

// ---------------------------------------------------------------------
bool CmdElement::getPen(const QString& s, QPen& Pen, int i)
{
	unreachable();
	bool ok;
	QString n;

	n = s.section(' ',i,i);    // color
	QColor co;
	co.setNamedColor(n);
	Pen.setColor(co);
	if(!Pen.color().isValid()) return false;

	i++;
	n = s.section(' ',i,i);    // thickness
	Pen.setWidth(n.toInt(&ok));
	if(!ok) return false;

	i++;
	n = s.section(' ',i,i);    // line style
	Pen.setStyle((Qt::PenStyle)n.toInt(&ok));
	if(!ok) return false;

	return true;
}

// ---------------------------------------------------------------------
bool CmdElement::getBrush(const QString& s, QBrush& Brush, int i)
{
	unreachable();
	bool ok;
	QString n;

	n = s.section(' ',i,i);    // fill color
	QColor co;
	co.setNamedColor(n);
	Brush.setColor(co);
	if(!Brush.color().isValid()) return false;

	i++;
	n = s.section(' ',i,i);    // fill style
	Brush.setStyle((Qt::BrushStyle)n.toInt(&ok));
	if(!ok) return false;

	i++;
	n = s.section(' ',i,i);    // filled
	if(n.toInt(&ok) == 0) Brush.setStyle(Qt::NoBrush);
	if(!ok) return false;

	return true;
}

// ---------------------------------------------------------------------
Property * CmdElement::getProperty(const QString& name)
{
  incomplete();
  for(Property *pp = Props.first(); pp != 0; pp = Props.next()){
    if(pp->Name == name) {
      return pp;
    }else{
    }
  }
  return NULL;
}

// ---------------------------------------------------------------------
// // BUG: implements assign
void CmdElement::copyComponent(Component *)
{
  incomplete();
#if 0 // WTF?
  Type = pc->Type;
  x1 = pc->x1;
  y1 = pc->y1;
  x2 = pc->x2;
  y2 = pc->y2;

  Model = pc->Model;
  Name  = pc->Name;
  showName = pc->showName;
  Description = pc->Description;

  isActive = pc->isActive;
  rotated  = pc->rotated;
  mirroredX = pc->mirroredX;
  tx = pc->tx;
  ty = pc->ty;

  Props  = pc->Props;
  Ports  = pc->Ports;
  Lines  = pc->Lines;
  Arcs   = pc->Arcs;
  Rects  = pc->Rects;
  Ellips = pc->Ellips;
  Texts  = pc->Texts;
#endif
}

// do something with Dialog Buttons
void CmdElement::dialgButtStuff(ComponentDialog& /*d*/)const
{
  incomplete();
  // d.disableButtons();
}
//
// BUG, tmp.
//void SchematicModel::simpleInsertCommand(CmdElement *)
//{
//  unreachable();
//}
