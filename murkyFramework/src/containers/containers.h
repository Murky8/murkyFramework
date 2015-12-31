#ifndef			CONTAINERS_H
#define			CONTAINERS_H

//#include		"wickedGlobalTypes.h"

// NOTE!!! these will not invoke constructors/destructors use STL.

#include	"memory.h"
#include	"array.h"
#include	"list.h"
#include	"pool.h"
#include	"listArray.h"
#include	"fifo.h"

#define	ITERATE( it, lis )  it=lis.pBegin ; it!=lis.pEnd ; it=lis.pNext( it )
#define	ITERATE_STL( it, lis ) it=lis.begin() ; it!=lis.end() ; ++( it )
#define	ITERATE_STL2( type, it, lis ) type::iterator it=lis.begin() ; it!=lis.end() ; ++( it )

/*
template < CArray< T > array, _sortFunct >
void	sort()
{

}
*/
// TO DO
// implement pBegin() instead of pBegin
extern	bool	decomposeFileName( char*,  char*, char*, char*);

template< class T >
inline	void	operator +=( Clist< T > &_list, Carray< T > &_array )
{
	if( !_list.pBegin )
		exit2( "list should be initialised before use (?)" );

	if( !_array.pBegin )
		exit2( "array should be initialised before use (?)" );

	for( T* ITERATE( pArrayEl, _array ) )	
	{
		_list.add( *pArrayEl );
	}
}
#endif


