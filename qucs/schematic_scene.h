/*
 * schematic_scene.h - define schematic scene
 *
 * Copyright (C) 2014, Yodalee, lc85301@gmail.com
 *               2018, Felix Salfelder
 *
 * This file is part of Qucs
 *
 * Qucs is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Qucs.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SCHEMATICSCENE_H_
#define SCHEMATICSCENE_H_

#include <QGraphicsScene>
#include <QList> // BUG
#include <QGraphicsItem>
#include <assert.h>

#include "viewpainter.h"
#include "qt_compat.h"

class Element;

#ifdef USE_SCROLLVIEW
class SchematicScene {
  SchematicScene (QObject *);
  virtual ~SchematicScene ();
};
#else
class ElementGraphics;
class SchematicScene : public QGraphicsScene
{
Q_OBJECT
public:
  SchematicScene (QObject *);
  virtual ~SchematicScene ();

//  void addItem(ElementGraphics*);
  void addItem(QGraphicsItem* x){
	  QGraphicsScene::addItem(x);
  }
  void removeItem(QGraphicsItem const*x){
	  QGraphicsScene::removeItem((QGraphicsItem*)x);
  }

  void selectedItemsAndBoundingBox(QList<ElementGraphics*>& ElementCache, QRectF& BB);
  void removeItem(Element const*);

private:

protected:
  void drawBackground(QPainter *painter, const QRectF& rect);
};
#endif

#include "qt_compat.h"
#include "element.h" // TODO: move implementation to .cpp
                     // also: this relates to scene, but is this the right place?
							// (having other problems, still)

#if QT_MAJOR_VERSION < 5
// use naked pointer, as legacy qucs does.
typedef Element ElementGraphics;
#else
// A graphics element on the screen.
// kind of smart-pointer/proxy.
class ElementGraphics : public QGraphicsItem {
private:
	ElementGraphics();
	ElementGraphics(ElementGraphics const&){unreachable();}
public:
	explicit ElementGraphics(Element* e);
	~ElementGraphics(){ }
public:
	bool operator!=(Element const* e) const{
		return _e!=e;
	}
public: //?
  void paintScheme(Schematic *p);
private: // later: Qgraphics virtual overrides
//  void paint() { assert(_e); _e->paint(); }
//  void paintScheme(Schematic *s) { assert(_e); _e->paintScheme(s); }
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*); // const...

public:
  // TODO: move coordinate stuff here.
  void setPos(int a, int b);
public:
  Element& operator*(){ itested();
	  assert(_e); return *_e;
  }
  Element* operator->(){ itested();
	  assert(_e); return _e;
  }
  Element const* operator->() const{ untested();
	  assert(_e); return _e;
  }
  void setCenter(int i, int j, bool relative=false){
	  assert(_e);
	  _e->setCenter(i, j, relative);
  }
  void getCenter(int& i, int& j){
	  assert(_e);
	  _e->getCenter(i, j);
  }
	void toggleSelected(){
		assert(_e);
		setSelected(!isSelected());
	}

	// BUG: selected is stored in Element.
	void setSelected(bool s);

	int const& cx_() const { assert(_e); return _e->cx_(); }
	int const& cy_() const { assert(_e); return _e->cy_(); }
	int const& x1_() const { assert(_e); return _e->x1_(); }
	int const& y1_() const { assert(_e); return _e->y1_(); }
	int const& x2_() const { assert(_e); return _e->x2_(); }
	int const& y2_() const { assert(_e); return _e->y2_(); }
private:
  Element* _e;
}; // ElementGraphics

Component const* const_component(ElementGraphics const);
Wire const* const_wire(ElementGraphics const);
WireLabel const* const_wireLabel(ElementGraphics const);
Diagram const* const_diagram(ElementGraphics const);
Painting const* const_painting(ElementGraphics const);

Element* element(QGraphicsItem*);
Component* component(QGraphicsItem*);
Wire* wire(QGraphicsItem*);
WireLabel* wireLabel(QGraphicsItem*);
Diagram* diagram(QGraphicsItem*);
Painting* painting(QGraphicsItem*);
Graph* graph(QGraphicsItem*);
Marker* marker(QGraphicsItem*);
Node* node(QGraphicsItem*);
// Label* label(QGraphicsItem*);
#endif


// a mouse action on an element.
// formerly, a mouse action was implemented by means of messing with element
// internals.

#if 0
Component const* component(ElementMouseAction const*);
Wire const* wire(ElementMouseAction const*);
WireLabel const* wireLabel(ElementMouseAction const*);
Diagram const* diagram(ElementMouseAction const*);
Painting const* painting(ElementMouseAction const*);

Component* component(ElementMouseAction*);
Wire* wire(ElementMouseAction*);
WireLabel* wireLabel(ElementMouseAction*);
Diagram* diagram(ElementMouseAction*);
Painting* painting(ElementMouseAction*);
#endif

#endif /* SCHEMATICSCENE_H_ */