/***************************************************************************
    copyright            : (C) 2018, 2020 Felix Salfelder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "command.h"
#include "element.h"
#include "globals.h"
#include "task_element.h"
#include "schematic_model.h"
#include "symbol.h"
#include "painting.h"
#include "language.h"
#include "d_dot.h"
#include "diagram.h"

Element* DocumentLanguage::parseItem(istream_t& s, Element* c) const
{
  if (DEV_DOT* d = dynamic_cast<DEV_DOT*>(c)) { untested();
    return parseCommand(s, d);
  }else{
	  incomplete();
	  return nullptr;
  }
}

void DocumentLanguage::printItem(Element const* c, ostream_t& s) const
{
  if(!c){
	  s << "unreachable, no item\n";
	  // assert(c);
  }else if (auto C=dynamic_cast<const TaskElement*>(c)) {
    printtaskElement(C, s);
  }else if (auto C=dynamic_cast<const Symbol*>(c)) {
    printSymbol(C, s);
  }else{
    incomplete();
  }
}

// borrowed from u_lang.h
void DocumentLanguage::new__instance(istream_t& cmd, Symbol* /*sckt?*/ owner,
		SchematicModel* Scope) const
{
	if (cmd.atEnd()) {untested();
		incomplete();
		assert(false);
		// nothing
	}else{
		std::string type = findType(cmd);
		trace2("new_instance", type, cmd.fullString());
		if (const Element* proto = find_proto(type, owner)) {
			if (Element* new_instance = proto->clone_instance()) {
				new_instance->setOwner(owner);
				Element* o = parseItem(cmd, new_instance);
				if (Element* x=dynamic_cast<Element*>(o)) {
					assert(Scope);
					Scope->pushBack(x);
				}else{
				}
			}else{ untested();
				cmd.warn(bDANGER, type + ": incomplete prototype");
			}
		}else{
			cmd.warn(bDANGER, type + ": no match");
		}
	}
}
/*--------------------------------------------------------------------------*/
Element const* DocumentLanguage::find_proto(const std::string& Name, const Element* Scope) const
{
	trace1("fp", Name);
	if(Scope){
		incomplete();
	}else{
	}
#if 0
  const CARD* p = NULL;
  if (Scope) {
    try {
      p = Scope->find_looking_out(Name);
    }catch (Exception_Cant_Find& e) {
      assert(!p);
    }
  }else{
    CARD_LIST::const_iterator i = CARD_LIST::card_list.find_(Name);
    if (i != CARD_LIST::card_list.end()) {
      p = *i;
    }else{
      assert(!p);
    }
  }
#else
  Element* p=nullptr;
#endif
  if (p) {
    return p;
  }else if ((command_dispatcher[Name])) { untested();
    return new DEV_DOT;	//BUG// memory leak
  }else if ((p = element_dispatcher[Name])) {
    return p;
  }else if ((p = symbol_dispatcher[Name])) {
    return p;
  }else if ((p = painting_dispatcher[Name])) {
    return p;
  }else if ((p = diagram_dispatcher[Name])) { untested();
    return p;
  }else{
#if 0
    assert(!p);
    std::string s;
    /* */if (Umatch(Name, "b{uild} "))      {untested(); s = "build";}
    else if (Umatch(Name, "del{ete} "))     {		 s = "delete";}
    else if (Umatch(Name, "fo{urier} "))    {untested(); s = "fourier";}
    else if (Umatch(Name, "gen{erator} "))  {		 s = "generator";}
    else if (Umatch(Name, "inc{lude} "))    {untested(); s = "include";}
    else if (Umatch(Name, "l{ist} "))       {untested(); s = "list";}
    else if (Umatch(Name, "m{odify} "))     {untested(); s = "modify";}
    else if (Umatch(Name, "opt{ions} "))    {            s = "options";}
    else if (Umatch(Name, "par{ameter} "))  {untested(); s = "param";}
    else if (Umatch(Name, "pr{int} "))      {untested(); s = "print";}
    else if (Umatch(Name, "q{uit} "))       {untested(); s = "quit";}
    else if (Umatch(Name, "st{atus} "))     {            s = "status";}
    else if (Umatch(Name, "te{mperature} ")){untested(); s = "temperature";}
    else if (Umatch(Name, "tr{ansient} "))  {            s = "transient";}
    else if (Umatch(Name, "!"))		    {untested(); s = "system";}
    else if (Umatch(Name, "<"))		    {untested(); s = "<";}
    else if (Umatch(Name, ">"))		    {untested(); s = ">";}
    else{ /* no shortcut available */
      s = Name;
    }
    if ((command_dispatcher[s])) {
      return new DEV_DOT; //BUG// we will look it up twice, //BUG// memory leak
    }else{
      return NULL;
    }
#endif
  }
      return NULL;
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
