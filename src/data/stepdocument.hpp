#ifndef EZ_DATA_STEPDOCUMENT_HPP
#define EZ_DATA_STEPDOCUMENT_HPP

#include <vector>
#include "../core.hpp"

namespace data
{
    class StepDocument : public Object
    {
        protected:
            size_t m_line; //currently selected line
            size_t m_caret; //current 'selected' char
            std::vector<std::string> m_document; //document contained.

            void increment_caret(); //safely moves caret.

        public:
            StepDocument(); //initializes empty
            StepDocument(const std::string &doc); //initializes with doc in 1 string.
            StepDocument(std::vector<std::string> doc); //initializes with given doc.

            bool eof() const; //returns whether the end of the file is 'selected'
            bool peek(char c) const; //returns whether the selected char is c.
            bool accept(char c); //peek, but increments caret when true.

            char peek() const; //returns currently selected char
            char accept(); //returns currently selected char and increments caret.

            void jump(size_t line, size_t pos); //jumps caret to line:index
            void reset(); //returns caret to line 0 index 0.
            void reset_line(); //returns caret to start of current line.

            void add_line(const std::string &str); //adds line to end of document.

            size_t lines() const; //returns amount of lines in the doc
            size_t line_size() const; //returns size of current line.
            size_t character_count() const; //returns total amount of all characters

            std::vector<std::string> get_document() const; //returns document
            std::string string_pos() const; //returns stringed version of line/pos.
            size_t current_pos() const; //returns current caret index.
            size_t current_line() const; //returns currently selected line

            void skip(const std::string &ws); //skips any and all specific characters.
            void skip_whitespace(); //skips whitespace any and all of " \t\n\r".

            //EzBase virtuals
            virtual std::string to_string() const;
    };
}

#endif // EZ_DATA_STEPDOCUMENT_HPP
