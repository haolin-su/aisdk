/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:    cv_api_jni.cc

  Author:  galis.huang
  Version: 1.0
  Date:  2024.10

  Description:   CV model APIs

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** galis.huang                    09-Oct-2024     LIBtt54238   Init
** -----------------------------------------------------------------------------
******************************************************************************/
#include <jni.h>
#include <string>
#include <map>
#include <opencv2/opencv.hpp>
#include <mutex>
#include "fibo/api/api_cv.h"
#include "fibo/model/model_cv.h"
#include "fibo/result.h"

using namespace aisdk;
using namespace aisdk::result;

struct CvApiCache{
    JavaVM *jvm{nullptr};
    jobject globalRef{nullptr};
    CVInferAsyncCallback callback{nullptr};
};
std::mutex gCvApiCacheMutex;
std::map<long,CvApiCache> gCvApiCache;

FIBO_CV_Img JavaMatToFiboImg(JNIEnv *pEnv, jobject pJobject) {
    if (pJobject == nullptr) {
        return FIBO_CV_Img();
    }
    jclass matClass = pEnv->GetObjectClass(pJobject);
    jmethodID mid = pEnv->GetMethodID(matClass, "getNativeObjAddr", "()J");
    jlong addr = pEnv->CallLongMethod(pJobject, mid);
    cv::Mat mat = *(cv::Mat *) (addr);
    FIBO_CV_Img fiboImg;
    fiboImg.width = mat.cols;
    fiboImg.height = mat.rows;
    fiboImg.channels = mat.channels();
    fiboImg.size = mat.total() * mat.elemSize();
    fiboImg.data.assign(mat.datastart, mat.dataend);
    return fiboImg;
}

FIBO_CV_Img CppMatToFiboImg(JNIEnv *pEnv, cv::Mat mat) {
    FIBO_CV_Img fiboImg;
    fiboImg.width = mat.cols;
    fiboImg.height = mat.rows;
    fiboImg.channels = mat.channels();
    fiboImg.size = mat.total() * mat.elemSize();
    fiboImg.data.assign(mat.datastart, mat.dataend);
    return fiboImg;
}

void SetClassifyResult(JNIEnv *env, jobject resultcv, const aisdk::result::ResultCv &resultCv) {
    // 设置分类结果
    if (resultCv.classify.count > 0) {
        jobject classifyObj = env->AllocObject(
                env->FindClass("com/fibocom/ai/sdk/Result$ResultCvClassify"));
        jfieldID fidClassifyCount = env->GetFieldID(env->GetObjectClass(classifyObj), "count", "I");
        jfieldID fidClassifyObjects = env->GetFieldID(env->GetObjectClass(classifyObj), "objects",
                                                      "[Lcom/fibocom/ai/sdk/Result$ResultCvClassifyObject;");

        env->SetIntField(classifyObj, fidClassifyCount, resultCv.classify.count);
        jobjectArray objectsArray = env->NewObjectArray(resultCv.classify.count, env->FindClass(
                "com/fibocom/ai/sdk/Result$ResultCvClassifyObject"), nullptr);

        for (int i = 0; i < resultCv.classify.count; ++i) {
            jobject object = env->AllocObject(
                    env->FindClass("com/fibocom/ai/sdk/Result$ResultCvClassifyObject"));
            env->SetObjectField(object, env->GetFieldID(env->GetObjectClass(object), "label",
                                                        "Ljava/lang/String;"),
                                env->NewStringUTF(resultCv.classify.objects[i].label.c_str()));
            env->SetFloatField(object, env->GetFieldID(env->GetObjectClass(object), "score", "F"),
                               resultCv.classify.objects[i].score);
            env->SetIntField(object, env->GetFieldID(env->GetObjectClass(object), "classId", "I"),
                             resultCv.classify.objects[i].class_id);
            env->SetObjectArrayElement(objectsArray, i, object);
        }

        env->SetObjectField(classifyObj, fidClassifyObjects, objectsArray);
        env->SetObjectField(resultcv, env->GetFieldID(env->GetObjectClass(resultcv), "classify",
                                                      "Lcom/fibocom/ai/sdk/Result$ResultCvClassify;"),
                            classifyObj);
    }
}

