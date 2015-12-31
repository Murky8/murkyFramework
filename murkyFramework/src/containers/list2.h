#pragma once
/*
//#include		"wickedGlobalTypes.h"
#include		"memory.h"


//////////////////////////////////////////////////////////////////////////
// wrapper for list element containing refs to adjacent element
//////////////////////////////////////////////////////////////////////////
template<class T>
class	ClistElement
{
public:
	ClistElement<T>	*pPrev;
	ClistElement<T>	*pNext;
	T	data;
	
	T operator->()
	{
		return	this->data;
	}
};


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
template<class T>
class	ClistElementPool
{
public:
    u32	nElementsMax;
	u32	nUsedElements;
	ClistElement<T>		*pData;
	u32	*pFreeElementIndicies;
	bool	bNamed;

	ClistElementPool() : pData( 0 )
	{
		bNamed	= true;
	}

	void	initialise( u32 _nElements )
	{
		if( pData )
			exit2("already init!?" );

		nElementsMax	= _nElements;
		//pData	= new  ClistElement<T>[ nElementsMax ];
		pData	=(ClistElement<T>*)memoryManager.walloc( sizeof( ClistElement<T> ) *nElementsMax  );

		//pFreeElementIndicies	= new	u32[ nElementsMax ];
		pFreeElementIndicies	=(u32*) memoryManager.walloc( sizeof( u32 ) *nElementsMax  );

		nUsedElements	= 0;
		LOOP( i, 0, nElementsMax-1 )
		{
			pFreeElementIndicies[ i ]	= i;
		}
	}

	void	free()
	{
		//delete	[] pFreeElementIndicies;
		//delete	[] pData;

		memoryManager.wfree( (char*)pFreeElementIndicies );
		memoryManager.wfree( (char*)pData );
	}

	ClistElement<T>	*getNewElement()
	{
		if( nUsedElements==nElementsMax )
			exit2("get el overfolw");

		return	&pData[ pFreeElementIndicies[ nUsedElements++] ];
	}

	void	returnElement( ClistElement<T> *pElement )
	{
		pFreeElementIndicies[ --nUsedElements ]	= pElement -pData;
	}
};

//class	Clist
template	< class T >
class	Clist
{
public:

	ClistElementPool<T>	*pPool;
	ClistElement<T>		*pHeaderElement;
	ClistElement<T>		*pTailerElement;
	int nElements;

	T	*pBegin;	// cause probs if client trys to get index using this.. make private	
	T	*pEnd;
	
	Clist() : pBegin(0), nElements(0)
	{}

	inline		T	*pNext(  T *p ) const
	{		
		// ATTN!!! find a nicer, safer way to do this ?
		return	&
			( (ClistElement<T>*)(	(char*)p -2*sizeof(ClistElement<T>*) ) )
			->pNext->data ;
	}
		
	
	void	alloc( ClistElementPool<T> &pool )
	{
		pPool	= &pool;
		pPool->bNamed	= true;
		pHeaderElement	=  pPool->getNewElement();
		pTailerElement	=  pPool->getNewElement();

		pHeaderElement->pPrev	= ( ClistElement<T>* )NULL;
		pHeaderElement->pNext	=pTailerElement;

		pTailerElement->pPrev	= pHeaderElement;
		pTailerElement->pNext	=( ClistElement<T>* )NULL;

		pBegin	= &pHeaderElement->pNext->data;
		pEnd		= &pTailerElement->data;
		nElements= 0;

	}

	// unnamed pool
	void	alloc( u32 _nElements )
	{
		pPool	= new ClistElementPool< T >;
		pPool->bNamed	= false;
		pPool->initialise( _nElements+2 );//header and tailer

		pHeaderElement	=  pPool->getNewElement();
		pTailerElement	=  pPool->getNewElement();

		pHeaderElement->pPrev	= ( ClistElement<T>* )NULL;
		pHeaderElement->pNext	=pTailerElement;

		pTailerElement->pPrev	= pHeaderElement;
		pTailerElement->pNext	=( ClistElement<T>* )NULL;

		pBegin	= &pHeaderElement->pNext->data;
		pEnd		= &pTailerElement->data;

		nElements= 0;
	}

	void	free()
	{
		returnAllElements();
		if( pPool->bNamed )
		{
			pPool->returnElement( pTailerElement );
			pPool->returnElement( pHeaderElement );
		}
		else
		{
			pPool->free();
			delete	pPool;
		}
	}

	// from end of list
	// iteration order
	// HEAD-c-d-e-f-g-TAIL
	// HEAD-c-d-e-f-g-new-TAIL
	T	*getNewElement()
	{
		ClistElement<T>	*pNewElement;
		pNewElement	= pPool->getNewElement();

		//add to back of list

		// point new element to adjacent elements
		pNewElement->pPrev	= pTailerElement->pPrev;
		pNewElement->pNext	= pTailerElement;

		//point ajacent elements to this new element
		pTailerElement->pPrev->pNext	= pNewElement;
		pTailerElement->pPrev	= pNewElement;
		
		pBegin	= &pHeaderElement->pNext->data;
		pEnd		= &pTailerElement->data;

		nElements++;
		return	&( pNewElement->data );
	}

	T	&add( T &el )
	{
		T	*pNewEle	=getNewElement();
		//*pNewEle	= el;
		memcpy( pNewEle, &el, sizeof( T ) );
		
		return	*pNewEle;
	}

	T	&add( T *el )
	{
		T	*pNewEle	=getNewElement();
		//*pNewEle	= el;
		memcpy( pNewEle, el, sizeof( T ) );

		return	*pNewEle;
	}

	void	addIfUnique( T &newElement )
	{
		
		for( T *pElement=pBegin ; pElement!=pEnd ; ++pElement )
			if( *pElement==newElement )
				return;
	
		add( newElement );
	}

	void	add( Carray< T > &arry )
	{
		for( T* ITERATE( p, arry) )
		{
			add( *p );
		}
	}

	void	remove( T *pEle )
	{
		// ATTN!!! find a nicer, safer way to do this
		ClistElement<T>*pListEle	= (ClistElement<T>*)( (char*)pEle -2*sizeof(ClistElement<T>*) );
	
		pListEle->pNext->pPrev	= pListEle->pPrev;
		pListEle->pPrev->pNext	= pListEle->pNext;
		pPool->returnElement( pListEle );

		pBegin	= &pHeaderElement->pNext->data;//jc:why?
		pEnd		= &pTailerElement->data;//jc:why?
		nElements--;
	}

	u32	count()
	{

	//	ClistElement<T>	*pIt;
		//u32	nElements	= 0;

		//for( pIt = pHeaderElement->pNext ; pIt != pTailerElement ; pIt = pIt->pNext )
//			nElements++;

		return nElements;
	}

	void	returnAllElements()
	{
		//removeAllElements

		ClistElement<T>	*pIt;

		for( pIt = pHeaderElement->pNext ; pIt != pTailerElement ; pIt = pIt->pNext )
		{
			pPool->returnElement( pIt );
		}

		pHeaderElement->pPrev	= ( ClistElement<T>* )NULL;
		pHeaderElement->pNext	=pTailerElement;

		pTailerElement->pPrev	= pHeaderElement;
		pTailerElement->pNext	=( ClistElement<T>* )NULL;

		pBegin	= &pHeaderElement->pNext->data;
		pEnd		= &pTailerElement->data;
		nElements	= 0;
	}
	
	bool	bIsEmpty()
	{
		if( pBegin==pEnd )
			return true;
		else
			return false;
	}
	
	//WARNING: performance issue!
	// make sure not to use
	
	//u32	getLogicalIndex
	u32	getIndex( T &_data )
	{
		u32	i=0;
		
		for( T *p=pBegin ; p!=pEnd ; p=pNext( p ), i++ )
			if( p==&_data )
				return i;
					
		exit2("ovfdi");	
		return -1;
	}

	//WARNING: performance issue!
	T &operator[ ]( u32 i )
	{
		T *p;
		for( p=pBegin ; p!=pEnd && i; p=pNext( p ), i-- )
		{
		}

		return	*p;
	}
		
	//ATTN!

};

*/