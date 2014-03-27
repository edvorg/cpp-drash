#ifndef DIMENSIONS_HPP
#define DIMENSIONS_HPP

namespace drash {

    // TODO extend with more complex dimensions definition (maby by polygons)
    // dimensions definition using object radius
    class Dimensions {
    public:
        Dimensions() = default;
        Dimensions(float newX, float newY, float newRad);

        inline bool Intersect(const Dimensions & another) const;

    protected:
    private:
        float x = 0;
        float y = 0;
        float rad = 0;
    };

    inline bool Dimensions::Intersect(const Dimensions & another) const {
        float deltax = x - another.x;
        float deltay = y - another.y;
        float rads = rad + another.rad;
        return (deltax * deltax + deltay * deltay < rads * rads);
    }
}

#endif// DIMENSIONS_HPP
