#include <iostream>
#include <list>
#include <ctime>
#include <thread>
#include <fstream>
using namespace std;

void sleep_millis(unsigned int ms)
{
  this_thread::sleep_for(chrono::milliseconds(ms));
}

class item{
    public:
        item(string _name, string _description) {
            name = _name;
            description = _description;
            time_t current_time = time(nullptr);
            tm* local_time = localtime(&current_time);
            char datestr[100];
            strftime(datestr, sizeof(datestr), "%m/%d/%Y", local_time);
            date_made = string(datestr);
            is_complete = false;
        };

        item(string _name, string _description, string _date_made, bool _is_complete) {
            name = _name;
            description = _description;
            date_made = _date_made;
            is_complete = _is_complete;
        };

        void display_item_full() {
            string check = " ";
            if (is_complete) {
                check = "X";
            }
            cout << "Item: " << name << "\nDate Created: " << date_made << "\nDescription: " << description << "\nCompleted: (" << check << ")" << endl;
            cout << endl;
        };

        void display_item_short() {
            string check = " ";
            if (is_complete) {
                check = "X";
            }
            cout << name << " (" << check << ")" << endl;
        }

        string edit_data(string variable, string value) {
            cout << "Current " << variable << ": " << value << endl;
            cout << "New " << variable << ": ";
            string new_value;
            getline(cin, new_value);
            cout << endl;

            cout << "Would you like to replace \"" << value << "\" with \"" << new_value << "\"? [Y/N]: ";
            string answer;
            cin >> answer;
            cin.ignore();
            if (answer == "Y") {
                cout << "\"" << variable << "\" changed to \"" << new_value << "\"" << endl;
                sleep_millis(2000);
                cout << endl;
                return new_value;
            } else {
                cout << "\"" << variable << "\" not changed" << endl;
                sleep_millis(2000);
                cout << endl;
                return value;
            }
        }

        void edit_name() {
            name = edit_data("name", name);
        }

        void edit_description() {
            description = edit_data("description", description);
        }

        void check_completed() {
            is_complete = true;
        }

        string get_name() {
            return name;
        }

        string get_description() {
            return description;
        }

        string get_date() {
            return date_made;
        }

        bool get_completed() {
            return is_complete;
        }

    private:
        string name;
        string date_made;
        string description;
        bool is_complete;
};

class to_do{

    private:
        list<item> items;

    public:
        to_do() {
            items = {};
        }

        void print_items() {
            if (items.size() < 1){
                cout << "No items in list" << endl;
            } else {
                int list_num = 0;
                for(auto it = items.begin(); it != items.end(); ++it) {
                    list_num ++;
                    cout << list_num << ". ";
                    (*it).display_item_short();
                }
            }
            cout << endl;
        }

        void add_item() {
            cout << "Adding new item to the list \nItem name: ";
            string name;
            getline(cin, name);
            cout << "Description: ";
            string description;
            getline(cin, description);

            cout << "Do you want to add \"" << name << "\" to the list? [Y/N]: ";
            string input;
            cin >> input;
            cin.ignore();
            if (input == "Y") {
                items.push_back(item(name, description));
                cout << "\"" << name << "\" added to items" << endl;
            } else {
                cout << name << " not added" << endl;
            }
            sleep_millis(2000);
            cout << endl;

        }

        void select_item() {
            print_items();
            cout << "Select an item: ";
            int item_num;
            cin >> item_num;
            cin.ignore();

            if (item_num >= 0 && item_num <= items.size()) {
                list<item>::iterator it;
                it = items.begin();
                for(int i = 0; i < item_num - 1; i++) {
                    ++it;
                }
                cout << endl;
                
                string menu[5] = {"1. Edit name", "2. Edit description", "3. Mark complete", "4. Delete", "5. Back to list"};
                bool quit = false;

                do {
                    (*it).display_item_full();
                    for(string option : menu) {
                        cout << option << endl;
                    }
                    

                    cout << "Choose an option number: ";
                    int user_input;
                    cin >> user_input;
                    cin.ignore();
                    cout << endl;

                    switch(user_input) {
                        case 1:
                            (*it).edit_name();
                            break;

                        case 2:
                            (*it).edit_description();
                            break;

                        case 3:
                            mark_completed(item_num);
                            break;

                        case 4:
                            if (delete_item(item_num)) {
                                quit = true;
                            };
                            break;
                        
                        case 5:
                            quit = true;
                            break;

                    }
                } while(!quit);
                
            } else {
                cout << "Item not found" << endl;
            }
        }

