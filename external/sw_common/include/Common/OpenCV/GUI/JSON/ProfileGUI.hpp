#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "Common/Ballistic/JSON/BallisticTable.hpp"
#include "Common/OpenCV/GUI/JSON/Align.hpp"
#include "Common/OpenCV/GUI/JSON/iGUI.hpp"
#include "Common/OpenCV/GUI/ProfileGUI.hpp"
#include "Common/OpenCV/JSON.hpp"
#include "Common/System/ImageReader.hpp"
#include "Common/System/JSON/JSONReader.hpp"

inline void from_json(const json& j, ProfileGUI::CameraLayoutConfig& pc) {
    j.at("cameraId").get_to(pc.cameraId);
    j.at("relativePosition").get_to(pc.relativePosition);
    j.at("relativeSize").get_to(pc.relativeSize);
    j.at("alignment").get_to(pc.alignment);
    j.at("title").get_to(pc.title);
    j.at("isAiming").get_to(pc.isAiming);
}

inline void from_json(const json& j, ProfileGUI::Config& p) {
    j.at("style").get_to(p.style);
    j.at("listCameraLayoutConfig").get_to(p.listCameraLayoutConfig);

    j.at("resolution").get_to(p.resolution);
    p.defaultImage = ImageReader::readImage(j.at("defaultImage").get< std::string >());

    std::string ballisticTableFile = j.at("ballisticTable").get< std::string >();
    JSONReader::readJSON(ballisticTableFile).get_to(p.ballisticTable);

    j.at("pitchTagsIntervalDistance").get_to(p.pitchTagsIntervalDistance);
    j.at("pitchTagsMaxDistance").get_to(p.pitchTagsMaxDistance);
    j.at("pitchTagWidth").get_to(p.pitchTagWidth);

    j.at("crosshairRadius").get_to(p.crosshairRadius);
    j.at("crosshairThickness").get_to(p.crosshairThickness);
}