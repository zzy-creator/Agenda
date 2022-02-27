#include <fstream>
#include "Storage.hpp"
#include"Path.hpp"
#include<string>
#include<sstream>
#include<iostream>
using namespace std;

Storage::Storage() {
	m_dirty = false;
	readFromFile();
};
bool Storage::readFromFile(void)
{
	ifstream userin("/tmp/users.csv");
	ifstream meetingin("/tmp/meetings.csv");
	if (!userin.is_open())return false;
	if (!meetingin.is_open())return false;
	string line1;
	while (getline(userin, line1)) {
		vector<string> datas;
		istringstream sin(line1);
		int i = 0;
		string data;
		while (getline(sin, data, ',')) datas.push_back(data);
		User tempuser(datas[0].substr(1, datas[0].length() - 2), datas[1].substr(1, datas[1].length() - 2),
		datas[2].substr(1, datas[2].length() - 2), datas[3].substr(1, datas[3].length() - 2));
		m_userList.push_back(tempuser);
	}
	userin.close();
	string line2;
	while (getline(meetingin, line2)) {
		istringstream sin2(line2);
		string tmp;
		vector<string> datas;
		while (getline(sin2, tmp, ',')) datas.push_back(tmp);
		Date date1(datas[2].substr(1, datas[2].length() - 2)), date2(datas[3].substr(1, datas[3].length() - 2));
		vector<string> par;
		istringstream ss(datas[1].substr(1, datas[1].length() - 2));
		while (getline(ss, tmp, '&')) {
			par.push_back(tmp);
		}
		Meeting tempmeeting(datas[0].substr(1, datas[0].length() - 2), par, date1, date2, datas[4].substr(1, datas[4].length() - 2));
		if (datas[5].length() != 2) tempmeeting.setAdmin(datas[5].substr(1, datas[5].length() - 2));//如果有会议管理员 
		m_meetingList.push_back(tempmeeting);
	}
	meetingin.close();
	return true;
}


/**
*   write file content from memory
*   @return if success, true will be returned
*/
bool Storage::writeToFile(void)
{
	ofstream userout,meetingout;
	userout.open("/tmp/users.csv");
	meetingout.open("/tmp/meetings.csv");
	if (!userout.is_open())return false;
	if (!meetingout.is_open())return false;
	for (auto i = m_userList.begin();i != m_userList.end();i++) {
		userout << '"' << (*i).getName() << '"' << ',' << '"' << (*i).getPassword() << '"' << ',' << '"' << (*i).getEmail() << '"' << ',' << '"' << (*i).getPhone() << '"' << '\n';
	}
	userout.close();
	for (auto j = m_meetingList.begin();j != m_meetingList.end();j++) {
		meetingout << '"' << (*j).getSponsor() << '"' << ',' << '"';
		vector<string> par = (*j).getParticipator();
		int i = 0;
		while (i != par.size()) {
			meetingout << par[i];
			if (i != par.size() - 1)meetingout << "&";
			i++;
		}
		meetingout << '"' << ',' << '"' << Date::dateToString((*j).getStartDate()) << '"' << ','
			<< '"' << Date::dateToString((*j).getEndDate()) << '"' << ',' << '"' << (*j).getTitle() << '"'<<','<<'"' << (*j).getAdmin() << '"' << '\n';
	}
	meetingout.close();
	return true;
}

  std::shared_ptr<Storage> Storage::getInstance(void) {
	  if (m_instance == nullptr) {
		  m_instance = shared_ptr<Storage>(new Storage());
	  }
	  return m_instance;
  };

  Storage::~Storage() {
	  sync();
  };
  // CRUD for User & Meeting
  // using C++11 Function Template and Lambda Expressions

  /**
  * create a user
  * @param a user object
  */
	 void Storage::createUser(const User& t_user) {
		 m_userList.push_back(t_user);
		 m_dirty = true;
	 };

  /**
  * query users
  * @param a lambda function as the filter
  * @return a list of fitted users
  */
	 std::list<User> Storage::queryUser(std::function<bool(const User&)> filter) const {
		 list<User> a;
		 for (auto i = m_userList.begin();i != m_userList.end();++i) {
			 if (filter(*i)) a.push_back(*i);
		 }
		 return a;
	 };

  /**
  * update users
  * @param a lambda function as the filter
  * @param a lambda function as the method to update the user
  * @return the number of updated users
  */
	 int Storage::updateUser(std::function<bool(const User&)> filter,
		 std::function<void(User&)> switcher) {
		 int n = 0;
		 for (auto i = m_userList.begin();i != m_userList.end();++i) {
			 if (filter(*i)) {
				 switcher(*i);
				 ++n;
			 }
		 }
		 if (n > 0) m_dirty = true;
		 return n;
	 };

  /**
  * delete users
  * @param a lambda function as the filter
  * @return the number of deleted users
  */
	 int Storage::deleteUser(std::function<bool(const User&)> filter) {
		 int n = 0;
		 for (auto i = m_userList.begin();i != m_userList.end();) {
			 if (filter(*i)) {
				 i=m_userList.erase(i);
				 ++n;
			 }
			 else i++;
		 }
		 if (n > 0) m_dirty = true;
		 return n;
	 };

  /**
  * create a meeting
  * @param a meeting object
  */
	 void Storage::createMeeting(const Meeting& t_meeting) {
		 m_meetingList.push_back(t_meeting);
		 m_dirty = true;
	 };

  /**
  * query meetings
  * @param a lambda function as the filter
  * @return a list of fitted meetings
  */
	 std::list<Meeting> Storage::queryMeeting(
		 std::function<bool(const Meeting&)> filter) const {
		 list<Meeting> a;
		 for (auto i = m_meetingList.begin();i != m_meetingList.end();++i) {
			 if (filter(*i)) a.push_back(*i);
		 }
		 return a;
	 };

  /**
  * update meetings
  * @param a lambda function as the filter
  * @param a lambda function as the method to update the meeting
  * @return the number of updated meetings
  */
	 int Storage::updateMeeting(std::function<bool(const Meeting&)> filter,
		 std::function<void(Meeting&)> switcher) {
		 int n = 0;
		 for (auto i = m_meetingList.begin();i != m_meetingList.end();++i) {
			 if (filter(*i)) {
				 switcher(*i);
				 ++n;
			 }
		 }
		 if (n > 0) m_dirty = true;
		 return n;
	 };

  /**
  * delete meetings
  * @param a lambda function as the filter
  * @return the number of deleted meetings
  */
	 int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
		 int n = 0;
		 for (auto i = m_meetingList.begin();i != m_meetingList.end();) {
			 if (filter(*i)) {
				i= m_meetingList.erase(i);
				 ++n;
			 }
			 else i++;
		 }
		 if (n > 0) m_dirty = true;
		 return n;
	 };

  /**
  * sync with the file
  */
	 bool Storage::sync(void) {
		 if (m_dirty)writeToFile();
		 m_dirty = false;
		 return true;
	 };
	 std::shared_ptr<Storage> Storage::m_instance = nullptr;
