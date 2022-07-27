#include <opencv2/opencv.hpp>
#include <iostream>
#include <sys/time.h>

using namespace std;
using namespace cv;

void rgb_deinterleave_c(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *rgb, int len_color) {
    /*
     * Take the elements of "rgb" and store the individual colors "r", "g", and "b".
     */
    for (int i=0; i < len_color; i++) {
        r[i] = rgb[3*i];
        g[i] = rgb[3*i+1];
        b[i] = rgb[3*i+2];
    }
}

int main(int argc, char** argv)
{
        Mat src = imread("Lenna.png");
        if(src.data != nullptr)
        {
                size_t len = src.rows * src.cols;
                uint8_t *r = new uint8_t[len];
                uint8_t *g = new uint8_t[len];
                uint8_t *b = new uint8_t[len];
                timeval start, end; 
                gettimeofday(&start, NULL);
                rgb_deinterleave_c(r, g, b, src.data, len);
                gettimeofday(&end, NULL);

                double t = (end.tv_usec - start.tv_usec);
                cout << "Cost " << t << " us." << endl;
                
                cv::Size sz(src.cols, src.rows);
                cv::Mat r_img(sz, CV_8UC1, r);
                cv::Mat g_img(sz, CV_8UC1, g);
                cv::Mat b_img(sz, CV_8UC1, b);
                imwrite("r_channel.jpg", r_img);
                imwrite("g_channel.jpg", g_img);
                imwrite("b_channel.jpg", b_img);
                delete r;
                delete g;
                delete b;
                cout << "Finished." << endl;
        }
        else{
                cout << "Error when reading image." << endl;
        }
        return 0;
}