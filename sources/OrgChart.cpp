#include "OrgChart.hpp"
using namespace ariel;

namespace ariel
{

    OrgChart::~OrgChart()
    {
        if (root_exist)
        {
            queue<Person *> temp;
            queue<Person *> Q;

            temp.push(root);
            while (!temp.empty())
            {
                Q.push(temp.front());
                for (Person *son : temp.front()->get_sub_workers())
                {
                    temp.push(son);
                }
                temp.pop();
            }

            while (!Q.empty())
            {
                Person *curr = Q.front();
                Q.pop();
                delete curr;
            }
        }
    };

    OrgChart &OrgChart::add_root(string const &name)
    {
        if (root_exist)
        {
            root->set_name(name);
        }
        else
        {
            root = new Person(name);
            root_exist = true;
        }
        return *this;
    };
    
    OrgChart &OrgChart::add_sub(string const &father, string const &son)
    {
        if (root_exist)
        {
            Person *new_father = find(father);
            new_father->add_sub_worker(new Person(son));
        }

        else{
            throw runtime_error("root does not exists yet");
        }
        return *this;
    };
    
    OrgChart::Person *OrgChart::find(string const &name)
    {
        queue<Person *> helper_Q;
        helper_Q.push(root);

        while (!helper_Q.empty())
        {
            if (helper_Q.front()->get_name() == name)
            {
                return helper_Q.front();
            }

            for (Person *node : helper_Q.front()->get_sub_workers())
            {
                helper_Q.push(node);
            }

            helper_Q.pop();
        }

        throw runtime_error("person does not exist");
    }


    ostream &operator<<(ostream &out, const OrgChart &chart)
    {
        if (chart.root_exist)
        {
            out << endl;
            out << chart.root->get_name() << endl;
            chart.root->print_rest(out, "");
            out << endl;
            return out;
        }

        out << "OrgChart is empty." << endl;
        return out;
    };

    OrgChart::Iterator OrgChart::begin_level_order()
    {
        if (root_exist)
        {
            return Iterator(root, order::level);
        }
        throw logic_error("Cannot init iterator from empty chart");
    };

    OrgChart::Iterator OrgChart::end_level_order() const
    {
        if (root_exist)
        {
            return Iterator();
            
        }
        throw logic_error("Cannot init iterator from empty chart");
    };

    OrgChart::Iterator OrgChart::begin_reverse_order()
    {
        if (root_exist)
        {
            return Iterator(root, order::reverse);
        }
        throw logic_error("Cannot init iterator from empty chart");
    };

    OrgChart::Iterator OrgChart::reverse_order() const
    {
        if (root_exist)
        {
            return Iterator();
        }
        throw logic_error("Cannot init iterator from empty chart");
    };

    OrgChart::Iterator OrgChart::begin_preorder()
    {
        if (root_exist){
            return Iterator(root, order::preorder);
        }
        throw logic_error("Cannot init iterator from empty chart");
    };

    OrgChart::Iterator OrgChart::end_preorder() const
    {
        if (root_exist){
            return Iterator();
        }
        throw logic_error("Cannot init iterator from empty chart");
    };

    void OrgChart::Iterator::preorder_case(Person *root)
    {
        Q.push(root->get_name());

        for (Person *n : root->get_sub_workers())
        {
            preorder_case(n);
        }
    };

    void OrgChart::Iterator::level_case()
    {
        queue<Person *> temp;
        temp.push(curr_person);
        while (!temp.empty())
        {
            Q.push(temp.front()->get_name());
            for (Person *son : temp.front()->get_sub_workers())
            {
                temp.push(son);
            }
            temp.pop();
        }
    }

    void OrgChart::Iterator::reverse_case(){
        queue<Person *> temp;
        stack<string> names;
        stack<Person *> revrese_queue;
        temp.push(curr_person);

        while (!temp.empty())
        {
            names.push(temp.front()->get_name());
            for (Person *nod : temp.front()->get_sub_workers())
            {
                revrese_queue.push(nod);
            }

            while (!revrese_queue.empty())
            {
                temp.push(revrese_queue.top());
                revrese_queue.pop();
            }

            temp.pop();
        }

        while (!names.empty())
        {
            Q.push(names.top());
            names.pop();
        }
    }

    void OrgChart::Iterator::make_Q(order what_order)
    {
        if (what_order == order::level)
        {
            level_case();
        }

        else if (what_order == order::reverse)
        {
            reverse_case();
        }
        else if (what_order == order::preorder)
        {
            preorder_case(curr_person);
        }
    }

    OrgChart::Iterator::Iterator(Person *root, order what_order)
    {
        curr_person = root;
        make_Q(what_order);
    };

    bool OrgChart::Iterator::operator!=(const OrgChart::Iterator &it)
    {
        return (!this->Q.empty());
    };
    
    OrgChart::Iterator &OrgChart::Iterator::operator++()
    {
        if (!Q.empty())
        {
            Q.pop();
        }
        return *this;
    };
    
    string *OrgChart::Iterator::operator->() { return &(Q.front()); };

    string &OrgChart::Iterator::operator*() { return Q.front(); };

    
    OrgChart::Person::Person(string name) : name(move(name)){};

    vector<OrgChart::Person *> &OrgChart::Person::get_sub_workers() { return sub_workers; };

    void OrgChart::Person::add_sub_worker(Person *person) { this->sub_workers.push_back(person); };

    void OrgChart::Person::set_name(string name) { this->name = move(name); };

    string &OrgChart::Person::get_name()
    {
        return this->name;
    };

    void OrgChart::Person::print_rest(ostream &out, const string &reamaning)
    {
        using std::cout;
        using std::endl;
        if (this->sub_workers.empty())
        {
            return;
        }
        out << reamaning;
        size_t n_children = sub_workers.size();

        if (n_children > 1)
        {
            out << "├── ";
        }

        for (size_t i = 0; i < n_children; ++i)
        {
            Person *c = sub_workers[i];
            if (i < n_children - 1)
            {
                if (i > 0)
                {
                    out << reamaning << "├── ";
                }
                bool printStrand = n_children > 1 && !c->sub_workers.empty();
                std::string newPrefix = reamaning + (printStrand ? "│" + string(c->name.size() + 1, ' ') : string(c->name.size() + 2, ' '));
                out << c->name << "\n";
                c->print_rest(out, newPrefix);
            }
            else
            {
                cout << (n_children > 1 ? reamaning : "") << "└── ";
                std::cout << c->name << "\n";
                c->print_rest(out, reamaning + string(c->name.size() + 2, ' '));
            }
        }
    }

};
