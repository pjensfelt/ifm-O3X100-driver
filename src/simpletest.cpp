#include "ifm_O3X100_driver/O3X100.h"
#include <chrono>
#include <iostream>


int main(void) {

    ifm::O3X100 camera;

    std::cout << "Camera: " << camera.get_name() << std::endl;



    return 0;
}


    // // Double size
    // std::vector<cv::Point2f> O3X100::Point2Pixel(std::vector<Point> points, cv::Size sz) {
    //     // Field of view 45 and 60 degrees according to spec
    //     // tan(30) = 1 / sqrt(3)
    //     // tan(22.5) = sqrt(2)-1
    //     // Assuming double image size

    //     int nrows = sz.height;
    //     int ncols = sz.width;

    //     double sfy = 0.41;
    //     double sfx = 0.51;

    //     std::cout << "Rows:" << nrows << " Cols:" << ncols << std::endl;

    
    //     std::vector<cv::Point2f> pixels;
    //     for (size_t j=0;j < points.size();j++) {
    //         //float dxmax = points[j].z * 1.0 / sqrt(3.0);
    //         float dxmax = points[j].z * sfx;
    //         //float dx_im = -points[j].x / dxmax * nrows/2 + nrows/2 + 41;
    //         float dx_im = -points[j].x / dxmax * nrows/2 + nrows/2 - 5 ;
    //         //float dymax = points[j].z * (sqrt(2.0)-1);
    //         float dymax = points[j].z * sfy;
    //         //float dy_im = points[j].y / dymax * ncols / 2 + ncols/2 + 3;            
    //         float dy_im = points[j].y / dymax * ncols / 2 + ncols/2 - 14;            
    //         pixels.push_back(cv::Point2f(dy_im,dx_im));
    //     }
    //     return pixels;
    // }




    // void O3X100::Store(std::string filename) {
    //     std::ofstream outfile;
    //     outfile.open( "/home/urban/images/" + filename, std::ios::app); //app is append which means it will put the text at the end
    //     int N = m_pcl.size();
    //     for (int i = 0; i < N; i++)
    //     {
    //        outfile << m_pcl[i].x << " " <<m_pcl[i].y << " " << m_pcl[i].z << std::endl;
    //     }
    //     outfile.close();
    // }

    // void O3X100::SaveImage(std::string filename) {
    //     cv::imwrite( "/home/urban/images/" + filename, m_img );
    // }


