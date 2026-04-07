#pragma once
#include "Common/OpenCV/GUI/iGUI.hpp"
#include "Common/OpenCV/JSON.hpp"
#include "Common/System/FontReader.hpp"

inline void from_json(const json& j, iGUI::Style& g) {
    const json& jColor = j.at("color");
    jColor.at("primary").get_to(g.color.primary);
    jColor.at("secondary").get_to(g.color.secondary);
    jColor.at("enabled").get_to(g.color.enabled);
    jColor.at("disabled").get_to(g.color.disabled);
    jColor.at("blink").get_to(g.color.blink);

    const json& jVisual = j.at("visual");
    jVisual.at("lineThickness").get_to(g.visual.lineThickness);
    jVisual.at("borderThickness").get_to(g.visual.borderThickness);
    jVisual.at("smallFontHeight").get_to(g.visual.smallFontHeight);
    jVisual.at("mediumFontHeight").get_to(g.visual.mediumFontHeight);

    g.visual.font = FontReader::readFont(jVisual.at("font").get< std::string >());
}