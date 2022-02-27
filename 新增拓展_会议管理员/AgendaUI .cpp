#include"AgendaUI.hpp"
#include "AgendaService.hpp"
#include <iostream>
#include <string>
#include<iomanip>
using namespace std;
static bool userin = false;//if user log in
static bool out = false; //if user out
void print()
{
	cout << "----------Agenda----------" << endl;
	cout << "Action :" << endl;
	cout << "o    -- log out the Agenda" << endl;
	cout << "dc   -- delete Agenda account" << endl;
	cout << "lu   -- list all Agenda User" << endl;
	cout << "cm   -- create a meeting" << endl;
	cout << "amp  -- add meeting participator" << endl;
	cout << "rmp  -- remove meeting participator" << endl;
	cout << "rqm  -- request to quit meeting" << endl;
	cout << "la   -- list all meetings" << endl;
	cout << "las  -- list all sponsor meetings" << endl;
	cout << "lap  -- list all participator meetings" << endl;
	cout << "qm   -- query meeting by title" << endl;
	cout << "qt   -- query meeting by time interval " << endl;
	cout << "dm   -- delete all meeting by title" << endl;
	cout << "da   -- delete all meeting " << endl;
	cout << "sa   -- set your meeting's admin" << endl;
	cout << "--------------------------" << endl;
}
AgendaUI::AgendaUI() {
	startAgenda();
	}
void AgendaUI::OperationLoop(void) {
	while (out == false) {
		while (userin == false && out == false)startAgenda();
		if (out == true) break;
		string str = getOperation();
		if (str == "q") {
			out = true;
			m_agendaService.quitAgenda();
		}
		bool flag1 = executeOperation(str);
	}
}
void AgendaUI::startAgenda(void) {
	cout << "----------Agenda----------" << endl;
	cout << "Action :" << endl;
	cout << "l  --log in Agenda by user name and password" << endl;
	cout << "r  --register an Agenda account" << endl;
	cout << "q  --quit Agenda" << endl;
	cout << "--------------------------" << endl << endl;
	char ch;
	bool flag;
	while (out == false && userin == false) {
		cout << "Agenda : ~$ ";
		cin >> ch;
		if (ch == 'l') {
			string str1, str2;
			cout << endl << " [log in] [name] [password] " << endl << " [log in] ";
			cin >> str1 >> str2;
			flag = m_agendaService.userLogIn(str1, str2);
			if (flag == true) {
				cout << " [log in] : succeed!" << endl << endl;
				userin = true;
				m_userName = str1;
				m_userPassword = str2;
				print();
				OperationLoop();
			}
			else cout << " [log in] : Password error or User doesn't exixt" << endl << endl;
		}
		else if (ch == 'r') {
			string str1, str2, str3, str4;
			cout << endl << " [register] [username] [password] [email] [phone]" << endl;
			cout << " [register] ";
			cin >> str1 >> str2 >> str3 >> str4;
			flag = m_agendaService.userRegister(str1, str2, str3, str4);
			if (flag == true) {
				cout << " [register] succeed!" << endl << endl;
				print();
				userin = true;
				m_userName = str1;
				m_userPassword = str2;
				OperationLoop();
			}
			else {
				cout << " [register] this username has been registered!" << endl << endl;
			}
		}
		else if (ch == 'q') quitAgenda();
	}
	}

std::string AgendaUI::getOperation() {
	cout << "Agenda@" << m_userName << " : ~#";
	string str;
	cin >> str;
	return str;
	}

bool AgendaUI::executeOperation(std::string t_operation)
{
	string str = t_operation;
	if (str == "o") {
		userLogOut();
		return true;
	}
	if (str == "dc") {
		m_agendaService.deleteUser(m_userName, m_userPassword);
		userin = false;
		cout << endl << " [delete agenda account] succeed!" << endl << endl;
		return true;
	}
	if (str == "lu") {
		listAllUsers();
		return true;
	}
	if (str == "cm") {
		createMeeting();
		return true;
	}
	if (str == "amp") {
		cout << "[add participator] [meeting title] [participator username]" << endl;
		string str1, str2;
		cout << "[add participator] ";
		cin >> str1 >> str2;
		bool flag1 = m_agendaService.addMeetingParticipator(m_userName, str1, str2);
		if (flag1 == true)
			cout << "[add participator] succeed!" << endl << endl;
		else
			cout << "[add participator] error!" << endl << endl;
		return true;
	}
	if (str == "rmp") {
		cout << "[remove participator] [meeting title] [participator username]" << endl;
		string str1, str2;
		cout << "[remove participator] ";
		cin >> str1 >> str2;
		bool flag1 = m_agendaService.removeMeetingParticipator(m_userName, str1, str2);

		if (flag1 == true)
			cout << "[remove participator] succeed!" << endl << endl;
		else
			cout << "[remove participator] error!" << endl << endl;
		auto x = m_agendaService.meetingQuery(m_userName, str1);
		auto y = x.begin();
		auto z = y->getParticipator();
		auto z1 = y->getSponsor();
		if (z.size() == 0) m_agendaService.deleteMeeting(z1, str1);
		return true;
	}
	if (str == "rqm") {
		cout << endl;
		cout << "[quit meeting] [meeting title]" << endl;
		string str1;
		cout << "[quit meeting] ";
		cin >> str1;
		bool flag1 = m_agendaService.quitMeeting(m_userName, str1);
		if (flag1 == true) cout << "[quit meeting] succeed!" << endl << endl;
		else cout << "[quit meeting] error!" << endl << endl;
		return true;
	}
	if (str == "la") {
		listAllMeetings();
		return true;
	}
	if (str == "las")
	{
		listAllSponsorMeetings();
		return true;
	}
	if (str == "lap") {
		listAllParticipateMeetings();
		return true;
	}
	if (str == "qm") {
		queryMeetingByTitle();
		return true;
	}
	if (str == "qt") {
		queryMeetingByTimeInterval();
		return true;
	}
	if (str == "dm") {
		deleteMeetingByTitle();
		return true;
	}
	if (str == "da") {
		deleteAllMeetings();
		return true;
	}
	if (str == "sa") {
		setAdmin();
		return true;
	}
	return false;
}
void AgendaUI::setAdmin(void) {
	cout << "[set admin] [meeting title] [admin username]" << endl;
	string str1, str2;
	cout << "[set admin] ";
	cin >> str1 >> str2;
	bool flag1 = m_agendaService.setAdmin(m_userName, str2, str1);
	if (flag1 == true)
		cout << "[set admin] succeed!" << endl << endl;
	else
		cout << "[set admin] error!" << endl << endl;
}
    /**
     * user Login
     */