void SetDetectResult(JNIEnv *env, jobject resultcv, const aisdk::result::ResultCv &resultCv) {
    // 设置检测结果
    if (resultCv.detect.count > 0) {
        jobject detectObj = env->AllocObject(
                env->FindClass("com/fibocom/ai/sdk/Result$ResultCvDetect"));
        jfieldID fidDetectCount = env->GetFieldID(env->GetObjectClass(detectObj), "count", "I");
        jfieldID fidDetectObjects = env->GetFieldID(env->GetObjectClass(detectObj), "objects",
                                                    "[Lcom/fibocom/ai/sdk/Result$ResultCvDetectObject;");

        env->SetIntField(detectObj, fidDetectCount, resultCv.detect.count);
        jobjectArray detectObjectsArray = env->NewObjectArray(resultCv.detect.count, env->FindClass(
                "com/fibocom/ai/sdk/Result$ResultCvDetectObject"), nullptr);

        for (int i = 0; i < resultCv.detect.count; ++i) {
            jobject detectObject = env->AllocObject(
                    env->FindClass("com/fibocom/ai/sdk/Result$ResultCvDetectObject"));
            env->SetObjectField(detectObject,
                                env->GetFieldID(env->GetObjectClass(detectObject), "label",
                                                "Ljava/lang/String;"),
                                env->NewStringUTF(resultCv.detect.objects[i].label.c_str()));
            env->SetFloatField(detectObject,
                               env->GetFieldID(env->GetObjectClass(detectObject), "score", "F"),
                               resultCv.detect.objects[i].score);
            env->SetIntField(detectObject,
                             env->GetFieldID(env->GetObjectClass(detectObject), "classId", "I"),
                             resultCv.detect.objects[i].class_id);

            // 设置 Bbox
            jobject bboxObj = env->AllocObject(env->FindClass("com/fibocom/ai/sdk/Result$Bbox"));
            env->SetIntField(bboxObj, env->GetFieldID(env->GetObjectClass(bboxObj), "x", "I"),
                             resultCv.detect.objects[i].bbox.x);
            env->SetIntField(bboxObj, env->GetFieldID(env->GetObjectClass(bboxObj), "y", "I"),
                             resultCv.detect.objects[i].bbox.y);
            env->SetIntField(bboxObj, env->GetFieldID(env->GetObjectClass(bboxObj), "w", "I"),
                             resultCv.detect.objects[i].bbox.w);
            env->SetIntField(bboxObj, env->GetFieldID(env->GetObjectClass(bboxObj), "h", "I"),
                             resultCv.detect.objects[i].bbox.h);

            env->SetObjectField(detectObject,
                                env->GetFieldID(env->GetObjectClass(detectObject), "bbox",
                                                "Lcom/fibocom/ai/sdk/Result$Bbox;"), bboxObj);
            env->SetObjectArrayElement(detectObjectsArray, i, detectObject);
        }
        env->SetObjectField(detectObj, fidDetectObjects, detectObjectsArray);
        env->SetObjectField(resultcv, env->GetFieldID(env->GetObjectClass(resultcv), "detect",
                                                      "Lcom/fibocom/ai/sdk/Result$ResultCvDetect;"),
                            detectObj);
    }
}

void SetPoseResult(JNIEnv *env, jobject resultcv, const aisdk::result::ResultCv &resultCv) {
    // 设置姿态识别结果
    if (resultCv.pose.count > 0) {
        jobject poseObj = env->AllocObject(
                env->FindClass("com/fibocom/ai/sdk/Result$ResultCvKeypoint"));
        jfieldID fidPoseCount = env->GetFieldID(env->GetObjectClass(poseObj), "count", "I");
        jfieldID fidPoseObjects = env->GetFieldID(env->GetObjectClass(poseObj), "objects",
                                                  "[Lcom/fibocom/ai/sdk/Result$ResultCvKeypointObject;");

        env->SetIntField(poseObj, fidPoseCount, resultCv.pose.count);
        jobjectArray poseObjectsArray = env->NewObjectArray(resultCv.pose.count, env->FindClass(
                "com/fibocom/ai/sdk/Result$ResultCvKeypointObject"), nullptr);

        for (int i = 0; i < resultCv.pose.count; ++i) {
            jobject poseObject = env->AllocObject(
                    env->FindClass("com/fibocom/ai/sdk/Result$ResultCvKeypointObject"));
            env->SetObjectField(poseObject,
                                env->GetFieldID(env->GetObjectClass(poseObject), "label",
                                                "Ljava/lang/String;"),
                                env->NewStringUTF(resultCv.pose.objects[i].label.c_str()));
            env->SetFloatField(poseObject,
                               env->GetFieldID(env->GetObjectClass(poseObject), "score", "F"),
                               resultCv.pose.objects[i].score);
            env->SetIntField(poseObject,
                             env->GetFieldID(env->GetObjectClass(poseObject), "classId", "I"),
                             resultCv.pose.objects[i].class_id);

            // 设置 Bbox
            jobject poseBboxObj = env->AllocObject(
                    env->FindClass("com/fibocom/ai/sdk/Result$Bbox"));
            env->SetIntField(poseBboxObj,
                             env->GetFieldID(env->GetObjectClass(poseBboxObj), "x", "I"),
                             resultCv.pose.objects[i].bbox.x);
            env->SetIntField(poseBboxObj,
                             env->GetFieldID(env->GetObjectClass(poseBboxObj), "y", "I"),
                             resultCv.pose.objects[i].bbox.y);
            env->SetIntField(poseBboxObj,
                             env->GetFieldID(env->GetObjectClass(poseBboxObj), "w", "I"),
                             resultCv.pose.objects[i].bbox.w);
            env->SetIntField(poseBboxObj,
                             env->GetFieldID(env->GetObjectClass(poseBboxObj), "h", "I"),
                             resultCv.pose.objects[i].bbox.h);

            env->SetObjectField(poseObject, env->GetFieldID(env->GetObjectClass(poseObject), "bbox",
                                                            "Lcom/fibocom/ai/sdk/Result$Bbox;"),
                                poseBboxObj);

            // 设置关键点
            jobjectArray keypointsArray = env->NewObjectArray(
                    resultCv.pose.objects[i].keypoints.size(),
                    env->FindClass("com/fibocom/ai/sdk/Result$Point"), nullptr);
            for (size_t j = 0; j < resultCv.pose.objects[i].keypoints.size(); ++j) {
                jobject point = env->AllocObject(env->FindClass("com/fibocom/ai/sdk/Result$Point"));
                env->SetIntField(point, env->GetFieldID(env->GetObjectClass(point), "x", "I"),
                                 resultCv.pose.objects[i].keypoints[j].x);
                env->SetIntField(point, env->GetFieldID(env->GetObjectClass(point), "y", "I"),
                                 resultCv.pose.objects[i].keypoints[j].y);
                env->SetFloatField(point, env->GetFieldID(env->GetObjectClass(point), "score", "F"),
                                   resultCv.pose.objects[i].keypoints[j].score);
                env->SetObjectArrayElement(keypointsArray, j, point);
            }
            env->SetObjectField(poseObject,
                                env->GetFieldID(env->GetObjectClass(poseObject), "keypoints",
                                                "[Lcom/fibocom/ai/sdk/Result$Point;"),
                                keypointsArray);
            env->SetObjectArrayElement(poseObjectsArray, i, poseObject);
        }
        env->SetObjectField(poseObj, fidPoseObjects, poseObjectsArray);
        env->SetObjectField(resultcv, env->GetFieldID(env->GetObjectClass(resultcv), "pose",
                                                      "Lcom/fibocom/ai/sdk/Result$ResultCvPose;"),
                            poseObj);
    }
}

