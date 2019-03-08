// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "carla/opendrive/parser/GeoReferenceParser.h"

#include "carla/Logging.h"
#include "carla/StringUtil.h"

#include <limits>
#include <sstream>

namespace carla {
namespace opendrive {
namespace parser {

  static double ParseDouble(const std::string &string_value) {
    double value;
    std::istringstream istr(string_value);
    istr.imbue(std::locale("C"));
    istr >> value;
    if (istr.fail() || !istr.eof()) {
      value = std::numeric_limits<double>::quiet_NaN();
    }
    return value;
  }

  geom::GeoLocation GeoReferenceParser::Parse(const std::string &geo_reference_string) {
    geom::GeoLocation result{
        std::numeric_limits<double>::quiet_NaN(),
        std::numeric_limits<double>::quiet_NaN(),
        0.0};

    std::vector<std::string> geo_ref_splitted;
    StringUtil::Split(geo_ref_splitted, geo_reference_string, " ");

    for (auto element: geo_ref_splitted) {
      std::vector<std::string> geo_ref_key_value;
      StringUtil::Split(geo_ref_key_value, element, "=");
      if (geo_ref_key_value.size() != 2u) {
        continue;
      }

      if (geo_ref_key_value[0] == "+lat_0") {
        result.latitude = ParseDouble(geo_ref_key_value[1]);
      } else if (geo_ref_key_value[0] == "+lon_0") {
        result.longitude = ParseDouble(geo_ref_key_value[1]);
      }
    }

    if (std::isnan(result.latitude) || std::isnan(result.longitude)) {
      log_warning("cannot parse georeference: '" + geo_reference_string + "'. Using default values.");
      result.latitude = 42.0;
      result.longitude = 2.0;
    }

    log_debug("map geo reference: latitude ", result.latitude, ", longitude ", result.longitude);

    return result;
  }

} // parser
} // opendrive
} // carla
