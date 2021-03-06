#pragma once

//
// SmallString is a string implementation that prioritizes small memory footprint 
// over speed of access. this project came about because I wanted a library that 
// provided a lot of strings without the mandatory minimum overhead of the 
// modern std::string (32 bytes). A lot of effort has gone into making this library 
// a drop-in replacement for std::string. Obviously, that will never always be 
// the case but a guy can dream, right? An added bonus of the architecture used here 
// is that it avoids memory fragmentation as all data used is in two contiguous memory
// chunks
//
// Due to the way the library works, it is impossible to say exactly how much memory is 
// in use at any one time. This is due, in part, to the replacement policy used when 
// finding offset table entries. However, new objects will always take the first 
// available entry that is closest to the beginning, creating a new one if none are available
//
// this library is NOT thread safe! DO NOT USE WITH MULTITHREADED CODE, USE std::string INSTEAD!!!!!!
// EDIT: you can include this library in multithreaded code but preprocessor guards should 
// include std::string instead.
//
// named requirements implemented by small_string_t:
// 1.) MoveAssignable           [true]
// 2.) CopyConstructible        [true]
// 3.) TriviallyCopyable        [false, never will be]
// 4.) EqualityComparable       [true]
// 5.) Swappable                [true]
// 6.) Container                [true, iterable]
//
// Notes:
//      Never create or modify small_string_t objects while iterating. there is a very coupled 
// relationship between ALL small_string_t objects as they all internally share a single 
// static character buffer. This buffer may re-allocate itself at any time while modifying 
// strings and so any iterators will be invalidated
//

#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <algorithm> // lexicographical_compare

// comment out for release. be advised, this thing produces a LOT of output
//#define SMALL_STRING_DEBUG

#ifdef SMALLSTRING_USING_THREADS
typedef std::string small_string_t;
#warning small_string_t is typedefed to std::string 
#else

template<typename char_type_t>
class SmallString {
private:

    typedef typename std::vector<char_type_t>::iterator small_string_iterator;

    static std::vector<char_type_t>         char_table;
    static std::vector<std::pair<int, int>> offset_table; // allows char table to be compressed when strings are destroyed

    typedef std::pair<int, int> offset_table_entry_t;

    // every additional object has an overhead of 4 bytes here 
    // plus the two ints in the offset table (pair of ints 
    // describing a subset of characters: 8 bytes) for a total 
    // of 12 bytes per object
    int table_entry; 

    // returns the index of the offset table entry whose .first value 
    // matches the argument. returns -1 if none exist, i.e. there are 
    // no more entries that logically come next
    int find_offset_table_match(int arg) {
        int sz = SmallString::offset_table.size();

        // yup. iterate through the entire thing
        for(int i = 0; i < sz; i++) {
            if(SmallString::offset_table[i].first == arg)
                return i;
        }

        return -1;
    }

    // removes characters for a specific string and moves the 
    // remaining characters to fill in the space. updates 
    // remaining offset table entries to reflect the shift
    inline void compress_around(int index) {

        #ifdef SMALL_STRING_DEBUG
        std::cout << "compressing character buffer...";
        #endif

        auto& p = offset_table.at(index);
        int sz = p.second - p.first;

        auto iter = char_table.begin();
        char_table.erase(iter + p.first, iter + p.second);

        // iterate through offset table entries that logically come 
        // after this string (not physically in the offset table)
        // this is a very long process that requires many memory 
        // accesses
        int next_entry = this->find_offset_table_match(p.second);
        while(next_entry != -1) {
            auto& p_curr = SmallString::offset_table.at(next_entry);
            next_entry = this->find_offset_table_match(p_curr.second);

            // adjust this entry
            p_curr.first -= sz;
            p_curr.second -= sz;
        }

        #ifdef SMALL_STRING_DEBUG
        std::cout << "DONE\n";
        #endif
    }

    inline int find_empty_entry(void) {
        int vsz = this->offset_table.size();
        for(int i = 0; i < vsz; i++) {
            if(offset_table[i].first == -1 && offset_table[i].second == -1)
                return i;
        }

        // no free entry. place another one and return new index
        offset_table.push_back({-1, -1});
        return offset_table.size()-1;
    }

    int compare(const SmallString& rhs) {
    }

public:

