#include "data_records.h"

#include <array>
#include <iomanip>

////////////////////////////////////////////////////////////////////////////
// These methods are not virtual, they are intended to be statically
// dispatched only.
// 
////////////////////////////////////////////////////////////////////////////
void data_record::print(std::ostream& os) const {
  os << std::setw(25) << std::left << "chrom_id" << chrom_id << '\n';
  os << std::setw(25) << std::left << "chrom_start" << chrom_start << '\n';
  os << std::setw(25) << std::left << "chrom_end" << chrom_end << '\n';
}
void data_record::pack(std::ostream& os) const {
  os.write((char*)&chrom_id, sizeof chrom_id);
  os.write((char*)&chrom_start, sizeof chrom_start);
  os.write((char*)&chrom_end, sizeof chrom_end);
}
void data_record::unpack(std::istream& os) {
  os.read((char*)&chrom_id, sizeof chrom_id);
  os.read((char*)&chrom_start, sizeof chrom_start);
  os.read((char*)&chrom_end, sizeof chrom_end);
}

////////////////////////////////////////////////////////////////////////////
// zoom_data_record
// 
//     common to wig and bed
// 
////////////////////////////////////////////////////////////////////////////

void zoom_data_record::print(std::ostream& os) const {
  data_record::print(os);
  os << std::setw(25) << std::left << "valid_count" << valid_count << '\n';
  os << std::setw(25) << std::left << "min_val" << min_val << '\n';
  os << std::setw(25) << std::left << "max_val" << max_val << '\n';
  os << std::setw(25) << std::left << "sum_data" << sum_data << '\n';
  os << std::setw(25) << std::left << "sum_squares" << sum_squares << '\n';
}

void zoom_data_record::pack(std::ostream& os) const {
  data_record::pack(os);
  os.write((char*)&valid_count, sizeof valid_count);
  os.write((char*)&min_val, sizeof min_val);
  os.write((char*)&max_val, sizeof max_val);
  os.write((char*)&sum_data, sizeof sum_data);
  os.write((char*)&sum_squares, sizeof sum_squares);
}

void zoom_data_record::unpack(std::istream& os) {
  data_record::unpack(os);
  os.read((char*)&valid_count, sizeof valid_count);
  os.read((char*)&min_val, sizeof min_val);
  os.read((char*)&max_val, sizeof max_val);
  os.read((char*)&sum_data, sizeof sum_data);
  os.read((char*)&sum_squares, sizeof sum_squares);
}


///////////////////////////////////////////////////////////////////////////
// bed_data_record
// 
///////////////////////////////////////////////////////////////////////////
void bed_data_record::print(std::ostream& os) const {
  data_record::print(os);
  os << std::setw(25) << std::left << "rest" << rest << '\n';
}

void bed_data_record::pack(std::ostream& os) const {
  data_record::pack(os);
  os.write(rest.c_str(), rest.size());
}

void bed_data_record::unpack(std::istream& is) {
  data_record::unpack(is);

  // FIXME: What is a reasonable upper bound for the size of the string rest?
  //        Have chosen 4 * 1024 here.
  //
  std::array<char, 4 * 1024> buff;
  is.getline(buff.data(), 4 * 1024, '\0');
  std::streamsize count = is.gcount();
  if (count > 0)
    rest.assign(buff.data(), count - 1);
}


///////////////////////////////////////////////////////////////////////////
// wig_data_record
// 
///////////////////////////////////////////////////////////////////////////
void wig_data_record::print(std::ostream& os) const {
  data_record::print(os);
  os << std::setw(25) << std::left << "item_step" << item_step << '\n';
  os << std::setw(25) << std::left << "item_span" << item_span << '\n';
  os << std::setw(25) << std::left << "type" << type << '\n';
  os << std::setw(25) << std::left << "reserved" << reserved << '\n';
  os << std::setw(25) << std::left << "item_count" << item_count << '\n';
}

void wig_data_record::pack(std::ostream& os) const {
  data_record::pack(os);
  os.write((char*)&item_step, sizeof item_step);
  os.write((char*)&item_span, sizeof item_span);
  os.write((char*)&type, sizeof type);
  os.write((char*)&reserved, sizeof reserved);
  os.write((char*)&item_count, sizeof item_count);
}

void wig_data_record::unpack(std::istream& os) {
  data_record::unpack(os);
  os.read((char*)&item_step, sizeof item_step);
  os.read((char*)&item_span, sizeof item_span);
  os.read((char*)&type, sizeof type);
  os.read((char*)&reserved, sizeof reserved);
  os.read((char*)&item_count, sizeof item_count);
}



