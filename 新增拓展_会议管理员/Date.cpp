#include"Date.hpp"
#include<iostream>
  /**
  * @brief default constructor
  */
	 Date::Date() :m_day(0), m_hour(0), m_minute(0), m_month(0), m_year(0) {};

  /**
  * @brief constructor with arguments
  */
	 Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) :
	  m_year(t_year), m_day(t_day), m_hour(t_hour), m_minute(t_minute), m_month(t_month) {};

  /**
  * @brief constructor with a string
  */
	 Date::Date(const std::string& dateString) :m_day(0), m_hour(0), m_minute(0), m_month(0), m_year(0) {
	  int l = dateString.length();
	  if (l != 16) return;
	  for (int i = 0;i <= 3;++i) { if (dateString[i] < '0' || dateString[i]>'9') return ; }
	  if (dateString[4] != '-' || dateString[7] != '-' || dateString[10] != '/' || dateString[13] != ':') return ;
	  for (int i = 5;i <= 6;++i) { if (dateString[i] < '0' || dateString[i]>'9') return ; }
	  for (int i = 8;i <= 9;++i) { if (dateString[i] < '0' || dateString[i]>'9') return ; }
	  for (int i = 11;i <= 12;++i) { if (dateString[i] < '0' || dateString[i]>'9') return ; }
	  for (int i = 14;i <= 15;++i) { if (dateString[i] < '0' || dateString[i]>'9') return ; }
	  for (int i = 0;i <= 3;i++) {
		  m_year = m_year * 10 + dateString[i]-'0';
	  }
	  for (int i = 5;i <= 6;i++) {
		  m_month = m_month * 10 + dateString[i] - '0';
	  }
	  for (int i = 8;i <= 9;i++) {
		  m_day = m_day * 10 + dateString[i] - '0';
	  }
	  for (int i = 11;i <= 12;i++) {
		  m_hour = m_hour * 10 + dateString[i] - '0';
	  }
	  for (int i = 14;i <= 15;i++) {
		  m_minute = m_minute * 10 + dateString[i] - '0';
	  }
  };
  /**
  * @brief return the year of a Date
  * @return   a integer indicate the year of a date
  */
  int  Date::getYear(void) const {
	  return m_year;
  };

  /**
  * @brief set the year of a date
  * @param a integer indicate the new year of a date
  */
  void  Date::setYear(const int t_year) {
	  m_year = t_year;
  };

  /**
  * @brief return the month of a Date
  * @return   a integer indicate the month of a date
  */
  int  Date::getMonth(void) const {
	  return m_month;
  };

  /**
  * @brief set the month of a date
  * @param a integer indicate the new month of a date
  */
  void  Date::setMonth(const int t_month) {
	  m_month = t_month;
  };

  /**
  * @brief return the day of a Date
  * @return   a integer indicate the day of a date
  */
  int  Date::getDay(void) const {
	  return m_day;
  };

  /**
  * @brief set the day of a date
  * @param a integer indicate the new day of a date
  */
  void Date::setDay(const int t_day) {
	  m_day = t_day;
  };

  /**
  * @brief return the hour of a Date
  * @return   a integer indicate the hour of a date
  */
  int  Date::getHour(void) const {
	  return m_hour;
  };

  /**
  * @brief set the hour of a date
  * @param a integer indicate the new hour of a date
  */
  void  Date::setHour(const int t_hour) {
	  m_hour = t_hour;
  };

  /**
  * @brief return the minute of a Date
  * @return   a integer indicate the minute of a date
  */
  int  Date::getMinute(void) const {
	  return m_minute;
  };

  /**
  * @brief set the minute of a date
  * @param a integer indicate the new minute of a date
  */
  void  Date::setMinute(const int t_minute) {
	  m_minute = t_minute;
  };

  /**
  *   @brief check whether the date is valid or not
  *   @return the bool indicate valid or not
  */
  bool  Date::isValid(const Date& t_date) {
	  int y = t_date.getYear();
	  int m = t_date.getMonth();
	  int d = t_date.getDay();
	  int h = t_date.getHour();
	  int min = t_date.getMinute();
	  //基本范围
	  if (d < 1 || d>31) return false;
	  if (y < 1000 || y>9999) return false;
	  if (m < 1 || m>12) return false;
	  if (h < 0 || h>23) return false;
	  if (min < 0 || min>59) return false;
	  //2月天数判断
	  if (y % 4 == 0 && y % 100 != 0 || y % 400 == 0) {
		  if (m == 2 && d > 29) return false;
	  }
	  else {
		  if (m == 2 && d > 28) return false;
	  }
	  //剩下最大天数为30的月份天数判断
	  if (m == 4 && d > 30) return false;
	  if (m == 6 && d > 30) return false;
	  if (m == 9 && d > 30) return false;
	  if (m == 11 && d > 30) return false;

	  return true;
  };

  /**
  * @brief convert a string to date, if the format is not correct return
  * 0000-00-00/00:00
  * @return a date
  */
 Date  Date::stringToDate(const std::string& t_dateString) {
	  int l = t_dateString.length();
	  Date a;
	  if (l != 16) {
		  return a;
	  }
	  for (int i = 0;i <= 3;++i) { if (t_dateString[i] < '0' || t_dateString[i]>'9') return a; }
	  if (t_dateString[4] != '-' || t_dateString[7] != '-' || t_dateString[10] != '/' || t_dateString[13] != ':') return a;
	  for (int i = 5;i <= 6;++i) { if (t_dateString[i] < '0' || t_dateString[i]>'9') return a; }
	  for (int i = 8;i <= 9;++i) { if (t_dateString[i] < '0' || t_dateString[i]>'9') return a; }
	  for (int i = 11;i <= 12;++i) { if (t_dateString[i] < '0' || t_dateString[i]>'9') return a; }
	  for (int i = 14;i <= 15;++i) { if (t_dateString[i] < '0' || t_dateString[i]>'9') return a; }
	  Date b(t_dateString);
	  return b;
  };
  /**
  * @brief convert a date to string, if the date is invalid return
  * 0000-00-00/00:00
  */