    SmallString(void) {
        #ifdef SMALL_STRING_DEBUG
        std::cout << "small_string_t constructor, void argument" << std::endl;
        #endif

        this->table_entry = -1;
    }

    SmallString(const char* c) {
        int len = strlen(c);

        #ifdef SMALL_STRING_DEBUG
        std::cout << "small_string_t constructor, const char*: " << (len ? c : "EMPTYSTRING") << std::endl;
        #endif

        if(len == 0) { // empty string
            this->table_entry = -1;

            #ifdef SMALL_STRING_DEBUG
            std::cout << "    constructor returns early for empty string\n";
            #endif

            return;
        }

        #ifdef SMALL_STRING_DEBUG
        //std::cout << "    string length: " << len << std::endl;
        #endif

        int index = this->find_empty_entry();
        auto& p = offset_table.at(index);

        p.first  = char_table.size();
        p.second = p.first + len;

        // copy the characters into the character buffer
        for(int i = 0; i < len; i++)
            this->char_table.push_back(c[i]);

        // set the index this object will use
        this->table_entry = index;
    }

    SmallString(const SmallString& s) {
        #ifdef SMALL_STRING_DEBUG
        std::cout << "small_string_t constructor, const small_string_t ref: ";
        for(char c : s)
            std::cout << c;
        std::cout << std::endl;
        #endif

        if(s.table_entry != -1) {
            int index = this->find_empty_entry();
            auto& p = offset_table.at(index);
            p.first = char_table.size();
            p.second = p.first + s.size();

            // make sure character buffer doesnt reallocate during copy
            SmallString::char_table.reserve(SmallString::char_table.size() + s.size());
            for(char c : s)
                char_table.push_back(c);

            this->table_entry = index;
        }
    }

    SmallString(const std::string& s) {
        #ifdef SMALL_STRING_DEBUG
        std::cout << "small_string_t constructor, const std::string ref: " << s << std::endl;
        #endif

        int index = this->find_empty_entry();
        auto& p = offset_table.at(index);
        p.first = char_table.size();
        p.second = p.first + s.size();

        // dont need to reserve space this time because the argument is 
        // independent of the existing buffer. add away!
        for(char c : s)
            char_table.push_back(c);

        this->table_entry = index;
    }

    ~SmallString(void) {

        #ifdef SMALL_STRING_DEBUG
        std::cout << "small_string_t destructor. index: " << this->table_entry << ", contents: " << *this << std::endl;
        SmallString::print_info(std::cout) << std::endl;
        #endif

        if(this->table_entry != -1) {
            this->compress_around(this->table_entry);
            auto& ot = SmallString::offset_table;
            auto& p  = ot.at(table_entry);
            p.first  = -1;
            p.second = -1;
        }

        this->clean();
    }

    // im debating whether this should actually be a public method. Calling 
    // it too often will harm performance to an excessive degree. it is 
    // already called automatically when a small_string_t object is destroyed
    static void clean(void) {
        auto& ot = SmallString::offset_table;
        while(ot.back().first == -1 && ot.back().second == -1)
            ot.pop_back();
    }

    struct iterator_t {
        // maintaining these pieces of information allows the char buffer 
        // to change i.e. due to other small_string objects going out of scope
        int ot_index;   // offset table entry
        int char_index; // current 0-based offset within the string

        bool operator==(iterator_t& rhs) {
            return 
                this->ot_index == rhs.ot_index &&
                this->char_index == rhs.char_index;
        }

        bool operator!=(iterator_t& rhs) {
            //return !(*this == rhs);
            return !this->operator==(rhs);
        }

        // pre-increment
        iterator_t& operator++() {
            this->char_index++;
            return *this;
        }

        // post-increment
        iterator_t operator++(int) const {
            // trivally copyable
            iterator_t iter = *this;
            iter.char_index++;
            return iter;
        }

        char_type_t& operator*(void) const {
            auto& ot = SmallString::offset_table;
            auto& ct = SmallString::char_table;

            offset_table_entry_t p = ot.at(this->ot_index);
            return ct.at(p.first + this->char_index);
        }

        // implicit user-defined type cast
        operator std::string() const {
            
        }

    };

    //typedef typename std::vector<char_type_t>::iterator small_string_iterator;
    typedef typename SmallString::iterator_t small_string_iterator_t;

    // seriously considering making a different iterator type that wont 
    // invalidate iterators if strings are modified while iterating

