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

void EXPECT_EQ(size_t a, size_t b) {
    std::cout << "equal: " << a << " " << b << std::endl;
}

void stress_test() {
    const vector<int> sizemap =
            {
                    11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717, 51437, 102877, 205759
            };
    HashTable<int, int, LinearProber<int>, std::hash<int>, equal_to<int>> ht(1.0, LinearProber<int>());
    set<pair<int, int>> items;
    int prevsize;
    for(size_t i = 0;i<sizemap.size()-1;i++){
        if(i == 0){prevsize = 0;}
        else{prevsize = sizemap[i-1]+1;}
        for(int j = prevsize; j<=sizemap[i];j++){
            pair<int,int> pair(j,j);
            items.insert(pair);
            ht.insert(pair);
            if(j == sizemap[i]-1){
                EXPECT_EQ(ht.full_table_size(),sizemap[i]);
                EXPECT_EQ(ht.size(),items.size());
            }
            if(j == sizemap[i]){
                //resize should happen.
                EXPECT_EQ(ht.full_table_size(),sizemap[i+1]);
                EXPECT_EQ(ht.size(),items.size());
            }
        }
    }
    //All items should still be there
//    EXPECT_TRUE(verifyItems(ht, items));
}

void insert_resize() {
    //Reach the default alpha factor of .4 (5 items /11 items = .45) to force a resize
    HashTable<string, int, DoubleHashProber<string, std::hash<string>>, hash<string>, equal_to<string> > ht;
    set<pair<string, int>> items;
    for(int i = 0; i < 5; i++) {
    pair<string, int> newItem(to_string(i), i);
    ht.insert(newItem);
    items.insert(newItem);
    }
    EXPECT_EQ(ht.full_table_size(), 11);
    std::cout << ht.load_factor() << std::endl;
    std::cout << "----" << std::endl;

    //add another item should resize
    pair<string,int> newItem(to_string(5),5);
    ht.insert(newItem);
    items.insert(newItem);
    EXPECT_EQ(ht.full_table_size(), 23);
    //check that all the items are still there
    ht.reportAll(std::cout);
//    EXPECT_TRUE(verifyItems(ht, items));
    std::cout << ht.load_factor() << std::endl;
    std::cout << "----" << std::endl;

}

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
//    insert_duplicate();
//    insert_resize();
    stress_test();
    return 0;
}
