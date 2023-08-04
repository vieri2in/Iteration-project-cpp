#include "Event.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <regex>
#include <ctime>
using namespace std;
ostream& operator<<(std::ostream& os, const Event& ev){
  const auto save_fill = os.fill();
  os  << setfill('0') << setw(4) << ev.field_year << '-'
      << setfill('0') << setw(2) << ev.field_month << '-'
      << setfill('0') << setw(2) << ev.field_mday;
  os.fill(save_fill);
  return os;
}

void Event::getTimeYnYD (tm* rawtm, const int year, const int day){
  rawtm->tm_year = year - 1900;
  rawtm->tm_mon = 0;
  rawtm->tm_mday = day;
  mktime(rawtm);
  //ensure monthday should one to however many days are in that month that year.
  if(rawtm->tm_year != year - 1900){
    string reminder =  "The day of input data (" + to_string(day) + ") is beyond however many days are in that year.";
    throw runtime_error(reminder);
  }
}

void Event::getTimeYnMnD (tm* rawtm, const int year, const int month, const int mday){
  rawtm->tm_year = year - 1900;
  rawtm->tm_mon = month - 1;
  rawtm->tm_mday = mday;
  mktime(rawtm);
  //ensure monthday should one to however many days are in that month that year.
  if(rawtm->tm_mon != month - 1){
    string reminder =  "The day of input data (" + to_string(mday) + ") is beyond however many days are in that month that year.";
    throw runtime_error(reminder);
  }
}

bool Event::is_digits(const string &str) const{
    return str.find_first_not_of("0123456789") == string::npos;
}

void Event::toLowerCase(string &str){  //modify string
    for (size_t i = 0; i < str.length(); i++){
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] + 32;
    }
}

string Event::ltrim(const string &s) {
    return regex_replace(s, regex("^\\s+"), string(""));
}
 
string Event::rtrim(const string &s) {
    return regex_replace(s, regex("\\s+$"), string(""));
}
 
string Event::trim(const string &s) {
    return ltrim(rtrim(s));
}

Event::Event(const string str){
  string line(str);
  line = trim(line);
  string dot = ".";
  string dash = "-";
  time_t now = time(NULL);
  tm* rawNow = localtime(&now);
  
  if(line == "") return;
  //handle today yesterday tomorrow
  if(line.front() == 't' || line.front() == 'T'
     || line.front() == 'y' || line.front() == 'Y')
    Event::toLowerCase(line);
  if (line == "yesterday"){
    rawNow->tm_mday -= 1;
    mktime(rawNow);
  }
  if (line == "tomorrow"){
    rawNow->tm_mday += 1;
    mktime(rawNow);
  }
  if (line == "today" || line == "yesterday" || line == "tomorrow"){
    field_year = rawNow->tm_year + 1900;
    field_month = rawNow->tm_mon + 1;
    field_mday = rawNow->tm_mday;
    /*delete rawNow;*/ return;
  }
  //input format checking
  size_t dofound = line.find(dot);
  size_t dafound = line.find(dash);
  if ((dofound == string::npos && dafound == string::npos)
      || (dofound != string::npos && dafound != string::npos)){
    string reminder = "The input data (" + line + ") is not in correct format [year.day] [YYYY-MM-DD].";
    throw runtime_error(reminder);
  }
  
  if (dofound != string::npos){   //handle [year.day] format
    if( is_digits(line.substr(0, dofound)) == 0
       ||is_digits(line.substr(dofound+1)) == 0){   //contains non-digits
      string reminder = "The input data (" + line + ") contains non-digits.";
      throw runtime_error(reminder);
    }
    int year = stoi(line.substr(0, dofound));
    int yday = stoi(line.substr(dofound+1));
    if (year < 1 || year > 9999     //checking out-of-range year or day
        || yday < 1 || yday > 366){
      string reminder = "The input data (" + line + ") has out-of-range year or day.";
      throw runtime_error(reminder);
    }
    getTimeYnYD(rawNow, year, yday);
    field_year = rawNow->tm_year + 1900;
    field_month = rawNow->tm_mon + 1;
    field_mday = rawNow->tm_mday;
    /*delete rawNow;*/ return;
  }
  
  if (dafound != string::npos){                   //handle [YYYY-MM-DD] format
    size_t daLastfound = line.rfind(dash);      //ensuring the input contain two hyphens
    if (dafound == daLastfound
        || is_digits(line.substr(0, dafound)) == 0
        || is_digits(line.substr(dafound + 1, daLastfound - dafound - 1)) == 0
        || is_digits(line.substr(daLastfound + 1)) == 0){
      string reminder = "The input data (" + line + ") is not in correct format [YYYY-MM-DD] or contains non-digits.";
      throw runtime_error(reminder);
    }
    int year = stoi(line.substr(0, dafound));
    int month = stoi(line.substr(dafound + 1, daLastfound - dafound - 1));
    int mday = stoi(line.substr(daLastfound + 1));
    if (year < 1 || year > 9999
        || month < 1 || month > 12
        || mday < 1 || mday > 31){//checking out-of-range year, month, or monthday
      string reminder = "The input data (" + line + ") has out-of-range year, month, monthday.";
      throw runtime_error(reminder);
    }
    getTimeYnMnD(rawNow, year, month, mday);
    field_year = rawNow->tm_year + 1900;
    field_month = rawNow->tm_mon + 1;
    field_mday = rawNow->tm_mday;
    /*delete rawNow;*/ return;
  }
}

