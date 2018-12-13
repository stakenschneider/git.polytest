#include <utility>

#include <iostream>
#include <any>
#include <OBJ_Loader.h>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cxxopts.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "render.h"
#include "Drawer.h"
#include "transformers.h"

const int FRAME_PER_SECOND = 10;
const int FRAME_COUNT = 10000;


template<int index>
float min(const std::vector<glm::vec3> &vertices) {
    float result = FLT_MAX;
    for (auto &&vex : vertices) {
        result = std::min(result, vex[index]);
    }
    return result;
}

template<int index>
float max(const std::vector<glm::vec3> &vertices) {
    float result = FLT_MIN;
    for (auto &&vex : vertices) {
        result = std::max(result, vex[index]);
    }
    return result;
}

template<int index>
float getCenter(const std::vector<glm::vec3> &vertices) {
    auto &&min_point = min<index>(vertices);
    auto &&max_point = max<index>(vertices);
    return (min_point + max_point) / 2;
}

glm::vec3 getModelCenter(const std::vector<glm::vec3> &vertices) {
    auto &&center_x = getCenter<0>(vertices);
    auto &&center_y = getCenter<1>(vertices);
    auto &&center_z = getCenter<2>(vertices);
    return glm::vec3(center_x, center_y, center_z);
}

void render(Drawer &drawer, const std::vector<glm::vec3> &vertices, const std::vector<unsigned int> &indices) {
    for (auto i = 0; i < indices.size(); i += 3) {
        Triangle triangle{vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]};
        drawer.draw(triangle);
    }
}


int main(int argc, char **argv) {
    cxxopts::Options options("Lba3", "Render teapot and maybe something else");
    std::string default_file_path = "../teapot.obj";
    std::string default_save_path = "../teapot.avi";

    options.add_options()
            ("w,width", "Width of image", cxxopts::value<int>()->default_value("800"))
            ("h,height", "Height of image", cxxopts::value<int>()->default_value("600"))
            ("s,speed", "Camera speed", cxxopts::value<float>()->default_value("2.0"))
            ("v,fovy", "fovy", cxxopts::value<float>()->default_value("-50.0"))
            ("dx", "Distance to model", cxxopts::value<int>()->default_value("120"))
            ("dy", "Distance to model", cxxopts::value<int>()->default_value("100"))
            ("f,front", "Front cut plane", cxxopts::value<float>()->default_value("0.1"))
            ("b,back", "Back cut plane", cxxopts::value<float>()->default_value("10000.0"))
            ("i,in_file", "Input filename ", cxxopts::value<std::string>()->default_value(default_file_path))
            ("o,out_file", "Output filename ", cxxopts::value<std::string>()->default_value(default_save_path));


    auto &&arguments = options.parse(argc, argv);

    auto &&width = arguments["width"].as<int>();
    auto &&height = arguments["height"].as<int>();
    auto &&speed = arguments["speed"].as<float>();
    auto &&fovy = arguments["fovy"].as<float>();
    auto &&distanceX = arguments["dx"].as<int>();
    auto &&distanceY = arguments["dy"].as<int>();
    auto &&front = arguments["front"].as<float>();
    auto &&back = arguments["back"].as<float>();
    auto &&file_name = arguments["in_file"].as<std::string>();
    auto &&res_file_name = arguments["out_file"].as<std::string>();

    objl::Loader loader;
    loader.LoadFile(file_name);
    auto &&mesh = loader.LoadedMeshes[0];

    auto &&model_vertices = ToGLMVertices().applyList<objl::Vertex, glm::vec3>(mesh.Vertices);
    auto &&model_center = getModelCenter(model_vertices);

    auto &&screen_ratio = static_cast<float>(width) / static_cast<float>(height);
    auto &&projection = glm::perspective(
            glm::radians(fovy),
            screen_ratio,
            front,
            back
    );

    float angle = 0;
    float angle_per_frame = speed / FRAME_PER_SECOND;

    auto &&start_camera_position = glm::vec4(distanceX, distanceY, 0, 1);

    //    cv::VideoWriter result(res_file_name, -1, FRAME_PER_SECOND, cv::Size(width, height));

    //    CvLineDrawer drawer(width, height);
//	LineDrawer drawer(width, height);
    TriangleDrawer drawer(width, height);


    for (auto i = 0; i < FRAME_COUNT; i++) {
        drawer.resetImage();
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0));
        glm::vec3 camera_position = (rotation_matrix * start_camera_position);
        auto &&camera = glm::lookAt(
                camera_position,
                model_center,
                glm::vec3(0, 1, 0)
        );

//		drawer.updatePipeline(std::make_unique<LineTransformationPipeline>(camera, projection, width, height));
        drawer.updatePipeline(std::make_unique<TriangleTransformationPipeline>(camera, projection, width, height));
        render(drawer, model_vertices, mesh.Indices);

//        double min, max;
//        cv::minMaxLoc(drawer.zBuffer, &min, &max);
//        cv::Mat zNorm;
//        cv::normalize(drawer.zBuffer, zNorm, 0.0, 1.0, cv::NORM_MINMAX, CV_64F);
//        cv::imshow("ZZZZ", zNorm);

        cv::imshow("Aaaa", drawer.getImage());
        //        result.write(drawer.getImage());
        cv::waitKey(2000);

        angle += angle_per_frame;
    }
    //    result.release();
    return 0;
}