#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
using namespace std;
//final code

// ===== TEMPLATE FUNCTION =====
template <typename T>
void showList(vector<T*> &v){
    for(int i=0;i<v.size();i++){
        v[i]->display();
    }
}

// ===== BASE =====
class MenuItem{
protected:
    int id; string name,desc,type; double price;
public:
    MenuItem(){}
    MenuItem(int i,string n,string d,double p,string t):id(i),name(n),desc(d),price(p),type(t){}

    virtual double getPrice(){ return price; }
    virtual string getCategory(){ return "Normal"; }

    void display(){
        cout<<id<<" | "<<name<<" | "<<type<<" | Rs "<<getPrice()<<"\n";
    }

    int getId(){ return id; }
    string getName(){ return name; }
    string getType(){ return type; }
    string getDesc(){ return desc; }

    // Operator Overloading
    double operator+(MenuItem &m){
        return this->getPrice() + m.getPrice();
    }
};

// ===== SPECIAL =====
class SpecialItem:public MenuItem{
    double discount;
public:
    SpecialItem(int i,string n,string d,double p,string t,double dis)
        :MenuItem(i,n,d,p,t),discount(dis){}

    double getPrice(){ return price-(price*discount/100); }
    string getCategory(){ return "Special"; }
    double getDiscount(){ return discount; }
};

// ===== COMBO =====
class ComboItem:public MenuItem{
    double discount;
public:
    ComboItem(int i,string n,string d,double p,string t,double dis)
        :MenuItem(i,n,d,p,t),discount(dis){}

    double getPrice(){ return price-(price*discount/100); }
    string getCategory(){ return "Combo"; }
    double getDiscount(){ return discount; }
};

// ===== CART =====
class CartItem{
public:
    MenuItem* item;
    int qty;

    CartItem(MenuItem* i,int q){ item=i; qty=q; }

    double total(){ return item->getPrice()*qty; }
};

// ===== ORDER =====
class Order{
public:
    int id;
    string status;
    double total;

    Order(int i,double t){
        id=i; total=t; status="Placed";
    }

    void update(){
        if(status=="Placed") status="Preparing";
        else if(status=="Preparing") status="OutForDelivery";
        else if(status=="OutForDelivery") status="Delivered";
    }

    void show(){
        cout<<"Order "<<id<<" | "<<status<<" | "<<total<<"\n";
    }
};

// ===== SYSTEM =====
class FoodSystem{

