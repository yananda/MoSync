/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/** 
* \file Queue.h
* \brief Template for a simple object queue
* \author Lars-Åke Vinberg
*/

#ifndef QUEUE_H_
#define QUEUE_H_

#include "MemoryMgr.h"

/**
* \brief MoSync slippy map utility classes
*/

namespace MAPUtil
{
	//=========================================================================
	/**
	 * \brief Template class for a queue with a limited fixed capacity.
	 */
	template<class T>
	class Queue
	//=========================================================================
	{
	public:
		/**
		 * Creates a queue with fixed capacity.
		 * Queue keeps heap-allocated objects of type T.
		 * Client must create objects.
		 * Queue assumes ownership of objects as long as objects are in queue.
		 * Dequeueing an object means ownership is transferred to client.
		 */
		Queue( const int capacity )
			: mItems( NULL ),
			mCapacity( capacity )
		{
			mItems = newobject( T*, new T*[capacity] );
			for ( int i = 0; i < mCapacity; i++ )
				mItems[i] = NULL;
			mEnqueuePosition = 0;
			mDequeuePosition = 0;
		}
		/**
		 * Destroys a queue
		 */
		virtual ~Queue( )
		{
			// Delete each item
			clear( );
			// Delete array
			deleteobject( mItems );
		}
		/**
		 * Returns item at location in queue.
		 * Returns null if item is unused.
		 */
		T* peekAt( int location )
		{
			return mItems[location];
		}
		/**
		 * Returns item capacity of queue.
		 */
		int getCapacity( ) const { return mCapacity; }
		/**
		 * Returns count of queued items.
		 */
		int getCount( ) const
		{
			int count = mEnqueuePosition - mDequeuePosition;
			if ( count < 0 )
				count += mCapacity;
			return count;
		}
		/**
		 * Removes all items in queue.
		 */
		void clear( )
		{
			for ( int i = 0; i < mCapacity; i++ )
				deleteobject( mItems[i] );
			mEnqueuePosition = 0;
			mDequeuePosition = 0;
		}
		/**
		 * returns true if queue contains item.
		 */
		bool contains( const T* item ) const { return Find( item ) != -1; }
		/**
		 * Returns position in array of item.
		 * If not found, returns -1.
		 */
		int find ( const T* item ) const
		{
			int count = getCount( );
			for ( int i = 0; i < count; i++ )
				if ( mItems[i] != NULL )
					if ( mItems[i] == item )
						return i;
			return -1;
		}
		/**
		 * Returns ref to next T in queue.
		 * Caller assumes ownership of item, and is responsible for deleting item.
		 * Returns InvalidPtr if queue is empty.
		 */
		T* dequeue( )
		{
			if ( getCount( ) == 0 )
				return NULL;
			T* ret = mItems[mDequeuePosition];
			mItems[mDequeuePosition] = NULL;
			mDequeuePosition++;
			if ( mDequeuePosition >= mCapacity )
				mDequeuePosition = 0;
			return ret;
		}
		/**
		 * Enters T into queue.
		 * Queue takes ownership of item.
		 */
		void enqueue( T* item )
		{
			// TODO: raise exception
			if ( getCount( ) >= mCapacity)
				return;
			mItems[mEnqueuePosition] = item;
			mEnqueuePosition ++;
			if ( mEnqueuePosition >= mCapacity )
				mEnqueuePosition = 0;
		}
		/**
		 * Returns next item in queue, without removing it from queue.
		 */
		const T* peek( ) const
		{
			if ( getCount( ) == 0 )
				return NULL;
			return mItems[mDequeuePosition];
		}

	private:
		T** mItems;
		int mCapacity;
		int mEnqueuePosition;
		int mDequeuePosition;
	};
}

#endif // QUEUE_H_