void SetSegmentResult(JNIEnv *env, jobject resultcv, const aisdk::result::ResultCv &resultCv) {
    // 设置图像分割结果
    if (resultCv.segment.count > 0) {
        jobject segmentObj = env->AllocObject(
                env->FindClass("com/fibocom/ai/sdk/Result$ResultCvSegment"));
        jfieldID fidSegmentCount = env->GetFieldID(env->GetObjectClass(segmentObj), "count", "I");
        jfieldID fidSegmentObjects = env->GetFieldID(env->GetObjectClass(segmentObj), "objects",
                                                     "[Lcom/fibocom/ai/sdk/Result$SegPlanners;");

        env->SetIntField(segmentObj, fidSegmentCount, resultCv.segment.count);
        jobjectArray segmentObjectsArray = env->NewObjectArray(resultCv.segment.count,
                                                               env->FindClass(
                                                                       "com/fibocom/ai/sdk/Result$SegPlanners"),
                                                               nullptr);

        for (int i = 0; i < resultCv.segment.count; ++i) {
            jobject plannersObj = env->AllocObject(
                    env->FindClass("com/fibocom/ai/sdk/Result$SegPlanners"));
            jfieldID fidPlannerNum = env->GetFieldID(env->GetObjectClass(plannersObj), "plannerNum",
                                                     "I");
            jfieldID fidPlanners = env->GetFieldID(env->GetObjectClass(plannersObj), "planners",
                                                   "[Lcom/fibocom/ai/sdk/Result$SegPlanner;");

            env->SetIntField(plannersObj, fidPlannerNum, resultCv.segment.objects[i].planner_num);
            jobjectArray plannersArray = env->NewObjectArray(
                    resultCv.segment.objects[i].planners.size(),
                    env->FindClass("com/fibocom/ai/sdk/Result$SegPlanner"), nullptr);
            for (size_t j = 0; j < resultCv.segment.objects[i].planners.size(); ++j) {
                jobject segPlanner = env->AllocObject(
                        env->FindClass("com/fibocom/ai/sdk/Result$SegPlanner"));
                env->SetIntField(segPlanner,
                                 env->GetFieldID(env->GetObjectClass(segPlanner), "width", "I"),
                                 resultCv.segment.objects[i].planners[j].width);
                env->SetIntField(segPlanner,
                                 env->GetFieldID(env->GetObjectClass(segPlanner), "height", "I"),
                                 resultCv.segment.objects[i].planners[j].height);
                env->SetIntField(segPlanner,
                                 env->GetFieldID(env->GetObjectClass(segPlanner), "size", "I"),
                                 resultCv.segment.objects[i].planners[j].size);
                // Set image data assuming data is a byte array
                jbyteArray dataArray = env->NewByteArray(
                        resultCv.segment.objects[i].planners[j].data.size());
                env->SetByteArrayRegion(dataArray, 0,
                                        resultCv.segment.objects[i].planners[j].data.size(),
                                        reinterpret_cast<const jbyte *>(resultCv.segment.objects[i].planners[j].data.data()));
                env->SetObjectField(segPlanner,
                                    env->GetFieldID(env->GetObjectClass(segPlanner), "data", "[B"),
                                    dataArray);

                env->SetObjectArrayElement(plannersArray, j, segPlanner);
            }
            env->SetObjectField(plannersObj, fidPlanners, plannersArray);
            env->SetObjectArrayElement(segmentObjectsArray, i, plannersObj);
        }
        env->SetObjectField(segmentObj, fidSegmentObjects, segmentObjectsArray);
        env->SetObjectField(resultcv, env->GetFieldID(env->GetObjectClass(resultcv), "segment",
                                                      "Lcom/fibocom/ai/sdk/Result$ResultCvSegment;"),
                            segmentObj);
    }
}

