#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>


/*
// inline namespace
namespace God
{
    namespace v0
    {
        const int moof = 123;
    }

    inline namespace v1
    {
        const int moof = 321;
    }

}
*/

//
//
//
//// http://www.cplusplus.com/doc/tutorial
//
//using std::vector;
//
//// template skool
//
//
//// basic template
//template<typename T>
//void clamp(T &v0, T min, T max)
//{
//}
//
//// specialization
//template<>
//void clamp<vec3>(vec3 &v, vec3 min, vec3 max)
//{
//    clamp(v.x, 0.f, 1.f);
//}
//
//// class template
//template<class T>
//class Contain
//{
//public:        
//private:
//    T a;
//};

// template in class
// nested template

/*
template< class T >
using NamedMap = std::map< std::wstring, T >;

template< class T >
class FinderObject
{
public:
    FinderObject() = delete;
    FinderObject( NamedMap<T> &rmap) : cached(false), rmap(rmap)
    {}

    bool    cached;
    const   NamedMap<T> &rmap;
};
*/
void skool()
{
  
}
// template skool


/*
struct thingy 
{
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

struct B : thingy 
{
    std::string s2;
    int n;
    // implicit move assignment operator B& B::operator=(B&&)
    // calls A's move assignment operator
    // calls s2's move assignment operator
    // and makes a bitwise copy of n
};

struct C : B 
{
    ~C() {} // destructor prevents implicit move assignment
};

struct D : B 
{
    D() {}
    ~D() {} // destructor would prevent implicit move assignment
    //D& operator=(D&&) = default; // force a move assignment anyway 
};

void mainThingy()
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
}
*/

/*
class Cl
{
public:
    int *pints;
    int s;
    static int idEntCtr;
    int idEnt;

    Cl()    // =delete if not necessary to have uninitialised objects lying around.
    {
        pints = nullptr;
        idEnt = idEntCtr++;
        
        debugLog << L"default constructor. id:" << idEnt << "\n";                
    }

    // paremeterised constructor
    Cl(int v0, int v1, int s) : s(s)
    {
        pints = new int[2];
        pints[0] = v0; pints[1] = v1;

        idEnt = idEntCtr++;

        debugLog << L"constructed with parameters. id:" << idEnt << "\n";
        debugLog << L"pints:" << pints[0] << " " << pints[1] << "\n";
        debugLog << L"s: " << s << L"\n";


    }
    
    // copy constructor. duplicates resorces. =delete to prevent copying
    Cl(const Cl &rhs)
    {
        pints = new int[2];
        pints[0] = rhs.pints[0]; pints[1] = rhs.pints[1];// deep copy

        idEnt = idEntCtr++;        
        debugLog << L"copy constructor id:" << idEnt << L"=id:" << rhs.idEnt << "\n";
        debugLog << L"pints:" << pints[0] << " " << pints[1] << "\n";
        debugLog << L"s: " << s << L"\n";        
    }

    //  assignment operator
    Cl& operator=(const Cl& rhs)
    {         
        delete[] pints; // overwriting
        pints = new int[2];
        pints[0] = rhs.pints[0]; pints[1] = rhs.pints[1];// deep copy

        debugLog << L"assignment operator id:" << idEnt << L"= id:" << rhs.idEnt << "\n";
        debugLog << L"pints:" << pints[0] << " " << pints[1] << "\n";
        debugLog << L"s: " << s << L"\n";

        return *this;
    }
    
    // move assignment operator
    Cl& Cl::operator=(Cl&& rhs)
    {        
        delete[] pints;

        pints = rhs.pints;
        s = rhs.s;

        rhs.pints = nullptr;
        rhs.s = -1;

        debugLog << L"move assignment operator " << idEnt << L"=" << rhs.idEnt << "\n";
        debugLog << L"pints:" << pints[0] << " " << pints[1] << "\n";
        debugLog << L"s: " << s << L"\n";

        return *this;
    }

    // move constructor
    Cl(Cl &&rhs) : pints(nullptr), s(0)
    {
        *this = std::move(rhs);
                
        idEnt = idEntCtr++;
        
        debugLog << L"move constructor " << idEnt << L"=" << rhs.idEnt << "\n";
        debugLog << L"pints:" << pints[0] << " " << pints[1] << "\n";
        debugLog << L"s: " << s << L"\n";        
    }
    
    ~Cl()
    {
        if (pints!= nullptr)
            debugLog << L"destructor " << idEnt << "\n";
        else
            debugLog << L"destructor on nullptr" << idEnt << "\n";

        delete[] pints;
    }
};

int Cl::idEntCtr = 0;
*/


