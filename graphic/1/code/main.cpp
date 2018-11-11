#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <unistd.h>
#include <opencv2/opencv.hpp>
using namespace cv;

struct BlockCoords {
    int row;
    int column;

    BlockCoords() {};

    BlockCoords(int row, int column) {
        this->row = row;
        this->column = column;
    }
};

Mat get_block(Mat image, BlockCoords coords, int block_size) {
    return image.rowRange(
                    coords.row * block_size, (coords.row + 1) * block_size)
            .colRange(
                    coords.column * block_size, (coords.column + 1) * block_size);
}

Scalar sad(Mat prev, Mat curr) {

    Mat result;
    absdiff(prev, curr, result);
    return sum(result);
}

BlockCoords get_matching_block(Mat prev, Mat curr, BlockCoords base,
                               int block_size, int search_area) {
    BlockCoords matching_block_coords = base;
    int min_sad = INT_MAX;
    Mat base_block = get_block(prev, base, block_size);

    int lbound_row = max(base.row - search_area, 0);
    int lbound_column = max(base.column - search_area, 0);
    int hbound_row = min(base.row + search_area, (prev.rows - 1) / block_size);
    int hbound_column = min(base.column + search_area, (prev.cols - 1) / block_size);

    for (int row = lbound_row; row <= hbound_row; row++) {
        for (int column = lbound_column; column <= hbound_column; column++) {

            BlockCoords curr_coords(row, column);
            Mat curr_block = get_block(curr, curr_coords, block_size);

            int curr_sad = sad(base_block, curr_block)[0];
            if (curr_sad < min_sad) {
                matching_block_coords = curr_coords;
                min_sad = curr_sad;
            }
        }
    }
    return matching_block_coords;
}

int main() {

//    if (argc != 3) {
//        printf("usage: DisplayImage.out <Image_Path>\n");
//        return -1;
//    }
    Mat prev = imread("/Users/Maria/Documents/git.polytest/graphic/1/code/hol01.jpg", 1);
    Mat curr = imread("/Users/Maria/Documents/git.polytest/graphic/1/code/hol02.jpg", 1);
    if (!(prev.data && curr.data)) {
        printf("No image data \n");
        return -1;
    }

    cvtColor(prev, prev, COLOR_RGB2GRAY);
    cvtColor(curr, curr, COLOR_RGB2GRAY);

    int block_size = 16;    // pixels
    int search_area = 32;    // blocks
    int img_block_rows = prev.rows / block_size;
    int img_block_columns = prev.cols / block_size;

    // #define DEBUG
#ifdef DEBUG
    int image_size = 512;
    int shift = block_size;
    Mat template_img(image_size, image_size + shift, CV_8UC1);
    randu(template_img, Scalar(0, 0, 0), Scalar(255, 255, 255));
    Mat prev_test = template_img.colRange(0, image_size);
    Mat curr_test = template_img.colRange(shift, image_size + shift);
    prev = prev_test;
    curr = curr_test;
#endif

    BlockCoords** matching_blocks = new BlockCoords*[img_block_rows];
    for (int row = 0; row < img_block_rows; row++) {
        matching_blocks[row] = new BlockCoords[img_block_columns];
    }
    for (int row = 0; row < img_block_rows; row++) {
        for (int column = 0; column < img_block_columns; column++) {
            BlockCoords curr_coords(row, column);
            matching_blocks[row][column] = get_matching_block(
                    prev, curr, curr_coords, block_size, search_area);
        }
    }

    Mat result = prev;
    for (int row = 0; row < img_block_rows; row++) {
        for (int column = 0; column < img_block_columns; column++) {
            int arrow_radius = (block_size - 1) / 2;
            int arrow_center_row = row*block_size + arrow_radius;
            int arrow_center_column = column*block_size + arrow_radius;
            float dy = matching_blocks[row][column].row - row;
            float dx = matching_blocks[row][column].column - column;
            std::cout << dx << " " << dy << std::endl;
            if (dx != 0) {
                dx = dx / pow((pow(dx, 2) + pow(dy, 2)), 0.5) * arrow_radius;
            }
            if (dx != 0) {
                dy = dy / pow((pow(dx, 2) + pow(dy, 2)), 0.5) * arrow_radius;
            }
            Point arrow_start(-dx + arrow_center_column, -dy + arrow_center_row);
            Point arrow_end(dx + arrow_center_column, dy + arrow_center_row);
            arrowedLine(result, arrow_start, arrow_end, Scalar(255,255,255), 1);

        }
    }

    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", result);
    waitKey(0);
    return 0;
}



//#include "opencv2/core.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/highgui.hpp"
////#include "opencv2/types_c.h"
//#include <vector>
//#include <stdio.h>
//#include <iostream>
//
////typedef struct CvPoint
////{
////    int x;
////    int y;
////}
////
////        CvPoint;
//
//int borders(int a, int b)
//{
//    return std::max(std::min(a, b), 0);
//}
//
//int SAD(cv::Mat mat1, cv::Mat mat2)
//{
//    int sum = 0;
//    for (int i = 0; i < mat1.size().height; i++)
//    {
//        for (int j = 0; j < mat1.size().width; j++)
//        {
//            for (int k = 0; k < 3; k++)
//            {
//                sum += abs(mat1.at<cv::Vec3b>(i, j)[k] - mat2.at<cv::Vec3b>(i, j)[k]);
//            }
//        }
//    }
//    return sum;
//}
//
//void ProccesImages(cv::Mat mat1, cv::Mat mat2)
//{
//    int height1 = mat1.size().height;
//    std::cout << height1 << std::endl;
//
//    int width1 = mat1.size().width;
//    std::cout << width1 << std::endl;
//
//    int height2 = mat2.size().height;
//    int width2 = mat2.size().width;
//
//    CvPoint a;
//    CvPoint b;
//
//    for (int j = 0; j < width1 - 8; j += 8)
//    {
//        for (int i = 0; i < height1 - 8; i += 8)
//        {
//            a.x = j;
//            a.y = i;
//            int dif = std::numeric_limits<int>::max();
//            int temp;
//
//            if (SAD(mat1(cv::Rect(j, i, 8, 8)), mat2(cv::Rect(j, i, 8, 8))) == 0)
//            {
//                b.x = j;
//                b.y = i;
//                dif = 0;
//            }
//            else
//            {
//                int c = borders(i - 16, height2);
//                int d = borders(i + 16, height2 - 8);
//                int n = borders(j - 16, width2);
//                int m = borders(j + 16, width2 - 8);
//                for (int l = n; l <= m; l += 1)
//                {
//                    for (int k = c; k <= d; k += 1)
//                    {
//                        temp = SAD(mat1(cv::Rect(j, i, 8, 8)), mat2(cv::Rect(l, k, 8, 8)));
//                        if (temp < dif)
//                        {
//                            dif = temp;
//                            b.x = l;
//                            b.y = k;
//                        }
//                    }
//                }
//                cv::arrowedLine(mat1, a, b, CV_RGB(0, 0, 255), 1, CV_AA, 0, 0.6);
//            }
//        }
//    }
//    cv::imshow("Changed photo", mat1);
//}
//
//
//int main(int argc, char** argv)
//{
//    cv::Mat mat1, mat2;
//    mat1 = cv::imread("hol01.png", 1);
//    mat2 = cv::imread("hol02.png", 1);
//
//    cv::imshow("Photo 1", mat1);
//    cv::imshow("Photo 2", mat2);
//    ProccesImages(mat1, mat2);
//    cv::waitKey(0);
//    return 0;
//}