void SetFeatureResult(JNIEnv *env, jobject resultcv, const aisdk::result::ResultCv &resultCv) {
    // 设置特征值结果
    if (resultCv.feature.count > 0) {
        jobject featureObj = env->AllocObject(
                env->FindClass("com/fibocom/ai/sdk/Result$ResultCvFeature"));
        jfieldID fidFeatureCount = env->GetFieldID(env->GetObjectClass(featureObj), "count", "I");
        jfieldID fidFeatureObjects = env->GetFieldID(env->GetObjectClass(featureObj), "objects",
                                                     "[Lcom/fibocom/ai/sdk/Result$ResultCvFeatureObject;");

        env->SetIntField(featureObj, fidFeatureCount, resultCv.feature.count);
        jobjectArray featureObjectsArray = env->NewObjectArray(resultCv.feature.count,
                                                               env->FindClass(
                                                                       "com/fibocom/ai/sdk/Result$ResultCvFeatureObject"),
                                                               nullptr);

        for (int i = 0; i < resultCv.feature.count; ++i) {
            jobject featureObject = env->AllocObject(
                    env->FindClass("com/fibocom/ai/sdk/Result$ResultCvFeatureObject"));
            env->SetObjectField(featureObject,
                                env->GetFieldID(env->GetObjectClass(featureObject), "label",
                                                "Ljava/lang/String;"),
                                env->NewStringUTF(resultCv.feature.objects[i].label.c_str()));
            env->SetFloatField(featureObject,
                               env->GetFieldID(env->GetObjectClass(featureObject), "score", "F"),
                               resultCv.feature.objects[i].score);
            env->SetIntField(featureObject,
                             env->GetFieldID(env->GetObjectClass(featureObject), "classId", "I"),
                             resultCv.feature.objects[i].class_id);

            // 设置 Bbox
            jobject featureBboxObj = env->AllocObject(
                    env->FindClass("com/fibocom/ai/sdk/Result$Bbox"));
            env->SetIntField(featureBboxObj,
                             env->GetFieldID(env->GetObjectClass(featureBboxObj), "x", "I"),
                             resultCv.feature.objects[i].bbox.x);
            env->SetIntField(featureBboxObj,
                             env->GetFieldID(env->GetObjectClass(featureBboxObj), "y", "I"),
                             resultCv.feature.objects[i].bbox.y);
            env->SetIntField(featureBboxObj,
                             env->GetFieldID(env->GetObjectClass(featureBboxObj), "w", "I"),
                             resultCv.feature.objects[i].bbox.w);
            env->SetIntField(featureBboxObj,
                             env->GetFieldID(env->GetObjectClass(featureBboxObj), "h", "I"),
                             resultCv.feature.objects[i].bbox.h);
            env->SetObjectField(featureObject,
                                env->GetFieldID(env->GetObjectClass(featureObject), "bbox",
                                                "Lcom/fibocom/ai/sdk/Result$Bbox;"),
                                featureBboxObj);

            // 设置特征值
            jfloatArray featureArray = env->NewFloatArray(
                    resultCv.feature.objects[i].feature.size());
            env->SetFloatArrayRegion(featureArray, 0, resultCv.feature.objects[i].feature.size(),
                                     resultCv.feature.objects[i].feature.data());
            env->SetObjectField(featureObject,
                                env->GetFieldID(env->GetObjectClass(featureObject), "feature",
                                                "[F"), featureArray);
            env->SetObjectField(featureObject,
                                env->GetFieldID(env->GetObjectClass(featureObject), "featureLabel",
                                                "Ljava/lang/String;"), env->NewStringUTF(
                            resultCv.feature.objects[i].feature_abel.c_str()));

            env->SetObjectArrayElement(featureObjectsArray, i, featureObject);
        }
        env->SetObjectField(featureObj, fidFeatureObjects, featureObjectsArray);
        env->SetObjectField(resultcv, env->GetFieldID(env->GetObjectClass(resultcv), "feature",
                                                      "Lcom/fibocom/ai/sdk/Result$ResultCvFeature;"),
                            featureObj);
    }
}

void
SetSuperResolutionResult(JNIEnv *env, jobject resultcv, const aisdk::result::ResultCv &resultCv) {
    // 设置超分结果
    if (resultCv.super_resolution.count > 0) {
        jobject superResObj = env->AllocObject(
                env->FindClass("com/fibocom/ai/sdk/Result$ResultCvSuperResolution"));
        jfieldID fidSuperResCount = env->GetFieldID(env->GetObjectClass(superResObj), "count", "I");
        jfieldID fidSuperResObjects = env->GetFieldID(env->GetObjectClass(superResObj), "objects",
                                                      "[Lcom/fibocom/ai/sdk/Result$ResultCvSuperResolutionObject;");

        env->SetIntField(superResObj, fidSuperResCount, resultCv.super_resolution.count);
        jobjectArray superResObjectsArray = env->NewObjectArray(resultCv.super_resolution.count,
                                                                env->FindClass(
                                                                        "com/fibocom/ai/sdk/Result$ResultCvSuperResolutionObject"),
                                                                nullptr);

        for (int i = 0; i < resultCv.super_resolution.count; ++i) {
            jobject superResObject = env->AllocObject(
                    env->FindClass("com/fibocom/ai/sdk/Result$ResultCvSuperResolutionObject"));
            env->SetIntField(superResObject,
                             env->GetFieldID(env->GetObjectClass(superResObject), "width", "I"),
                             resultCv.super_resolution.objects[i].width);
            env->SetIntField(superResObject,
                             env->GetFieldID(env->GetObjectClass(superResObject), "height", "I"),
                             resultCv.super_resolution.objects[i].height);
            env->SetIntField(superResObject,
                             env->GetFieldID(env->GetObjectClass(superResObject), "channel", "I"),
                             resultCv.super_resolution.objects[i].channel);
            env->SetIntField(superResObject,
                                env->GetFieldID(env->GetObjectClass(superResObject), "format",
                                                "I"), resultCv.super_resolution.objects[i].format);
            env->SetIntField(superResObject,
                             env->GetFieldID(env->GetObjectClass(superResObject), "size", "I"),
                             resultCv.super_resolution.objects[i].size);

            // 设置数据
            jbyteArray dataArray = env->NewByteArray(
                    resultCv.super_resolution.objects[i].data.size());
            env->SetByteArrayRegion(dataArray, 0, resultCv.super_resolution.objects[i].data.size(),
                                    reinterpret_cast<const jbyte *>(resultCv.super_resolution.objects[i].data.data()));
            env->SetObjectField(superResObject,
                                env->GetFieldID(env->GetObjectClass(superResObject), "data", "[B"),
                                dataArray);

            env->SetObjectArrayElement(superResObjectsArray, i, superResObject);
        }
        env->SetObjectField(superResObj, fidSuperResObjects, superResObjectsArray);
        env->SetObjectField(resultcv,
                            env->GetFieldID(env->GetObjectClass(resultcv), "superResolution",
                                            "Lcom/fibocom/ai/sdk/Result$ResultCvSuperResolution;"),
                            superResObj);
    }
}

