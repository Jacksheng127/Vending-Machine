#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<cfloat>
#include<string>
#include<vector>
#include<fstream>
#include<ctime>
using namespace std;

int getInteger();
void print_changes(vector<int> count, vector<double> money);
void store_user_input (int n, vector<int>& y);
void read_stock(vector<string>& item, vector<double>& price, vector<int>& quantity);
void read_cash(vector<double>& money, vector<int>& amount);
void menu_stock(vector<string> item, vector<double> price, vector<int> quantity);
double cal_price(vector<int>& check_order, vector<int>& quantity, double total, vector<double> price);
bool check_even(double change, vector<double> money);
void check_odd(double& change, vector<double> money, vector<int>&count, vector<int>& amount);
void calcu_odd(double& change, vector<double> money, vector<int>&count, vector<int>& amount);
void minus_even(double& change, vector<double> money, vector<int>&count, vector<int>& amount);
void cal_change(vector<double> money, double& change, vector<int>& count, vector<int>& amount);
void update_item(vector<string>& item, vector<double>& price, vector<int>& quantity, vector<int> check_order);
void update_cash(vector<double>& money, vector<int>& amount);
void print_receipt(vector<string> item, vector<double> price, vector<int> receipt_order, double total);
double cal_total_sales(vector<int> report_order, vector<double> price);
void generate_report(vector<string> item, vector<int>& report_order, vector<double> price);
void menu_cash(vector<double> money, vector<int> amount);
int width_to_set(vector<string> item, int i);
void cal_pay_amount(vector<double> money, vector<int>& u_pay, vector<int>& t_u_pay, vector<int>& amount, double& pay_amount);

int main()
{   
    
    vector<string> item;
    vector<double> price;
    vector<int> quantity;
    read_stock(item, price, quantity);
	vector<int>report_order(item.size(), 0);

    do
    {
        vector<string> item;
        vector<double> price;
        vector<int> quantity;
        read_stock(item, price, quantity);
        
        vector<int>check_order(item.size(), 0);
        vector<int>receipt_order(item.size(), 0);
        double total = 0;
        int num;
        
        menu_stock(item, price, quantity);
        bool status2 = true;
        bool status = true;
        do
        {
            cout << "press the number to order (press 9 to stop) : ";
            num = getInteger();
            if (num == 999)//this is passcode for owner to enter before get the report
            {   
                int pin;
                cout << "please enter the pin : ";
                cin >> pin;

                if (pin == 9015)//this is the pin number 
                {
                    generate_report(item, report_order, price);
                    exit(0);
                }
                else
                    continue;
            }

            if (num == 9)
                break;
            
            check_order[num - 1]++;
            
        }while(num != 9);

        for (int i= 0; i < check_order.size(); i++)
        {
            if (check_order[i] > quantity[i])
            {
                cout << item[i] << " have run out. Please order again! \n\n";
                status = false;
            }
        }

        if (status == false)
        {
            continue;
        }
        
        //copy from check_order
        for(int i = 0; i < check_order.size(); i++)
        {
           receipt_order[i] = check_order[i];
        }

        //total order form user 
         for(int i = 0; i < check_order.size(); i++)
        {
           report_order[i] += receipt_order[i];
        }
        
        double total2 = cal_price(check_order, quantity, total, price);
        cout << "total : RM " << total2 << endl << endl;

        vector<double> money;
        vector<int> amount;
        read_cash(money, amount);
        
        //cash part
        menu_cash(money, amount);
        vector<int>user_pay(money.size(), 0);
        vector<int>temp_user_pay(money.size(), 0);
        double pay_amount = 0, temp_pay_amount = 0;
        int num1, num2;

        store_user_input (num1 , user_pay);
        
        cal_pay_amount(money, user_pay, temp_user_pay, amount, pay_amount);
        
        cout << "You have inserted RM " << pay_amount << endl;

        double change = pay_amount - total2;

        while (change < 0.00)
        {
            //double change = pay_amount - total2;
            cout << "You still need to pay RM " << (change * -1.00) << endl;
            cout << "If you dont want to buy, please enter 99 to exit! \n";
            do
            {
                cout << "press the number to order (press 9 to stop) : ";
                num2 = getInteger();
                
                if (num2 == 9)
                    break;

                if (num2 == 99)
                {   
                    report_order.clear();
                    for(int i = 0; i < money.size(); i++)
                    {   
                        while(temp_user_pay[i] > 0)
                        {
                            cout << " ho " << endl;
                            temp_user_pay[i]--;
                            amount[i]--;
                        }
                    }
                    break;
                }

                user_pay[num2 - 1]++;
                
            }while(num2 != 9);
    
            cout << endl;

            if (num2 == 99)
            {   
                status2 = false;
                change = 0.00;
                break;
            }
        
            cal_pay_amount(money, user_pay, temp_user_pay, amount, temp_pay_amount);
            change = temp_pay_amount - (change * -1.00);
        }

        if (status2 == false)
        {
            continue;
        }

        cout << "the change is RM " << change << " : ";

        vector<int> count(money.size(), 0);
        change += 0.00001;

        if (amount[0] >= 1)
        {
            cal_change(money, change, count, amount);
        }
        else
        {
            while(change > 5.30002)
            {
                change -= money[4];
                count[4]++;
                amount[4]--;
            }

            while(change > 1.30002)
            {
                change -= money[3];
                count[3]++;
                amount[3]--;
            }

            if (check_even(change, money))
            {	
                minus_even(change, money, count, amount);
            }
            else 
            {
                check_odd(change, money, count, amount);
            }
            
        }

        print_changes(count, money);
        count.clear();
        cout << "\nPlease collect your items and receipt. \n" << "Thank you very much. Please come again! \n\n";

        update_item(item, price, quantity, check_order);
        update_cash( money, amount);
        print_receipt(item, price, receipt_order, total2);

    }while(true);

	return 0;
}