/*
void skool()
{        

    if (1) // construction of single instance
    {
    debugLog << "construction of single instance" << L"\n";
    debugLog << "a: "; Cl  a;
    debugLog << "\n";

    //debugLog << "b: "; Cl  b();  //warning C4930 : 'Cl b(void)' : prototyped function not called(was a variable definition intended ? )
    //debugLog << "\n";

    debugLog << "c: "; Cl  c(123, 124, 444);
    debugLog << "\n";

    debugLog << "d: "; Cl  d = { 123, 124, 444 };
    debugLog << "\n";

    debugLog << "e: "; Cl  e = Cl( 123, 124, 444 ); //only one construction despite standard saying two
    debugLog << "\n";

    debugLog << "f: "; Cl  f = { 123, 124, 444 };
    debugLog << "g: "; Cl  g = f;                   // copy constructor. potential problematic.

    debugLog << "h: "; Cl  h = { 123, 124, 444 };
    debugLog << "i: "; Cl  i = Cl(h);                   // copy constructor. potential problematic.

    debugLog << "\n";

    debugLog << L"ending scope \n";
    }
    debugLog << "\n\n";

    if (1) // construction of array
    {
    debugLog << "construction of array" << L"\n";

    debugLog << "a: "; Cl a[2];
    debugLog << "\n";

    debugLog << "b: "; Cl b[2] ={{1,2,3}, {4,5,6}};
    debugLog << "\n";

    debugLog << L"ending scope \n";
    }
    debugLog << "\n\n";

    if (1) // construction of vector
    {
    debugLog << "construction of vector" << L"\n";

    debugLog << "a: "; vector < Cl > a;
    debugLog << "\n";

    debugLog << "b: "; vector < Cl > b[5];
    debugLog << "\n";

    debugLog << "c: "; vector < Cl > c{ { 104, 105, 106 } };

    debugLog << "c: "; vector < Cl > c{ Cl(104, 105, 106) };// warning C6011: Dereferencing NULL pointer '?/* Sorry I don't currently handle AST_POINTSTO nodes!  ? '. See line 0 for an earlier location where this can occur
    debugLog << "\n";
    debugLog << L"ending scope \n";
    }
    debugLog << "\n\n";
    
    if (1) // move constructor
    {
    {
    debugLog << "Testing: move constructor \n";
    debugLog << "a: "; Cl  a(100, 101, 102);

    Cl b = std::move(a);
    debugLog << L"ending scope \n";
    }
    debugLog << "Finished \n\n";
    }

    if (1) // move assignment
    {

    {
    debugLog << "Testing: move assignment \n";
    debugLog << "a: "; Cl  a;
    debugLog << "b: "; Cl  b(100, 101, 102);
    a = std::move(b);
    debugLog << L"ending scope \n";
    }
    debugLog << "Finished \n\n";
    }
    */

/*
    if (0) // mniplutating vector    
    {
        {
            debugLog << "Testing: vector ops\n";

            debugLog << "a: "; vector < Cl > a;
            debugLog << "\n";

            debugLog << "b: "; Cl  b(123, 234, 345);
            debugLog << "\n";

            a.push_back(std::move(b));
            debugLog << "\n";
        }
    }
    */


    /*
        {
        std::vector<Cl> conts[5];
        //std::vector<Cont> conts = { 1, 2, 3 };
        //std::vector<Cont> conts = { Cont(100), Cont(100), Cont(100), Cont(100) };
        //Cont c, d;
        //d = std::move(c);
        //conts.push_back(std::move(d));
        Cl a;
        //conts[0] = a;
 
        Cl c, d;
        d = c;
        */