void SetRotatedBoxResult(JNIEnv *env, jobject resultcv, const aisdk::result::ResultCv &resultCv) {
    // 设置旋转框结果
    if (resultCv.rotated_box.count > 0) {
        jobject rotatedBoxObj = env->AllocObject(
                env->FindClass("com/fibocom/ai/sdk/Result$ResultCvRotatedBox"));
        jfieldID fidRotatedBoxCount = env->GetFieldID(env->GetObjectClass(rotatedBoxObj), "count",
                                                      "I");
        jfieldID fidRotatedBoxObjects = env->GetFieldID(env->GetObjectClass(rotatedBoxObj),
                                                        "objects",
                                                        "[Lcom/fibocom/ai/sdk/Result$ResultCvRotatedBoxObject;");

        env->SetIntField(rotatedBoxObj, fidRotatedBoxCount, resultCv.rotated_box.count);
        jobjectArray rotatedBoxObjectsArray = env->NewObjectArray(resultCv.rotated_box.count,
                                                                  env->FindClass(
                                                                          "com/fibocom/ai/sdk/Result$ResultCvRotatedBoxObject"),
                                                                  nullptr);

        for (int i = 0; i < resultCv.rotated_box.count; ++i) {
            jobject rotatedBoxObject = env->AllocObject(
                    env->FindClass("com/fibocom/ai/sdk/Result$ResultCvRotatedBoxObject"));
            env->SetObjectField(rotatedBoxObject,
                                env->GetFieldID(env->GetObjectClass(rotatedBoxObject), "label",
                                                "Ljava/lang/String;"),
                                env->NewStringUTF(resultCv.rotated_box.objects[i].label.c_str()));
            env->SetFloatField(rotatedBoxObject,
                               env->GetFieldID(env->GetObjectClass(rotatedBoxObject), "score", "F"),
                               resultCv.rotated_box.objects[i].score);
            env->SetIntField(rotatedBoxObject,
                             env->GetFieldID(env->GetObjectClass(rotatedBoxObject), "classId", "I"),
                             resultCv.rotated_box.objects[i].class_id);

            // 设置旋转框
            jobjectArray bboxArray = env->NewObjectArray(4, env->FindClass(
                    "com/fibocom/ai/sdk/Result$Point"), nullptr);
            for (int j = 0; j < 4; ++j) {
                jobject point = env->AllocObject(env->FindClass("com/fibocom/ai/sdk/Result$Point"));
                env->SetIntField(point, env->GetFieldID(env->GetObjectClass(point), "x", "I"),
                                 resultCv.rotated_box.objects[i].bbox[j].x);
                env->SetIntField(point, env->GetFieldID(env->GetObjectClass(point), "y", "I"),
                                 resultCv.rotated_box.objects[i].bbox[j].y);
                env->SetFloatField(point, env->GetFieldID(env->GetObjectClass(point), "score", "F"),
                                   resultCv.rotated_box.objects[i].bbox[j].score);
                env->SetObjectArrayElement(bboxArray, j, point);
            }
            env->SetObjectField(rotatedBoxObject,
                                env->GetFieldID(env->GetObjectClass(rotatedBoxObject), "bbox",
                                                "[Lcom/fibocom/ai/sdk/Result$Point;"), bboxArray);
            env->SetFloatField(rotatedBoxObject,
                               env->GetFieldID(env->GetObjectClass(rotatedBoxObject), "angle", "F"),
                               resultCv.rotated_box.objects[i].angle);

            env->SetObjectArrayElement(rotatedBoxObjectsArray, i, rotatedBoxObject);
        }
        env->SetObjectField(rotatedBoxObj, fidRotatedBoxObjects, rotatedBoxObjectsArray);
        env->SetObjectField(resultcv, env->GetFieldID(env->GetObjectClass(resultcv), "rotatedBox",
                                                      "Lcom/fibocom/ai/sdk/Result$ResultCvRotatedBox;"),
                            rotatedBoxObj);
    }
}

