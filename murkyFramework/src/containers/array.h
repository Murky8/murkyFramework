#ifndef			ARRAY_H
#define			ARRAY_H

#include	"wickedGlobal.h"
#include		"memory.h"

//#define	ITARRAY( it, array )  it=array.pBegin ; it<array.pEnd ; it=array.pNext( it ) 
// instead of doing lots of calls to malloc for elements of the same family, use a big pool instead

template < class T >
class	CarrayPool
{
public:
	T	*pBegin;
	T	*pEnd;
	u32	capacity;
	//bool	bDynamicCapacity;

	// main alloc funct
	void	alloc( u32 _nElements )
	{
		exit2("");
		bDynamicCapacity	= false;
		capacity	= _nElements;
		pBegin= pEnd	= ( T* )memoryManager.walloc( capacity*sizeof( T ) );
	}
	/*
	CarrayPool()
	{
	}

	~CarrayPool()
	{
	}
	// calls to array.free() now invalid. del arrayPool not array
	*/

};
//class	Carray 
template < class T >
class	Carray 
{
public:
	T	*pBegin;	//!!! make me private
	T	*pEnd;		//!!! make me private
	u32	capacity;
	bool	bDynamicCapacity;
	
	//T	*begin(){ return pBegin; }
	//T	*end(){ return pEnd; }
	
	//Carray() : pBegin(NULL), pEnd(NULL), capacity(0), bDynamicCapacity(false){}
	
	void	alloc( u32 _n )
	{
		bDynamicCapacity	= false;
		capacity	= _n;
		pBegin= pEnd	= ( T* )memoryManager.walloc( capacity*sizeof( T ) );
	}

	void	alloc( u32 _n, bool _bDynamicCapacity )
	{
		bDynamicCapacity	= _bDynamicCapacity;
		capacity	= _n;
		pBegin= pEnd	= ( T* )memoryManager.walloc( capacity*sizeof( T ) );
	}

	void	alloc( Carray< T > &sourceArray )
	{
		alloc( sourceArray.count() );
		memcpy( (void*)pBegin, (void*)sourceArray.pBegin, sourceArray.count() *sizeof( T ) );
		pEnd	+=	sourceArray.count();
	}

	void	alloc( u32 _n, CarrayPool< T > &arrayPool )
	{
		bDynamicCapacity	= false;
		capacity	= _n;
		pBegin= pEnd	= ( T* )memoryManager.walloc( capacity*sizeof( T ) );
	}

	void	set( T *_pData, u32 _nElements )
	{
		pBegin	= _pData;
		capacity	= _nElements;
		pEnd	= pBegin +_nElements;
		bDynamicCapacity	= false;
	}

	Carray()
	{
		capacity	= 0;
		pBegin	= pEnd	= (T*)NULL;
	}

	Carray( u32 _n )
	{	alloc( _n ); }//should be alloced on stack	
		
	Carray( u32 _n, bool _b  )
	{	alloc( _n, _b ); }

	~Carray()
	{		
		//if( pBegin!=0 )		
		//	exit2( "array destructor called on un-freed array" );
	}

	bool	initFromFile( 	const char *pFileName, u32 _extraCapacity=0 )
	{
		// empty unninitialised array only
		s32	nFileBytes	= wickedLibMisc::sizeFile( pFileName );

		//couldnt open file to read
		if( nFileBytes<0 )
			nFileBytes	= 0;

		capacity	= nFileBytes/sizeof( T ) +_extraCapacity;
		alloc( capacity*sizeof( T ) );
		
		pEnd	= pBegin +nFileBytes/sizeof( T );

		if( nFileBytes ==0 )
			return true;
		
		return	wickedLibMisc::bLoadFile( pFileName, pBegin );		
	}

	void	saveFile( char *fileName )
	{
		wickedLibMisc::saveFile( fileName, (char*)pBegin, sizeof( T )*count() );
	}

	Carray( const char *pFileName ) 
	{
		pBegin	= 0;
		initFromFile(  pFileName ); 
	}

	u32	count()
	{	return pEnd -pBegin;	}

	void	free()
	{
		memoryManager.wfree( ( char* )pBegin );
		pBegin	= 0;
	}
	
	void	flush()
	{	pEnd	= pBegin;	}

	T	*pNext( T *p ) const
	{	return p+1; }

