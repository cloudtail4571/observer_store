#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
*   Почему Observer лучше, чем прямые вызовы методов клиентов:
* 
*   Слабая связь - Store ничего не знает о конкретных классах клиентов (Email, VIP и т.д.), 
*   он работает только через общий интерфейс Customer. Это делает код гибким.
* 
*   Легкое добавление наблюдателей — можно создавать новые типы клиентов (например, SMSCustomer),
*   не меняя ни единой строчки кода в классе Store.
*
*   Динамическое управление — подписка и отписка происходят во время выполнения программы (runtime),
*   что позволяет гибко управлять списком рассылки.
*   
*/

class Store;

class Customer
{
public:
    virtual ~Customer() {}
    virtual void update(const string& discountMessage, double discountPercent, Store* store) = 0;
};


class Store
{
private:
    vector< Customer*> Customers;
    string currentDiscount = "";
    double currentPercent = 0.0;
public:
    void subscribe(Customer* cust)
    {
        Customers.push_back(cust);
    }
    void unsubscribe(Customer* cust)
    {
        auto iterator = find(Customers.begin(), Customers.end(), cust);
        if (iterator != Customers.end())
        {
            Customers.erase(iterator);
        }
    }
    void notify()
    {
        for (auto cust : Customers)
        {
            cust->update(currentDiscount, currentPercent, this);
        }
    }
    void setDiscount(const string& disc, double percent)
    {
        currentDiscount = disc;
        currentPercent = percent;
        notify();
    }
    string getCurrentDiscount() const
    {
        return currentDiscount + " (информация получена через Pull)";
    }
};


class EmailCustomer : public Customer
{
private:
    string name = "";
public:
    EmailCustomer(const string &n) : name(n){}
    void update(const string& message, double percent, Store* store) override
    {
        if (store) {
            cout << "[PULL] Email to " << name << ": " << store->getCurrentDiscount() << endl;
        }
    }
};

class RegelarCustomer : public Customer
{
private:
    string name = "";
public:
    RegelarCustomer(const string& n) : name(n) {}
    void update(const string& message, double percent, Store* store) override
    {
        cout << "[PUSH] Покупатель " << name << ": " << message << " (экономия " << percent << "%)" << endl;
    }
};

class VIPCustomer : public Customer
{
private:
    string name = "";
public:
    VIPCustomer(const string& n) : name(n) {}
    void update(const string& message, double percent, Store* store) override
    {
        cout << "[PUSH] VIP " << name << ": " << message << " + Бонус!" << endl;
    }
};

int main()
{
    setlocale(LC_ALL, "RUS");
    Store store;
    RegelarCustomer a("A");
    RegelarCustomer b("B");
    VIPCustomer c("C");
    EmailCustomer david("David");
    store.subscribe(&a);
    store.subscribe(&b);
    store.subscribe(&c);
    store.subscribe(&david);

    store.setDiscount("Скидка 20% на все товары!", 20.0);
    cout << endl;
    store.unsubscribe(&b);
    store.setDiscount("Скидка 90% на все товары!", 90.0);
}