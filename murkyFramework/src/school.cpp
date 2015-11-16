#include <murkyFramework/src/private/pch.hpp>

//#include <string>
//#include <iostream>
//#include <utility>
//#include <murkyFramework/include/debugUtils.hpp>
//#include <murkyFramework/include/types.hpp>
//#include <murkyFramework/include/vectorMatrix.hpp>
//#include <memory>

int idProvider = 0;
struct Zoof
{
    // data 
    int id;
    std::unique_ptr<int> pint;
    // default constructor
    Zoof()
    {        
        id = idProvider++;
        debugLog << id << L":  default constructor\n";
    }

    // copy constructor
    Zoof(const Zoof& rhs)
    {
        id = idProvider++;
        debugLog << id << L":  copy constructor\n";
    }

    // move constructor
    Zoof(Zoof&& rhs)
    {
        id = idProvider++;
        debugLog << id << L":  move constructor\n";
    }

    // copy assignment
    Zoof &operator=(const Zoof& rhs)
    {
        debugLog << id << L"<-" << rhs.id << L":  copy assigmnet\n";
    }

    // move assignment 
    Zoof &operator=(Zoof&& rhs)
    {
        debugLog << id << L"<-" << rhs.id << L":  move assigmnet\n";
    }

    ~Zoof()
    {
        debugLog << id << L": destructor\n";

    }
};

typedef std::unique_ptr<Zoof> upMoof;
void skool()
{
    {
        //std::unique_ptr<Zoof> p(new Zoof);
        //std::vector<  std::unique_ptr<Zoof> > moofs;
        //moofs.push_back(std::move(p));
        //std::unique_ptr<Zoof> d;
        //d = std::move(p);

        //std::shared_ptr<int> pintsh(new int{ 123 });
        debugLog << L"finished \n";
    }

    exit(0);
}


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

//
//struct HandleDeviceTexture2
//{
//// this is just a container, no constructorx destructors of rescources
//public:
//    //ogl
//    //int a;
//    //int b;
//
//    //d3d
//    void *p;
//    HandleDeviceTexture2() = delete;
//    HandleDeviceTexture2(void *p) : p(p)
//    {
//        debugLog << L"HandleDeviceTexture2:: constructor\n";
//    }
//
//    ~HandleDeviceTexture2()
//    {    
//         debugLog << L"HandleDeviceTexture2: destructor\n";          
//    }
//};
//
//class TextureRef2
//{
//public:
//    HandleDeviceTexture2 *pHandleDeviceTexture2;        
//        
//    // constructor
//    TextureRef2( int a) 
//    {
//        //would ususally call GlGentextures here and pass instead of nullptr
//        this->pHandleDeviceTexture2 = new HandleDeviceTexture2(nullptr);
//
//        debugLog << L"TextureRef2: constructed " << a <<"\n";        
//    }
//
//    TextureRef2(const TextureRef2 &v) = delete;
//    TextureRef2 &operator=(const TextureRef2 &rhs) = delete;
//    TextureRef2 &operator=(TextureRef2 &&rhs) = delete;
//    
//    TextureRef2(TextureRef2&& rhs)
//    {
//        debugLog << L"TextureRef2: moved" << "\n";
//
//        this->pHandleDeviceTexture2 = rhs.pHandleDeviceTexture2;
//        rhs.pHandleDeviceTexture2 = nullptr;
//    }
//
//     ~TextureRef2()
//    {                
//        if (pHandleDeviceTexture2 != nullptr)
//        {
//            debugLog << L"TextureRef2: destructor on obj" << "\n";
//            //would ususally call GlDeletetextures
//            delete pHandleDeviceTexture2;
//            pHandleDeviceTexture2 = nullptr;
//        }
//        else
//        {
//            debugLog << L"TextureRef2: destructor on nullptr" << "\n";
//        }
//
//    }
//};
//
//class TexMAnager
//{
//public:
//    std::map<std::wstring, TextureRef2> mapi;
//    void add(std::wstring name, TextureRef2 textureRef2)
//    {
//        //mapi.emplace(std::piecewise_construct,
//            //std::make_tuple(L"hello"),
//            //std::make_tuple(textureRef2));
//
//        mapi.insert(std::pair<std::wstring, TextureRef2>( L"hello", std::move(textureRef2)));
//    }
//};

//class DebObj2Base
//{
//};
//
//class DebObj2Screen : public DebObj2Base
//{
//};
//
//class DebObjFile : public DebObj2Base
//{
//
//};
//
//DebObj2Base& operator << (DebObj2Base &debObj, const std::wstring &rhs)
//{
//    if (typeid(debObj) == typeid(DebObj2Screen))
//    {
//
//    }
//
//    return debObj;
//}



/*
template<typename... T>
void print_all_values(int value, T... values)
{
    printf("%i ", value);
    print_all_values(values...);
}


void skool()
{        
ins to map
constructed with parameters. id:100
pints:123  1
move constructor -842150451 =100
move assignment operator
pints:123  1
pints:123  1
destructor on nullptr100
end of scope
destructor 101

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
