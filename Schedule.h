#ifndef Schedule_H_INCLUDED
#define Schedule_H_INCLUDED
#include "Event.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Schedule{
  using string = std::string;
  friend std::ostream& operator<<(std::ostream&, const Schedule&);
public:
  // Constructors
  Schedule() = default;
  Schedule(std::istream& in): Schedule() {this->read(in);}
  Schedule(const string filename): Schedule(){
    std::ifstream ifn(filename);
    if(!ifn)
      throw std::runtime_error("Bad filename[" + filename +"]");
    this->read(ifn);
  }
  Schedule(const char* chs): Schedule(string(chs)){}
  //  Copy constructor
  Schedule(const Schedule&);
  //  Assignment operator
  Schedule& operator=(const Schedule&);
  //  Destructor
  ~Schedule() = default;

  void read(std::istream&);
  void clear();
  size_t size() const;
  bool empty() const;
  const Event& operator[](size_t) const;

  class iterator;
  iterator begin() const;
  iterator end() const;
public:
  std::vector<Event> events = {};
  static bool chronological(const Event&, const Event&);  
};

class Schedule::iterator {
public:
  // iterator() = delete;
  iterator(const std::vector<Event> * ve, size_t i): con(ve), index(i){}
  //  Copy constructor
  iterator(const iterator&) = default;
  //  Assignment operator
  iterator& operator=(const iterator&) = default;
  //  Destructor
  ~iterator() = default;

  const Event& operator*() const;
  iterator& operator++();
  iterator operator++(int);
  iterator& operator--();
  iterator operator--(int);
  bool operator==(const iterator&) const;
  bool operator!=(const iterator&) const;

public:
  const std::vector<Event> * con = nullptr;
  std::size_t index = 0;
};
#endif /* Schedule_H_INCLUDED */

