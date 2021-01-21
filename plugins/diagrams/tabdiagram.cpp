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

#include <QPolygon>
#include <QPainter>
#include <QPlainTextEdit>

#include "diagram.h"
#include "qucs.h"
#include <cmath>
#include "misc.h"
#include "globals.h"
#include "module.h"

#include "some_font_stuff.h"
/*--------------------------------------------------------------------------*/
namespace{
class DiagramWidget : public QPlainTextEdit{
public:
	DiagramWidget(Diagram* d)
	: _diag(d){ untested();
		assert(d);
		Element const*e = d;
		auto br(e->bounding_rect());
		trace2("DiagramWidget", br.tl(), br.br());
		setGeometry(br.toRectF().toRect()); // this only sets the SIZE.
		                          // origin is in topleft corner.
										  //
		setPlainText("TABDIAGRAM");
	}
// 	QSize sizeHint() const override{
// 		return QSize(30,30);
// 	}

private:
	Diagram* _diag;
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
class TabDiagram : public Diagram  {
private:
	TabDiagram(TabDiagram const& d) : Diagram(d) {}
public: 
  TabDiagram(int _cx=0, int _cy=0);
 ~TabDiagram();

  Element* clone() const { return new TabDiagram(*this); }
  static Element* info(QString&, char* &, bool getNewOne=false);
  virtual void paint(ViewPainter*);
  virtual void paintDiagram(ViewPainter *p);

private:
	QWidget* newWidget(){ untested();
		QWidget* w=new DiagramWidget(this);
		//w->move(_cx, _cy-y2); // gaah. the origin must be in the top left corner.
		w->move(0, -y2); // gaah. the origin must be in the top left corner.
		return w;
	}

protected:
  void calcData(Graph*) {};  // no graph data
}D;
Dispatcher<Diagram>::INSTALL p(&diagram_dispatcher, "Tab", &D);
Module::INSTALL pp("diagrams", &D);
/*--------------------------------------------------------------------------*/
TabDiagram::TabDiagram(int _cx, int _cy) : Diagram(_cx, _cy)
{
  x1 = 0;    // no extension to select area
  y1 = 0;
  x2 = x3 = 300;  // initial size of diagram
  y2 = 200;
  Name = "Tab";
  xAxis.limit_min = 0.0;  // scroll bar position (needs to be saved in file)

  // setName("Tab");
  calcDiagram();
}
/*--------------------------------------------------------------------------*/
TabDiagram::~TabDiagram()
{
}
/*--------------------------------------------------------------------------*/
void TabDiagram::paint(ViewPainter *p)
{
    paintDiagram(p);
}
/*--------------------------------------------------------------------------*/
void TabDiagram::paintDiagram(ViewPainter *p)
{
}
/*--------------------------------------------------------------------------*/
} // namespace
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
