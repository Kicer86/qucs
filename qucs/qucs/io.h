/***************************************************************************
    copyright            : 2018, 2020 Felix Salfelder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QUCS_IO_H
#define QUCS_IO_H

#include <QTextStream> // BUG
#include "trace.h"

class QFile;

// output stream
class DocumentStream : public QTextStream {
private:
	DocumentStream(DocumentStream const&) = delete;
public:
//	explicit DocumentStream(){ incomplete(); }
	explicit DocumentStream(QFile* /* BUG const */ file);
	explicit DocumentStream(QString /* BUG const */ * filename) :
		QTextStream(filename, QIODevice::WriteOnly){}

public:
	DocumentStream& operator<<(std::string const& x){
		QTextStream::operator<<(QString::fromStdString(x));
		return *this;
	}
	template<class T>
	DocumentStream& operator<<(T x){
		QTextStream::operator<<(x);
		return *this;
	}
	//bool atEnd() const{return QTextStream::atEnd();}
};

typedef DocumentStream ostream_t;

// CS like stream. replace by CS...
class istream_t : private QTextStream{
public:
  enum STRING {_STRING};
private:
	istream_t(istream_t const&) = delete;
public:
	template<class T>
	istream_t(T t) : QTextStream(t), _cmd(""), _cnt(0), _ok(true) { }
	istream_t(istream_t::STRING, const std::string&s );

private: // there are more in ap.h
	std::string ctos(const std::string& term=",=(){};",
	                 const std::string& b="\"'{",
	                 const std::string& e="\"'}",
	                 const std::string& trap="");
	int ctoi();

  istream_t&	      skip(int c=1) 
    {_cnt=static_cast<size_t>(static_cast<int>(_cnt)+c); _ok=_cnt<=_cmd.size(); return *this;}
  istream_t& skipbl();
  istream_t& skip1b(char);
  istream_t& skip1(char);
  istream_t& skip1(const std::string&);
  istream_t& skip1b(const std::string&);
  istream_t& skipcom(){return skip1b(",");}
  char		    peek()const			{
	  if(_cnt <_cmd.size()){
		  return _cmd[_cnt];
	  }else{
		  return '\0';
	  }
  }
public: // match
  bool	      ns_more()const	{return peek()!='\0';}
  bool	      match1(char c)const{return (peek()==c);}
  bool	      match1(const std::string& c)const
		{return ns_more() && strchr(c.c_str(),peek());}
public:
	QString const readLine();
	std::string fullString() const { return _cmd; }
	bool atEnd() const{return QTextStream::atEnd();}
	istream_t& operator>>(std::string& x) {x=ctos();return *this;}
	istream_t& operator>>(int& x) {x=ctoi();return *this;}
	istream_t& warn(int, size_t, const std::string&){ incomplete(); return *this;}
	istream_t& warn(int i, const std::string& s)	{return warn(i,0 , s);}

	size_t cursor() const{ return _cnt; }
	operator bool()const	{return _ok;}

  bool	      is_digit()const	{return (match1("0123456789"));}
  char	      ctoc();
private:
	size_t _current_start;
	std::string _cmd;
	size_t _cnt;
	bool _ok;
};

// not here
inline std::ostream& operator<<(std::ostream&o, std::pair<int, int> const& p)
{ itested(); // still used? yes :(
  return o << "(" << p.first << ", " << p.second << ")";
}


#endif
