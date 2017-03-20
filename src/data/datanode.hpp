#ifndef EZ_DATA_DATANODE_HPP
#define EZ_DATA_DATANODE_HPP

#include <vector>
#include <utility>
#include <typeinfo>

#include "../core.hpp"

/*
    This templated class provides a node that can be used to build a data-tree
    Every childnode can have a name (names may be duplicates), and it accepts
    a templated m_value T t to store.
*/

namespace data
{

    template<typename T> class DataNode : public Object
    {
        public:
            DataNode() = default; //default empty constructor, constructs empty T
            DataNode(T t) : m_value(t) {}; //sets t as this nodes data.

            //Generates a new datanode that contains T
            void add(const std::string &key, const T &m_value)
            {
                m_children.push_back(std::make_pair(key, DataNode<T>(m_value)));
            }

            //pushes the passed datanode into m_children under the given name.
            void add(const std::string &key, const DataNode<T> &m_value)
            {
                m_children.push_back(std::make_pair(key, m_value));
            }

            //returns the data of this node.
            T get_value() const
            {
                return m_value;
            }

            void set_value(T t)
            {
                m_value = t;
            }

            //returns the m_value of the data at index x
            T get_value(int idx) const
            {
                if(!contains(idx)) throw Exception(__PRETTY_FUNCTION__, "no m_value at index " + idx);
                return m_children.at(idx).second.get_value();
            }

            std::string get_key(int idx) const
            {
                if(!contains(idx)) throw Exception(__PRETTY_FUNCTION__, "no node at index " + idx);
                return m_children.at(idx).first;
            }

            //returns the first m_value under the given key (assuming it exists).
            T get_value(const std::string &key) const
            {
                int idx = this->find(key, 0);
                if(!contains(idx)) throw Exception(__PRETTY_FUNCTION__, "no m_value under key: " + key);

                return m_children.at(idx).second.get_value();
            }

            //returns the node under the given index.
            DataNode<T> get_node(int idx) const
            {
                if(!contains(idx)) throw Exception(__PRETTY_FUNCTION__, "no node at index " + idx);
                return m_children.at(idx).second;
            }

            //returns the first node under the given key
            DataNode<T> get_node(const std::string &key) const
            {
                int idx = this->find(key, 0);
                if(!contains(idx)) throw Exception(__PRETTY_FUNCTION__, "no node under key: " + key);
                return m_children.at(idx).second;
            }

            //removes first node under this key
            void remove_first(const std::string &key)
            {
                int idx = find(key);
                if(!contains(idx)) return;
                remove(idx);
            }

            void remove_all(const std::string &key)
            {
                for(int idx = find(key); idx != -1; idx = find(key, idx))
                {
                    remove(idx);
                }
            }

            void remove(int idx)
            {
                if(!contains(idx)) return;

                auto it = m_children.begin();
                std::advance(it, idx);
                m_children.erase(it);
            }

            //searches for the key in the list starting at idx.
            //when not found or idx out of bounds returns -1. (thus this is exception safe).
            int find(const std::string &key, int idx = 0) const noexcept
            {
                if(!contains(idx)) return -1;

                for(int i = 0; (unsigned int)i < m_children.size(); ++i)
                {
                    if(m_children.at(i).first == key) return i;
                }

                return -1;
            }

            bool empty() const
            {
                return m_children.size() == 0;
            }

            //returns wether this node contains an array, or no named m_children (all keys are "").
            bool is_array() const
            {
                for(size_t i = 0; i < m_children.size(); ++i)
                {
                    if(m_children.at(i).first != "") return false;
                }
                return true;
            }

            //returns ammount of m_children.
            size_t size() const
            {
                return m_children.size();
            }

            //returns the total ammount of leafs
            size_t leaf_count() const
            {
                if(m_children.size() == 0) return 1;

                size_t rval = 0;
                for(auto child : m_children)
                {
                    rval += child.second.leaf_count();
                }
                return rval;
            }

            //returns wether this node contains a node at given index
            bool contains(int idx) const
            {
                return (idx >= 0) && ((unsigned int)idx < m_children.size());
                //return !(idx < 0 || m_children.size() >= (unsigned int)idx);
            }

            virtual std::string to_string() const
            {
                std::string s = "data::DataNode<";
                s += typeid(T).name();
                s += ">:\n";
                s += "    Containing " + std::to_string(m_children.size()) + " immediate m_children.\n";
                s += "    Tree's leafcount: " + std::to_string(leaf_count());

                return s;
            }

        protected:
            T m_value; //the data stored in the node.
            std::vector<std::pair<std::string, DataNode<T>>> m_children;
    };

    //Function generates DataNodes without template diamond
    template<typename T> DataNode<T> new_node(T t)
    {
        return DataNode<T>(t);
    }
}

#endif // EZ_DATANODE_HPP
