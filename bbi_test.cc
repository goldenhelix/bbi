#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "main_header.h"
#include "zoom_header.h"
#include "data_records.h"
#include "total_summary_header.h"
#include "bp_tree.h"
#include "r_tree.h"
#include "chromosome_tree.h"


#include "bbi_file.h"

int main(int argc, char *argv[])
{
  
  if (argc < 2) {
    std::cerr << "Usage: $ bbi-tools <filename>\n";
    exit(EXIT_FAILURE);
  }

  std::string filename(argv[1]);
  std::ifstream is(filename);

  if (!is.good()) {
    std::cerr << "couldn't open file " << filename << '\n';
    exit(1);
  }
  

  bbi_file bbi(is);

  // Prints the main headers.
  //
  bbi.print_headers(std::cout);
  
  
  // Prints the main zoom headers.
  //
  for (auto& zh : bbi.z_hdrs)
    zh.print(std::cout);
  
  // Obtains any r-tree leaf nodes whose intervals contain our records in
  // the main data section, (zoom level 0).
  //
  auto chrom_id = bbi.chrom_tree.chrom_id("chr21");
  auto leaves = bbi.search_r_tree({chrom_id, 9500000, 45000000}, 0);
  
  
  // Prints the r-tree leaf nodes.
  //
  for (auto& ln : leaves) {
    std::cout << "\n**** r-tree leaf node ****\n";
    ln.print(std::cout);
    std::cout << '\n';
  }
  
  // Prints out the data blocks for the first leaf node if one was returned.
  //
  if (!leaves.empty()) {
    
    std::cout << "\n**** data records ****\n";
    
    r_tree::leaf_node ln = leaves.front();
  
    auto bdrs = bbi.records_for_leaf<bed_data_record>(ln);
  
    for (auto& bdr : bdrs)
      bdr.print(std::cout);
  }
  
  // Obtains any r-tree leaf nodes whose intervals contain our records in
  // the first zoom data section, (zoom level 1).
  //
  chrom_id = bbi.chrom_tree.chrom_id("chr21");
  leaves = bbi.search_r_tree({chrom_id, 9500000, 45000000}, 1);
  
  // Prints out the data blocks for the first leaf node if one was returned.
  //
  if (!leaves.empty()) {
    
    std::cout << "\n**** data records ****\n";
    
    r_tree::leaf_node ln = leaves.front();
    
    auto zdrs = bbi.records_for_leaf<zoom_data_record>(ln);
    
    for (auto& zdr : zdrs)
      zdr.print(std::cout);
    
  }
  
  return 0;
}







