#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
  basic_block_size = _basic_block_size, total_memory_size = _total_memory_length;
}

BuddyAllocator::~BuddyAllocator (){

}

BlockHeader* BuddyAllocator::getbuddy (BlockHeader * addr){
  //BlockHeader* buddy = addr ^ addr->block_size;
}

bool BuddyAllocator::arebuddies (BlockHeader* block1, BlockHeader* block2){
  return (block1->block_size == block2->block_size) && (block1->next == block2);
}

BlockHeader* BuddyAllocator::merge (BlockHeader* block1, BlockHeader* block2){
    if(block1->next != block2)
    {
        block2->block_size = block2->block_size*2;
        return block2;
    }
    block1->block_size = block1->block_size*2;
    return block1;
}

BlockHeader* BuddyAllocator::split (BlockHeader* block){
  char* start = block;
  start += block->block_size >> 1;
  ((BlockHeader*)start)->block_size = block->block_size >> 1;
  ((BlockHeader*)start)->next = block->next;
  ((BlockHeader*)start)->is_free = true;
  //Push (BlockHeader*)start into freelist

  block->block_size = block->block_size/2;
  block->next = ((BlockHeader*) start);
  return block;
}

void* BuddyAllocator::alloc(int length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
  if(length > total_memory_size)
  {
      return NULL;
  }


  return malloc (length);
}

void BuddyAllocator::free(void* a) {
  /* Same here! */
  ::free (a);
}

void BuddyAllocator::printlist (){
  cout << "Printing the Freelist in the format \"[index] (block size) : # of blocks\"" << endl;
  int64_t total_free_memory = 0;
  for (int i=0; i<FreeList.size(); i++){
    int blocksize = ((1<<i) * basic_block_size); // all blocks at this level are this size
    cout << "[" << i <<"] (" << blocksize << ") : ";  // block size at index should always be 2^i * bbs
    int count = 0;
    BlockHeader* b = FreeList [i].head;
    // go through the list from head to tail and count
    while (b){
      total_free_memory += blocksize;
      count ++;
      // block size at index should always be 2^i * bbs
      // checking to make sure that the block is not out of place
      if (b->block_size != blocksize){
        cerr << "ERROR:: Block is in a wrong list" << endl;
        exit (-1);
      }
      b = b->next;
    }
    cout << count << endl;
    cout << "Amount of available free memory: " << total_free_memory << " byes" << endl;  
  }
}

