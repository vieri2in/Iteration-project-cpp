#include "Schedule.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // for copy() and assign()
#include <iterator> // for back_inserter
#include <cassert>
using namespace std;
ostream& operator<<(std::ostream& os, const Schedule& sched){
  for (auto item: sched.events){
    os << item << '\n';
  }
  return os;
}

Schedule::Schedule(const Schedule& rhs){
  events.clear();
  copy(rhs.events.begin(), rhs.events.end(), back_inserter(events));
}

Schedule& Schedule::operator=(const Schedule& rhs) {
  events.clear();
  copy(rhs.events.begin(), rhs.events.end(), back_inserter(events));
  return *this;
}

bool Schedule::chronological(const Event& x, const Event& y){
  if (x.year() != y.year()){
    return x.year() < y.year();
  }
  else if (x.month() != y.month()){
    return x.month() < y.month();
  }
  else if (x.day() != y.day()){
    return x.day() < y.day();
  }
  return false;
}

void Schedule::read(istream& in){
  for (string line = ""; in >> line; ){
    try{
        Event temp(line);
        events.push_back(temp);
        sort(events.begin(), events.end(), &chronological);
    }
    catch(const exception &e){
        in.clear(ios::failbit);
        throw;
    }
  }
}

void Schedule::clear(){
  events.clear();
}

size_t Schedule::size() const{
  return events.size();
}

bool Schedule::empty() const{
  return size() == 0;
}

const Event& Schedule::operator[](size_t n) const{
  size_t zero = 0;
  if( n < zero || n >= size())
    throw  range_error("Invalid index [" + to_string(n) + "], the number of Events in this Schedule is " + to_string(size()));
  return events[n];
}

const Event& Schedule::iterator::operator*() const{
  assert(index < con->size());
  return (*con)[index];
}

Schedule::iterator& Schedule::iterator::operator++(){
  index += 1;
  return *this;
}

Schedule::iterator Schedule::iterator::operator++(int){
  const auto save = *this;
  ++*this;
  return save;
}

Schedule::iterator& Schedule::iterator::operator--(){
  index -= 1;
  return *this;
}

Schedule::iterator Schedule::iterator::operator--(int){
  const auto save = *this;
  --*this;
  return save;
}

bool Schedule::iterator::operator!=(const Schedule::iterator& rhs) const{
  return con != rhs.con || index != rhs.index;
}
bool Schedule::iterator::operator==(const Schedule::iterator& rhs) const{
  return !(*this != rhs);
}

Schedule::iterator Schedule::begin() const{
  return iterator(&events, 0);
}

Schedule::iterator Schedule::end() const{
  size_t count = this->size();
  return iterator(&events, count);
}