    vector<MenuItem*> menu;
    vector<CartItem*> cart;
    vector<Order*> orders;

public:

// ===== CUSTOMER DETAILS =====
void customerDetails(){
    ofstream f("data.txt", ios::app);

    string name, address;
    int age;
    long phone;

    cout<<"Enter Name: ";
    cin>>name;

    cout<<"Enter Age: ";
    cin>>age;

    cout<<"Enter Phone: ";
    cin>>phone;

    cout<<"Enter Address: ";
    cin>>address;

    f<<"Customer: "<<name<<" | Age: "<<age
     <<" | Phone: "<<phone
     <<" | Address: "<<address<<"\n";

    cout<<"Details saved successfully!\n";
}

// ===== LOAD =====
void loadMenu(){
    try{
        ifstream f("ELproject.txt");
        if(!f) throw runtime_error("ERROR: File not found!");

        int id; string n,d,t,cat; double p,disc;

        while(f>>id>>n>>d>>p>>t>>cat>>disc){
            if(cat=="Normal")
                menu.push_back(new MenuItem(id,n,d,p,t));
            else if(cat=="Special")
                menu.push_back(new SpecialItem(id,n,d,p,t,disc));
            else
                menu.push_back(new ComboItem(id,n,d,p,t,disc));
        }

        cout<<"Loaded items: "<<menu.size()<<endl;
    }
    catch(exception &e){
        cout<<e.what()<<endl;
    }
}

// ===== SAVE =====
void saveMenu(){
    ofstream f("ELproject.txt");

    for(int i=0;i<menu.size();i++){
        double disc=0;

        if(menu[i]->getCategory()=="Special")
            disc=((SpecialItem*)menu[i])->getDiscount();
        else if(menu[i]->getCategory()=="Combo")
            disc=((ComboItem*)menu[i])->getDiscount();

        f<<menu[i]->getId()<<" "<<menu[i]->getName()<<" "<<menu[i]->getDesc()<<" "
         <<menu[i]->getPrice()<<" "<<menu[i]->getType()<<" "
         <<menu[i]->getCategory()<<" "<<disc<<"\n";
    }
}

// ===== DISPLAY =====
void showMenu(){
    cout<<"\n===== FULL MENU =====\n";
    showList(menu); // TEMPLATE USED
}

void showVeg(){
    cout<<"\n===== VEG MENU =====\n";
    for(int i=0;i<menu.size();i++)
        if(menu[i]->getType()=="Veg") menu[i]->display();
}

void showNonVeg(){
    cout<<"\n===== NON-VEG MENU =====\n";
    for(int i=0;i<menu.size();i++)
        if(menu[i]->getType()=="NonVeg") menu[i]->display();
}

void showSpecial(){
    cout<<"\n===== SPECIAL MENU =====\n";
    for(int i=0;i<menu.size();i++)
        if(menu[i]->getCategory()=="Special") menu[i]->display();
}

void showCombo(){
    cout<<"\n===== COMBO MENU =====\n";
    for(int i=0;i<menu.size();i++)
        if(menu[i]->getCategory()=="Combo") menu[i]->display();
}

// ===== FIND =====
MenuItem* find(int id){
    for(int i=0;i<menu.size();i++)
        if(menu[i]->getId()==id)
            return menu[i];

    throw runtime_error("Item not found!");
}

// ===== CART =====
void addToCart(){
    try{
        int id,q;
        cout<<"ID : "; cin>>id;
        cout<<"\nQty: "; cin>>q;

        if(q<=0) throw invalid_argument("Quantity must be positive!");

        MenuItem* m=find(id);
        cart.push_back(new CartItem(m,q));
    }
    catch(exception &e){
        cout<<e.what()<<endl;
    }
}

void viewCart(){
    if(cart.empty()){
        cout<<"Cart empty\n";
        return;
    }

    for(int i=0;i<cart.size();i++)
        cout<<i+1<<" "<<cart[i]->item->getName()<<" x"<<cart[i]->qty<<" = "<<cart[i]->total()<<"\n";
}

void editCart(){
    int ch;
    while(true){
        cout<<"1 Update\n2 Remove\n3 Clear\n4 Back\n";
        cout<<"Enter Choice : ";
        cin>>ch;

        if(ch==1){
            int i,q;
            cout<<"Enter id and quantity:";
            cin>>i>>q;
            if(i>0 && i<=cart.size() && q>0)
                cart[i-1]->qty=q;
            else cout<<"Invalid\n";
        }
        else if(ch==2){
            int i;
            cout<<"Enter Id";
            cin>>i;
            if(i>0 && i<=cart.size())
                cart.erase(cart.begin()+i-1);
        }
        else if(ch==3) cart.clear();
        else break;
    }
}

// ===== BILL =====
void bill(){
    if(cart.empty()){
        cout<<"Cart is empty!\n";
        return;
    }

    double total=0;

    cout<<"\n=========== BILL ===========\n";
    cout<<left<<setw(5)<<"No"<<setw(15)<<"Item"<<setw(10)<<"Price"<<setw(8)<<"Qty"<<setw(10)<<"Total"<<endl;
    cout<<"---------------------------------------------\n";

    for(int i=0;i<cart.size();i++){
        double price = cart[i]->item->getPrice();
        double t = cart[i]->total();

        cout<<setw(5)<<i+1
            <<setw(15)<<cart[i]->item->getName()
            <<setw(10)<<price
            <<setw(8)<<cart[i]->qty
            <<setw(10)<<t<<endl;

        total += t;
    }

    cout<<"---------------------------------------------\n";
    cout<<"GRAND TOTAL: Rs "<<total<<endl;
    cout<<"=============================================\n";
}

// ===== REVIEW =====
void addReview(){
    ofstream f("data.txt",ios::app);
    int r;
    cout<<"Give rating out of 5: ";
    cin>>r;

    if(r>=1&&r<=5){
        f<<"Rating: "<<r<<"/5\n";
        cout<<"Thank you for your feedback!\n";
    }
    else cout<<"Invalid rating! Skipped.\n";
}

void viewReviews(){
    ifstream f("data.txt");
    string line;

    cout<<"\n===== CUSTOMER REVIEWS =====\n";
    while(getline(f,line))
        if(line.find("Rating:")!=string::npos)
            cout<<line<<"\n";
}

void averageRating(){
    ifstream f("data.txt");
    string line;
    int sum=0,c=0;

    while(getline(f,line))
        if(line.find("Rating:")!=string::npos){
            sum+=line[8]-'0';
            c++;
        }

    if(c>0) cout<<"Average Rating: "<<(double)sum/c<<"/5\n";
    else cout<<"No ratings yet\n";
}

// ===== ORDER =====
void placeOrder(){
    double t=0;

    for(int i=0;i<cart.size();i++)
        t+=cart[i]->total();

    if(t==0){
        cout<<"Cart empty!\n";
        return;
    }

    orders.push_back(new Order(orders.size()+1,t));
    cout<<"Order ID: "<<orders.size()<<"\n";

    cart.clear();
    addReview();
}

void track(){
    try{
        int id;
        cout<<"Enter Id : ";
        cin>>id;

        if(id<=0 || id>orders.size())
            throw out_of_range("Invalid Order ID");

        orders[id-1]->show();
    }
    catch(exception &e){
        cout<<e.what()<<endl;
    }
}

void updateOrder(){
    int id;
    cout<<"Enter Id : ";
    cin>>id;

    if(id>0 && id<=orders.size())
        orders[id-1]->update();
}

// ===== CUSTOMER =====
void customer(){
    customerDetails();

    int ch;

    while(true){
        cout<<"\n1 Full Menu\n2 Veg\n3 NonVeg\n4 Special\n5 Combo\n";
        cout<<"6 Add\n7 Cart\n8 Edit\n9 Bill\n10 Order\n11 Track\n12 Exit\n";
        cout<<"Enter Choice : ";
        cin>>ch;

        if(ch==1) showMenu();
        else if(ch==2) showVeg();
        else if(ch==3) showNonVeg();
        else if(ch==4) showSpecial();
        else if(ch==5) showCombo();
        else if(ch==6) addToCart();
        else if(ch==7) viewCart();
        else if(ch==8) editCart();
        else if(ch==9) bill();
        else if(ch==10) placeOrder();
        else if(ch==11) track();
        else if(ch==12) break;
        else break;
    }
}

// ===== ADMIN =====
void addItem(){
    int id;
    string n,d,t,cat;
    double p,disc;

    cout<<"Enter details: ";
    cin>>id>>n>>d>>p>>t>>cat;

    if(cat=="Normal")
        menu.push_back(new MenuItem(id,n,d,p,t));
    else{
        cin>>disc;
        if(cat=="Special")
            menu.push_back(new SpecialItem(id,n,d,p,t,disc));
        else
            menu.push_back(new ComboItem(id,n,d,p,t,disc));
    }

    saveMenu();
}

void deleteItem(){
    int id;
    cout<<"Enter Id: ";
    cin>>id;

    for(int i=0;i<menu.size();i++)
        if(menu[i]->getId()==id){
            menu.erase(menu.begin()+i);
            break;
        }

    saveMenu();
}

void admin(){
    string p;
    cout<<"Enter Password: ";
    cin>>p;

    if(p!="admin"){
        cout<<"Wrong Password\n";
        return;
    }

    int ch;

    while(true){
        cout<<"1 View\n2 Add\n3 Delete\n4 Orders\n5 UpdateStatus\n6 Reviews\n7 AvgRating\n8 Exit\n";
        cout<<"Enter Choice : ";
        cin>>ch;

        if(ch==1) showMenu();
        else if(ch==2) addItem();
        else if(ch==3) deleteItem();
        else if(ch==4)
            for(int i=0;i<orders.size();i++)
                orders[i]->show();
        else if(ch==5) updateOrder();
        else if(ch==6) viewReviews();
        else if(ch==7) averageRating();
        else break;
    }
}
};

// ===== MAIN =====
int main(){
    FoodSystem f;
    f.loadMenu();

    int ch;

    while(true){
        cout<<"1 Customer\n2 Admin\n3 Exit\n";
        cout<<"Enter Choice : ";
        cin>>ch;

        if(ch==1) f.customer();
        else if(ch==2) f.admin();
        else break;
    }

    return 0;
}