	// 
	inline	T	*getNewElements( const int _nEleReq )
	{
		if( _nEleReq==0 )
			exit2("asked for 0 elements?" );

		int		nEleTot	= pEnd -pBegin +_nEleReq;
		if( nEleTot > capacity )
		{
			if( !bDynamicCapacity )
					exit2( "capacity overflow %d %d", capacity, nEleTot );

			// realloc
			capacity	= nEleTot*2;
			pBegin	= ( T* )memoryManager.wrealloc( (char*)pBegin, capacity*sizeof( T ) )   ;
			//memcpy( (void*)pNew, (void*)pBegin, capacity*sizeof( T ) ); not needed
			
			pEnd		= pBegin +nEleTot;						
		}
		else
		{
			pEnd	+=	_nEleReq;			
		}

		return	pEnd -_nEleReq;		
	}

	inline	T	*getNewElement()
	{		return getNewElements( 1 );	}


	// cuts any slack from array
	/*
	void	removeSlack()
	{
		exit2("untested")
		pBegin = ( T* )memoryManager.wrealloc( (char*)pBegin, newCapacity*sizeof( T ) );
		pEnd	= pBegin +capacity;
	}
	*/
	


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

	void	add( Carray< T > &arrayEles )
	{
		add( arrayEles.pBegin,  arrayEles.count() );
	}

	inline	void	addQ( T *pElement )
	{
		*pEnd = *pElement;
		pEnd++;
	}
	
	// dont pass aptr to this cos it could be invalidated y a realloc
	void	insert( int	iIns, const T *pFirst, int nElements )
	{		
		getNewElements( nElements );

		T *pInsertPos	= &pBegin[ iIns ];
		// shift data up	
		memmove( pInsertPos+nElements, pInsertPos, (pEnd -pInsertPos -1 )*sizeof(T) );

		// insert new data
		memcpy( pInsertPos, pFirst, nElements*sizeof(T) );
	}

	void	insert( T *pInsertPos, const T *pFirst, int nElements )
	{		
		getNewElements( nElements );
		
		// shift data up	
		memmove( pInsertPos+nElements, pInsertPos, (pEnd -pInsertPos -1 )*sizeof(T) );

		// insert new data
		memcpy( pInsertPos, pFirst, nElements*sizeof(T) );
	}



	// T must provide < operator
	T *insertSorted( T &newEle )
	{
		T	*pElement;

		for( pElement=pBegin ; pElement!=pEnd ; ++pElement )
		{
			if( newEle<*pElement )
			{
				break;
			}
		}

		insert( pElement, &newEle, 1 );
		return	pElement;
	}

	//!!!SLOW
	// returns pointer to either existing ele or new ele
	
	T*	addIfUnique( T *pNewElement )
	{
		T	*pElement;

		for( pElement=pBegin ; pElement!=pEnd ; ++pElement )
		{
			//if( *pElement==newElement )return;

			if( memcmp( (void*)pElement, (void*)pNewElement, sizeof( T ) )==0)
			{
				return pElement;
			}
		}

		pElement	= getNewElements( 1 );
		memcpy( pElement, pNewElement, sizeof( T ) );		
		return pElement;
	}

	T*	addIfUnique( T newElement )
	{
		return addIfUnique( &newElement );
	}


	// interprets T as an int for sorting
	T*		insertSortedRawIfUnique( T &newElement )
	{
		int	iLowerEle	= 0;
		int	iHigherEle	= pEnd -pBegin -1;		
		int	res;
		// keep bisecting

		// empty array, just add
		if( pEnd -pBegin==0 )
		{
			add( newElement );
			return pBegin;
		}

		// insert at beginning?
		res=memcmp( (void*)&newElement, (void*)pBegin, sizeof( T ) );
		if( res<=0 )
		{
			if(res!=0) 
				insert( 0, &newElement, 1 );
			return pBegin;
		}

		// insert at end
		res=memcmp( (void*)&newElement, (void*)(pEnd-1), sizeof( T ) );
		if( res>=0 )
		{
			if(res!=0)
				insert( count(), &newElement, 1 );
			return pEnd -1;
		}
		
		// binary search
		int iMidEle	;
		while( 1 )
		{
			iMidEle		= (iLowerEle +iHigherEle )>>1;
		
			res=memcmp( (void*)&newElement, (void*)&( pBegin[iMidEle] ), sizeof( T ) );
			if( res==0 )
			{
				return &pBegin[ iMidEle ];
			}

			if( res < 0 )
				iHigherEle= iMidEle;				
			else
				iLowerEle	= iMidEle;				
		
			if( iHigherEle -iLowerEle<=1  )break;
		}

		insert( iHigherEle, &newElement, 1 );
		return &pBegin[ iHigherEle ];
	}

