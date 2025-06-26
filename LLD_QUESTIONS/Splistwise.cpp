#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
using namespace std;
class user {
	string id;
	string name;
	string email;
	string phone;
public:
	user(string id, string name, string email, string phone) :id(id), name(name), email(email), phone(phone){}
	string getid()const {
		return id;
	}
	string getname()const {
		return name;
	}

};
//split type
class split {
protected:
	user* users;
	double amount;
public:
	split(user*users):users(users),amount(0.0){}
	virtual ~split(){}

	void setamount(double amt) {
		amount = amt;
	}
	double getamount()const {
		return amount;
	}
	user* getuser() {
		return users;
	}
};

class Equalsplit :public split {
public:
	Equalsplit(user*users):split(users){}
};
class Exactsplit :public split {
	Exactsplit(user* users, double amounts) :split(users) {
		this->amount = amounts;
	}
};
class percentsplit :public split {
	double percent;
public:
	percentsplit(user* users, double percent) :split(users), percent(percent) {	}
	double getpercent()const {
		return percent;
	}
};

//Expense type
enum class Expensetype{Equal,Exact,Percent};


class Expense {
protected:
	double amount;
	user* paidby;
	vector<split*>splits;
	Expensetype expensetype;

public:
	Expense(double amount,user*paidby,vector<split*>splits,Expensetype type):amount(amount),
		paidby(paidby),splits(splits),expensetype(type){}
	virtual bool validate() = 0;
	vector < split*>getsplits()const {
		return splits;
	}
	user* getpaidby()const {
		return paidby;
	}
	double getamount()const {
		return amount;
	}
};
class EqualExpense :public Expense {
public:
	EqualExpense(double amount, user*paidby,vector<split*>splits):Expense(amount,paidby,splits,Expensetype::Equal){}
	bool validate()override {
		if (splits.empty())return false;
		double share = amount / splits.size();
		for (auto& split : splits)
			split->setamount(share);
		return true;
	}
};
class ExactExpense :public Expense {
public:
	ExactExpense(double amount,user*paidby,vector<split*>splits):Expense(amount,paidby,splits,Expensetype::Exact){}
	bool validate()override {
		double total = 0;
		for (auto& split : splits) {
			total += split->getamount();
		}
		return total == amount;
	}
};

class PercentExpense :public Expense {
public:
	PercentExpense(double amount,user*paidby,vector<split*>splits):Expense(amount,paidby,splits,Expensetype::Percent){}
	bool validate()override {
		double totalpercent = 0;
		for (auto& split : splits) {
			percentsplit* ps = dynamic_cast<percentsplit*>(split);
			if (!ps)return false;
			totalpercent += ps->getpercent();
		}
		if (totalpercent != 100.0)return false;
		for (auto& split : splits) {
			percentsplit* ps = dynamic_cast<percentsplit*>(split);
			double share = (amount * ps->getpercent()) / 100.0;
			split->setamount(share);
		}
		return true;
	}
};

//EXPENSE MANAGER
class ExpenseManger {
	unordered_map<string, user*>usermap;
	unordered_map<string, unordered_map<string, double>>balancesheet;

public:
	void adduser(user* user) {
		usermap[user->getid()] = user;
	}
	void addExpense(Expensetype type, double amount, user* paidby, vector<split*>splits) {
		Expense* expense = nullptr;

		switch (type) {
		case Expensetype::Equal:
			expense = new EqualExpense(amount, paidby, splits);
			break;
		
		case Expensetype::Exact:
			expense = new ExactExpense(amount, paidby, splits);
			break;
	
		case Expensetype::Percent:
		expense = new PercentExpense(amount, paidby, splits);
		break;
		
		}
		if (expense && expense->validate()) {
			for (auto& split : splits) {
				string paidid = paidby->getid();
				string oweid = split->getuser()->getid();
				double oweamount = split->getamount();

				balancesheet[oweid][paidid] += oweamount;
				balancesheet[paidid][oweid] -= oweamount;
			}
		}
		delete expense;
	}
	void showBalance(const string& userid) const {
		auto it = balancesheet.find(userid);
		bool any = false;
		for (const auto& entry : it->second) {
			const string& otherid = entry.first;
			double amount = entry.second;
			if (amount > 0) {
				cout << usermap.at(userid)->getname() << " owes " << usermap.at(otherid)->getname() << " : " << amount << endl;
			}
		}
		if (!any) {
			cout << "no balance for " << usermap.at(userid)->getname() << endl;
		}
	}

	void showbalance()const {
		for (auto userentry = balancesheet.begin(); userentry != balancesheet.end(); ++userentry) {
			const string& userid = userentry->first;
			const auto& balances = userentry->second;
			for (auto balanceentry = balances.begin(); balanceentry != balances.end(); ++balanceentry) {
				const string& otherid = balanceentry->first;
				double amount = balanceentry->second;
				if (amount > 0) {
					cout << usermap.at(userid)->getname() << "owes " << usermap.at(otherid)->getname() << " : " << amount << endl;
				}
			}
		}
	}
};
int main() {
	ExpenseManger manager;
	user* u1 = new user("u1", "ajit", "ajit@gmail.com", "999888");
	user* u2 = new user("u2", "amit", "amit@gmail.com", "999888");
	user* u3 = new user("u3", "rahul", "rahul@gmail.com", "999888");

	manager.adduser(u1);
	manager.adduser(u2);
	manager.adduser(u3);
	

	vector<split*>splits1 = { new Equalsplit(u1),new Equalsplit(u2),new Equalsplit(u3) };
	manager.addExpense(Expensetype::Equal, 300, u1, splits1);

	vector<split*>splits2 = { new Equalsplit(u1),new Equalsplit(u2),new Equalsplit(u3) };
	manager.addExpense(Expensetype::Exact, 300, u1, splits2);

	vector<split*>splits3 = { new Equalsplit(u1),new Equalsplit(u2),new Equalsplit(u3) };
	manager.addExpense(Expensetype::Percent, 300, u1, splits3);

	manager.showbalance();
	return 0;

}



