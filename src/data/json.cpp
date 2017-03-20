#include "json.hpp"

#include <fstream>

using data::DataNode;
using data::StepDocument;

namespace data
{

    /////////////////////////////////////////////////////////////////////////////////////////////
    // JSON PARSER
    /////////////////////////////////////////////////////////////////////////////////////////////


    //FORWARD DECLARATIONS
    std::string parse_value(StepDocument *document);
    std::string parse_string(StepDocument *document);
    DataNode<std::string> parse_array(StepDocument *document);
    DataNode<std::string> parse_object(StepDocument *document);

    DataNode<std::string> parse_json(std::string doc) { return parse_json(StepDocument(doc)); }
    DataNode<std::string> parse_json(std::vector<std::string> doc) { return parse_json(StepDocument(doc)); }

    DataNode<std::string> parse_json(StepDocument sd)
    {
        //return rootnode.
        return parse_object(&sd);
    }


    //Parses an json object
    DataNode<std::string> parse_object(StepDocument *document)
    {
        document->skip_whitespace(); //skip leading whitespace
        DataNode<std::string> object; //prepare rval

        //if object isn't being opened something went terribly wrong.
        if(!document->accept('{')) throw Exception(__PRETTY_FUNCTION__, "expected '{' at " + document->string_pos());

        //loop til eof or when broken
        while(!document->eof())
        {
            document->skip_whitespace();
            if(document->accept('}')) break; //end of object
            std::string name = parse_string(document); //parse key of object

            document->skip_whitespace();
            if(!document->accept(':')) throw Exception(__PRETTY_FUNCTION__, "expected ':' at " + document->string_pos());
            document->skip_whitespace();

            switch(document->peek()) //check what kind of value follows
            {
                case '"': object.add(name, parse_string(document)); break;
                case '{': object.add(name, parse_object(document)); break;
                case '[': object.add(name, parse_array(document)); break;
                default: object.add(name, parse_value(document));
            }

            document->skip_whitespace();
            document->accept(','); //splitter between values.
        }

        return object;
    }

    DataNode<std::string> parse_array(StepDocument *document)
    {
        document->skip_whitespace(); //shouldnt do anything tho.
        DataNode<std::string> array;

        if(!document->accept('[')) throw Exception(__PRETTY_FUNCTION__, "expected '[' at " + document->string_pos());

        while(!document->eof())
        {
            document->skip_whitespace();
            if(document->accept(']')) break; //end of array

            switch(document->peek())
            {
                case '"': array.add("", parse_string(document)); break;
                case '{': array.add("", parse_object(document)); break;
                case '[': array.add("", parse_array(document)); break;
                default: array.add("", parse_value(document));
            }

            document->skip_whitespace();
            document->accept(',');
        }

        return array;
    }

    //parses a value that is not an object, array or string.
    std::string parse_value(StepDocument *document)
    {
        document->skip_whitespace();
        std::string rval;

        while(!document->eof())
        {
            switch(document->peek())
            {
                case ',': return rval;
                case ']': return rval;
                case '}': return rval;
            }
            rval += document->accept();
        }

        throw Exception(__PRETTY_FUNCTION__, "unexpected end of file");
    }

    //Parses a string, manages some escape characters
    std::string parse_string(StepDocument *document)
    {
        document->skip_whitespace(); //always just do this cant hurt.
        if(!document->accept('"')) throw Exception(__PRETTY_FUNCTION__, "expected '\"' at " + document->string_pos());

        std::string rval;

        while(!document->eof())
        {
            if(document->accept('"')) break; //end of string
            if(document->accept('\\')) //escape character
            {
                char c = document->accept();
                switch(c)
                {
                    case 'n': rval += '\n'; break;
                    case 'r': rval += '\r'; break;
                    case 't': rval += '\t'; break;
                    case '"': rval += '"'; break;
                    case 'b': rval += '\b'; break;
                    default: rval += '\\'; rval += c; //dunno just add both.
                }
            } else {
                rval += document->accept();
            }

        }
        return rval;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////
    // JSON WRITER - Seems to be okay now, special characters may still break it.
    /////////////////////////////////////////////////////////////////////////////////////////////

    //forward declarations
    void write_array(DataNode<std::string> doc, std::ofstream &out);
    void write_object(DataNode<std::string> doc, std::ofstream &out);
    void write_string(std::string str, std::ofstream &out);
    //void writeValue(std::string val, ofstream &out);

    void write_json(DataNode<std::string> doc, std::string file)
    {
        if(doc.is_array()) throw Exception(__PRETTY_FUNCTION__, "initial object expected to contain children.");

        std::ofstream out;
        out.open(file.c_str());
        if(!out.is_open()) throw Exception(__PRETTY_FUNCTION__, "failed to open file: " + file);

        write_object(doc, out);

        out.flush();
        out.close();
    }

    void write_object(DataNode<std::string> doc, std::ofstream &out)
    {

        out << "{";

        for(size_t i = 0; i < doc.size(); ++i)
        {
            write_string(doc.get_key(i), out);
            out << ":";

            DataNode<std::string> node = doc.get_node(i);
            if(node.empty()) write_string(node.get_value(), out); //write_string or value depending on what is contained. only as string for now.
            else if(node.is_array()) write_array(node, out);
            else write_object(node, out);

            if(i != (doc.size() - 1)) out << ",";
        }

        out << "}";
    }

    void write_array(DataNode<std::string> doc, std::ofstream &out)
    {
        out << "[";

        for(size_t i = 0; i < doc.size(); ++i)
        {
            DataNode<std::string> node = doc.get_node(i);
            if(node.empty()) write_string(node.get_value(), out);
            else if(node.is_array()) write_array(node, out);
            else write_object(node, out);

            if(i != (doc.size() - 1)) out << ",";
        }

        out << "]";
    }

    void write_string(std::string str, std::ofstream &out)
    {
        str = trim(str);
        out << "\"";

        for(size_t i = 0; i < str.size(); ++i)
        {
            switch(str[i])
            {
                case '"': out << "\\\""; break;
                case '\n': out << "\\n"; break;
                case '\r': out << "\\r"; break;
                case '\t': out << "\\t"; break;
                case '\b': out << "\\b"; break;
                case '\\': out << "\\"; break;
                default: out << str[i];
            }

        }

        out << "\"";
    }

}