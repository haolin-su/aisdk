#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

// 定义边界框结构
struct BBox
{
    float x;                 // bbox 左上角的 x 坐标
    float y;                 // bbox 左上角的 y 坐标
    float width;             // bbox 的宽度
    float height;            // bbox 的高度
    float confidence;        // 置信度
    int class_id;            // 类别ID
    std::vector<float> mask; // 分割掩码
};

// 计算两个框之间的交并比（IoU）
float IoU(const BBox &box1, const BBox &box2)
{
    // 计算交集的坐标
    float x1 = std::max(box1.x, box2.x);
    float y1 = std::max(box1.y, box2.y);
    float x2 = std::min(box1.x + box1.width, box2.x + box2.width);
    float y2 = std::min(box1.y + box1.height, box2.y + box2.height);

    float inter_width = std::max(0.0f, x2 - x1);
    float inter_height = std::max(0.0f, y2 - y1);
    float inter_area = inter_width * inter_height;

    // 计算每个框的面积
    float box1_area = box1.width * box1.height;
    float box2_area = box2.width * box2.height;

    // 计算并集的面积
    float union_area = box1_area + box2_area - inter_area;

    // 返回交并比
    return inter_area / union_area;
}

// NMS算法实现
std::vector<BBox> NMS(const std::vector<BBox> &bboxes, float iou_threshold)
{
    std::vector<BBox> result;
    std::vector<BBox> sorted_bboxes = bboxes;

    // 按置信度从高到低排序
    std::sort(sorted_bboxes.begin(), sorted_bboxes.end(), [](const BBox &a, const BBox &b)
              { return a.confidence > b.confidence; });

    // 标记框是否已被保留
    std::vector<bool> suppressed(sorted_bboxes.size(), false);

    // 遍历所有框
    for (size_t i = 0; i < sorted_bboxes.size(); ++i)
    {
        if (suppressed[i])
            continue; // 如果已经被抑制，跳过

        BBox &current_box = sorted_bboxes[i];
        result.push_back(current_box); // 将当前框加入结果列表

        for (size_t j = i + 1; j < sorted_bboxes.size(); ++j)
        {
            if (suppressed[j])
                continue; // 跳过已经抑制的框

            float iou = IoU(current_box, sorted_bboxes[j]);
            if (iou > iou_threshold)
            {
                suppressed[j] = true; // IoU超过阈值的框被抑制
            }
        }
    }

    return result;
}

// YOLO的推论后处理（带分割掩码）
std::vector<BBox> postProcessYOLOSegmentation(const std::vector<float> &output, int num_classes, float conf_threshold, float iou_threshold, int img_width, int img_height, int mask_dim)
{
    std::vector<BBox> bboxes;

    int num_boxes = output.size() / (5 + num_classes + mask_dim); // 根据输出的大小确定总的预测框数

    for (int i = 0; i < num_boxes; ++i)
    {
        float x = output[i * (5 + num_classes + mask_dim)];          // bbox 中心 x
        float y = output[i * (5 + num_classes + mask_dim) + 1];      // bbox 中心 y
        float width = output[i * (5 + num_classes + mask_dim) + 2];  // bbox 宽度
        float height = output[i * (5 + num_classes + mask_dim) + 3]; // bbox 高度
        float conf = output[i * (5 + num_classes + mask_dim) + 4];   // 置信度

        if (conf < conf_threshold)
            continue; // 跳过低置信度框

        // 查找置信度最高的类别
        float max_class_conf = 0.0f;
        int class_id = -1;
        for (int c = 0; c < num_classes; ++c)
        {
            float class_conf = output[i * (5 + num_classes + mask_dim) + 5 + c];
            if (class_conf > max_class_conf)
            {
                max_class_conf = class_conf;
                class_id = c;
            }
        }

        // 合并置信度
        float final_conf = conf * max_class_conf;

        // 提取掩码
        std::vector<float> mask(mask_dim);
        for (int j = 0; j < mask_dim; ++j)
        {
            mask[j] = output[i * (5 + num_classes + mask_dim) + 5 + num_classes + j];
        }

        // 转换bbox的坐标
        BBox bbox;
        bbox.x = (x - width / 2) * img_width;   // bbox左上角 x
        bbox.y = (y - height / 2) * img_height; // bbox左上角 y
        bbox.width = width * img_width;         // bbox 宽度
        bbox.height = height * img_height;      // bbox 高度
        bbox.confidence = final_conf;           // 最终置信度
        bbox.class_id = class_id;
        bbox.mask = mask; // 分配掩码

        bboxes.push_back(bbox);
    }

    // 应用NMS
    return NMS(bboxes, iou_threshold);
}

// 测试代码
int main()
{
    // 模拟YOLO Segmentation网络输出：[1, num_predictions, 85 + mask_dim]，展开为一维数组
    int mask_dim = 32 * 32;                                           // 掩码的尺寸
    std::vector<float> yolo_output((5 + 80 + mask_dim) * 6300, 0.5f); // 假设所有值都为0.5，仅用于演示
    int num_classes = 80;                                             // 类别数量（例如COCO）
    float conf_threshold = 0.5f;
    float iou_threshold = 0.4f;
    int img_width = 640;
    int img_height = 480;

    // 后处理输出
    std::vector<BBox> bboxes = postProcessYOLOSegmentation(yolo_output, num_classes, conf_threshold, iou_threshold, img_width, img_height, mask_dim);

    // 打印结果
    for (const auto &bbox : bboxes)
    {
        std::cout << "BBox: x=" << bbox.x << ", y=" << bbox.y << ", width=" << bbox.width << ", height=" << bbox.height
                  << ", confidence=" << bbox.confidence << ", class_id=" << bbox.class_id << std::endl;
        for (auto &m : bbox.mask)
        {
            std::cout << m << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