void SetOcrResult(JNIEnv *env, jobject resultcv, const aisdk::result::ResultCv &resultCv) {
    // 设置 OCR 结果
    if (resultCv.ocr.count > 0) {
        jobject ocrObj = env->AllocObject(env->FindClass("com/fibocom/ai/sdk/Result$ResultCvOcr"));
        jfieldID fidOcrCount = env->GetFieldID(env->GetObjectClass(ocrObj), "count", "I");
        jfieldID fidOcrObjects = env->GetFieldID(env->GetObjectClass(ocrObj), "objects",
                                                 "[Lcom/fibocom/ai/sdk/Result$ResultCvOcrObject;");

        env->SetIntField(ocrObj, fidOcrCount, resultCv.ocr.count);
        jobjectArray ocrObjectsArray = env->NewObjectArray(resultCv.ocr.count, env->FindClass(
                "com/fibocom/ai/sdk/Result$ResultCvOcrObject"), nullptr);

        for (int i = 0; i < resultCv.ocr.count; ++i) {
            jobject ocrObject = env->AllocObject(
                    env->FindClass("com/fibocom/ai/sdk/Result$ResultCvOcrObject"));
            env->SetObjectField(ocrObject, env->GetFieldID(env->GetObjectClass(ocrObject), "label",
                                                           "Ljava/lang/String;"),
                                env->NewStringUTF(resultCv.ocr.objects[i].label.c_str()));
            env->SetFloatField(ocrObject,
                               env->GetFieldID(env->GetObjectClass(ocrObject), "score", "F"),
                               resultCv.ocr.objects[i].score);
            env->SetIntField(ocrObject,
                             env->GetFieldID(env->GetObjectClass(ocrObject), "classId", "I"),
                             resultCv.ocr.objects[i].class_id);

            // 设置旋转框
            jobject rotatedBoxObj = env->AllocObject(
                    env->FindClass("com/fibocom/ai/sdk/Result$ResultCvRotatedBoxObject"));
            env->SetObjectField(rotatedBoxObj,
                                env->GetFieldID(env->GetObjectClass(rotatedBoxObj), "label",
                                                "Ljava/lang/String;"), env->NewStringUTF(
                            resultCv.ocr.objects[i].rotated_box.label.c_str()));
            env->SetFloatField(rotatedBoxObj,
                               env->GetFieldID(env->GetObjectClass(rotatedBoxObj), "score", "F"),
                               resultCv.ocr.objects[i].rotated_box.score);
            env->SetIntField(rotatedBoxObj,
                             env->GetFieldID(env->GetObjectClass(rotatedBoxObj), "classId", "I"),
                             resultCv.ocr.objects[i].rotated_box.class_id);

            // 设置旋转框的角度及点
            jobjectArray ocrBboxArray = env->NewObjectArray(4, env->FindClass(
                    "com/fibocom/ai/sdk/Result$Point"), nullptr);
            for (int j = 0; j < 4; ++j) {
                jobject point = env->AllocObject(env->FindClass("com/fibocom/ai/sdk/Result$Point"));
                env->SetIntField(point, env->GetFieldID(env->GetObjectClass(point), "x", "I"),
                                 resultCv.ocr.objects[i].rotated_box.bbox[j].x);
                env->SetIntField(point, env->GetFieldID(env->GetObjectClass(point), "y", "I"),
                                 resultCv.ocr.objects[i].rotated_box.bbox[j].y);
                env->SetFloatField(point, env->GetFieldID(env->GetObjectClass(point), "score", "F"),
                                   resultCv.ocr.objects[i].rotated_box.bbox[j].score);
                env->SetObjectArrayElement(ocrBboxArray, j, point);
            }
            env->SetObjectField(rotatedBoxObj,
                                env->GetFieldID(env->GetObjectClass(rotatedBoxObj), "bbox",
                                                "[Lcom/fibocom/ai/sdk/Result$Point;"),
                                ocrBboxArray);
            env->SetFloatField(rotatedBoxObj,
                               env->GetFieldID(env->GetObjectClass(rotatedBoxObj), "angle", "F"),
                               resultCv.ocr.objects[i].rotated_box.angle);
            env->SetObjectField(ocrObject,
                                env->GetFieldID(env->GetObjectClass(ocrObject), "rotatedBox",
                                                "Lcom/fibocom/ai/sdk/Result$ResultCvRotatedBoxObject;"),
                                rotatedBoxObj);
            env->SetObjectField(ocrObject, env->GetFieldID(env->GetObjectClass(ocrObject), "text",
                                                           "Ljava/lang/String;"),
                                env->NewStringUTF(resultCv.ocr.objects[i].text.c_str()));

            env->SetObjectArrayElement(ocrObjectsArray, i, ocrObject);
        }
        env->SetObjectField(ocrObj, fidOcrObjects, ocrObjectsArray);
        env->SetObjectField(resultcv, env->GetFieldID(env->GetObjectClass(resultcv), "ocr",
                                                      "Lcom/fibocom/ai/sdk/Result$ResultCvOcr;"),
                            ocrObj);
    }
}

void ResultCvCppToJava(JNIEnv *env, const aisdk::result::ResultCv &cppResultCv, jobject javaResultcv) {
    SetClassifyResult(env, javaResultcv, cppResultCv);
    SetDetectResult(env, javaResultcv, cppResultCv);
    SetPoseResult(env, javaResultcv, cppResultCv);
    SetSegmentResult(env, javaResultcv, cppResultCv);
    SetFeatureResult(env, javaResultcv, cppResultCv);
    SetSuperResolutionResult(env, javaResultcv, cppResultCv);
    SetRotatedBoxResult(env, javaResultcv, cppResultCv);
    SetOcrResult(env, javaResultcv, cppResultCv);
}

