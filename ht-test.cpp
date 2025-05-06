#include "ht.h"
#include "hash.h"
#include <unordered_map>
#include <iostream>
#include <utility>
#include <string>
#include <sstream>
#include <functional>
#include <set>
using namespace std;

void insert_duplicate() {
    HashTable<string, int, LinearProber<string>, hash<string>, equal_to<string> > ht;
    std::set<pair<string, int>> items;
    //Insert (one, 1)
    pair<string, int> pair1("one", 1);
    ht.insert(pair1);
    items.insert(pair1);
    ht.reportAll(std::cout);
    std::cout << "----" << std::endl;
//    EXPECT_TRUE(verifyItems(ht, items));
    //Insert (two, 2)
    pair<string, int> pair2("two", 2);
    ht.insert(pair2);
    items.insert(pair2);
    ht.reportAll(std::cout);
    std::cout << "----" << std::endl;

//    EXPECT_TRUE(verifyItems(ht, items));
    pair<string, int> pair1dup("one", 3);
    ht.insert(pair1dup);
    items.erase(pair1);
    items.insert(pair1dup);
    ht.reportAll(std::cout);
    std::cout << "----" << std::endl;
    
//    EXPECT_TRUE(verifyItems(ht, items));
}

void logic_error_test() {
    std::cout << "working" << std::endl;
    HashTable<int, int, LinearProber<int>, std::hash<int>, equal_to<int>> ht(2.0, LinearProber<int>());
    bool correctThrow = false;
    for(int i = 0; i<=11; i++){
        if (i < 11){
            pair<int,int> pair(i,i);
            ht.insert(pair);
        }
        else{
            //collision will occur at i == 11 (11 mod 11 = 0)
            try{
                pair<int,int> pair(i,i);
                ht.insert(pair);
            }
            catch(std::logic_error const & err){
                correctThrow = true;
            }
            catch(...) {
                correctThrow = false;
            }
        }
    }
    std::cout << "throw true: " << correctThrow << std::endl;
    std::cout << "size: " << ht.size() << std::endl;
}

void test0() {
    DoubleHashProber<std::string, MyStringHash > dh;
    LinearProber<std::string > lh;
    HashTable<
            std::string,
            int,
            DoubleHashProber<std::string, MyStringHash >,
            std::hash<std::string>,
            std::equal_to<std::string> > ht(0.7, dh);
//    HashTable<
//        std::string,
//        int,
//        LinearProber<std::string >,
//        std::hash<std::string>,
//        std::equal_to<std::string> > ht(0.7, lh);

    // This is just arbitrary code. Change it to test whatever you like about your
    // hash table implementation.
    for(size_t i = 0; i < 10; i++){
        std::stringstream ss;
        ss << "hi" << i;
        ht.insert({ss.str(), i});
    }
//    ht.reportAll(std::cout);
    if( ht.find("hi1") != nullptr ){
        cout << "Found hi1" << endl;
        ht["hi1"] += 1;
        cout << "Incremented hi1's value to: " << ht["hi1"] << endl;
    }
//    return 0;
    if( ht.find("doesnotexist") == nullptr ){
        cout << "Did not find: doesnotexist" << endl;
    }
    cout << "HT size: " << ht.size() << endl;
    ht.remove("hi7");
    ht.remove("hi9");
    cout << "HT size: " << ht.size() << endl;
    if( ht.find("hi9") != nullptr ){
        cout << "Found hi9" << endl;
    }
    else {
        cout << "Did not find hi9" << endl;
    }
    ht.insert({"hi7",17});
    cout << "size: " << ht.size() << endl;
}


int main()
{
//    logic_error_test();
    insert_duplicate();
    return 0;
}
