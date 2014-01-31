#include "chromosome_tree.h"

#include <iostream>
#include <iomanip>

#include <vector>


// FIXME: We're returning a valid ID for missing chrom_name here, bad.
//
uint32_t chromosome_tree::id_for_chrom_name(std::string chrom_name) {
  return id_hash[chrom_name].chrom_id;
}

// FIXME: We're returning chrom_size == 0 for missing chrom_name here, less bad.
//
uint32_t chromosome_tree::chrom_size_for_chrom_name(std::string chrom_name) {
  return id_hash[chrom_name].chrom_size;
}


void chromosome_tree::make_in_memory_hash(std::istream& is) {
  
  header.unpack(is);
  
  bp_tree::header_node n_hdr;
  n_hdr.unpack(is);
  
  recursive_fill_map(n_hdr, is);
  
}

void chromosome_tree::recursive_fill_map(bp_tree::header_node n_h, std::istream& is) {
  
  if (n_h.is_leaf == false) {
    
    // Drops down a level for each node header pointed to by this
    // internal_node. Probably never happens since the number of chromosomes is small.
    //
    for (int i = 0; i < n_h.count; ++i) {
      
      bp_tree::internal_node in_node;
      in_node.unpack(is, header.key_size);
      
      // This is the next internal_node position, out sibling, so save pos for next
      // time through the loop.
      //
      std::streamsize curr_off = is.tellg();
      
      // Now go down a level.
      //
      is.seekg(in_node.child_offset);
      bp_tree::header_node n_h_n;
      recursive_fill_map(n_h_n, is);
      
      // We're done below so seek back to our sibling's position.
      is.seekg(curr_off);
      
    }
    
  } else {

    // We're on the leaf level so we can just fill up the hash map.
    //
    for (int i = 0; i < n_h.count; ++i) {
      bp_tree::leaf_node ln;
      ln.unpack(is, header.key_size);
      id_hash[ln.key] = ln;
    }
    
  }
}