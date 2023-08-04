#ifndef Event_H_INCLUDED
#define Event_H_INCLUDED
#include <iostream>
#include <string>
class Event{
  using string = std::string;
  friend std::ostream& operator<<(std::ostream&, const Event&);
public:
  // Constructors
  Event() = delete;
  Event(const string);
  Event(const char* chs): Event(string(chs)){}
  //  Copy constructor
  Event(const Event&) = default;
  //  Assignment operator
  Event& operator=(const Event&) = default;
  //  Destructor
  ~Event() = default;
  
  void set(int year, int month, int day);
  int year() const;
  int month() const;
  int day() const;
  string fmt(const string format = "%04Y-%m-%d") const;

  Event& operator++();
  Event operator++(int);
  Event& operator--();
  Event operator--(int);
  bool operator==(const Event&) const;
  bool operator!=(const Event&) const;
  bool operator<(const Event&) const;
  bool operator>(const Event&) const;
  bool operator<=(const Event&) const;
  bool operator>=(const Event&) const;

private:
  int field_year;
  int field_month;
  int field_mday;
  void getTimeYnYD (tm* rawtm, const int year, const int day);
  void getTimeYnMnD (tm* rawtm, const int year, const int month, const int mday);
  bool is_digits(const string &str) const;
  void toLowerCase(string &str);
  string ltrim(const string &s);
  string rtrim(const string &s);
  string trim(const string &s);
  bool isLastDayOfMonth() const;
  bool isLeapYear() const;
};
#endif /* Event_H_INCLUDED */
