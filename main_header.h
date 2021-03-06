#ifndef _MAIN_HEADER_H_
#define _MAIN_HEADER_H_


#include <stdint.h>

#include <iostream>

// BigBed magic: 2273964779, little endian hex: ebf28987
// BigWig magic: 2291157574, little endian hex: 464a9088

enum bbi_type {
  wig = 2291157574,
  bed = 2273964779
};

struct main_header
{
  static const uint16_t byte_size = 64;

  uint32_t magic;
  uint16_t version;
  uint16_t zoom_levels;
  uint64_t chromosome_tree_offset;
  uint64_t full_data_offset;
  uint64_t full_index_offset;
  uint16_t field_count;
  uint16_t defined_field_count;
  uint64_t auto_sql_offset;
  uint64_t total_summary_offset;
  uint32_t uncompress_buf_size;
  uint64_t extension_offset;

  void print(std::ostream& os) const;
  void pack(std::ostream& os) const;
  void unpack(std::istream& os);

};

#endif /* _MAIN_HEADER_H_ */