int getInteger()
{
    string str;

    do
    {
        getline(cin, str);
    }while (str.find_first_not_of( "-0123456789" ) != string::npos);
    //int num = atoi(str.c_str());
    //return num;
    return atoi(str.c_str());
}

void print_changes(vector<int> count, vector<double> money)
{
	for (int i = 0; i < count.size(); i++)
	{	
		if (count[i] > 0)
		{
			cout << "\n"<< count[i] << " x RM " << money[i];
		}
	}

	return;
}

void store_user_input (int n, vector<int>& y)
{	
	do
	{	
		cout << "press the number to order/insert (press 9 to stop) : ";
		n = getInteger();

        if (n == 9)
            break;

        y[n - 1]++;
        
	}while(n != 9);

	return;
}

void read_stock(vector<string>& item, vector<double>& price, vector<int>& quantity)
{
	string filename = "stock.dat";

    ifstream file_in;
    file_in.open( filename.c_str() );

    if( !file_in.is_open() )
        exit(0);

    char comma;
	string lineOfText;
    int numOfCol, numOfItems;
	string word;
	string delimiter = ",";
	size_t delimLoc = 0;

	// Line 1 is an int rep. the num of columns.
	file_in >> numOfCol;
	
	// Line 2 is an int rep. the num of items we sell.
	file_in >> numOfItems;

	double tempPrice;
	int tempQuantity;
	
	// Reading in data:
	for (int i = 0; i < numOfItems; i++) {
		file_in >> lineOfText;
		
		// First is "item"
		delimLoc = lineOfText.find(delimiter);
		word = lineOfText.substr(0, delimLoc);
		item.push_back(word);
		lineOfText.erase(0, delimLoc + 1);
		
		// 2nd is "price"
		delimLoc = lineOfText.find(delimiter);
		word = lineOfText.substr(0, delimLoc);
		tempPrice = stod(word);
		price.push_back(tempPrice);
		lineOfText.erase(0, delimLoc + 1);
	
		// The last is "quantity"
		delimLoc = lineOfText.find(13); 		// 13 is CR in ASCII
		word = lineOfText.substr(0, delimLoc);
		tempQuantity = stoi(word);
		quantity.push_back(tempQuantity);
		lineOfText.erase(0, delimLoc + 1);	
	}

    file_in.close();

    return;
}

