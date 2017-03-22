#include "image.hpp"
#include "../lib/lodepng.hpp"

namespace data
{

    Image::Image(std::string file)
    {
        read_from_file(file);
    }

    Image::Image(size_t w, size_t h)
    {
        m_width = w;
        m_height = h;
        m_pixels = nullptr;
        allocate_data();
    }

    Image::~Image()
    {
        if(m_pixels) delete[] m_pixels;
    }

    size_t Image::width() const { return m_width; }
    size_t Image::height() const { return m_height; }

    const Vector3d& Image::color_at(double x, double y) const
    {
        return m_pixels[index(x, y)];
    }

    Vector3d Image::get_pixel(size_t x, size_t y) const
    {
        return m_pixels[index(x, y)];
    }

    void Image::set_pixel(const Vector3d &color, size_t x, size_t y)
    {
        m_pixels[index(x, y)] = color;
    }

    Vector3d& Image::operator()(size_t x, size_t y)
    {
        return m_pixels[index(x, y)];
    }

    const Vector3d& Image::operator()(size_t x, size_t y) const
    {
        return m_pixels[index(x, y)];
    }

    void Image::read_from_file(std::string file)
    {
        unsigned int width, height;
        std::vector<unsigned char> image;
        unsigned int error = lodepng::decode(image, width, height, file);

        if(error != 0) throw Exception(__PRETTY_FUNCTION__, "failed to read image - " + std::string(lodepng_error_text(error)));

        m_width = width;
        m_height = height;
        allocate_data();

        auto it = image.begin();
        Vector3d *color = m_pixels;
        while(it != image.end())
        {
            color->m_x = *(it) / 255.0; it++;
            color->m_y = *(it) / 255.0; it++;
            color->m_z = *(it) / 255.0; it++;

            //ignore alpha
            it++; color++;
        }
    }

    void Image::write_to_file(std::string file) const
    {
        std::vector<unsigned char> image;
        image.reserve(m_width * m_height * 4);
        
        for(size_t h = 0; h < m_height; ++h)
            for(size_t w = 0; w < m_width; ++w)
            {
                image.push_back((unsigned char)(m_pixels[index(w, h)].m_x * 255.0));
                image.push_back((unsigned char)(m_pixels[index(w, h)].m_y * 255.0));
                image.push_back((unsigned char)(m_pixels[index(w, h)].m_z * 255.0));
                image.push_back(255);
            }

        unsigned int error = lodepng::encode(file, image.data(), m_width, m_height);
        if(error != 0) throw Exception(__PRETTY_FUNCTION__, "failed to write image - " + std::string(lodepng_error_text(error)));
    }

    std::string Image::to_string() const
    {
        std::string s = "data::Image - dimensions [";
        s += std::to_string(m_width) + ",";
        s += std::to_string(m_height) + "]";
        return s;
    }

    void Image::allocate_data()
    {
        if(m_pixels) delete[] m_pixels;
        m_pixels = new Vector3d[m_width * m_height];
    }

    size_t Image::index(size_t x, size_t y) const
    {
        return (y * m_width) + x;
    }

    size_t Image::index(double x, double y) const
    {
        size_t sx = int(x * (m_width - 1));
        size_t sy = int(y * (m_height - 1));
        return index(sx, sy);
    }

}