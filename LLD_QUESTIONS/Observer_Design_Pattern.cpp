#include<iostream>
using namespace std;
#include<list>
#include<vector>


//Notification interface
class INotificationStratergy {
public:
	virtual void send(string message, int userid) = 0;

	virtual ~INotificationStratergy() = default;
};
class EmailNotification :public INotificationStratergy {
public:
	void send(string message, int userid) override {
		cout << "[Email] to user" << userid << ": " << message << endl;
	}
};
class SMSNotification :public INotificationStratergy {
	void send(string message, int userid) override {
		cout << "[SMS] to user" << userid << ": " << message << endl;
	}
}; 
class WatsappNotification : public INotificationStratergy {
	void send(string message, int userid) override {
		cout << "[WhatsApp] to user" << userid << ": " << message << endl;
	}
};


//******* Factory method to create notification strategies**********
INotificationStratergy* createNotification(const string& type) {
	if (type == "email") {
		return new EmailNotification();
	}
	else if (type == "sms") {
		return new SMSNotification();
	}
	else if (type == "whatsapp") {
		return new WatsappNotification();
	}
	else {
		cout << "Unknown notification type, defaulting to Email." << endl;
		return new EmailNotification(); // default fallback
	}
}

//interface for subscribers
//*********used stratergy design to send type of notification(meaing using different algo at runtime****
class ISubscriber {
public:
	virtual void notify(string msg) = 0;
	virtual ~ISubscriber() = default;
};

class user :public ISubscriber {
private:
	int userid;
	vector<INotificationStratergy*> strategies;  // multiple notification strategies
public:
	user(int userid) {
		this->userid = userid;
		
	}
	// Add a new notification strategy
	void addStrategy(INotificationStratergy* strategy) {
		strategies.push_back(strategy);
	}
	void notify(string msg) override {
		for (auto& strategy : strategies) {
			strategy->send(msg, userid);
		}
	}

	~user() {
		// Clean up allocated strategies
		for (auto& strategy : strategies) {
			delete strategy;
		}
	}
};
///*******observer design pattern used here**********
class Group {
private:
	list<ISubscriber*>users;
public:
	void subscribe(ISubscriber* user) {
		users.push_back(user);
	}
	void unsubscribe(ISubscriber* user) {
		users.remove(user);
	}
	void notify(string msg) {
		for (auto user : users) {
			user->notify(msg);
		}
	}
	~Group() {
		for (auto user : users) {
			delete user; // Cleanup
		}
	}
};


//int main() {
//	Group* group = new Group;
//
//	string type1 = "email";
//	string type2 = "sms";
//	string type3 = "whatsapp";
//
//	user* user1 = new user(1);
//	//*********stratergy design pattern**********
//	user1->addStrategy(createNotification("email"));
//	user1->addStrategy(createNotification("sms"));  // user1 gets email + sms
//
//	user* user2 = new user(2);
//	user2->addStrategy(createNotification("sms"));  // user2 gets only sms
//
//	user* user3 = new user(3);
//	user3->addStrategy(createNotification("whatsapp"));
//	user3->addStrategy(createNotification("email"));  // user3 gets whatsapp + email
//	//*****observer design pattern*****
//	group->subscribe(user1);
//	group->subscribe(user2);
//	group->subscribe(user3);
//
//	group->notify("Hello everyone!");
//	group->unsubscribe(user1);
//	group->notify("User1 left the group.");
//	return 0; 
//}
/*
* UML DIAGRAM
+ ---------------------------- - +
| INotificationStratergy  |<<interface >>
+---------------------------- - +
| +send(message, userid) |
+------------ - +-------------- - +
^
+-------------- - +---------------------------- - +
|               |               |             |
+---------------- + +---------------- + +---------------------- +
| EmailNotification | | SMSNotification | | WatsappNotification |
+---------------- + +---------------- + +---------------------- +
| +send(...)     | | +send(...)     | | +send(...) |
+---------------- + +---------------- + +---------------------- +

+-------------------- - +
| ISubscriber      |<<interface >>
+-------------------- - +
| +notify(msg) |
+-------- - +---------- - +
|
+------ - +-------- +
| user |
+---------------- +
| -userid        |
| -stratergy |
+---------------- +
| +notify(msg) |
+---------------- +

+-------------- - +
| Group |
+-------------- - +
| -users |
+-------------- - +
| +subscribe()  |
| +unsubscribe()|
| +notify() |
+-------------- - +

+---------------------------- - +
| createNotification     |<<Factory Method >>
+---------------------------- - +
| +returns Email / SMS / Watsapp |
+---------------------------- - +
*/