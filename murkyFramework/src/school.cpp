#include <string>
#include <iostream>
#include <utility>
#include "debugUtils.hpp"
#include <vector>

struct thingy {
    std::string s;
    thingy() : s("test") {}
    thingy(const thingy& o) : s(o.s) { std::cout << "move failed!\n"; }
    thingy(thingy&& o) : s(std::move(o.s)) {}
    thingy& operator=(const thingy& other) {
        s = other.s;
        std::cout << "copy assigned\n";
        return *this;
    }
    thingy& operator=(thingy&& other) {
        s = std::move(other.s);
        std::cout << "move assigned\n";
        return *this;
    }
};

thingy f(thingy a) { return a; }

struct B : thingy {
    std::string s2;
    int n;
    // implicit move assignment operator B& B::operator=(B&&)
    // calls A's move assignment operator
    // calls s2's move assignment operator
    // and makes a bitwise copy of n
};

struct C : B {
    ~C() {} // destructor prevents implicit move assignment
};

struct D : B {
    D() {}
    ~D() {} // destructor would prevent implicit move assignment
    //D& operator=(D&&) = default; // force a move assignment anyway 
};

class Cont
{
public:
    static int idEntCtr;
    int idEnt;

    //cont() = delete;

    Cont()
    {
        idEnt = idEntCtr;
        debugLog << L"default constructeder " << idEnt << "\n";

        ++idEntCtr;
    }

    // copy constructor
    Cont(const Cont &other)
    {
        idEnt = idEntCtr;
        debugLog << L"copy cons " << other.idEnt << L"to " << idEnt << "\n";

        ++idEntCtr;
    }

    //  assingment operator
    Cont& operator=(const Cont& rhs)
    {
        a = rhs.a;
        debugLog << L"assignment " << idEnt << L" = " << rhs.idEnt << "\n";

        return *this;
    }

    Cont(int j) : a(a)
    {
        idEnt = idEntCtr;
        debugLog << L"constructed with parameter" << idEnt << "\n";

        ++idEntCtr;
    }

    // move constructor
    Cont(Cont &&other)
    {
        idEnt = idEntCtr;
        debugLog << L"move constructor" << idEnt << L"<-" << other.idEnt << "\n";

        ++idEntCtr;

    }

    // move assignment
    Cont& Cont::operator=(Cont&& other)
    {
        debugLog << L"move assignment" << idEnt << L"<-" << other.idEnt << "\n";
        return *this;
    }

    ~Cont()
    {
        debugLog << L"deconstructed " << idEnt << "\n";
    }
    int a = 0;;
};

int Cont::idEntCtr = 0;

void skool()
{
    thingy a1, a2;
    std::cout << "Trying to move-assign A from rvalue temporary\n";
    a1 = f(thingy()); // move-assignment from rvalue temporary
    std::cout << "Trying to move-assign A from xvalue\n";
    a2 = std::move(a1); // move-assignment from xvalue

    std::cout << "Trying to move-assign B\n";
    B b1, b2;
    std::cout << "Before move, b1.s = \"" << b1.s << "\"\n";
    b2 = std::move(b1); // calls implicit move assignment
    std::cout << "After move, b1.s = \"" << b1.s << "\"\n";

    std::cout << "Trying to move-assign C\n";
    C c1, c2;
    c2 = std::move(c1); // calls the copy assignment operator

    std::cout << "Trying to move-assign D\n";
    D d1, d2;
    d2 = std::move(d1);



    {
        std::vector<Cont> conts[5];
        //std::vector<Cont> conts = { 1, 2, 3 };
        //std::vector<Cont> conts = { Cont(100), Cont(100), Cont(100), Cont(100) };
        //Cont c, d;
        //d = std::move(c);
        //conts.push_back(std::move(d));
        Cont a;
        //conts[0] = a;
        debugLog << L"ending scope \n";
    }
    debugLog << L"\n\n";

    {
        std::vector<Cont> conts;
        //std::vector<Cont> conts = { 1, 2, 3 };
        //std::vector<Cont> conts = { Cont(100), Cont(100), Cont(100), Cont(100) };
        Cont c, d;
        d = c;

        //conts.push_back(std::move(d));

        debugLog << L"ending scope \n";
    }

}
/*
class Cont
{
public:
    static int idEntCtr;

    //cont() = delete;

    Cont()
    {
        a++;
        idEnt = idEntCtr;
        debugLog << L"default constructeder " << idEnt << "\n";

        ++idEntCtr;
    }

    Cont(const Cont &other)
    {
        idEnt = idEntCtr;
        debugLog << L"copy cons " << other.idEnt << L"to " << idEnt << "\n";
        ++idEntCtr;
    }

    Cont(int j) : a(j)
    {
        a++;
        idEnt = idEntCtr;
        debugLog << L"constructed j" << idEnt << "\n";
        ++idEntCtr;

    }

    ~Cont()
    {
        a--;
        debugLog << L"deconstructed " << idEnt << "\n";
    }
    int a = 100;
    int idEnt;
};

int Cont::idEntCtr = 0;
*/
/*
{
    std::vector<Cont> conts = { 1, 2, 3 };    
    debugLog << L"ending scope \n";
}

    constructed j0
    constructed j1
    constructed j2
    copy cons 0 to 3
    copy cons 1 to 4
    copy cons 2 to 5
    deconstructed 2
    deconstructed 1
    deconstructed 0
    ending scope
    deconstructed 3
    deconstructed 4
    deconstructed 5
 */

/*
{
std::vector<Cont> conts;

Cont c = Cont();
conts.push_back(c);
}
default constructeder 0
copy cons 0 to 1
ending scope
deconstructed 0
deconstructed 1

*/
/*
   {
       std::vector<Cont> conts;
       conts.push_back(Cont());
    }
    default constructeder 0
    copy cons 0 to 1
    deconstructed 0
    ending scope
    deconstructed 1

*/

/*

*/
