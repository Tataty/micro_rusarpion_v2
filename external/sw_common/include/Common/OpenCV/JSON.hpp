#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "Common/OpenCV/Common.hpp"

namespace cv {

template < typename T > void from_json(const nlohmann::json& j, Point_< T >& p) {
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
}

template < typename T > void from_json(const nlohmann::json& j, Point3_< T >& p) {
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
    j.at("z").get_to(p.z);
}

template < typename T > void from_json(const nlohmann::json& j, Size_< T >& s) {
    j.at("width").get_to(s.width);
    j.at("height").get_to(s.height);
}

template < typename T > void from_json(const nlohmann::json& j, Rect_< T >& r) {
    j.at("x").get_to(r.x);
    j.at("y").get_to(r.y);
    j.at("width").get_to(r.width);
    j.at("height").get_to(r.height);
}

inline void from_json(const json& j, cv::Scalar& s) { s = hexToScalar(j.get< std::string >()); }

}// namespace cv