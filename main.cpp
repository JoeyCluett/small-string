#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <assert.h>

//#define SMALLSTRING_USING_THREADS
#include "smallstring.h"

using namespace std;

int main(int argc, char* argv[]) {

#ifndef SMALLSTRING_USING_THREADS
    auto ss0 = new small_string_t("Hello World");
    small_string_t::print_info(cout) << "\n\n";

    auto ss1 = new small_string_t("Tomorrow");
    small_string_t::print_info(cout) << "\n\n";

    cout << "Deleting 'Hello World'\n";
    delete ss0;
    small_string_t::print_info(cout) << "\n\n";

    ss0 = new small_string_t("An african swallow");
    small_string_t::print_info(cout) << "\n\n";

    auto ss2 = new small_string_t("COMMUNICATE");
    small_string_t::print_info(cout) << "\n\n";

    cout << "auto for loop:\n";
    for(auto& c : *ss0)
        cout << c << endl;

    cout << "COMMUNICATE = An african swallow\n\n";

    for(int i = 0; i < 5; i++) {
        *ss2 = *ss0;
        small_string_t::print_info(cout) << "\n\n";
    }

    delete ss2;
    small_string_t::print_info(cout) << "\n\n";

    ss2 = new small_string_t;
    *ss2 = "Yeet cannon";
    small_string_t::print_info(cout) << "\n\n";

    cout << ((*ss2) + " hero") << endl;
    small_string_t::print_info(cout) << "\n\n";

    cout << "deleting tomorrow\n";
    delete ss1;
    small_string_t::print_info(cout) << "\n\n";

    cout << "deleting remaining strings\n";
    delete ss0;
    delete ss2;
    small_string_t::print_info(cout) << "\n\n";

    // a bunch of add/assign operations
    {
        small_string_t s = small_string_t("Hello ") + "World" + to_string(125);
        cout << "Newly constructed string: '" << s << "'\n";
        small_string_t::print_info(cout) << "\n\n";
    }

    {
        // a bunch of strings again
        small_string_t 
            a = "Appliance", 
            b = "Gorgeous", 
            c = "Impress",
            d = "Respect",
            e = "Hideous",
            f = "Popcorn";

        small_string_t::print_info(cout) << "\n\n";
    
        c = "Delirious";
        small_string_t::print_info(cout) << "\n\n";
    }

    cout << "SHOULD BE EMPTY NOW...\n" << flush;
    small_string_t::print_info(cout) << "\n\n";

    cout << "creating a std::map\n";
    // test using small_string_t as a std::map key
    {
        std::map<small_string_t, small_string_t> string_map;
        string_map.insert({ "Kappliance", "Vcherries" });
        string_map.insert({ "Kgorgeous",  "Vfinger" });
        string_map.insert({ "Kimpress",   "Vcomplete" });
        string_map.insert({ "Khideous",   "Vabject" });
    
        cout << "iterating through list and printing\n";
        for(auto& p : string_map) {
            cout << p.first << " -> " << p.second << endl;
        }
    }

    cout << "SHOULD BE EMPTY NOW...\n";
    small_string_t::print_info(cout) << "\n\n";

    //cout << "cleaning...\n";
    //small_string_t::clean();
    //cout << "should be zero entries now\n";
    //small_string_t::print_info(cout) << "\n\n";

     {
        // need these again
        small_string_t 
            a = "Appliance", 
            b = "Gorgeous", 
            c = "Impress",
            d = "Respect",
            e = "Hideous",
            f = "Popcorn";

        small_string_t::print_info(cout) << "\n\n";
    
        c = c + c + c;
        cout << "Triple copy of string: " << c << endl;
        small_string_t::print_info(cout) << "\n\n";
    }

    auto returns_small_string = [](void) -> small_string_t {
        return "Cool new string";
    };

    cout << "Returns small_string_t: '" << returns_small_string() << "'\n";

    {
        cout << "Testing with std::swap...\n";
        small_string_t a = "Astring", b = "Bstring";
        small_string_t::print_info(cout) << "\n";
        std::swap(a, b);
        small_string_t::print_info(cout) << "\n";
        cout << "Swapping again...\n";
        std::swap(a, b);
        small_string_t::print_info(cout) << "\n";
        cout << "Swapping a third time...\n";
        std::swap(a, b);
        small_string_t::print_info(cout) << "\n";
    }

   

#else
    
    cout << ":)\n";

#endif
    return 0;
}

