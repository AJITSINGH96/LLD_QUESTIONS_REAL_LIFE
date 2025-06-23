#include<iostream>
#include <memory>
using namespace std;


//Problem Statement(Strategy Pattern Use)
//A system supports multiple payment methods :
//
//Credit Card
//
//PayPal
//
//UPI
//
//The user should be able to choose the payment method at runtime — so we encapsulate the varying logic using Strategy Pattern.

//When the algorithm can vary independently from the client-->IN THIS CASE PAYMENT ALGORITHM
//stratergy interface
class payment_stratergy {
public:
	virtual void pay(int amount) const = 0;
	virtual ~payment_stratergy() {}
};

//concrete stratergy 1
class CreditCardPayment :public payment_stratergy {
public:
	void pay(int amount)const override {
		cout << "pay vid credit card";
		return ;
	}
};

//concrete stratergy 2
class PayPalPayment :public payment_stratergy {
public:
	void pay(int amount)const override {
		cout << "pay vid Paypal";
		return ;
	}
};

//concrete stratergy 2
class UPI_Payment :public payment_stratergy {
public:
	void pay(int amount)const override {
		cout << "pay vid UPI";
		return ;
	}
};

//context class
class context {
private:
	unique_ptr<payment_stratergy>str;
public:
	//context(unique_ptr<stratergy>s):str(move(s)){}
	context(std::unique_ptr<payment_stratergy> p) {
		str = move(p); // assign inside the body
	}

	void set_payment_method(unique_ptr<payment_stratergy>s) {
		str = move(s);
	}
	void execute_payment(int amount)const {
		return str->pay(amount);
	}
};

int main() {
	context context(make_unique<CreditCardPayment>());
	context.execute_payment(5000) ;

	context.set_payment_method(make_unique<PayPalPayment>());
	context.execute_payment(5000);

	context.set_payment_method(make_unique<UPI_Payment>());
	context.execute_payment(5000);
	return 0;
}
//UML DIAGRAM
/*
+ ---------------------------- +
|    <<interface>>          |
|       PaymentStrategy |
+---------------------------- +
| +pay(amount : double) |
+---------------------------- +
▲
------------------------------ -
|              |             |
+------------ + +------------ + +------------ +
| CreditCard | |   PayPal   | |    UPI |
+------------ + +------------ + +------------ +
| +pay()    | | +pay()    | | +pay() |
+------------ + +------------ + +------------ +

+------------------------ - +
| PaymentContext |
+------------------------ - +
| -strategy: PaymentStrategy |
+------------------------ - +
| +setStrategy(s : PaymentStrategy) |
| +payBill(amount : double) |
+------------------------ - +
*/
