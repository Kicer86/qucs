/***************************************************************************
    copyright            : (C) 2015, 2020 Felix Salfelder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sim.h"
#include "node.h"
#include <QString>
#include "globals.h"
#include "docfmt.h"
#include "command.h"
#include "schematic_lang.h"
#include "schematic_model.h"
#include "net.h"

// #include "schematic.h" // BUG, transition

unsigned gndhackn=0;

class PaintingList;
class DiagramList;
class WireList;
class ComponentList;

namespace {

static std::string netLabel(Net const* n)
{ untested();
	if(!n){ untested();
		unreachable();
		return("(null)");
	}else if(n->hasLabel()){ untested();
		return n->label().toStdString();
	}else{ untested();
		return "_net" + std::to_string(n->pos());
	}
}

class Verilog : public NetLang {
  virtual void printCommand(Command const*, QTextStream&) const;
  virtual void printSymbol(Symbol const*, QTextStream&) const;

private:
  SchematicModel const* modelhack;
} V;

static Dispatcher<DocumentLanguage>::INSTALL p(&doclang_dispatcher, "verilog", &V);

/*!
 * verilog does not know about commands
 */
void Verilog::printCommand(Command const* c, QTextStream& s) const
{ untested();
  s << "//" << c->label() << "\n";
}

void Verilog::printSymbol(Symbol const* sym, QTextStream& s) const
{ untested();
	Component const* c=dynamic_cast<Component const*>(sym);
#if 0
	if(!c){ untested();
		/// wires...
		incomplete();
		return;
// 	}else if(!c->is_device) { ?
	}else if(c->isOpen()) { untested();
		unreachable(); // wrong place.
	}else if(c->isShort()){ untested();
		// replace by some resistors (hack?)
		int z=0;
		QListIterator<Port *> iport(c->ports());
		Port *pp = iport.next();
		QString Node1 = pp->netLabel();
		while (iport.hasNext()){ untested();
			s << "R:" << c->label() << "." << QString::number(z++) << " "
				<< Node1 << " " << iport.next()->netLabel() << " R=\"0\"\n";
		}
	}else
#endif
	{ untested();
		s << QString::fromStdString(c->type()) << " ";

		QString comma="";
		s << "#(";
		for(auto p2 : c->params()) { untested();
			s << comma << "." << p2->name() << "(" << p2->Value << ")";
			comma = ", ";
		}
		s << ") ";
		s << c->label() << "(";

		// printPorts()
		comma = "";
		for(unsigned i=0; i < sym->numPorts(); ++i){ untested();
			s << comma << netLabel(sym->portValue(i));
			comma = ", ";
		}

		s << ");\n";
	}
}

/// --------------------------------------------------------
class VerilogSchematicFormat : public DocumentFormat{
  void save(DocumentStream& stream, SchematicSymbol const&) const;
  void load(DocumentStream& stream, SchematicSymbol&) const;

private: // legacy cruft
  bool isSymbolMode() const{ return false; }
  PaintingList const& symbolPaints(SchematicSymbol const& m) const{ untested();
    return m.symbolPaintings();
  }
  DiagramList const& diagrams(SchematicSymbol const& m) const{ untested();
    return m.diagrams();
  }
  PaintingList const& paintings(SchematicSymbol const& m) const{ untested();
    return m.paintings();
  }
  WireList const& wires(SchematicSymbol const& m) const{ untested();
    return m.wires();
  }
  NodeMap const& nodes(SchematicSymbol const& m) const{ untested();
    return m.nodes();
  }
  ComponentList const& components(SchematicSymbol const& m) const{ untested();
    return m.components();
  }

private: // hacks.
  void printSymbol(Symbol const*, stream_t&) const;
}VS;

static Dispatcher<DocumentFormat>::INSTALL
pp(&docfmt_dispatcher, "v_sch", &VS);
// ------------------------------------------------------

void VerilogSchematicFormat::load(DocumentStream& stream, SchematicSymbol& s) const
{ untested();
  incomplete();
}

void VerilogSchematicFormat::save(DocumentStream& stream, SchematicSymbol const& m) const
{ untested();
  for(auto pc : components(m)){ untested();
	  if(dynamic_cast<Command const*>(pc)){ untested();
		  unreachable();
		  // BUG. a Command is not a Component
		  continue;
	  }
	  printSymbol(pc, stream); // BUG: use V::printItem
	                           // but uses different port names...
  }
  for(auto w : wires(m)){ untested();
	  printSymbol(w, stream); // BUG: use V::printItem
  }
  for(auto const& n : nodes(m)){ untested();
	  int x, y;
	  std::tie(x, y) = n.position();
	  stream << "place #(.$xposition(" << x << "),"
		                 ".$yposition(" << y << "))"
							 <<  " place_" << x << "_" << y
							 << "(net_" << x << "_" << y << ");\n";
  }
}

//// not here.
//QTextStream& operator<<(QTextStream& o, std::string const& s)
//{ untested();
//		return o << QString::fromStdString(s);
//}

// similar to Verilog::printSymbol, but with the actual node names and
// coordinates.
void VerilogSchematicFormat::printSymbol(Symbol const* sym, stream_t& s) const
{ untested();
#if 0
	Component const* c=dynamic_cast<Component const*>(sym);
	if(!c){ untested();
		incomplete();
		unreachable();
		return;
	}else if(c->isOpen()) { untested();
		// parameter?
		incomplete();
		// }else if(c->isShort()){ untested();
		//   // parameter?
		//   incomplete();
	}else
#endif
	
	{ untested();
		std::string type = sym->type();
		std::string label = sym->label().toStdString();
		s << QString::fromStdString(type) << " ";

		if(label == "*"){ untested();
			// bug/feature/wtf?
			label="anonymous_gnd_hack_" + std::to_string(gndhackn++);
		}else{ untested();
		}

		QString comma="";
		s << "#(";
		for(unsigned i=0; i<sym->paramCount(); ++i) { untested();
			s << comma << "." << sym->paramName(i) << "(" << sym->paramValue(i) << ")";
			comma = ", ";
		}
		s << ") ";
		s << QString::fromStdString(label) << "(";

		comma = "";
		for(unsigned i=0; i<sym->numPorts(); ++i){ untested();
			trace3("...", sym->label(), i, sym->numPorts());
			auto p = sym->portPosition(i);
			auto x = p.first;
			auto y = p.second;
			s << comma << "net_" << x << "_" << y;
			comma = ", ";
		}

		s << ");\n";
	}
}

} // namespace