void AgendaUI::userLogIn(void);

    /**
     * user regist
     */
    void AgendaUI::userRegister(void);

    /**
     * user logout
     */
	void AgendaUI::userLogOut() {
		userin = false;
	}

	void AgendaUI::quitAgenda(void) {
		out = true;
	}

	void AgendaUI::deleteUser() {
		string str1;
		string str2;
		cin >> str1 >> str2;
		m_agendaService.deleteUser(str1, str2);
		userin = false;
	}
	void AgendaUI::listAllUsers() {
		cout << endl;
		cout << "[all users]" << endl;
		cout << setw(20) << "name" << setw(20) << "email" << setw(20) << "phone" << endl;
		auto k = m_agendaService.listAllUsers();
		auto i = k.begin();
		for (; i != k.end(); i++)
			cout << setw(20) << i->getName() << setw(20) << i->getEmail() << setw(20) << i->getPhone() << endl;
		cout << endl;
	}
	void AgendaUI::createMeeting(void) {
		cout << endl;
		cout << "[create meeting] [the number of participators] ";
		vector<string> v;
		int n;
		cin >> n;
		for (int i = 0; i < n; i++) {
			cout << "[create meeting] [please enter the participator " << i + 1 << "]" << endl;
			string str;
			cout << "[create meeting] ";
			cin >> str;
			v.push_back(str);
		}
		cout << "[create meeting] " << "[title] [start time (yyyy-mm-dd/hh:mm)]  [end time (yyyy-mm-dd/hh:mm)]" << endl;
		string title;
		string t1, t2;
		cout << "[create meeting] ";
		cin >> title;
		cin >> t1 >> t2;
		bool flag1 = m_agendaService.createMeeting(m_userName, title, t1, t2, v);
		if (flag1 == false) cout << "[create meeting] error!" << endl << endl;
		else cout << "[create meeting] succeed!" << endl << endl;
	}
	void AgendaUI::listAllMeetings()
	{
		cout << endl;
		cout << "[list all meetings]" << endl;
		printMeetings(m_agendaService.listAllMeetings(m_userName));
	}
	void AgendaUI::listAllSponsorMeetings() {
		cout << endl;
		cout << "[list all sponsor meetings]" << endl;
		printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
	}
	void AgendaUI::listAllParticipateMeetings() {
		cout << endl;
		cout << "[list all participator meetings]" << endl;
		printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
	}
	void AgendaUI::queryMeetingByTitle() {
		cout << endl;
		cout << "[query meeting] [title]" << endl;
		cout << "[query meeting] :";
		string title;
		cin >> title;
		cout << endl;
		printMeetings(m_agendaService.meetingQuery(m_userName, title));
	}
	void AgendaUI::queryMeetingByTimeInterval() {
		cout << endl;
		cout << "[query meeting] [begin] [end]" << endl;
		cout << "[query meeting] :";
		string str1, str2;
		cin >> str1 >> str2;
		Date tem1(str1);
		Date tem2(str2);
		if (tem1.isValid(tem1) == false || tem2.isValid(tem2) == false) {
			cout << "[query meeting] : error!" << endl;
			return;
		}
		cout << endl;
		printMeetings(m_agendaService.meetingQuery(m_userName, str1, str2));
	}
	void AgendaUI::deleteMeetingByTitle() {
		cout << endl;
		cout << "[delete meeting] [title]" << endl;
		cout << "[delete meeting] :";
		string title;
		cin >> title;
		cout << endl;
		auto x = m_agendaService.deleteMeeting(m_userName, title);
		if (x == true) cout << "[delete meeting] : succees!" << endl;
		else cout << "[delete meeting] : error! " << endl;
	}
	void AgendaUI::deleteAllMeetings() {
		cout << endl;
		cout << "[delete all] [title]";
		auto x = m_agendaService.deleteAllMeetings(m_userName);
		if (x == true) cout << "[delete all] : succees!" << endl;
		else cout << "[delete all] : error! " << endl;
	}
    /**
     * show the meetings in the screen
     */
	void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
		auto x = t_meetings;
		cout << setw(20) << "title" << setw(20) << "sponsor" << setw(20) << "start date" << setw(20) << "end date" << setw(20) << "participator" 
			<< setw(20) << "admin" << endl << endl;
		for (auto i = x.begin(); i != x.end(); i++)
		{
			cout << setw(20) << i->getTitle() << setw(20) << i->getSponsor();
			string str1 = i->getStartDate().dateToString(i->getStartDate());
			string str2 = i->getEndDate().dateToString(i->getEndDate());
			cout << setw(20) << str1 << setw(20) << str2 << setw(20);
			auto p = i->getParticipator();
			auto s = p.begin();
			for (s; s != p.end(); s++) {
				if (s != p.begin()) cout << ',';
				cout << *s;
			}
			cout << setw(20) << i->getAdmin();
			cout << endl;
		}
	}