    //auto begin(void) const -> typename std::vector<char_type_t>::iterator {
    auto begin(void) const -> small_string_iterator_t {

        iterator_t iter;

        if(this->table_entry == -1) {
            
            iter.ot_index   = -1;
            iter.char_index = -1;
            return iter;

        }
        else {
            
            iter.ot_index   = this->table_entry;
            iter.char_index = 0;
            return iter;

        }
    }

    //auto end(void) const -> typename std::vector<char_type_t>::iterator {
    auto end(void) const -> small_string_iterator_t {

        iterator_t iter;

        if(this->table_entry == -1) {
            
            iter.ot_index   = -1;
            iter.char_index = -1;
            return iter;

            //return SmallString::char_table.end();
        }
        else {
            auto& p = SmallString::offset_table.at(this->table_entry);

            iter.ot_index   = this->table_entry;
            iter.char_index = this->size();
            return iter;

            //return SmallString::char_table.begin() + p.second;
        }
    }

    // access individual character in string. throws runtime_error if 
    // index is out of bounds
    char& at(int index) {
        if(index >= this->size() || index < 0)
            throw std::runtime_error("SmallString error: out of range using .at()");
        else
            return ((*this)[index]);
    }

    // remove all characters from string. after this operation, object has 
    // the same state as though it had been constructed with the void constructor
    void clear(void) {
        if(this->table_entry != -1) {
            this->compress_around(this->table_entry);
            auto& p = offset_table.at(this->table_entry);
            
            // allow this offset table entry to be reclaimed
            p.first = -1;
            p.second = -1;

            this->table_entry = -1;
        }
    }

    auto print_buffer(std::ostream& os) -> std::ostream& {
        auto& cv = SmallString::char_table;
        os.write(&cv[0], cv.size());
        return os;
    }

    auto print_offset_table(std::ostream& os) -> std::ostream& {
        for(auto& p : SmallString::offset_table) {
            os << "[" << p.first << ", " << p.second << "] ";
        }
        return os;
    }

    static auto print_info(std::ostream& os) -> std::ostream& {
        auto& cv = SmallString::char_table;
        auto& ot = SmallString::offset_table;

        os.write(&cv[0], cv.size());
        os << "\n";

        for(auto& p : ot)
            os << "[" << p.first << ", " << p.second << "] ";
        
        os << "\n";

        return os;
    }

    // add new character to this string. behavior can be optimized in certain cases
    void push_back(char_type_t c) {
        if(this->table_entry == -1) {
            // first character of a new string
            int index = this->find_empty_entry();
            auto& p = offset_table.at(index);
            p.first  = char_table.size();
            p.second = p.first + 1;
            char_table.push_back(c);
            this->table_entry = index;
        }
        else {
            auto& p = offset_table.at(table_entry);
            if(p.second == char_table.size()) {
                // just add to the end of the char buffer and update the 
                // offset table entry. this is the most ideal situation
                p.second++;
                char_table.push_back(c);
            }
            else {
                // relocate everything
                int new_index = this->find_empty_entry();
                auto& p_new = offset_table.at(new_index);

                p_new.first  = char_table.size();
                p_new.second = p_new.first + this->size() + 1;

                // ensure reallocation doesnt happen when we are copying
                char_table.reserve(p_new.second);
                for(char cc : *this)
                    char_table.push_back(cc);
                char_table.push_back(c); // add new character

                // remove the characters of the old string
                this->compress_around(this->table_entry);

                // reset the old offset table entry so it can be reclaimed
                auto& p_curr = offset_table.at(this->table_entry);
                p_curr.first  = -1;
                p_curr.second = -1;
            
                this->table_entry = new_index;
            }
        }
    }

    // access individual characters in a string. does not perform bounds 
    // checking. accessing chars outside of the strings bounds is 
    // undefined behavior
    char& operator[](int index) const {
        auto& cv = SmallString::char_table;
        auto& ot = SmallString::offset_table;
        auto& p = ot.at(this->table_entry);
        return *(cv.begin() + p.first + index);
    }

    int size(void) const {
        auto& p = offset_table.at(this->table_entry);
        return p.second - p.first;
    }

    // explicit std::string cast
    operator std::string() {
        if(this->table_entry == -1) {
            return std::string{""}; // empty string
        }
        else {
            auto& p = SmallString::offset_table.at(this->table_entry);
            auto& cv = SmallString::char_table;
            return std::string{ cv.begin() + p.first, cv.begin() + p.second };
        }
    }

