#ifndef EZ_DATA_JSON_HPP
#define EZ_DATA_JSON_HPP

#include "datanode.hpp"
#include "stepdocument.hpp"
#include "../core.hpp"

namespace data
{

    typedef data::DataNode<std::string> jsonNode;

    jsonNode parse_json(std::string doc);
    jsonNode parse_json(data::StepDocument sd);
    jsonNode parse_json(std::vector<std::string> doc);

    //write json
    void write_json(data::DataNode<std::string> doc, std::string file);
}

#endif // EZ_ALGORITHM_JSON_HPP
