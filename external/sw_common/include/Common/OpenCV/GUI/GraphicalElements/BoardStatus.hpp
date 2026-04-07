#pragma once
#include "Common/OpenCV/GUI/iGraphicalElement.hpp"

#include "Common/OpenCV/GUI/GraphicalElements/Label.hpp"

namespace gui {

struct BoardStatus : public iGraphicalElement< cv::Mat > {
public:
    struct Config {
        Label::Config labelConfig;
        std::string   boardTitle;
        int           margin;
    };

private:
    Config config;
    bool   isConnect;

    Label statusLabel;
    Label boardLabel;

    void drawElement(cv::Mat& image, cv::Rect busyRect) override {
        /* LABELS */
        statusLabel.setPosition(topLeft(busyRect));
        cv::Rect statusLabelBusyRect = statusLabel.getBusySpace();

        cv::Point boardLabelPosition = bottomLeft(statusLabelBusyRect);
        boardLabelPosition.y += config.margin * 2;

        boardLabel.setPosition(boardLabelPosition);

        cv::Point lineLeftPosition = bottomLeft(statusLabelBusyRect);
        lineLeftPosition.y += config.margin;

        cv::Point lineRightPosition = bottomRight(statusLabelBusyRect);
        lineRightPosition.y += config.margin;

        /* LINES */
        int lineThickness       = config.labelConfig.textThickness;
        int lineBorderThickness = lineThickness + config.labelConfig.textBorderThickness;

        if (lineBorderThickness > lineThickness && config.labelConfig.borderColor.has_value()) {
            cv::line(image,
                     lineLeftPosition,
                     lineRightPosition,
                     config.labelConfig.borderColor.value(),
                     lineBorderThickness);
        }
        if (lineThickness > 0) {
            cv::line(image, lineLeftPosition, lineRightPosition, config.labelConfig.frontColor, lineThickness);
        }

        statusLabel.draw(image);
        boardLabel.draw(image);
    }

public:
    BoardStatus(Config config) : config(config), statusLabel(config.labelConfig), boardLabel(config.labelConfig) {

        assert(config.boardTitle.size() == 2);
        boardLabel.setText(config.boardTitle);

        setStatus(false);
    }

    void setStatus(bool isConnect) {
        this->isConnect = isConnect;

        if (isConnect)
            statusLabel.setText("OK");
        else
            statusLabel.setText("XX");
    }
    bool getStatus() { return isConnect; }

    cv::Size elementSize() override {
        cv::Rect labelBusyRect = statusLabel.getBusySpace();
        return cv::Size(labelBusyRect.width, labelBusyRect.height * 2 + config.margin * 2);
    }
};

}// namespace gui
