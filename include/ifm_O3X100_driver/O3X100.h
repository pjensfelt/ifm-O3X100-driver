#pragma once
#include <string>
#include <vector>


namespace ifm {

    class Point {
        public:
        double x,y,z;
        Point() : x(0), y(0), z(0) {}
        Point(double x0, double y0, double z0) : x(x0),y(y0),z(z0){}
        friend std::ostream& operator<<(std::ostream& strm, const Point& p);
    };


    class O3X100 {
        public:
        void acquire_image();
        void connect_image_socket();
        void disconnect_image_socket();
        std::string get_name() const;
        std::vector<std::vector<u_char>> get_pixelmatrix() { return pixel_matrix_;};
        std::vector<Point> get_pointcloud() { return pcl_;};
        void trigger();

        private:
        std::vector<std::vector<u_char>> pixel_matrix_;
        std::vector<Point> pcl_;
        int image_socket_;
    };
}