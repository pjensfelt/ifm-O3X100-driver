#include "ifm_O3X100_driver/O3X100.h"
#include <opencv2/opencv.hpp>
#include <chrono>
#include <iostream>


int main(void) {

    ifm::O3X100 camera;

    std::cout << "Camera: " << camera.get_name() << std::endl;

    camera.connect_image_socket();

    while (true) {

        camera.trigger();
        camera.acquire_image();

        std::vector<std::vector<u_char>> pixels = camera.get_pixelmatrix();

        int nrows = 224;
        int ncols = 172;
        cv::Mat img(nrows,ncols,CV_8UC3);
        for (int irow = 0; irow < nrows; irow++){
            for (int icol = 0;icol < ncols; icol++) {
                u_char level = pixels[irow][icol];
                int x = icol;
                int y = irow;
                img.at<cv::Vec3b>(cv::Point(x,y)) = cv::Vec3b(level,level,level);
            }
        }

        cv::namedWindow("simpletest", cv::WINDOW_AUTOSIZE);
        cv::imshow("simpletest", img);
        cv::waitKey(1);


    }

    camera.disconnect_image_socket();

    return 0;
}

