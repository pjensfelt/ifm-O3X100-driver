#include "ifm_O3X100_driver/O3X100.h"
#include <math.h>  
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <chrono>
#include <thread>

namespace ifm {

    const char * CAMERA_IP = "192.168.0.69";

    // std::cout for the Point class
    std::ostream& operator<<(std::ostream& strm, const Point& p) {
        return strm << "(" << p.x << "," << p.y << "," << p.z << ")";
    }
    
    void O3X100::acquire_image() {

        char buffer[16];
        size_t bufsize = sizeof(buffer);
        size_t total = 0;
        size_t n_received = 0;
        std::cout << "Start receiving header..." << std::endl;
        while(true)    
        {    
            n_received = recv(image_socket_, buffer + total , bufsize - total, 0);    
            if (n_received <= 0)    
            {    
                break;    
            }    
            else {
                total += n_received;
            }    
        }

        int data_length = atoi(buffer+5);
        std::cout << "Data length: " << data_length << std::endl;

        char data_buffer[data_length];
        total = 0;
        n_received = 0;
        std::cout << "Start receiving payload..." << std::endl;
        while(total < data_length)    
        {    
            n_received = read(image_socket_, data_buffer + total , data_length - total);    
            if (n_received < 1 )    
            {
                std::cout << "Error!" << std::endl;    
                break;    
            }    
            else {
                total += n_received;
            }    
        }    

        int ncols = 172;
        int nrows = 224;

        float max_f = 0;
        int chunk_offs = 224 * 172 * 4 + 48;
        float pixel_values[nrows*ncols];
        for(int icol = 0; icol < ncols; icol++)
        {
            for(int irow = 0; irow < nrows; irow++)
            {
                int ibase = chunk_offs + (nrows * icol + irow) * 4 + 48 + 8;

                u_char c0 = data_buffer[ibase];
                u_char c1 = data_buffer[ibase+1];
                u_char c2 = data_buffer[ibase+2];
                u_char c3 = data_buffer[ibase+3];

                float pixel_value;
                *((u_char*)(&pixel_value) + 3) = c3;
                *((u_char*)(&pixel_value) + 2) = c2;
                *((u_char*)(&pixel_value) + 1) = c1;
                *((u_char*)(&pixel_value) + 0) = c0;

                max_f = std::max(max_f,std::log(pixel_value));
                pixel_values[nrows * icol + irow] = log(pixel_value);

            }
        }

        pixel_matrix_ = std::vector<std::vector<u_char>>(nrows,std::vector<u_char>(ncols));

        for(int icol = 0; icol < ncols; icol++)
        {
            for(int irow = 0; irow < nrows; irow++)
            {
                u_char v = 240 * pixel_values[nrows * icol + irow] / max_f;
                pixel_matrix_[nrows-irow-1][icol] = v;
            }
        }
        
        float x_values[nrows*ncols];
        float y_values[nrows*ncols];
        float z_values[nrows*ncols];
        for(int icol = 0; icol < ncols; icol++)
        {
            for(int irow = 0; irow < nrows; irow++)
            {
                int ibasex = chunk_offs *2 + (nrows * icol + irow) * 4 + 48 + 8;
                u_char c0x = data_buffer[ibasex];
                u_char c1x = data_buffer[ibasex+1];
                u_char c2x = data_buffer[ibasex+2];
                u_char c3x = data_buffer[ibasex+3];
                float x_value;
                *((u_char*)(&x_value) + 3) = c3x;
                *((u_char*)(&x_value) + 2) = c2x;
                *((u_char*)(&x_value) + 1) = c1x;
                *((u_char*)(&x_value) + 0) = c0x;
                x_values[nrows * icol + irow] = x_value;

                int ibasey = chunk_offs *3 + (nrows * icol + irow) * 4 + 48 + 8;
                u_char c0y = data_buffer[ibasey];
                u_char c1y = data_buffer[ibasey+1];
                u_char c2y = data_buffer[ibasey+2];
                u_char c3y = data_buffer[ibasey+3];
                float y_value;
                *((u_char*)(&y_value) + 3) = c3y;
                *((u_char*)(&y_value) + 2) = c2y;
                *((u_char*)(&y_value) + 1) = c1y;
                *((u_char*)(&y_value) + 0) = c0y;
                y_values[nrows * icol + irow] = y_value;

                int ibasez = chunk_offs *4 + (nrows * icol + irow) * 4 + 48 + 8;
                u_char c0z = data_buffer[ibasez];
                u_char c1z = data_buffer[ibasez+1];
                u_char c2z = data_buffer[ibasez+2];
                u_char c3z = data_buffer[ibasez+3];
                float z_value;
                *((u_char*)(&z_value) + 3) = c3z;
                *((u_char*)(&z_value) + 2) = c2z;
                *((u_char*)(&z_value) + 1) = c1z;
                *((u_char*)(&z_value) + 0) = c0z;
                z_values[nrows * icol + irow] = z_value;

            }
        }
        

        pcl_.clear();
        for (int i = 0; i < ncols*nrows; i++)
        {
            Point p(x_values[i],y_values[i],z_values[i]);
            pcl_.push_back(p);
        }

    }


    void O3X100::connect_image_socket() {

        image_socket_ = socket(AF_INET,SOCK_STREAM,0);
        if(image_socket_ <0)
        {
            std::cout << "Creating socket failed!\n";
            close(image_socket_);
            return;
        }

        struct sockaddr_in addr_image;
        addr_image.sin_family = AF_INET;
        addr_image.sin_port = htons(50010);
        inet_aton(CAMERA_IP, &addr_image.sin_addr);
        int error = connect(image_socket_, (sockaddr*)&addr_image, sizeof(addr_image));
        if(error!=0)
        {
            std::cout << "Connecting to camera failed!\n";
            close(image_socket_);
            return;
        }
        
        std::cout << "Connected!" << std::endl;

    }

    void O3X100::disconnect_image_socket() {

        close(image_socket_);

    }

    std::string O3X100::get_name() const {
        return "O3X100";
    }

    void O3X100::trigger() {

        int command_socket = socket(AF_INET,SOCK_STREAM,0);
        if(command_socket <0)
        {
            std::cout << "Creating command socket failed!\n";
            close(command_socket);
            return;
        }

        struct sockaddr_in addr_com;
        addr_com.sin_family = AF_INET;
        addr_com.sin_port = htons(80);
        inet_aton(CAMERA_IP, &addr_com.sin_addr);
        int error = connect(command_socket,(sockaddr*)&addr_com, sizeof(addr_com));
        if(error!=0)
        {
            std::cout << "Connecting to camera failed!\n";
            close(command_socket);
            return;
        }
        
        std::stringstream payload;
        payload << "<?xml version='1.0'?>\r\n";
        payload << "<methodCall>\r\n";
        payload << "<methodName>trigger</methodName>\r\n";
        payload << "</methodCall>\r\n";
        std::stringstream msg;
        msg << "POST /api/rpc/v1/com.ifm.efector/ HTTP/1.1\r\n";
        msg << "Host: " + std::string(CAMERA_IP) + "\r\n";
        msg << "Content-Type: text/xml\r\n";
        msg << "User-Agent: Python-xmlrpc/3.6\r\n";
        msg << "Content-Length: " << payload.str().size() << "\r\n";
        msg << "\r\n";
        msg << payload.str();
        char msgbuf[1024];
        strcpy(msgbuf, msg.str().c_str());
        std::cout << msg.str();
        std::cout << msg.str().size() << std::endl;
        send(command_socket, msgbuf, sizeof(msgbuf),0);
        close(command_socket);

    }
}