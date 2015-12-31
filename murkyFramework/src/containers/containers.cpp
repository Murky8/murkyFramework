#include	<windows.h>
#include	<io.h>
#include	<stdio.h>
//#include	<fcntl.h>
#include	<string.h>
#include	"wickedGlobal.h"


void	Cdarray::alloc( u32 _n, u32 _sizeofEl, bool _bDynCap  )
{
	bDynamicCapacity	= _bDynCap;
	capacity	= _n;
	sizeofEl	= _sizeofEl;
	pBegin= pEnd	= memoryManager.walloc( capacity*sizeofEl );
}

void	Cdarray::alloc( u32 _n, u32 _sizeofEl  )
{
	alloc( _n, _sizeofEl, false );
}

u32	Cdarray::count()
{	return (pEnd -pBegin)/sizeofEl;	}

void	Cdarray::free()
{
	memoryManager.wfree( pBegin );
	pBegin	= 0;
}
	
void	Cdarray::flush()
{	pEnd	= pBegin;	}

inline	char	*Cdarray::pNext( char *p ) const
{	return p +sizeofEl; }

	// 
inline	char	*Cdarray::getNewElements( const int _nEleReq )
{
	if( _nEleReq==0 )
		exit2("asked for 0 elements?" );

	int		nEleTot	= ( pEnd -pBegin )/sizeofEl +_nEleReq;
	if( nEleTot > capacity )
	{
		if( !bDynamicCapacity )
				exit2( "capacity overflow" );
			// realloc
		capacity	= nEleTot*2;
		pBegin	= memoryManager.wrealloc( pBegin, capacity*sizeofEl )   ;
				
		pEnd		= pBegin +nEleTot*sizeofEl;						
	}
	else
	{
		pEnd	+=	_nEleReq*sizeofEl;			
	}

	return	pEnd -_nEleReq*sizeofEl;
}


inline	char	*Cdarray::getNewElement()
{		return getNewElements( 1 );	}

// WARNIG!!!: speed O(n ele in array)

char		*Cdarray::add( char *_pNewElement )
{
	char	*p	= getNewElements( 1 );
	memcpy( p, _pNewElement, sizeofEl );
	return	p;
}

char	*Cdarray::add( char *_pNewElements,  u32 _nNewElements )
{
	if( _nNewElements==0 )
		return NULL;

	char	*p	=  getNewElements( _nNewElements );
	memoryManager.memCopy( p, _pNewElements, _nNewElements *sizeofEl );
	return	p;
}

char*	Cdarray::addIfUnique( char *_pNewElement )
{
	char	*pElement;

	for( pElement=pBegin ; pElement!=pEnd ; pElement+=sizeofEl )
	{
		if( memcmp( (void*)pElement, (void*)_pNewElement, sizeofEl )==0 )
		{
			return pElement;
		}
	}

	pElement	= getNewElements( 1 );
	memcpy( pElement, _pNewElement, sizeofEl );		
	return pElement;
}
	

	/*
	T	&add( T *pElement )
	{
		T	*pAddEle	= getNewElements( 1 );
		*pAddEle	= *pElement;			
		return	*pAddEle;
	}

	T	&add( T element )
	{
		T	*pAddEle	= getNewElements( 1 );
		*pAddEle	= element;			
		return	*pAddEle;
	}

	void	add( T *pElements,  u32 nNewElements )
	{
		if( nNewElements==0 )
			return;

		T	*pNewEles	=  getNewElements( nNewElements );
		memoryManager.memCopy( (char*)pNewEles, (char*)pElements, nNewElements *sizeof( T ) );
	}
	*/

// specific implementations
//string functions// move me to wstring.h

//void	removeText()
//void	insertText()


//  use iostream
// insert null term string
void	Carray<char>::insertString( char *pDest, char *pNewText )
{
	u32	wordLen	= (u32)strlen( pNewText );	// dont add pNewText's Null

	// make space.  !efficient
	for( int i=0 ; i<wordLen ; i++ )
		getNewElement();

	// shift
	memmove( pDest +wordLen, pDest, pEnd -pDest -wordLen );

	// add
	memcpy( pDest, pNewText, wordLen );
}

template<>
void	Carray<char>::addText( const char *formatString, ... )
{
	static	char	addTextBuffer[128*1024];	// SORT THIS!!!, not needed
	va_list 	arglist;

	va_start( arglist, formatString );
	
	vsprintf( addTextBuffer, formatString, arglist );
	
	va_end( arglist );	

	add( addTextBuffer, (u32)strlen( addTextBuffer ) );	
}


// pointer points to char after word
// warning :search for "qwer" in "qwerty" 
bool	Carray<char>::searchForText( char **ppText, char *pWord )
{
	s32	wordLen	= (u32)strlen( pWord );
	char	*pText		= *ppText;
	char	*pEndd	= pEnd;

pooop:	

	while(*pText != *pWord )
	{
		pText++;//search for 1st letter
		if( pText>= pEndd )		
			return false;//eoffile
	}

	//FIXME!!! search past end of file?
	LOOP( i, 0, wordLen -1 )
	{
		if( pText[i] != pWord[i] )
		{
			pText++;
			//ptr+=i will this work ?
			goto pooop;
		}
	}
	//U_print("strsearch found (%s)\n", pstr);
	pText	+= wordLen;
	
	*ppText	= pText;
	return	true;	
}

s32	Carray<char>::getInt( char **ppText )
{
	s32	inte=0;
	s32	sign;
	//search for number start
	while( 	!((**ppText>='0' && **ppText<='9') || **ppText=='-') )
	{
		(*ppText)++;
	}

	if(**ppText=='-')
	{
		sign	=	-1.0f;
		(*ppText)++;
	}
	else
	{
		sign	=	1.0f;
	}

	while(**ppText>='0' && **ppText<='9')
	{
		inte *= 10;
		inte +=(**ppText	-	'0');
		(*ppText)++;
	}

	//ret	=	atoi(*(char**)ppText);
	while( (**ppText>='0' && **ppText<='9') || **ppText=='-' )(*ppText)++;

	//U_print("get int found (%d)\n", ret);
	return	inte*sign;
}

