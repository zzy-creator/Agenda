#include <list>
#include <string>
#include "Storage.hpp"
#include"AgendaService.hpp"

  /**
   * constructor
   */
AgendaService::AgendaService() {
	m_storage = Storage::getInstance();
};

/**]
 * destructor
 */
	AgendaService::~AgendaService(){
};

  /**
   * check if the username match password
   * @param userName the username want to login
   * @param password the password user enter
   * @return if success, true will be returned
   */
	bool AgendaService::userLogIn(const std::string& userName, const std::string& password) {
		auto f = [userName, password](const User& a)->bool {
			if (userName == a.getName()&& password == a.getPassword())return true;
			else return false;
		};
		if (!m_storage->queryUser(f).empty())return true;
		else return false;
	};

  /**
   * regist a user
   * @param userName new user's username
   * @param password new user's password
   * @param email new user's email
   * @param phone new user's phone
   * @return if success, true will be returned
   */
	bool AgendaService::userRegister(const std::string& userName, const std::string& password,
		const std::string& email, const std::string& phone) {
		User a(userName, password, email, phone);
		auto f = [a](const User& b)->bool {
			if (a.getName() == b.getName()) return true;
			else return false;
		};
		if (!m_storage->queryUser(f).empty()) return false;
		else {
			m_storage->createUser(a);
			return true;
		}
	};

  /**
   * delete a user
   * @param userName user's username
   * @param password user's password
   * @return if success, true will be returned
   */
  bool AgendaService::deleteUser(const std::string& userName, const std::string& password) {
	  auto f = [userName, password](const User& a)->bool {
		  if (userName == a.getName() && password == a.getPassword()) return true;
		  else return false;
	  };
	  if (m_storage->queryUser(f).empty()) return false;
	  deleteAllMeetings(userName);
	  std::list <Meeting> attend = listAllParticipateMeetings(userName);
	  if(!attend.empty()) for (auto i = attend.begin();i != attend.end();++i) quitMeeting(userName, i->getTitle());
	  int n;
	  n = m_storage->deleteUser(f);
	  if (n <= 0)return false;
	  else return true;
  };

  /**
   * list all users from storage
   * @return a user list result
   */
  std::list<User> AgendaService::listAllUsers(void) const {
	  auto f = [](const User& a)->bool {return true;};
	  return m_storage->queryUser(f);
  };

  /**
   * create a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @param startData the meeting's start date
   * @param endData the meeting's end date
   * @return if success, true will be returned
   */
  bool AgendaService::createMeeting(const std::string& userName, const std::string& title,
	  const std::string& startDate, const std::string& endDate,
	  const std::vector<std::string>& participator) {
	  //检测日期是否格式正确
	  Date sd(startDate), ed(endDate);
	  if (sd >= ed||!sd.isValid(sd)||!ed.isValid(ed)) return false;
	  //检测参与者是否为空
	  if (participator.empty()) return false;
	  //检测发起者是否已注册
	  int j = 0;
	  std::list <User> u = listAllUsers();
	  for (auto i = u.begin();i != u.end();++i) {
		  if (userName == i->getName()) { j++;break; }
	  }
	  if (j == 0) return false;

	  for (auto t = participator.begin();t != participator.end();++t) {
		  //检测参与者是否已注册
		  int ji = 0;
		  for (auto i =u.begin();i != u.end();++i) {
			  if (*t == i->getName()) { ji++;break; }
		  }
		  if (ji == 0) return false;
		  //检测参与者和发起者是否存在重复用户
		  for (auto t = participator.begin();t != participator.end();++t) {
			  if (*t == userName) return false;
			  if (t != participator.end() - 1) for (auto ti = t + 1;ti != participator.end();++ti)
				  if (*ti == *t) return false;
		  }
		  //检测参与者是否都有空参加该会议
		  std::list<Meeting>mi = listAllMeetings(*t);
		  for (auto it = mi.begin();it != mi.end();it++) {
			  if (!(it->getEndDate() <= sd || it->getStartDate() >= ed)) return false;
		  }
	  }
	  //检测会议名是否唯一
	  auto filter = [title](const Meeting& a)->bool {
		  if (a.getTitle() == title) return true;
		  else return false;
	  };
	  if (!m_storage->queryMeeting(filter).empty()) return false;
	  //检测发起者是否有空参加该会议
	  std::list<Meeting>m = listAllMeetings(userName);
	  for (auto it = m.begin();it != m.end();it++) {
		  if (!(it->getEndDate() <= sd || it->getStartDate() >= ed)) return false;
	  }

	  Meeting meet(userName, participator, sd, ed, title);
	  m_storage->createMeeting(meet);
	  return true;
  };

  /**
   * add a participator to a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
  bool AgendaService::addMeetingParticipator(const std::string& userName,
	  const std::string& title,
	  const std::string& participator) {
	  //确认用户存在
	  int j = 0;
	  std::list<User> u = listAllUsers();
	  for (auto i = u.begin();i !=u.end();++i) {
		  if (participator == i->getName()) { j++;break; }
	  }
	  if (j == 0) return false;
	   //获得该会议
	  auto f = [userName, title](const Meeting& a)->bool {
		  if (a.getSponsor() == userName && a.getTitle() == title) return true;
		  else return false;
	  };
	  if (m_storage->queryMeeting(f).empty()) return false;
	  Meeting meet = *(m_storage->queryMeeting(f).begin());
	  //判断时间是否冲突
	  std::list<Meeting>m = listAllMeetings(participator);
	  for (auto it = m.begin();it != m.end();it++) {
		  if (!(it->getEndDate() <= meet.getStartDate() || it->getStartDate() >= meet.getEndDate())) return false;
	  }

	  auto s = [participator](Meeting& a) {
		  a.addParticipator(participator);
	  };
	  int n;
	  n = m_storage->updateMeeting(f, s);
	  if (n <= 0) return false;
	  else return true;
  };

  /**
   * remove a participator from a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
  bool AgendaService::removeMeetingParticipator(const std::string& userName,
	  const std::string& title,
	  const std::string& participator) {
	  //发起者不能remove
	  if (userName == participator) return false;

	  auto f = [userName, title,participator](const Meeting& a)->bool {
		  if (userName==a.getSponsor() && a.getTitle() == title && a.isParticipator(participator) )return true;
		  else return false;
	  };
	  auto s = [title,participator,this](Meeting& a) {
		  a.removeParticipator(participator);
	  };
	  int n;
	  n = m_storage->updateMeeting(f, s);
	  if (n <= 0) return false;
	  else {
		  //判断remove后参加者是否为空，为空删除会议
		  auto f1 = [title](const Meeting& a)->bool {
			  if (a.getTitle() == title) return true;else return false;
		  };
		  Meeting meet = *(m_storage->queryMeeting(f1).begin());
		  if (meet.getParticipator().empty()) deleteMeeting(userName, title);
		  return true;
	  }
  };

  /**
   * quit from a meeting
   * @param userName the current userName. need to be the participator (a sponsor can not quit his/her meeting)
   * @param title the meeting's title
   * @return if success, true will be returned
   */
  bool AgendaService::quitMeeting(const std::string& userName, const std::string& title) {
	  auto f = [userName, title](const Meeting& a)->bool {
		  if (a.isParticipator(userName) && a.getTitle() == title) return true;
		  else return false;
	  };
	  auto s = [userName](Meeting& a) {
		  a.removeParticipator(userName);
	  };
	  int n;
	  n = m_storage->updateMeeting(f, s);
	  if (n <= 0) return false;
	  else {
		  //判断remove后参加者是否为空，为空删除会议
		  auto f1 = [title](const Meeting& a)->bool {
			  if (a.getTitle() == title) return true;else return false;
		  };
		  Meeting meet = *(m_storage->queryMeeting(f1).begin());
		  if (meet.getParticipator().empty()) deleteMeeting(meet.getSponsor(), title);
		  return true;
	  }
  };

  /**
   * search a meeting by username and title
   * @param userName as a sponsor OR a participator
   * @param title the meeting's title
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::meetingQuery(const std::string& userName,
	  const std::string& title) const {
	  auto f = [userName, title](const Meeting& a)->bool {
		  if (a.getTitle()==title) {
			  if (a.getSponsor() == userName || a.isParticipator(userName)) return true;
			  else return false;
		  }
		  else return false;
	  };
	  std::list <Meeting> m = m_storage->queryMeeting(f);
	  return m;
  };
  /**
   * search a meeting by username, time interval
   * @param userName as a sponsor OR a participator
   * @param startDate time interval's start date
   * @param endDate time interval's end date
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::meetingQuery(const std::string& userName,
	  const std::string& startDate,
	  const std::string& endDate) const {
	  Date sd(startDate), ed(endDate);
	  std::list<Meeting> empty;
	  if (!Date::isValid(sd) || !Date::isValid(ed) || sd > ed) return empty;
	  auto f=[userName, ed, sd](const Meeting& a)->bool {
		  if (a.getSponsor() == userName || a.isParticipator(userName)) {
			  if (!(a.getEndDate() < sd || a.getStartDate() > ed)) return true;
			  else return false;
		  }
		  else return false;
	  };
	  std::list <Meeting> m = m_storage->queryMeeting(f);
	  return m;
  };

  /**
   * list all meetings the user take part in
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllMeetings(const std::string& userName) const {
	  auto f = [userName](const Meeting& a)->bool {
		  if (a.getSponsor() == userName || a.isParticipator(userName)) return true;
		  else return false;
	  };
	  std::list <Meeting> m = m_storage->queryMeeting(f);
	  return m;
  };

  /**
   * list all meetings the user sponsor
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string& userName) const {
	  auto f = [userName](const Meeting &a) {
		  if (userName == a.getSponsor()) return true;
		  else return false;
	  };
	  std::list <Meeting> m = m_storage->queryMeeting(f);
	  return m;
  };

  /**
   * list all meetings the user take part in and sponsor by other
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllParticipateMeetings(
	  const std::string& userName) const {
	  auto f = [userName](const Meeting& a)->bool {
		  if (a.isParticipator(userName)) return true;
		  else return false;
	  };
	  std::list <Meeting> m = m_storage->queryMeeting(f);
	  return m;
  };

  /**
   * delete a meeting by title and its sponsor
   * @param userName sponsor's username
   * @param title meeting's title
   * @return if success, true will be returned
   */
  bool AgendaService::deleteMeeting(const std::string& userName, const std::string& title) {
	  auto f = [userName, title](const Meeting& a)->bool {
		  if (title == a.getTitle() && userName == a.getSponsor()) return true;
		  else return false;
	  };
	  int n = m_storage->deleteMeeting(f);
	  if (n <= 0)return false;
	  else return true;
  };

  /**
   * delete all meetings by sponsor
   * @param userName sponsor's username
   * @return if success, true will be returned
   */
  bool AgendaService::deleteAllMeetings(const std::string& userName) {
	  auto f = [userName](const Meeting& a)->bool {
		  if (userName == a.getSponsor()) return true;
		  else return false;
	  };
	  int n = m_storage->deleteMeeting(f);
	  if (n <= 0)return false;
	  else return true;
  };

  /**
   * start Agenda service and connect to storage
   */
  void AgendaService::startAgenda(void) {
	  m_storage->sync();
  };

  /**
   * quit Agenda service
   */
  void AgendaService::quitAgenda(void) {
	  m_storage->sync();
  };