    // equality operators

    bool operator==(SmallString& ss) const {
        
        #ifdef SMALL_STRING_DEBUG
        std::cout << "small_string_t == lvalue ref\n";
        #endif

        if(&ss == this) return true; // same object is always equivalent

        if(this->table_entry == -1) {
            // this object contains an empty string
            return (ss.table_entry == -1);
        }
        else {
            if(ss.table_entry == -1) {
                return false;
            }
            else {
                int sz = this->size();

                if(ss.size() != sz) return false;

                // compare each character in each string one-by-one
                for(int i = 0; i < sz; i++)
                    if(ss[i] != ((*this)[i])) return false;

                return true;
            }
        }
    }

    bool operator==(const char* c) const {
        int c_sz = strlen(c);

        if(this->table_entry == -1) {
            // this object is an empty string
            return (c_sz == 0);
        }
        else {
            int sz = this->size();
            if(c_sz != sz) 
                return false;
            else {
                for(int i = 0; i < sz; i++)
                    if(c[i] != ((*this)[i])) 
                        return false;
                return true;
            }
        }
    }

    bool operator==(SmallString&& ss) const {
        
        #ifdef SMALL_STRING_DEBUG
        std::cout << "small_string_t == rvalue ref\n";
        #endif

        // if the argument is an rvalue, this will NEVER be true
        //if(&ss == this) return true; // same object is always equivalent

        if(this->table_entry == -1) {
            // this object contains an empty string
            return (ss.table_entry == -1);
        }
        else {
            if(ss.table_entry == -1) {
                return false;
            }
            else {
                int sz = this->size();

                if(ss.size() != sz) return false;

                // compare each character in each string one-by-one
                for(int i = 0; i < sz; i++)
                    if(ss[i] != ((*this)[i])) return false;

                return true;
            }
        }
    }

    // inequality operators

    bool operator!=(SmallString& ss) const {
        return !(*this == ss);
    }

    bool operator!=(const char* c) const {
        return !(*this == c);
    }

    bool operator!=(SmallString&& ss) const {
        return !(*this == ss);
    }

    // ========================================================================
    // redirect operators for outputting to std::ostream's
    // ========================================================================

    friend std::ostream& operator<<(std::ostream& os, const SmallString& ss) {
        if(ss.table_entry != -1) {
            auto& cv = SmallString::char_table;
            auto& ot = SmallString::offset_table;
            auto& p = ot.at(ss.table_entry);

            // write the correct subset of chars to os
            os.write(&cv[0] + p.first, ss.size());
        }

        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const SmallString&& ss) {
        if(ss.table_entry != -1) {
            auto& p = SmallString::offset_table.at(ss.table_entry);
            os.write(&SmallString::char_table[0] + p.first, ss.size());
        }

        return os;
    }

    // ========================================================================
    // assignment operators
    // ========================================================================

    SmallString& operator=(SmallString const& rhs) {

        #ifdef SMALL_STRING_DEBUG
        std::cout << "assignment operator, small_string_t ref\n";
        #endif

        // only do operations if you have to
        if(&rhs != this) {            

            int new_index = this->find_empty_entry();
            auto& p = SmallString::offset_table.at(new_index);

            p.first = SmallString::char_table.size();
            p.second = p.first + rhs.size();

            for(auto c : rhs)
                SmallString::char_table.push_back(c);

            if(this->table_entry != -1) {
                this->compress_around(this->table_entry);
                auto& p_curr = SmallString::offset_table.at(this->table_entry);
                p_curr.first  = -1;
                p_curr.second = -1;
            }

            this->table_entry = new_index;
        }

        return *this;
    }

    SmallString& operator=(const char* c) {

        #ifdef SMALL_STRING_DEBUG
        std::cout << "assignment operator, const char*\n";
        #endif
        
        int sz = strlen(c);

        int new_index = this->find_empty_entry();
        auto& p = SmallString::offset_table.at(new_index);

        p.first = SmallString::char_table.size();
        p.second = p.first + sz;

        for(int i = 0; i < sz; i++)
            SmallString::char_table.push_back(c[i]);

        if(this->table_entry != -1) {
            this->compress_around(this->table_entry);
            auto& p_curr = SmallString::offset_table.at(this->table_entry);
            p_curr.first  = -1;
            p_curr.second = -1;
        }

        this->table_entry = new_index;

        return *this;
    }

