#include "stepdocument.hpp"

namespace data
{
    //The heavyweight utility of this, this crosses the lines etc.
    void StepDocument::increment_caret()
    {
        if(m_line >= m_document.size()) return;

        ++m_caret;
        while(m_caret >= m_document.at(m_line).size()) //end of line
        {
            m_caret = 0;
            ++m_line;
            if(m_line >= m_document.size()) //beyond last line
            {
                m_line = m_document.size();
                return;
            }
        }
    }

    StepDocument::StepDocument()
    {
        m_line = 0;
        m_caret = 0;
    }

    StepDocument::StepDocument(const std::string &doc) : StepDocument()
    {
        m_document.push_back(doc);
    }

    StepDocument::StepDocument(std::vector<std::string> doc) : StepDocument()
    {
        m_document = doc;
    }

    bool StepDocument::eof() const
    {
        return m_line == m_document.size();
    }

    bool StepDocument::peek(char c) const
    {
        if(eof()) return false;
        return m_document.at(m_line).at(m_caret) == c;
    }

    bool StepDocument::accept(char c)
    {
        if(peek(c))
        {
            increment_caret();
            return true;
        }
        return false;
    }

    char StepDocument::peek() const
    {
        if(eof()) return '\0';
        return m_document.at(m_line).at(m_caret);
    }

    char StepDocument::accept()
    {
        char rval = peek();
        increment_caret();
        return rval;
    }

    //when line or pos too big it will go to next line or give EOF.
    void StepDocument::jump(size_t line, size_t pos)
    {
        m_line = line;
        m_caret = pos;
    }

    void StepDocument::reset()
    {
        m_line = 0;
        m_caret = 0;
    }

    void StepDocument::reset_line()
    {
        m_caret = 0;
    }

    void StepDocument::add_line(const std::string &str)
    {
        m_document.push_back(str);
    }

    size_t StepDocument::lines() const
    {
        return m_document.size();
    }

    size_t StepDocument::line_size() const
    {
        if(eof()) return 0;
        return m_document.at(m_line).size();
    }

    size_t StepDocument::character_count() const
    {
        size_t rval = 0;
        for(std::string line : m_document)
        {
            rval += line.size();
        }
        return rval;
    }

    std::vector<std::string> StepDocument::get_document() const
    {
        return m_document;
    }

    std::string StepDocument::string_pos() const
    {
        return std::to_string(m_line) + ":" + std::to_string(m_caret);
    }

    size_t StepDocument::current_pos() const
    {
        return m_caret;
    }

    size_t StepDocument::current_line() const
    {
        return m_line;
    }

    void StepDocument::skip(const std::string &ws)
    {
        while(!eof() && ws.find(m_document.at(m_line).at(m_caret)) != std::string::npos)
        {
            increment_caret();
        }
    }

    void StepDocument::skip_whitespace()
    {
        this->skip(" \t\n\r");
    }

    std::string StepDocument::to_string() const
    {
        std::string s = "ez::data::StepDocument\n";
        s += "    Containing " + std::to_string(m_document.size()) + " lines.\n";
        s += "    Current line/caret: " + std::to_string(m_line) + "/" + std::to_string(m_caret);
        return s;
    }
}
