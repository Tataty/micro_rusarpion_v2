#pragma once
#include "Common/Ballistic/JSON/BallisticTable.hpp"
#include "Common/JSON/File.hpp"
#include "Common/Test/TestFolders.hpp"

class TestBallistic {
public:
    static BallisticTable readBallisticTable(const std::string& fileName) {
        BallisticTable ballisticTable;

        readJSONFile(TEST_FILE(fileName)).get_to(ballisticTable);

        return ballisticTable;
    }
};