    // ========================================================================
    // + lvalue ref is on left hand side
    // ========================================================================

    friend SmallString operator+(SmallString& lhs, const char* rhs) {

        #ifdef SMALL_STRING_DEBUG
        std::cout << "lvalue ref + const char*\n";
        #endif

        SmallString new_string;
        int new_index = lhs.find_empty_entry();
        int sz = strlen(rhs);
        auto& p = lhs.offset_table.at(new_index);
        p.first = lhs.char_table.size();
        p.second = p.first + lhs.size() + sz;

        // ====================================================================
        // place contents of both strings in the char buffer
        // ====================================================================

        // make sure char buffer wont reallocate while copying
        lhs.char_table.reserve(lhs.char_table.size() + lhs.size() + sz);
        for(char c : lhs) lhs.char_table.push_back(c);
        for(int i = 0; i < sz; i++) lhs.char_table.push_back(rhs[i]);

        new_string.table_entry = new_index;
        return new_string;
    }

    friend SmallString operator+(SmallString& lhs, const SmallString& rhs) {

        #ifdef SMALL_STRING_DEBUG
        std::cout << "lvalue ref + lvalue ref\n";
        #endif

        SmallString new_string;
        int new_index = lhs.find_empty_entry();
        auto& p = lhs.offset_table.at(new_index);
        p.first = lhs.char_table.size();
        p.second = p.first + lhs.size() + rhs.size();

        // append chars to end of char buffer
        lhs.char_table.reserve(lhs.char_table.size() + lhs.size() + rhs.size());
        for(char c : lhs) lhs.char_table.push_back(c);
        for(char c : rhs) lhs.char_table.push_back(c);

        new_string.table_entry = new_index;
        return new_string;
    }

    friend SmallString operator+(SmallString& lhs, SmallString&& rhs) {

        #ifdef SMALL_STRING_DEBUG
        std::cout << "lvalue ref + rvalue ref\n";
        #endif

        SmallString new_string;
        int new_index = lhs.find_empty_entry();
        auto& p = lhs.offset_table.at(new_index);
        p.first = lhs.char_table.size();
        p.second = p.first + lhs.size() + rhs.size();

        // append chars to end of char buffer
        lhs.char_table.reserve(lhs.char_table.size() + lhs.size() + rhs.size());
        for(char c : lhs) lhs.char_table.push_back(c);
        for(char c : rhs) lhs.char_table.push_back(c);

        new_string.table_entry = new_index;
        return new_string;
    }

    friend SmallString operator+(SmallString& lhs, std::string rhs) {
        
        #ifdef SMALL_STRING_DEBUG
        std::cout << "lvalue ref + std::string\n";
        #endif

        SmallString new_string;
        int new_index = lhs.find_empty_entry();
        auto& p = lhs.offset_table.at(new_index);
        p.first = lhs.char_table.size();
        p.second = p.first + lhs.size() + rhs.size();

        // append chars to end of char buffer
        lhs.char_table.reserve(lhs.char_table.size() + lhs.size() + rhs.size());
        for(char c : lhs) lhs.char_table.push_back(c);
        for(char c : rhs) lhs.char_table.push_back(c);

        new_string.table_entry = new_index;
        return new_string;

    }

    // ========================================================================
    // + rvalue ref is on left hand side
    // ========================================================================

    friend SmallString operator+(SmallString&& lhs, const char* rhs) {

        #ifdef SMALL_STRING_DEBUG
        std::cout << "rvalue ref + const char*\n";
        #endif

        SmallString new_string;
        int new_index = lhs.find_empty_entry();
        int sz = strlen(rhs);
        auto& p = lhs.offset_table.at(new_index);
        p.first = lhs.char_table.size();
        p.second = p.first + lhs.size() + sz;

        // make sure char buffer wont reallocate while copying
        lhs.char_table.reserve(lhs.char_table.size() + lhs.size() + sz);
        for(char c : lhs) lhs.char_table.push_back(c);
        for(int i = 0; i < sz; i++) lhs.char_table.push_back(rhs[i]);

        new_string.table_entry = new_index;
        return new_string;
    }