std::string  Date::dateToString(const Date& t_date) {
	  std::string a("0000-00-00/00:00");
	  if (isValid(t_date)) {
		  int y = t_date.getYear();
		  int m = t_date.getMonth();
		  int d = t_date.getDay();
		  int h = t_date.getHour();
		  int min = t_date.getMinute();
		  int i = 3;
		  while (i>=0) {
			  a[i] = y % 10+'0';
			  y = y / 10;
			  i--;
		  }
		  i = 6;
		  while (i != 4) {
			  a[i] = m % 10 + '0';
			  m = m / 10;
			  i--;
		  }
		  i = 9;
		  while (i != 7) {
			  a[i] = d % 10 + '0';
			  d = d / 10;
			  i--;
		  }
		  i = 12;
		  while (i != 10) {
			  a[i] = h % 10 + '0';
			  h = h / 10;
			  i--;
		  }
		  i = 15;
		  while (i != 13) {
			  a[i] = min % 10 + '0';
			  min = min / 10;
			  i--;
		  }
	  }
	  return a;
  };

  /**
  *  @brief overload the assign operator
  */
  Date& Date::operator=(const Date& t_date) {
	  int y = t_date.getYear();
	  int m = t_date.getMonth();
	  int d = t_date.getDay();
	  int h = t_date.getHour();
	  int min = t_date.getMinute();
	  m_year = y;m_month = m;m_day = d;m_hour = h;m_minute = min;
	  return *this;
  };

  /**
  * @brief check whether the CurrentDate is equal to the t_date
  */
  bool  Date::operator==(const Date& t_date) const {
	  int y = t_date.getYear();
	  int m = t_date.getMonth();
	  int d = t_date.getDay();
	  int h = t_date.getHour();
	  int min = t_date.getMinute();
	  if (m_year == y && m_month == m && m_day == d && m_hour == h && m_minute == min) return true;
	  else return false;
  };

  /**
  * @brief check whether the CurrentDate is  greater than the t_date
  */
  bool  Date::operator>(const Date& t_date) const {
	  int y = t_date.getYear();
	  int m = t_date.getMonth();
	  int d = t_date.getDay();
	  int h = t_date.getHour();
	  int min = t_date.getMinute();
	  if (m_year > y) return true;
	  else if (m_year < y) return false;
	  else {
		  if (m_month > m) return true;
		  else if (m_month < m) return false;
		  else {
			  if (m_day > d) return true;
			  else if (m_day < d) return false;
			  else {
				  if (m_hour > h) return true;
				  else if (m_hour < h) return false;
				  else {
					  if (m_minute > min) return true;
					  else return false;
				  }
			  }
		  }
	  }
  };

  /**
  * @brief check whether the CurrentDate is  less than the t_date
  */
  bool  Date::operator<(const Date& t_date) const {
	  int y = t_date.getYear();
	  int m = t_date.getMonth();
	  int d = t_date.getDay();
	  int h = t_date.getHour();
	  int min = t_date.getMinute();
	  if (m_year < y) return true;
	  else if (m_year > y) return false;
	  else {
		  if (m_month < m) return true;
		  else if (m_month > m) return false;
		  else {
			  if (m_day < d) return true;
			  else if (m_day > d) return false;
			  else {
				  if (m_hour < h) return true;
				  else if (m_hour > h) return false;
				  else {
					  if (m_minute < min) return true;
					  else return false;
				  }
			  }
		  }
	  }
  };

  /**
  * @brief check whether the CurrentDate is  greater or equal than the t_date
  */
  bool  Date::operator>=(const Date& t_date) const {
	  if (t_date < *this || t_date == *this) return true;
	  else return false;
  };

  /**
  * @brief check whether the CurrentDate is  less than or equal to the t_date
  */
  bool  Date::operator<=(const Date& t_date) const {
	  if (t_date > *this || t_date == *this) return true;
	  else return false;
  };

