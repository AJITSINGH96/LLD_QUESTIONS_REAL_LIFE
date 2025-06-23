//question:Design Online Book Reader
#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
using namespace std;
//forward decleration
class Book;
class page;
class library;
class bookmark;

class page {
	int pageno;
	string content;
public:
	page(int num, string text) :pageno(num), content(text){}
	int pagenum() {
		return pageno;
	}
	string getcontent() {
		return content;
	}
};
class book {
	int bookid;
	string title;
	string author;
	vector<page*>pages;
public:
	book(int id, string title, string author) :bookid(id), title(title), author(author){}
	void addpage(page* page) {
		pages.push_back(page);
	}
	page* getpage(int num) {
		if (num > 0 && num < pages.size()) {
			return pages[num];
		}
		return nullptr;
	}
	string gettitle()const {
		return title;
	}
	string getauthor()const {
		return author;
	}
	int getbookid() {
		return bookid;
	}
	int getTotlaPages() {
		return pages.size();
	}

};

class bookmark {
	int bookid;
	int pageno;
public:
	bookmark(int id,int num): bookid(id),pageno(num){}
	int getbookid() {
		return bookid;
	}
	int getpagenum(){
		return pageno;
	}
};
class readingsession {
	book* currentbook;
	int currentpage;
public:
	readingsession(book*curbook):currentbook(curbook),currentpage(0){}
	void nextpage() {
		if (currentpage + 1 < currentbook->getTotlaPages()) {
			currentpage++;
		}
	}
	void previouspage() {
		if (currentpage  > 0) {
			currentpage--;
		}
	}
	void getpage(int page) {
		if (page >=0 && page < currentbook->getTotlaPages()) {
			currentpage = page;
		}
	}
	page* getcurrentpage() {
		return currentbook->getpage(currentpage);
	}
};

class user {
	int userid;
	string username;
	string email;
	string password;
	vector<bookmark>bookmarks;
public:
	user(int id, string name, string email, string password) :userid(id), username(name), email(email), password(password){}
	void addbookmark(bookmark  bookmark) {
		bookmarks.push_back(bookmark);
	}
	vector<bookmark>getbookmarks() {
		return bookmarks;
	}
	int getuserid() {
		return userid;
	}
};
class admin :public user {
public:
	admin(int id,string name, string email,string password):user(id,name,email,password){}
	void uploadbook(library& lib, book* book);
};

class library {
	static library* instance;
	library() {}
	unordered_map<int, book*>entry;
public:
	static library* getinstance() {
		if (!instance)
			instance = new library();
		return instance;
	}
	void addbook(book* book) {
		entry[book->getbookid()] = book;
	}
	book* searchByTitle(const string& title) {
		for (auto book : entry) {
			if (book.second->gettitle() == title) {
				return book.second;
			}
		}
		return nullptr;
	}
	book* searchByAuthor(const string& authorname) {
		for (auto book : entry) {
			if (book.second->getauthor() == authorname) {
				return book.second;
			}
		}
		return nullptr;
	}
	book* searchById(int id) {
		for (auto book : entry) {
			if (book.second->getbookid() == id) {
				return book.second;
			}
		}
		return nullptr;
	}
};
library* library::instance = nullptr;

class Istrategy_search {
public:
	virtual book* search(library &lib,const string &title) { return nullptr; };
	virtual book* search(library& lib, int id) { return nullptr; };

	virtual ~Istrategy_search() {}
};
class searchbytitle :public Istrategy_search {
public:
	book* search(library& lib, const string& title)override  {
		return lib.searchByTitle(title);
	}
};
class searchbyid :public Istrategy_search {
public:
	book* search(library& lib, int id) {
		return lib.searchById(id);
	}
};
class searchByAuthor :public Istrategy_search {
public:
	book* search(library& lib, const string& authorname) {
		return lib.searchByAuthor(authorname);
	}
};

void admin::uploadbook(library& lib, book* book) {
	lib.addbook(book);
}

class Isubscriber {
public:
	virtual void notify(book *book) = 0;
	virtual ~Isubscriber(){}
};
class subscriber :public Isubscriber {
	int userid;
	
public:
	subscriber(int id) :userid(id){}
	void notify( book*book) {
		cout << userid  << "get the notification about" << book->gettitle();
	}
	int getid() {
		return userid;
	}
};
class group {

	list<Isubscriber*>subscribers;
public:
	group(){}
	void addsubscriber(Isubscriber* user) {
		subscribers.push_back(user);
	}
	void unsubscribe(Isubscriber* user) {
		subscribers.remove(user);
	}
	void notifyall(book* book) {
		for (auto subscriber : subscribers) {
			subscriber->notify(book);
		}
	}
	void updatebook(book* book) {
		notifyall(book);
	}
};
int main() {
	//singleton used for library object
	library* lib=library::getinstance();
	admin admin(1, "Adminuser", "admin@gmail.com", "12345");
	book* book1 = new book(1, "book1", "author1");
	book1->addpage(new page(0, "page1 content"));
	book1->addpage(new page(0, "page2 content"));
	book1->addpage(new page(0, "page3 content"));
	admin.uploadbook(*lib, book1);

	user user1(1, "ajit", "ajit@gmail.com", "!12345");
	//observer pattern for notification, we can also create type of notification
	subscriber* subs1 = new subscriber(user1.getuserid());
	group *gr = new group;
	gr->addsubscriber(subs1);
	//strategy pattern to decide function type of search
	Istrategy_search* strategy = new searchbytitle();
	book* bookfound = strategy->search(*lib,"book1");
	if (bookfound) {
		readingsession session(bookfound);
		string content=session.getcurrentpage()->getcontent();

		bookmark bm(bookfound->getbookid(), 1);
		user1.addbookmark(bm);
	}

	gr->notifyall(bookfound);
	delete book1;
	delete subs1;
	delete gr;
	delete strategy;
	return 0;
}
