/***************************************************************************
    copyright            : (C) 2015, 2019, 2020 Felix Salfelder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// qucsator simulator "driver"

#include "sim/sim.h"
#include "sckt_proto.h"
#include "net.h"
#include "docfmt.h" // <<
#include "paintings/paintings.h" // really??
#include "globals.h"

namespace {

static std::string netLabel(Net const* n)
{
	if(!n){
		unreachable();
		return("(null)");
	}else if(n->hasLabel()){
		return n->label().toStdString();
	}else{
		return "_net" + std::to_string(n->pos());
	}
}

static std::string mangleType(std::string& t)
{
	auto pos = t.find("$");
	std::string ret="";
	if(pos == std::string::npos){
	}else{
		ret = " Type=\"" + t.substr(pos+1) + "\"";
	}
	t = t.substr(0, pos);
	return ret;
}

// qucslang language implementation
class QucsatorLang : public NetLang {
private: // NetLang
  // inline void printItem(Element const* c, stream_t& s) const;

private: // local
  void printCommand(CmdElement const*, stream_t&) const;
  void printSymbol(Symbol const*, stream_t&) const;
  void printSubckt(SubcktProto const*, stream_t&) const;
  void printComponent(Component const*, stream_t&) const;
}qucslang;
static Dispatcher<DocumentFormat>::INSTALL p(&doclang_dispatcher, "qucsator", &qucslang);


void QucsatorLang::printSymbol(Symbol const* d, stream_t& s) const
{
	if(!d){ untested();
		incomplete();
	}else if(auto c=dynamic_cast<SubcktProto const*>(d)){
		printSubckt(c, s);
	}else if(auto c=dynamic_cast<CmdElement const*>(d)){ untested();
		printCommand(c, s);
	}else if(auto c=dynamic_cast<Component const*>(d)){
		printComponent(c, s);
	}else{ untested();
		s << "QucsatorLang::printSymbol incomplete\n";
		incomplete();
	}
}

// partly from Schematic::createSubnetlistplain
void QucsatorLang::printSubckt(SubcktProto const* p, stream_t& s) const
{
	trace2("prinSckt", p, p->subckt());
	Symbol const* sym = p;
	std::string label = p->label().toStdString();

	s << "\n";
	if(label.c_str()[3] == '$'){
		s << ".Def:" << label.substr(4);
	}else{
		incomplete();
	}

	// print_ports();
	//
	for(unsigned i=0; sym->portExists(i); ++i){
		std::string N = netLabel(p->portValue(i));
//		if(N=="0"){ untested();
//			N = "gnd";
//		}else{ untested();
//		}
		s << " " << N;
	}
	s << "\n";

	for(auto pi : p->symbolPaintings()){
		incomplete();
		if(pi->name() == ".ID ") { untested();
			incomplete();
			s<<"TODO " << pi->label() << pi->name() << "\n";
	//		ID_Text *pid = (ID_Text*)pi;
	//		QList<SubParameter *>::const_iterator it;
	//		for(it = pid->Parameter.constBegin(); it != pid->Parameter.constEnd(); it++) { untested();
	//			s = (*it)->Name; // keep 'Name' unchanged
	//			(*tstream) << " " << s.replace("=", "=\"") << '"';
	//			}
		}else{
		}
	//		break;
	}
	//(*tstream) << '\n';

	// TODO: deduplicate.
	trace1("sckt components", &p->schematicModel());
	trace1("sckt components", sym->scope());
	assert(sym->scope());
	for(auto i : p->schematicModel().components()){
      if(!i){ untested();
			incomplete();
		}else if(i->type() == "Port"){
		}else if(i->type() == "GND"){
		}else{
			trace1("ps", i->type());
			printSymbol(i, s);
		}
	}

	s << ".Def:End\n";
}

void QucsatorLang::printCommand(CmdElement const* c, stream_t& s) const
{itested();
	assert(c);
	s << "." << c->name() << ":" << c->label();

	//for(auto p2 : c->params())
	for(auto p2 : c->Props){ // BUG
		if(p2->name() == "Symbol") { // hack??
		}else if(p2->name()=="p" && p2->value()==""){itested();
			// unreachable
		}else{
			s << " " << p2->name() << "=\"" << p2->value() << "\"";
		}
	}
	s << '\n';
}

// print Component in qucsator language
void QucsatorLang::printComponent(Component const* c, stream_t& s) const
{
	if(c->isActive != COMP_IS_ACTIVE){
		// comment out?
		incomplete();
	}else{
	}
	assert(c);
	trace2("pc", c->label(), c->type());

	if(c->isOpen()) {
		// nothing.
	}else if(c->isShort()){ untested();
		// replace by some resistors (hack?)
		incomplete();
		int z=0;
		QListIterator<Port *> iport(c->ports());
		iport.next(); // BUG
		unsigned k=0;
		std::string Node1 = netLabel(c->portValue(k));
		while (iport.hasNext()){ untested();
			++k;
			s << "R:" << c->label() << "." << QString::number(z++) << " "
				<< Node1 << " " << netLabel( c->portValue(k) ) << " R=\"0\"\n";
		}
	}else{

		std::string type = c->type();
		std::string hack_type = mangleType(type);

		s << type << ":" << c->label();

		Symbol const* sym=c;
		trace3("print", c->label(), sym->numPorts(), sym->label());
		for(unsigned i=0; i<sym->numPorts(); ++i){
			std::string N = netLabel(sym->portValue(i));

			s << " " << N;
		}

		for(unsigned ii=0; ii<sym->paramCount(); ++ii) {
			trace3("param", c->label(), ii, sym->paramCount());
			std::string name = sym->paramName(ii);
			std::string value = sym->paramValue(ii);
			trace2("param", name, value);

			if(name==""){itested();
				incomplete(); // is_printable...
			}else if(name == "File") {
				// hack
			}else if(name == "Symbol") {
				// hack??
			}else{
				s << " " << name << "=\"" << value << "\"";
			}
		}

		s << hack_type;
		s << '\n';
	}
}

class Qucsator : public Simulator{
public:
	explicit Qucsator() : Simulator() {}
	Qucsator(Qucsator const&) = delete;
	~Qucsator(){}
private: // Simulator
  NetLang const* netLang() const override { untested();
	  return dynamic_cast<NetLang const*>(doclang_dispatcher["qucsator"]);
  }
  DocumentFormat const* netLister() const override {return docfmt_dispatcher["qucsator"];}
}QS;
static Dispatcher<Simulator>::INSTALL qq(&simulator_dispatcher, "qucsator", &QS);

}//namespace
