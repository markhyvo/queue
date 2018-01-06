/*
 * This file contains interface for queue (container adapter).
 * It is designed to to operate in FIFO context(fisrt-in first-out)
 * manner.
 */
#ifndef MH_QUEUE_H
#define MH_QUEUE_H

namespace MH
{
   // Constants
   const int INIT_ELEM = 10;           // Initial buffer element count
   const int BUF_MULTIP = 3;           // buffer size algorithm
   const int BUF_ADD = 2;              // buffer size algorithm

   template <typename T>
   class Queue
   {
   public:
      Queue();                         // Construct queue
      ~Queue();                        // Destruct queue
      Queue(const Queue &);            // Copy constructor
      Queue &operator=(const Queue &); // Copy assignment operator                                             
      void push(const T &);            // Add element to back of queue      
      void pop();                      // Remove elem - f empty throws
      T &front();                      // Return reference to front elem in the queue   
      const T &front() const;          // Return reference to front elem in the queue   
      bool empty() const;              // Return whether queue is empty
      size_t size() const;             // Return number of elems in queue
   private:
      T *v_;                           // buffer for queue elements
      size_t vsize_;                   // enough to hold T's
      size_t vused_;                   // number of T's actually in use
      size_t getNewBufferSize();       // calculate new larger buffer
      size_t getOldBufferSize();       // retrieve original smaller buffer
   };

   // Helper function for class Queue
   template <typename T>
   T *
      newCopy(const T *src, size_t srcsize, rsize_t destsize);
}
#endif