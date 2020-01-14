#pragma once

#include <string>

namespace HepMC3Nu {
namespace strutil {
inline std::string EncodeForHepMC3VectorStorage(std::string in) {
  std::string out = in;
  size_t next = out.find(" ");
  while (next != std::string::npos) {
    out = out.substr(0, next) + "%20" + out.substr(next + 1);
    next = out.find(" ");
  }
  return out;
}
inline std::string UnEncodeFromHepMC3VectorStorage(std::string in) {
  std::string out = in;
  size_t next = out.find("%20");
  while (next != std::string::npos) {
    out = out.substr(0, next) + " " + out.substr(next + 3);
    next = out.find("%20");
  }
  return out;
}

//Required because of bug in HepMC3::VectorStringAttribute::from_string
inline std::string SerializeStringVector(std::vector<std::string> const &strs) {
  std::stringstream ss("");

  for (size_t i = 0; i < strs.size(); ++i) {
    ss << EncodeForHepMC3VectorStorage(strs[i])
       << ((i + 1) != strs.size() ? " " : "");
  }
  return ss.str();
}

inline std::vector<std::string> UnSerializeStringVector(std::string str) {
  std::vector<std::string> strs;

  size_t next = str.find(" ");
  while (next != std::string::npos) {
    strs.push_back(UnEncodeFromHepMC3VectorStorage(str.substr(0, next)));
    str = str.substr(next + 1);
    next = str.find(" ");
  }
  if (str.size()) {
    strs.push_back(UnEncodeFromHepMC3VectorStorage(str));
  }

  return strs;
}
} // namespace strutil
} // namespace HepMC3Nu
