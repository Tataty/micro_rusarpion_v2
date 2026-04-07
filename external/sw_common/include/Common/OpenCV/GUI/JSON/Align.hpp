#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Common/OpenCV/GUI/Align.hpp"

namespace gui {

NLOHMANN_JSON_SERIALIZE_ENUM(Align,
                             {
                                     { Align::TopLeft, "TopLeft" },
                                     { Align::TopRight, "TopRight" },
                                     { Align::BottomLeft, "BottomLeft" },
                                     { Align::BottomRight, "BottomRight" },
                                     { Align::Center, "Center" },
                                     { Align::LeftCenter, "LeftCenter" },
                                     { Align::TopCenter, "TopCenter" },
                                     { Align::RightCenter, "RightCenter" },
                                     { Align::BottomCenter, "BottomCenter" },
                             })
}