        bool delete_item(int list_num) {
            bool deleted = false;
            if (list_num >= 0 && list_num <= items.size()) {
                list<item>::iterator it;
                it = items.begin();
                for(int i = 0; i < list_num - 1; i++) {
                    ++it;
                }
                string del;
                cout << "Do you wish to delete \"" << (*it).get_name() << "\" from the list? [Y/N]: ";
                cin >> del;
                cin.ignore();
                if (del == "Y") {
                    items.erase(it);
                    cout << "Item deleted." << endl;
                    deleted = true;
                } else {
                    cout << "Item not deleted." << endl;
                    
                }
            } else {
                cout << "List doesn't contain that number." << endl;
            }
            sleep_millis(2000);
            cout << endl;
            return deleted;
        }

        void mark_completed(int list_num) {
            if (list_num >= 0 && list_num <= items.size()) {
                list<item>::iterator it;
                it = items.begin();
                for(int i = 0; i < list_num - 1; i++) {
                    ++it;
                }
                (*it).check_completed();
                cout << "\"" << (*it).get_name() << "\" marked complete" << endl;
                sleep_millis(2000);
                cout << endl;

        }


};

        void save_to_file(string file_name) {
            ofstream file;

            file.open(file_name);

            if(file) {
                for(auto it = items.begin(); it != items.end(); ++it) {
                    file << "|" << (*it).get_name() << "|,|" << (*it).get_description() << "|,|" << (*it).get_date() << "|,|" << (*it).get_completed() << "|" << endl;
                }
            }
        }

        void load_from_file(string file_name) {
            ifstream file;
            file.open(file_name);
            string line;
            if(file) {
                while (getline(file, line)) {
                    if (!line.empty()) {
                        items.push_back(create_item(line));
                    }
                }
            }
        }

        item create_item(string line) {
            list<string> data = {};
            bool in_quote = false;
            int start_position = 1;

            for (int i = 0; i < size(line); i++) {
                char letter = line[i];
                if (letter == '|') {
                    if (in_quote) {
                        in_quote = false;
                    } else {
                        in_quote = true;
                    }
                }
                if (letter == ',' && !in_quote) {
                    data.push_back(line.substr(start_position, i - start_position - 1));
                    start_position = i + 2;
                }
            }
            data.push_back(line.substr(start_position, size(line) - start_position - 1));
            list<string>::iterator it;
            it = data.begin();
            string name = *it;
            ++it;
            string description = *it;
            ++it;
            string date = *it;
            ++it;
            bool complete = false;
            if (*it == "1") {
                complete = true;
            }

            return item(name, description, date, complete);

        }
};

int main() {
    cout << "Welcome to the to_do_list program.\n" << endl;
    string menu[3] = {"1. Add", "2. Select", "3. Quit"};
    string file_name = "data.txt";
    bool quit = false;
    to_do program_items = to_do();
    program_items.load_from_file(file_name);

    do {
        program_items.print_items();
        sleep_millis(2000);
        for(string option : menu) {
            cout << option << endl;
        }
        cout << "Choose an option number: ";
        int user_input;
        cin >> user_input;
        cin.ignore();
        cout << endl;

        switch(user_input) {
            case 1:
                program_items.add_item();
                program_items.save_to_file(file_name);
                break;
            
            case 2:
                program_items.select_item();
                program_items.save_to_file(file_name);
                break;

            case 3:
                quit = true;
                program_items.save_to_file(file_name);
                break;

        }
    } while(!quit);

    cout << "\nThank you for using the to_do_list program!" << endl;
    sleep_millis(2000);
}