//非线程安全，需要注意
extern "C"
JNIEXPORT jlong JNICALL
Java_com_fibocom_ai_sdk_CvApi_createNative(JNIEnv *env, jobject thiz) {
    return (jlong) new CVAPI();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_fibocom_ai_sdk_CvApi_initNative(JNIEnv *env, jobject thiz, jlong handle, jstring config,
                                         jstring model) {
    CVAPI* api = (CVAPI*)handle;
    const char* configPath = env->GetStringUTFChars(config, 0);
    const char* modelPath = env->GetStringUTFChars(model, 0);
    int ret = api->Init(configPath, modelPath);
    env->ReleaseStringUTFChars(config, configPath);
    env->ReleaseStringUTFChars(model, modelPath);
    return ret;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_fibocom_ai_sdk_CvApi_inferSyncByPathNative(JNIEnv *env, jobject thiz, jlong handle,
                                                    jstring image, jobject result) {
    char* imagePath = (char*)env->GetStringUTFChars(image, 0);
    CVAPI* api = (CVAPI*)handle;
    aisdk::result::ResultCv resultCv;
    cv::Mat img = cv::imread(imagePath);
    aisdk::FIBO_CV_Img fiboImg = CppMatToFiboImg(env, img);
    api->InferCvSync(fiboImg,resultCv);
    ResultCvCppToJava(env, resultCv, result);
    env->ReleaseStringUTFChars(image, imagePath);
    return 0;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_fibocom_ai_sdk_CvApi_inferSyncByMatNative(JNIEnv *env, jobject thiz, jlong handle,
                                                   jobject image, jobject result) {
//    std::vector<std::vector<float>> data0(8400,std::vector<float>(116));
    CVAPI* api = (CVAPI*)handle;
    FIBO_CV_Img fiboImg = JavaMatToFiboImg(env, image);
    aisdk::result::ResultCv resultCv;
    int ret = api->InferCvSync(fiboImg,resultCv);
    ResultCvCppToJava(env, resultCv, result);
    return ret;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_fibocom_ai_sdk_CvApi_inferAsyncByPathNative(JNIEnv *env, jobject thiz, jlong handle,
                                                     jstring image, jobject cb, jlong timeout,
                                                     jstring tag) {
    std::lock_guard<std::mutex> lock(gCvApiCacheMutex);
    auto *api = (CVAPI *) handle;
    auto& curApi = gCvApiCache[handle];
    if(curApi.jvm == nullptr){
        if(env->GetJavaVM(&curApi.jvm)!=JNI_OK){
            return JNI_ERR;
        }
    }

    if(curApi.globalRef != nullptr){
        env->DeleteGlobalRef(curApi.globalRef);
        curApi.globalRef = nullptr;
    }
    curApi.globalRef = env->NewGlobalRef(cb);
    curApi.callback= [handle](std::string& tag, int status, void* response){
        std::lock_guard<std::mutex> lock(gCvApiCacheMutex);
        if(gCvApiCache.count(handle) == 0) return;
        auto& curApi = gCvApiCache[handle];
        if(curApi.globalRef != nullptr){
            JNIEnv * env;
            curApi.jvm->AttachCurrentThread(&env,nullptr);
            auto cbClass = env->FindClass("com/fibocom/ai/sdk/CvApi$InferCallback");
            result::ResultCv cppResultCv = *(aisdk::result::ResultCv*)response;
            jobject javaResultcv = env->AllocObject(env->FindClass("com/fibocom/ai/sdk/Result$ResultCv"));
            ResultCvCppToJava(env, cppResultCv, javaResultcv);
            jstring jtag = env->NewStringUTF(tag.c_str());
            env->CallVoidMethod(curApi.globalRef,
                                env->GetMethodID(cbClass,"onCallback", "(Ljava/lang/String;ILcom/fibocom/ai/sdk/Result$ResultCv;)V"),
                                jtag,status,javaResultcv);
            env->DeleteLocalRef(jtag);
            env->DeleteLocalRef(javaResultcv);
            curApi.jvm->DetachCurrentThread();
        }
    };
    const char* tagStr = env->GetStringUTFChars(tag, 0);
    cv::Mat mat = cv::imread(env->GetStringUTFChars(image, 0));
    aisdk::FIBO_CV_Img fiboImg = CppMatToFiboImg(env, mat);
    int ret = api->InferAsync(fiboImg, curApi.callback, timeout, tagStr);
    env->ReleaseStringUTFChars(image, env->GetStringUTFChars(image, 0));
    env->ReleaseStringUTFChars(tag, tagStr);
    return ret;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_fibocom_ai_sdk_CvApi_inferAsyncByMatNative(JNIEnv *env, jobject thiz, jlong handle,
                                                    jobject image, jobject cb, jlong timeout,
                                                    jstring tag) {
    std::lock_guard<std::mutex> lock(gCvApiCacheMutex);
    auto *api = (CVAPI *) handle;
    auto& curApi = gCvApiCache[handle];
    if(curApi.jvm == nullptr){
        if(env->GetJavaVM(&curApi.jvm)!=JNI_OK){
            return JNI_ERR;
        }
    }

    if(curApi.globalRef != nullptr){
        env->DeleteGlobalRef(curApi.globalRef);
        curApi.globalRef = nullptr;
    }
    curApi.globalRef = env->NewGlobalRef(cb);
    curApi.callback= [handle](std::string& tag, int status, void* response){
        std::lock_guard<std::mutex> lock(gCvApiCacheMutex);
        if(gCvApiCache.count(handle) == 0) return;
        auto& curApi = gCvApiCache[handle];
        if(curApi.globalRef != nullptr){
            JNIEnv * env;
            curApi.jvm->AttachCurrentThread(&env,nullptr);
            auto cbClass = env->FindClass("com/fibocom/ai/sdk/CvApi$InferCallback");
            result::ResultCv cppResultCv = *(aisdk::result::ResultCv*)response;
            jobject javaResultcv = env->AllocObject(env->FindClass("com/fibocom/ai/sdk/Result$ResultCv"));
            ResultCvCppToJava(env, cppResultCv, javaResultcv);
            jstring jtag = env->NewStringUTF(tag.c_str());
            env->CallVoidMethod(curApi.globalRef,
                                env->GetMethodID(cbClass,"onCallback", "(Ljava/lang/String;ILcom/fibocom/ai/sdk/Result$ResultCv;)V"),
                                jtag,status,javaResultcv);
            env->DeleteLocalRef(jtag);
            env->DeleteLocalRef(javaResultcv);
            curApi.jvm->DetachCurrentThread();
        }
    };
    const char* tagStr = env->GetStringUTFChars(tag, 0);
    FIBO_CV_Img fiboImg = JavaMatToFiboImg(env, image);
    int ret = api->InferAsync(fiboImg, curApi.callback, timeout, tagStr);
    env->ReleaseStringUTFChars(tag, tagStr);
    return ret;
}

void ModelInfoCppToJava(JNIEnv *env, const aisdk::FIBO_CV_Model &modelInfo, jobject model) {
    jclass modelClass = env->GetObjectClass(model);

    jfieldID nameField = env->GetFieldID(modelClass, "name", "Ljava/lang/String;");
    jfieldID typeField = env->GetFieldID(modelClass, "type", "Lcom/fibocom/ai/sdk/CvModel$FIBO_CV_Model_Type;"); // 修改为对应的Java类型
    jfieldID versionField = env->GetFieldID(modelClass, "version", "Ljava/lang/String;");
    jfieldID pathField = env->GetFieldID(modelClass, "path", "Ljava/lang/String;");
    jfieldID md5Field = env->GetFieldID(modelClass, "md5", "Ljava/lang/String;");
    jfieldID metaConfigField = env->GetFieldID(modelClass, "meta_config", "Ljava/lang/String;");

    env->SetObjectField(model, nameField, env->NewStringUTF(modelInfo.name.c_str()));

    // 根据modelInfo.type的值设置type字段
    jclass typeClass = env->FindClass("com/fibocom/ai/sdk/CvModel$FIBO_CV_Model_Type");
    jfieldID typeFieldID;
    switch (modelInfo.type) {
        case aisdk::FIBO_CV_MODEL_TYPE_CLASSIFY:
            typeFieldID = env->GetStaticFieldID(typeClass, "FIBO_CV_MODEL_TYPE_CLASSIFY", "Lcom/fibocom/ai/sdk/CvModel$FIBO_CV_Model_Type;");
            break;
        case aisdk::FIBO_CV_MODEL_TYPE_DETECT:
            typeFieldID = env->GetStaticFieldID(typeClass, "FIBO_CV_MODEL_TYPE_DETECT", "Lcom/fibocom/ai/sdk/CvModel$FIBO_CV_Model_Type;");
            break;
        case aisdk::FIBO_CV_MODEL_TYPE_SEGMENTATION:
            typeFieldID = env->GetStaticFieldID(typeClass, "FIBO_CV_MODEL_TYPE_SEGMENTATION", "Lcom/fibocom/ai/sdk/CvModel$FIBO_CV_Model_Type;");
            break;
        case aisdk::FIBO_CV_MODEL_TYPE_POSE:
            typeFieldID = env->GetStaticFieldID(typeClass, "FIBO_CV_MODEL_TYPE_POSE", "Lcom/fibocom/ai/sdk/CvModel$FIBO_CV_Model_Type;");
            break;
        case aisdk::FIBO_CV_MODEL_TYPE_OCR:
            typeFieldID = env->GetStaticFieldID(typeClass, "FIBO_CV_MODEL_TYPE_OCR", "Lcom/fibocom/ai/sdk/CvModel$FIBO_CV_Model_Type;");
            break;
        case aisdk::FIBO_CV_MODEL_TYPE_ROTATE:
            typeFieldID = env->GetStaticFieldID(typeClass, "FIBO_CV_MODEL_TYPE_ROTATE", "Lcom/fibocom/ai/sdk/CvModel$FIBO_CV_Model_Type;");
            break;
        case aisdk::FIBO_CV_MODEL_TYPE_GEN:
            typeFieldID = env->GetStaticFieldID(typeClass, "FIBO_CV_MODEL_TYPE_GEN", "Lcom/fibocom/ai/sdk/CvModel$FIBO_CV_Model_Type;");
            break;
        case aisdk::FIBO_CV_MODEL_TYPE_FEATURE:
            typeFieldID = env->GetStaticFieldID(typeClass, "FIBO_CV_MODEL_TYPE_FEATURE", "Lcom/fibocom/ai/sdk/CvModel$FIBO_CV_Model_Type;");
            break;
        default:
            typeFieldID = env->GetStaticFieldID(typeClass, "FIBO_CV_MODEL_TYPE_MAX", "Lcom/fibocom/ai/sdk/CvModel$FIBO_CV_Model_Type;");
            break;
    }
    jobject typeValue = env->GetStaticObjectField(typeClass, typeFieldID);
    env->SetObjectField(model, typeField, typeValue);

    env->SetObjectField(model, versionField, env->NewStringUTF(modelInfo.version.c_str()));
    env->SetObjectField(model, pathField, env->NewStringUTF(modelInfo.path.c_str()));
    env->SetObjectField(model, md5Field, env->NewStringUTF(modelInfo.md5.c_str()));
    env->SetObjectField(model, metaConfigField, env->NewStringUTF(modelInfo.meta_config.c_str()));
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_fibocom_ai_sdk_CvApi_getModelInfoNative(JNIEnv *env, jobject thiz, jlong handle,
                                                 jobject model) {
    CVAPI* api = (CVAPI*)handle;
    aisdk::FIBO_CV_Model modelInfo;
    int ret = api->GetModelInfo(modelInfo);
    ModelInfoCppToJava(env, modelInfo, model);
    return ret;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_fibocom_ai_sdk_CvApi_waitTaskDoneNative(JNIEnv *env, jobject thiz, jlong handle,
                                                 jstring tag) {
    CVAPI* api = (CVAPI*)handle;
    const char* tagStr = env->GetStringUTFChars(tag, 0);
    int ret = api->WaitTaskDone(tagStr);
    env->ReleaseStringUTFChars(tag, tagStr);
    return ret;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_fibocom_ai_sdk_CvApi_releaseNative(JNIEnv *env, jobject thiz, jlong handle) {
    if (handle == 0) {
        return 0;
    }
    if(gCvApiCache.count(handle)){
        env->DeleteGlobalRef(gCvApiCache[handle].globalRef);
        gCvApiCache.erase(handle);
    }
    auto *api = (CVAPI *) handle;
    delete api;
    return 0;
}