    friend SmallString operator+(SmallString&& lhs, SmallString const& rhs) {

        #ifdef SMALL_STRING_DEBUG
        std::cout << "rvalue ref + lvalue ref\n";
        #endif

        SmallString new_string;
        int new_index = lhs.find_empty_entry();
        auto& p = lhs.offset_table.at(new_index);
        p.first = lhs.char_table.size();
        p.second = p.first + lhs.size() + rhs.size();

        // append chars to end of char buffer
        lhs.char_table.reserve(lhs.char_table.size() + lhs.size() + rhs.size());
        for(char c : lhs) lhs.char_table.push_back(c);
        for(char c : rhs) lhs.char_table.push_back(c);

        new_string.table_entry = new_index;
        return new_string;
    }

    friend SmallString operator+(SmallString&& lhs, SmallString&& rhs) {
        
        #ifdef SMALL_STRING_DEBUG
        std::cout << "rvalue ref + rvalue ref\n";
        #endif

        SmallString new_string;
        int new_index = lhs.find_empty_entry();
        auto& p = lhs.offset_table.at(new_index);
        p.first = lhs.char_table.size();
        p.second = p.first + lhs.size() + rhs.size();

        // append chars to end of char buffer
        lhs.char_table.reserve(lhs.char_table.size() + lhs.size() + rhs.size());
        for(char c : lhs) lhs.char_table.push_back(c);
        for(char c : rhs) lhs.char_table.push_back(c);

        new_string.table_entry = new_index;
        return new_string;
    }

    friend SmallString operator+(SmallString&& lhs, std::string rhs) {

        #ifdef SMALL_STRING_DEBUG
        std::cout << "rvalue ref + std::string\n";
        #endif

        SmallString new_string;
        int new_index = lhs.find_empty_entry();
        auto& p = lhs.offset_table.at(new_index);
        p.first = lhs.char_table.size();
        p.second = p.first + lhs.size() + rhs.size();

        // append chars to end of char buffer
        lhs.char_table.reserve(lhs.char_table.size() + lhs.size() + rhs.size());
        for(char c : lhs) lhs.char_table.push_back(c);
        for(char c : rhs) lhs.char_table.push_back(c);

        new_string.table_entry = new_index;
        return new_string;
    }

    // ========================================================================
    // += lvalue ref is on left hand side
    // ========================================================================

    friend SmallString& operator+=(SmallString& lhs, const char* rhs) {
        auto tmp = lhs + rhs;
        lhs = tmp;
        return lhs;
    }

    friend SmallString& operator+=(SmallString& lhs, SmallString&& rhs) {
        lhs = lhs + rhs;
        return lhs;
    }

    // ========================================================================
    // lexicographical comparison operators (except == which is defined above)
    // ========================================================================
    
    friend bool operator<(SmallString lhs, SmallString rhs) {

        #ifdef SMALL_STRING_DEBUG
        std::cout << "small_string_t compare, <\n";
        #endif

        // what is the standard behavious if one of the strings is empty?
        if(lhs.table_entry == -1 || rhs.table_entry == -1)
            return false;

        auto& ot = SmallString::offset_table;
        auto& cv = SmallString::char_table;
        
        auto& p_lhs = ot.at(lhs.table_entry);
        auto& p_rhs = ot.at(rhs.table_entry);

        auto vbegin = cv.begin();

        return std::lexicographical_compare<small_string_iterator, small_string_iterator>(
            vbegin + p_lhs.first, vbegin + p_lhs.second,
            vbegin + p_rhs.first, vbegin + p_rhs.second
        );

    }

    friend bool operator>=(SmallString lhs, SmallString rhs) {

        #ifdef SMALL_STRING_DEBUG
        std::cout << "small_string_t compare, >=\n";
        #endif

        return !(lhs < rhs);
    }

    friend bool operator>(SmallString lhs, SmallString rhs) {
        return (lhs >= rhs) && !(lhs == rhs);
    }

    friend bool operator<=(SmallString lhs, SmallString rhs) {
        return !(lhs > rhs);
    }

};

// i give you intialization of static members in template classes... in c++
template<typename char_type_t> typename std::vector<char_type_t> SmallString<char_type_t>::char_table;
template<typename char_type_t> std::vector<std::pair<int, int>>  SmallString<char_type_t>::offset_table;

typedef SmallString<char> small_string_t;
#warning small_string_t is typedefed to SmallString<char>

#endif

