//#include "bst.h"
#include <iostream>
#include "movie.cpp"
#include "moviestore.cpp"
//#include "title_comparator.h"


// Output is in main_tree.out


class BalanceCondition
{
public:
    bool operator() (int current_height, int ideal_height) const
    {
        if (current_height > 2 * ideal_height)
            return false;
        return true;
    }
};

class DelicateBalance
{
public:
    bool operator() (int current_height, int ideal_height) const
    {
        return true;
    }
};


class AdditionalBalance
{
public:
    bool operator() (int current_height, int ideal_height) const
    {
        if (current_height > ideal_height)
        {
            return false;
        }
        return true;
    }
};


int main(void)
{
/*
    std::list<std::pair<int, float> > datalist;
    BinarySearchTree<int, float, DelicateBalance>lst;
    lst.insert(8,8);
    lst.insert(4,4);
    lst.insert(15,15);
    lst.insert(7,7);
    lst.insert(10,10);
    lst.insert(20,20);
    lst.insert(9,9);
    lst.insert(18,18);
    lst.insert(16,16);
    lst.insert(17,17);
    lst.insert(22,22);
    lst.print(cout);

    std::cout << "Removing 26" << std::endl;
    lst.remove(26);
    lst.print(std::cout);
    std::cout << std::endl;

    std::cout << "Removing 9" << std::endl;
    lst.remove(9);
    lst.print(std::cout);
    std::cout << std::endl;

    std::cout << "Removing 22" << std::endl;
    lst.remove(22);
    lst.print(std::cout);
    std::cout << std::endl;

    std::cout << "Removing 15" << std::endl;
    lst.remove(15);
    lst.print(std::cout);
    std::cout << std::endl;

    std::cout << "Removing 4" << std::endl;
    lst.remove(4);
    lst.print(std::cout);
    std::cout << std::endl;

    std::cout << "Removing 20" << std::endl;
    lst.remove(20);
    lst.print(std::cout);
    std::cout << std::endl;

    std::cout << "Removing 17" << std::endl;
    lst.remove(17);
    lst.print(std::cout);
    std::cout << std::endl;

    std::cout << "Removing 8" << std::endl;
    lst.remove(8);
    lst.print(std::cout);
    std::cout << std::endl;
    */

    BinarySearchTree<int, float, DelicateBalance>lst2;

    MovieStore myStore;

    myStore.insert(Movie("0120737", "The Lord of the Rings: The Fellowship of the Ring", "Peter Jackson",
                         "X Movies", 2001, 150));
    myStore.insert(Movie("1170358", "the hobbit: The Desolation of Smaug", "peter JACKSON",
                         "Y Movies", 2013, 160));
    myStore.insert(Movie("0103873", "Dead Alive", "Peter JACKSON",
                         "X Movies", 1994, 104));
    myStore.insert(Movie("0360717", "King Kong", "pEteR JackSon",
                         "Y Movies", 2005, 102));
    myStore.insert(Movie("0074751", "King Kong", "John Guillermin",
                         "X Classics", 1976, 136));
    myStore.insert(Movie("7905466", "They Shall Not Grow Old", "Peter Jackson",
                         "X Movies", 2018, 245));
    myStore.insert(Movie("0172495", "Gladiator", "Ridley Scott",
                         "Z Movies", 2000, 136));
    myStore.insert(Movie("0104346", "Gladiator", "Rowdy Herrington",
                         "O\'Reilly Movies", 1992, 104));
    myStore.updateCompany("Peter JACKSOn","Company X");
    cout << endl;
    //myStore.remove("0074751");
    //myStore.remove("Gladiator","Rowdy Herrington");
    //myStore.removeAllMoviesWithTitle("gladiator");
    myStore.printMoviesOfDirector("peter jackson","a","");
    myStore.printPrimarySorted();

    return 0;
}
