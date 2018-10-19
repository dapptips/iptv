/*  Copyright (C) 2014-2018 FastoGT. All right reserved.
    This file is part of iptv_cloud.
    iptv_cloud is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    iptv_cloud is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with iptv_cloud.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "utils/chunk_info.h"

#include <common/convert2string.h>
#include <common/sprintf.h>

namespace iptv_cloud {
namespace utils {

ChunkInfo::ChunkInfo() : path(), duration(0), index(0) {}

ChunkInfo::ChunkInfo(const std::string& path, uint64_t duration, uint64_t index)
    : path(path), duration(duration), index(index) {}

double ChunkInfo::GetDurationInSecconds() const {
  return duration / static_cast<double>(SECOND);
}

}  // namespace utils
}  // namespace iptv_cloud

namespace common {

std::string ConvertToString(const iptv_cloud::utils::ChunkInfo& value) {
  return MemSPrintf("%s:%llu:%llu", value.path, value.index, value.duration);
}

bool ConvertFromString(const std::string& from, iptv_cloud::utils::ChunkInfo* out) {
  if (from.empty() || !out) {
    return false;
  }

  iptv_cloud::utils::ChunkInfo res;
  std::string line;
  line.reserve(64);
  size_t pos = 0;
  for (size_t i = 0; i < from.size(); ++i) {
    char c = from[i];
    if (c == ':') {
      if (pos == 0) {
        res.path = line;
      } else {
        uint64_t ind;
        if (ConvertFromString(line, &ind)) {
          res.index = ind;
        }

        std::string lts_str = from.substr(i + 1);
        time_t lts;
        if (ConvertFromString(lts_str, &lts)) {
          res.duration = lts;
        }
        break;
      }
      line.clear();
      pos++;
    } else {
      line += c;
    }
  }

  *out = res;
  return true;
}
}  // namespace common
