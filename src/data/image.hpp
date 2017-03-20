#ifndef DATA_IMAGE_HPP
#define DATA_IMAGE_HPP

#include "../core.hpp"

namespace data 
{

    class Image : public Object
    {
    public:
        Image(std::string file);
        Image(size_t w = 0, size_t h = 0);
        Image(const Image&) = delete;
        virtual ~Image();

        //simple getters
        size_t width() const;
        size_t height() const;

        //normalized access (between 0-1)
        const Vector3d& color_at(double x, double y) const;

        //other accessors
        Vector3d get_pixel(size_t x, size_t y) const;
        void set_pixel(const Vector3d &color, size_t x, size_t y);
        Vector3d& operator()(size_t x, size_t y);
        const Vector3d& operator()(size_t x, size_t y) const;

        //read/write from/to file
        void read_from_file(std::string file);
        void write_to_file(std::string file) const;

        virtual std::string to_string() const;

    protected:
        size_t m_width;
        size_t m_height;
        Vector3d *m_pixels;

        void allocate_data();
        size_t index(size_t x, size_t y) const;
        size_t index(double x, double y) const;
    };

}

#endif