#ifndef			POOL_H
#define			POOL_H

const int g_MaxNumberOfObjectsInPool = 1000;

// element pool class

// uses an array of indices to elements to track allocation. 
// O(k) constant time per element allocation.
// to do: array alloc/dealloc.

template	<class T>
class	Cpool
{
private:
    const	u32	nElementsMax;
	u32	nUsedElements;
	
	T		*pData;
	u32	*pFreeElementIndicies;

	public:
	// constructors
	Cpool( u32 _nElementsMax = g_MaxNumberOfObjectsInPool ) : nElementsMax( _nElementsMax )
	{
		pData	= (T*)malloc( sizeof(T) *nElementsMax );// no 'new' as dont want constructors called at this point
		pFreeElementIndicies	= new  u32 [ nElementsMax ];
		nUsedElements	= 0;
		for( u32  i=0 ; i<nElementsMax ; ++i )
		{
			pFreeElementIndicies[ i ]	= i;
		}
	}

	// destructor
	~Cpool()	
	{
		delete	[] pFreeElementIndicies;
		free( (void*)pData ); 		
	}

	// use only for POD i.e. no constructor.
	T	*getNewElement()
	{
		if( nUsedElements==nElementsMax )
			exit2("get el overfolw");

		return	&pData[ pFreeElementIndicies[ nUsedElements++ ] ];
	}

	void	returnElement( T *_pElement )
	{
		if( _pElement==NULL )
			exit2( "Tried to return null" );
		pFreeElementIndicies[ --nUsedElements ]	= _pElement -pData;
	}
};

	//void *operator new( size_t sizeofT )
	//{
		//_pool.getNewElement();		
	//}

	//void *operator new( size_t sizeofT, Cpool< T > &_pool )
	//{
		//return _pool.getNewElement();		
	//}

	//void operator delete ( void *p )
	//{
		//_pool.returnElement( (T*)p );
	//}	
//void operator delete ( void *p,  Cpool< T > &_pool )
//{
	//_pool.returnElement( (T*)p );
//}	
// placement new
//template	<class T>
//void *operator new( size_t sizeofT, Cpool< T > &_pool  )
//{
	//return &_pool.getNewElement();		
//}

#endif