void Event::set(int year, int month, int day){
  string newVal = to_string(year) + "-" + to_string(month) + "-" + to_string(day);
  try{
    Event newObj(newVal);
    field_year = newObj.field_year;
    field_month = newObj.field_month;
    field_mday = newObj.field_mday;
  }
  catch(string msg){
    throw;
  }
}

int Event::year() const{
  return field_year;
}
int Event::month() const{
  return field_month;
}
int Event::day() const{
  return field_mday;
}

string Event::fmt(const string format) const{
  char buffer[256];
  time_t now = time(NULL);
  tm* rawNow = localtime(&now);
  rawNow->tm_year = field_year - 1900;
  rawNow->tm_mon = field_month - 1;
  rawNow->tm_mday = field_mday;
  mktime(rawNow);
  strftime(buffer, sizeof(buffer), format.c_str(), rawNow);
  return string(buffer);
}
bool Event::isLeapYear() const{
  return field_year % 400 == 0 
          || (field_year % 100 !=0 && field_year % 4 == 0);
}
bool Event::isLastDayOfMonth() const{
  //an array of varing monthdays with indices reprensenting month 0-11
  int days[] = {31, 28, 31,
                30, 31, 30,
                31, 31, 30,
                31, 30, 31};
  if (isLeapYear()) days[1] = 29;
  size_t m = field_month - 1;
  return days[m] == field_mday;
}
Event& Event::operator++(){
  if (isLastDayOfMonth()){
    if (field_month != 12)
      set(field_year, field_month + 1, 1);
    else
      set(field_year + 1, 1, 1);
  }
  else
    set(field_year, field_month, field_mday + 1);
  return *this;
}
Event Event::operator++(int){
  const auto save = *this;
  ++*this;
  return save;
}
Event& Event::operator--(){
  if(field_mday == 1){
    if(field_month == 1){
      set(field_year - 1, 12, 31);
    }else{
      //an array of varing monthdays with indices reprensenting month 0-11
      int days[] = {31, 28, 31,
                    30, 31, 30,
                    31, 31, 30,
                    31, 30, 31};
      if (isLeapYear()) days[1] = 29;
      size_t newM = field_month - 2;
      set(field_year, field_month - 1, days[newM]);
    }
  }
  else
    set(field_year, field_month, field_mday - 1);
  return *this;
}
Event Event::operator--(int){
  const auto save = *this;
  --*this;
  return save;
}
bool Event::operator==(const Event& rhs) const{
  return field_year == rhs.field_year
          && field_month == rhs.field_month
          && field_mday == rhs.field_mday;
}
bool Event::operator!=(const Event& rhs) const{
  return !(*this == rhs);
}
bool Event::operator<(const Event& rhs) const{
  bool res = false;
  if (field_year < rhs.field_year)
    return true;
  else if (field_year == rhs.field_year){
    if (field_month < rhs.field_month)
      return true;
    else if (field_month == rhs.field_month){
      if (field_mday < rhs.field_mday)
        return true;
    }
  }
  return res;
}
bool Event::operator>(const Event& rhs) const{
  return !(*this < rhs || *this == rhs);
}
bool Event::operator<=(const Event& rhs) const{
  return !(*this > rhs);
}
bool Event::operator>=(const Event& rhs) const{
  return !(*this < rhs);
}