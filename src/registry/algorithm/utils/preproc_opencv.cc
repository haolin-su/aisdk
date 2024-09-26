#include "preproc_opencv.h"

namespace aisdk{

cv::Mat CenterCropAndResize(cv::Mat img, int resize_shortedge = 256, int center_crop_size = 224)
    {
    // 计算缩放因子
    int height = img.rows;
    int width = img.cols;
    float scale;
    int new_height, new_width;

    if (height < width) {
        scale = static_cast<float>(resize_shortedge) / static_cast<float>(height);
        new_height = resize_shortedge;
        new_width = static_cast<int>(static_cast<float>(width) * scale);
    } else {
        scale = static_cast<float>(resize_shortedge) / static_cast<float>(width);
        new_width = resize_shortedge;
        new_height = static_cast<int>(static_cast<float>(height) * scale);
    }

    // 缩放图片
    cv::Mat resized;
    cv::Size size(new_width, new_height);
    cv::resize(img, resized, size, 0, 0, cv::INTER_AREA); // 使用INTER_AREA（等价于双线性插值）

    // 计算中心裁剪的 ROI
    int x = (resized.cols - center_crop_size) / 2;
    int y = (resized.rows - center_crop_size) / 2;
    cv::Rect roi(x, y, center_crop_size, center_crop_size);

    // 执行裁剪
    cv::Mat cropped = resized(roi);

    return cropped;
}

int ResnetPreprocOpencv(const cv::Mat& input_mat, cv::Mat& output_mat, int model_h, int model_w)
{
    cv::Mat img;

    if (input_mat.channels() == 1)
        cv::cvtColor(input_mat, img, cv::COLOR_GRAY2RGB);
    else
        cv::cvtColor(input_mat, img, cv::COLOR_BGR2RGB);

    // cv::Mat re(model_h, model_w, CV_8UC3);

    // cv::resize(img, re, re.size(), 0, 0, cv::INTER_LINEAR);
    cv::Mat re = CenterCropAndResize(img, 256, model_w);
    // cv::imwrite("./classify_preproc.jpg", re);
    float mean[] = {123.675, 116.28, 103.53};
    float std[] = {1 / 58.395, 1 / 57.12, 1 / 57.375};
    cv::Mat out_float = re.clone();

    for (int i = 0; i < out_float.rows; ++i)
    {
        cv::Vec3f *p = out_float.ptr<cv::Vec3f>(i);
        for (int j = 0; j < out_float.cols; ++j)
        {
            p[j][0] = (p[j][0] - mean[0]) * std[0];
            p[j][1] = (p[j][1] - mean[1]) * std[1];
            p[j][2] = (p[j][2] - mean[2]) * std[2];
        }
    }

    std::vector<cv::Mat> bgrChannels;
    bgrChannels.emplace_back(model_w, model_h, CV_32F);
    bgrChannels.emplace_back(model_w, model_h, CV_32F);
    bgrChannels.emplace_back(model_w, model_h, CV_32F);
    cv::split(out_float, bgrChannels);

    for (int i = 0; i < 3; i++)
    {
        std::memcpy((float *)output_mat.data + i * model_w * model_h, bgrChannels[i].data, model_w * model_h * sizeof(float));
    }
    
    
    return 0;
}

int YoloPreprocOpencv(const cv::Mat& input_mat, cv::Mat& output_mat, int model_h, int model_w) 
{
    cv::Mat img;

    if (input_mat.channels() == 1)
        cv::cvtColor(input_mat, img, cv::COLOR_GRAY2RGB);
    else
        cv::cvtColor(input_mat, img, cv::COLOR_BGR2RGB);

    int w, h, x, y;

    int input_mat_w_ = img.cols;
    int input_mat_h_ = img.rows;
    float r_w = model_h / (input_mat_w_ * 1.0);
    float r_h = model_w / (input_mat_h_ * 1.0);

    if (r_h > r_w)
    {
        w = model_h;
        h = r_w * input_mat_h_;
        x = 0;
        y = (model_w - h) / 2;
    }
    else
    {
        w = r_h * input_mat_w_;
        h = model_w;
        x = (model_h - w) / 2;
        y = 0;
    }
    cv::Mat re(h, w, CV_8UC3);
    cv::Mat out(model_w, model_h, CV_8UC3, cv::Scalar(114, 114, 114));

    cv::resize(img, re, re.size(), 0, 0, cv::INTER_LINEAR);

    re.copyTo(out(cv::Rect(x, y, re.cols, re.rows)));

    cv::Mat out_float;
    out.convertTo(out_float, CV_32FC3, 1 / 255.0);

    std::vector<cv::Mat> bgrChannels;
    bgrChannels.emplace_back(model_w, model_h, CV_32F);
    bgrChannels.emplace_back(model_w, model_h, CV_32F);
    bgrChannels.emplace_back(model_w, model_h, CV_32F);
    cv::split(out_float, bgrChannels);

    for (int i = 0; i < 3; i++)
    {
        std::memcpy((float *)output_mat.data + i * model_w * model_h, bgrChannels[i].data, model_w * model_h * sizeof(float));
    }

    return 0;
}

}