	T *find( T _el )
	{
		T	*pElement;

		for( pElement=pBegin ; pElement!=pEnd ; ++pElement )
		{
			if( memcmp( (void*)pElement, (void*)&_el, sizeof( T ) )==0 )
				return pElement;
		}

		return (T*)0;
	}

	void	remove( T *pFirst, int nElements )
	{
		if( pBegin > pEnd-nElements )
			exit2( "underljgflsdfooa" );

		memmove( pFirst, pFirst +nElements, ( pEnd -pFirst -nElements )*sizeof( T ) );
		pEnd	-= nElements;
	}

	void	remove( T *pEle ) 
	{
		remove( pEle, 1 );
	}

	void	remove( Carray< T* > &ptrsEles )
	{
		bubbleSort_maxMin(  ptrsEles.pBegin, ptrsEles.count() );

		for( T** ppEle=ptrsEles.pBegin ; ppEle!=ptrsEles.pEnd ; ppEle = ptrsEles.pNext( ppEle ) )
		{
			remove( *ppEle );
		}		
	}

	void	remove( T **_ppElements, int _nElements )
	{

		if( pBegin > pEnd -_nElements )
			exit2( "underljgflsdfooa2" );

		// sort pointers in to high-low memaddrs
		//bubbly sort			
		bubbleSort_maxMin( _ppElements, _nElements );

		LOOP( i, 0, _nElements-1 )
		{
			remove( _ppElements[i], 1 );
		}		
	}
	    
	T &operator[ ]( u32 i ) const
	{
		if(  i >= pEnd -pBegin  )
			exit2( "bad array index out of bounds\n" );

		return	pBegin[ i ];
	}
	
	// move me. string functions

	void	insertString( char *pDest, char *pNewText );
	void	addText( const char *formatString, ... );
	bool	searchForText( char **ppText, char *pWord );
	s32	getInt( char **ppText );
	f32	getFloat( char **pp );
	void	getWord( char **pp, char *pWord );
	void	getQuotedWord( char **pp, char *pWord );
	void	getFileNameFromPath( char **pp, char *pWord );
	bool	decomposeFileName( char *pText, char *pPath, char *pName, char *pExtension );
	// move me. string functions

};


class	Cdarray 
{
	public:
	u32	sizeofEl;
	char	*pBegin;
	char	*pEnd;
	u32	capacity;
	bool	bDynamicCapacity;
	
	//Carray() : pBegin(NULL), pEnd(NULL), capacity(0), bDynamicCapacity(false){}
	
	void	alloc( u32 _n, u32 _sizeofEl  );
	void	alloc( u32 _n, u32 _sizeofEl, bool _bDynCap  );
	Cdarray()
	{
		capacity	= 0;
		pBegin	= pEnd	= (char*)NULL;
	}

	Cdarray( u32 _n, u32 _sizeofEl )
	{
		alloc( _n, _sizeofEl );
	}

	~Cdarray()
	{
		//if( pBegin!=0 )
		//	exit2( "array destructor called on un-freed array" );
	}

	
	u32	count();
	void	free();
	void	flush();
	inline	char	*pNext( char *p ) const;
	inline	char	*getNewElements( const int _nEleReq );
	inline	char	*getNewElement();		
	char		*add( char *_pElement );
	char		*add( char *_pElement, u32 _nElements );
	char		*addIfUnique( char *_newElement );

	char *operator[ ]( u32 i ) const
	{
		if(  i*sizeofEl >=  pEnd -pBegin   )
			exit2( "bad array index out of bounds\n" );

		return	pBegin + i*sizeofEl;
	}

};

// array 
template < class T >
class	Clarray 
{
	const static u16 capacity;
	T data[ capacity ];				//allocated on stack
	u16	count;

public:

	T	*begin()
	{ return data;}

	T	*end()
	{ return &data[ count ]; }

	T * next( T *p )
	{ return p+1; }

	Clarray( int _capacity ) : capacity(_capacity )
	{
		count = 0;
	}

	void	add( T &_ele )
	{
		if( count>=capacity )
			exit2( "larray overflow" );
		data[count]=_ele;
		count++;
	}

	void add( T &_ele, bool _bAdd )
	{
		if( count>=capacity )
			exit2( "larray overflow" );
		data[count]=_ele;
		count+=_bAdd;
	}
};

// array local to function




// changing container type should not need lotd of re-coding

// attachable arrays?

// Carray

#endif