f32	Carray<char>::getFloat( char **pp )
{
	f32	frac=0;
	f32	expo=0.1f;
	s32	inte=0;
	f32	sign;

	//search for number start
	while( 	!((**pp>='0' && **pp<='9') || **pp=='-') )(*pp)++;

	if(**pp=='-')
	{
		sign	=	-1.0f;
		(*pp)++;
	}
	else
	{
		sign	=	1.0f;
	}

	while(**pp!='.')
	{
		inte	*=	10;
		inte	+=	(**pp	-	'0');
		(*pp)++;
	}

	(*pp)++;
	while(**pp>='0' && **pp<='9')
	{
		frac	+=	expo*(float)(**pp	-	'0');
		expo	*=0.1f;
		(*pp)++;
	}
	
	return	((float)inte+frac)*sign;
}

void	Carray<char>::getWord( char **pp, char *pWord )
{
	// search for start
	for( ; **pp==' ' || **pp==34 ; (*pp)++)
	{
		if( (*pp)>=pEnd )
			return;
	}
	// search for start

	// get word
	for( ; **pp!=' ' && **pp!=34 ; (*pp)++ )
	{
		*pWord	= **pp;
		pWord++;
	}
	// get word

	// append eof
	*pWord	= 0;
}

void	Carray<char>::getQuotedWord( char **pp, char *pWord )
{
	// search for start
	for( ; **pp!=34 ; (*pp)++)
	{
		if( (*pp)>=pEnd )
			return;
	}
	(*pp)++;
	// search for start

	// get word
	for( ; **pp!=34 ; (*pp)++ )
	{
		*pWord	= **pp;
		pWord++;
	}
	// get word

	// append eof
	*pWord	= 0;
	(*pp)++;
}

// given "c:\\blah\blah\name.xxx"
// returns name

void	getFileNameFromPath( char **pp, char *pWord )
{
	//search for null terminator
//	for( ; **pp!=0 ; (*pp)++)
	//{
		//if( (*pp)>=pEnd )
			//return;
	//}

	//search backwards for start of word

}

bool	decomposeFileName( char *_pText, char *_pPath, char *_pName, char *_pExtension )
{
	char *pText_end; 
	char *pText_name; 
	char *pText_ext; 

	char	*p	= _pText;

	// go to end
	while( *p!=0 || *p==' ' )p++;
	pText_end	= p;
	
	// find '.'
	while( *p!='.' )
	{
		p--;
		if( p<_pText )
		{// no . present
			exit2("coifewfew");
			pText_ext=pText_end;
			_pExtension	= NULL;
		}
	}
	pText_ext	= p+1;

	while( *p!='/' && *p!='\\')
	{
		p--;
		if( p<_pText )
		{
			// cant find it.
			_pPath	= NULL;
			break;
		}
	}
	pText_name	= p+1;

	if( _pPath )
	{
		int	len	= pText_name -_pText -1;
		memcpy( _pPath, _pText,  len );
		_pPath[ len ]	= 0;
	}

	if( _pName )
	{
		int	len	= pText_ext -pText_name -1;
		memcpy( _pName, pText_name, len);
		_pName[ len ]	= 0;

	}

	if( _pExtension )
	{
		int	len	= pText_end -pText_ext;
		memcpy( _pExtension, pText_ext, len);
		_pExtension[ len ] 	= 0;

	}

	return true;
}

//string functions // move me to wstring.h


/*

1) templated
2) non-templated
3) #defined
4) derived classes

template < class T >
class	itertor
{
T	*pData;
};

#define iterater( name, list )
*/
/*
Ccontainer( myFaces, Clist, vec )

#define CContainer( _name, _style, _T )	\
_Ccontainer<_T> _name( _style )				\



class _Ccontainer()
{
T		*pData	
Clist( _type )
{

}
};

1)
template < class T >
class Ccontainer()
{
u32	nBytesElement = sizeof( T );
}

class Citerator()
{
operator->()
}


class Ccontainer()
{

}
//////////////////////
*/




//iterator( dave )



// for( ITER1( ddf ) )
// for( ITER2( dfdf,dfdf ) )

// Ccontainer	blah( T, another container )
// Citerator blah( container )

// CarrayJagged<T> 
// Carray<T> blah;
// Carray<T> blah( another array )
// CList<T>

// pool( initialise(), getElement() )

// #define container( Carray, char, mynums )	/
// Carray<char> mynums									/
// typedef Cmynums_iter Carray<char>::iterator
// #if T == ''
//
// container( Clist, int, numbers(1000) );
// or
// container( Clist, int, numbers );
//




//Carray< Carray<char> > jag;
//char a= jag[10][10];
// Carray< Carray<char> >::iterator itFacegroup( jag );
// for( ITER( itFaceGroup ) )
//	Carray<char>::iterator itFace( itFacegroup );
//	for( ITER( itFace ) )

//STL: confusing, easy to create bugs. No error checking
//	iterator mismatch and invalidation. ergonomic inefficiency, too much stuff to learn
//
// WTL: good error detection. safer bound iterators.
// Clist.		Dynamic, performance optimized for changleablity 
// Carray.	Static, performance optimized for static walking and low mem usage

// run time functionallity flags:
//	CF_BOUNDSCHECK 
//	CF_CHECKUSAGE				check initialisation, adds, pointers.
//	CF_EXTENDABLE				allows arrays to grow and shrink past their max limits