void read_cash(vector<double>& money, vector<int>& amount)
{
	string filename1 = "cash.dat";

    ifstream file_in;
    file_in.open( filename1.c_str() );

    if( !file_in.is_open() )
        exit(0);

	char comma;
    int numOfCol, numOfItems;
    string lineOfText;
    string word;
	string delimiter = ",";
	size_t delimLoc = 0;
    double temp_money;
    int temp_amount;

	// Line 1 is an int rep. the num of columns.
	file_in >> numOfCol;
	
	// Line 2 is an int rep. the num of items we sell.
	file_in >> numOfItems;

    for (int i = 0; i < numOfItems; i++)
    {
        file_in >> lineOfText;

        delimLoc = lineOfText.find(delimiter);
		word = lineOfText.substr(0, delimLoc);
		temp_money= stod(word);
		money.push_back(temp_money);
		lineOfText.erase(0, delimLoc + 1);

        delimLoc = lineOfText.find(delimiter);
		word = lineOfText.substr(0, delimLoc);
		temp_amount= stod(word);
		amount.push_back(temp_amount);
		lineOfText.erase(0, delimLoc + 1);
    }
    file_in.close();
}

void menu_stock(vector<string> item, vector<double> price, vector<int> quantity)
{
    cout << setw(10) << "ITEM\t\t" << setw(7) << "PRICE\t" << setw(15) << "QUANTITY" << endl;
    cout << setw(12) << "========\t\t" << "========" << setw(15) << "=========" << endl;
    cout << setprecision(2) << fixed << showpoint;

    int i = 0;
    for (; i < item.size(); i++)
    {
        cout << (i + 1) << ".  " << item[i] << setw(width_to_set(item, i)) << price[i] << setw(13) << quantity[i] << endl;
    } 

    return;
}

int width_to_set(vector<string> item, int i)
{
    int value = 26 - (item[i].size());

    return value;
}


void menu_cash(vector<double> money, vector<int> amount)
{
    int j = 0;
    for (; j < money.size(); j++)
        cout << (j + 1) << ".  " << money[j] << setw(10) << amount[j] << endl;

    return;
}

	
double cal_price(vector<int>& check_order, vector<int>& quantity, double total, vector<double> price)
{
	for (int i = 0; i < check_order.size(); i++)
	{
		while(check_order[i] > 0)
		{	
			quantity[i] -= 1;
			total += price[i];
			check_order[i] -= 1;
		}
	}
	return total;
}


void cal_change(vector<double> money, double& change, vector<int>& count, vector<int>& amount)
{
	for (int k = 0; k <= money.size(); k++)
	{	
        for(int h = money.size() - 1 ; h >= h - 1 ; h--)            
		{	
			if(change >= 0.0001)
			{
				if (change >= money[h])
				{	
					change -= money[h];
					amount[h] -= 1;
					count[h] += 1;
					break; 
				}  
			}
			else
			{
				break;
			}
			
		}
   	}

    return;
}

bool check_even(double change, vector<double> money)
{   
	bool status = false;
    while(change > 0.0001)
    {	
        if (static_cast<int>(change * 10.0) % 2)
        {
			status = false;
            return status;
        }

        change -= money[1];
    }

	status  = true;
    return status;
}

void check_odd(double& change, vector<double> money, vector<int>& count, vector<int>& amount)
{
    if (change <= 0.30001 )
    {
        change -= money[1];
        amount[1]++;
        count[1]++;

        if (change <= 0.10001)
            cout << "sorry we dont have 10 sen to return to you ";
    }

    else 
    {
        calcu_odd(change, money, count, amount);
    }

    return;
}

void calcu_odd(double& change, vector<double> money, vector<int>& count, vector<int>& amount)
{
	
    change -= money[2];
    count[2]++;
    amount[2]--;
    minus_even(change, money, count, amount);
	
    return;
}

void minus_even(double& change, vector<double> money, vector<int>& count, vector<int>& amount)
{	
	
    while(change > 0.001)
    { 
        change -= money[1];
        count[1]++;
        amount[1]--;
    }

    return;
}

