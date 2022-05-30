#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <queue>
#include <stack>

using namespace std;

namespace ariel
{
    enum order
    {
        level,
        preorder,
        reverse
    };

    class OrgChart
    {
        class Person
        {
            string name;
            vector<Person *> sub_workers;

        public:
            Person(){};
            Person(string name);
            
            void add_sub_worker(Person *person);
            string &get_name();
            void set_name(string name);
            vector<Person *> &get_sub_workers();
            void print_rest(ostream &out, const string &reamaning);
            friend ostream &operator<<(ostream &out, Person &person)
            {
                out << person.name;
                person.print_rest(out, " ");
                return out;
            };
        };
        Person *root;
        bool root_exist;
        Person *find(string const &name);

    public:
        class Iterator
        {
            queue<string> Q;
            Person *curr_person;

            void make_Q(order what_order);
            void preorder_case(Person *root);
            void level_case();
            void reverse_case();

        public:
            Iterator() : curr_person(nullptr){};
            Iterator(Person *root, order what_order);
            string &operator*();
            bool operator!=(const Iterator &it);
            Iterator &operator++();
            string *operator->();
        };

        
        OrgChart() : root_exist(false), root(nullptr){};
        ~OrgChart();


        OrgChart &add_root(string const &name);
        OrgChart &add_sub(string const &father, string const &son);
        friend ostream &operator<<(ostream &out, const OrgChart &chart);

        Iterator begin() { return begin_level_order(); };
        Iterator end() { return end_level_order(); };
        Iterator begin_level_order();
        Iterator end_level_order() const;
        Iterator begin_reverse_order();
        Iterator reverse_order() const;
        Iterator begin_preorder();
        Iterator end_preorder() const;

    };
};