void update_item(vector<string>& item, vector<double>& price, vector<int>& quantity, vector<int> check_order)
{
	string filename = "stock.dat";

    ofstream update_stock;
    update_stock.open( filename.c_str() );

	update_stock << "3" << endl;
	update_stock << check_order.size() << endl;

	for (int i = 0; i < item.size(); i++)
    {
	    update_stock << item[i] << "," << price[i] << "," << quantity[i] << endl;
	}
	
   	update_stock.close();

	return;
}

void update_cash(vector<double>& money, vector<int>& amount)
{
	string filename = "cash.dat";

    ofstream update_cash;
    update_cash.open( filename.c_str() );

	update_cash << "2" << endl;
	update_cash << money.size() << endl;

	for (int i = 0; i < money.size(); i++)
    {
	    update_cash << money[i] << "," << amount[i] << endl;
	}
	
   	update_cash.close();

	return;
}

void print_receipt(vector<string> item, vector<double> price, vector<int> receipt_order, double total)
{
    time_t current = time(0);

    string filename = "receipt.txt";
    ofstream out_file;
    out_file.open( filename.c_str() );

    out_file << "*************** Receipt *****************" << endl;
    out_file << "Source: LIM JACK SHENG Vending Machine" << endl;
    out_file << "Date: " << put_time(localtime(&current), "%d %B %Y") << endl;
    out_file << "Time: " << put_time(localtime(&current), "%I:%M %p") << endl << endl;
    
    out_file << setprecision(2) << fixed << showpoint;

    for (int i = 0; i < item.size(); i++)
    {
        if (receipt_order[i] == 0)
            continue;
        else
        {
            out_file << setw(10) << left << item[i] << "x " << receipt_order[i] << setw(10) << right << "RM" << 
            (receipt_order[i] * price[i]) << endl;
        }
    }
    out_file << "--------------------------------------" << endl;
    out_file << setw(21) << left << "Total:" << "RM" << total << endl << endl;
    out_file << "\t\t\tThank You!" << endl;
    out_file << "********** LIM JACK SHENG Vending Machine *************";
    out_file.close();

    return;
}

void generate_report(vector<string> item, vector<int>& report_order, vector<double> price)
{
    time_t current = time(0);

    string file_name = "report.html";
    ofstream out_file;
    out_file.open(file_name.c_str() );

    out_file << "<b>**** Sales Report ****</b> </br>" << endl;
    out_file << "Source: LIM JACK SHENG Vending Machine </br>" << endl;
    out_file << "Date: " << put_time(localtime(&current), "%d %B %Y") << "</br>" << endl;
    out_file << "Time: " << put_time(localtime(&current), "%I:%M %p") << "</br>" << endl
            << "</br>"
            << endl;

    out_file << setprecision(2) << fixed << showpoint;

    out_file << "Items Sales:"
            << "</br>" << endl;
    out_file << "<table>" << endl;

    for (int i = 0; i < item.size(); i++)
    {
        out_file << "<tr>" << endl;
        out_file << "<td>" << i + 1 << ". "
                << "</td>"
                << "<td width='100px'>" << left << item[i]<< "</td>"
                << "<td width='50px'>" << right << setprecision(2) << fixed << showpoint << report_order[i] << "</td>" << endl;
       	out_file << "</tr>" << endl;
    }
   	out_file << "</table>" << endl;
    out_file << "````````</br>" << endl
            << "Total Sales: RM" << cal_total_sales(report_order, price) << "</br>" << endl;

    out_file.close();

    return;

}

double cal_total_sales(vector<int> report_order, vector<double> price)
{   
    double total = 0.00;

    for (int i = 0; i < report_order.size(); i++)
    {
        total += (report_order[i] * price[i]);
    }

    return total;
}

void cal_pay_amount(vector<double> money, vector<int>& u_pay, vector<int>& t_u_pay, vector<int>& amount, double& pay_amount)
{
    for(int i = 0; i < money.size(); i++)
    {	
        while(u_pay[i] > 0)
        {
            pay_amount += money[i];
            u_pay[i]--;
            t_u_pay[i]++;
            amount[i]++;
        }
    }

    return;
}