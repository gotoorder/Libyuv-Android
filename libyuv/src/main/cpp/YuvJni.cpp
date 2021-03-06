

#include <jni.h>
#include <string>
#include "libyuv.h"

#include <android/log.h>
#include <time.h>

#define LOG_TAG "Yuvjni.rzc"
#define DEBUG
#define ANDROID_PLATFORM

#ifdef DEBUG
#ifdef ANDROID_PLATFORM
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#else
#define LOGD(fmt, ...) printf(fmt"\n", ##__VA_ARGS__)
#define LOGI(fmt, ...) printf(fmt"\n", ##__VA_ARGS__)
#define LOGW(fmt, ...) printf(fmt"\n", ##__VA_ARGS__)
#define LOGE(fmt, ...) printf(fmt"\n", ##__VA_ARGS__)
#endif
#else
#define LOGD(...);
#define LOGI(...);
#define LOGW(...);
#define LOGE(...);
#endif

// nv21 --> i420
void nv21ToI420(jbyte *src_nv21_data, jint width, jint height, jbyte *src_i420_data) {
    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    jbyte *src_nv21_y_data = src_nv21_data;
    jbyte *src_nv21_vu_data = src_nv21_data + src_y_size;

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_y_size + src_u_size;

    libyuv::NV21ToI420((const uint8 *) src_nv21_y_data, width,
                       (const uint8 *) src_nv21_vu_data, width,
                       (uint8 *) src_i420_y_data, width,
                       (uint8 *) src_i420_u_data, width >> 1,
                       (uint8 *) src_i420_v_data, width >> 1,
                       width, height);
}

// i420 --> nv21
void i420ToNv21(jbyte *src_i420_data, jint width, jint height, jbyte *src_nv21_data) {
    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    jbyte *src_nv21_y_data = src_nv21_data;
    jbyte *src_nv21_uv_data = src_nv21_data + src_y_size;

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_y_size + src_u_size;


    libyuv::I420ToNV21(
            (const uint8 *) src_i420_y_data, width,
            (const uint8 *) src_i420_u_data, width >> 1,
            (const uint8 *) src_i420_v_data, width >> 1,
            (uint8 *) src_nv21_y_data, width,
            (uint8 *) src_nv21_uv_data, width,
            width, height);
}

// nv12 --> i420
void nv12ToI420(jbyte *Src_data, jint src_width, jint src_height, jbyte *Dst_data) {
    // NV12 video size
    jint NV12_Size = src_width * src_height * 3 / 2;
    jint NV12_Y_Size = src_width * src_height;

    // YUV420 video size
    jint I420_Size = src_width * src_height * 3 / 2;
    jint I420_Y_Size = src_width * src_height;
    jint I420_U_Size = (src_width >> 1) * (src_height >> 1);
    jint I420_V_Size = I420_U_Size;

    // src: buffer address of Y channel and UV channel
    jbyte *Y_data_Src = Src_data;
    jbyte *UV_data_Src = Src_data + NV12_Y_Size;
    jint src_stride_y = src_width;
    jint src_stride_uv = src_width;

    //dst: buffer address of Y channel、U channel and V channel
    jbyte *Y_data_Dst = Dst_data;
    jbyte *U_data_Dst = Dst_data + I420_Y_Size;
    jbyte *V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;
    jint Dst_Stride_Y = src_width;
    jint Dst_Stride_U = src_width >> 1;
    jint Dst_Stride_V = Dst_Stride_U;

    libyuv::NV12ToI420((const uint8 *) Y_data_Src, src_stride_y,
                       (const uint8 *) UV_data_Src, src_stride_uv,
                       (uint8 *) Y_data_Dst, Dst_Stride_Y,
                       (uint8 *) U_data_Dst, Dst_Stride_U,
                       (uint8 *) V_data_Dst, Dst_Stride_V,
                       src_width, src_height);
}

// i420 --> nv12
void i420ToNv12(jbyte *src_i420_data, jint width, jint height, jbyte *src_nv12_data) {
    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    jbyte *src_nv12_y_data = src_nv12_data;
    jbyte *src_nv12_uv_data = src_nv12_data + src_y_size;

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_y_size + src_u_size;

    libyuv::I420ToNV12(
            (const uint8 *) src_i420_y_data, width,
            (const uint8 *) src_i420_u_data, width >> 1,
            (const uint8 *) src_i420_v_data, width >> 1,
            (uint8 *) src_nv12_y_data, width,
            (uint8 *) src_nv12_uv_data, width,
            width, height);
}


// 镜像
void mirrorI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data) {
    jint src_i420_y_size = width * height;
    // jint src_i420_u_size = (width >> 1) * (height >> 1);
    jint src_i420_u_size = src_i420_y_size >> 2;

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + src_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + src_i420_y_size + src_i420_u_size;

    libyuv::I420Mirror((const uint8 *) src_i420_y_data, width,
                       (const uint8 *) src_i420_u_data, width >> 1,
                       (const uint8 *) src_i420_v_data, width >> 1,
                       (uint8 *) dst_i420_y_data, width,
                       (uint8 *) dst_i420_u_data, width >> 1,
                       (uint8 *) dst_i420_v_data, width >> 1,
                       width, height);
}

// 旋转
void rotateI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data, jint degree) {
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + src_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + src_i420_y_size + src_i420_u_size;

    //要注意这里的width和height在旋转之后是相反的
    if (degree == libyuv::kRotate90 || degree == libyuv::kRotate270) {
        libyuv::I420Rotate((const uint8 *) src_i420_y_data, width,
                           (const uint8 *) src_i420_u_data, width >> 1,
                           (const uint8 *) src_i420_v_data, width >> 1,
                           (uint8 *) dst_i420_y_data, height,
                           (uint8 *) dst_i420_u_data, height >> 1,
                           (uint8 *) dst_i420_v_data, height >> 1,
                           width, height,
                           (libyuv::RotationMode) degree);
    } else {
        libyuv::I420Rotate((const uint8 *) src_i420_y_data, width,
                           (const uint8 *) src_i420_u_data, width >> 1,
                           (const uint8 *) src_i420_v_data, width >> 1,
                           (uint8 *) dst_i420_y_data, width,
                           (uint8 *) dst_i420_u_data, width >> 1,
                           (uint8 *) dst_i420_v_data, width >> 1,
                           width, height,
                           (libyuv::RotationMode) degree);
    }
}

// 缩放
void scaleI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data, jint dst_width,
               jint dst_height, jint mode) {

    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);
    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jint dst_i420_y_size = dst_width * dst_height;
    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);
    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + dst_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + dst_i420_y_size + dst_i420_u_size;

    libyuv::I420Scale((const uint8 *) src_i420_y_data, width,
                      (const uint8 *) src_i420_u_data, width >> 1,
                      (const uint8 *) src_i420_v_data, width >> 1,
                      width, height,
                      (uint8 *) dst_i420_y_data, dst_width,
                      (uint8 *) dst_i420_u_data, dst_width >> 1,
                      (uint8 *) dst_i420_v_data, dst_width >> 1,
                      dst_width, dst_height,
                      (libyuv::FilterMode) mode);
}

// 裁剪
void cropI420(jbyte *src_i420_data, jint src_length, jint width, jint height,
              jbyte *dst_i420_data, jint dst_width, jint dst_height, jint left, jint top) {
    jint dst_i420_y_size = dst_width * dst_height;
    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);

    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + dst_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + dst_i420_y_size + dst_i420_u_size;

    libyuv::ConvertToI420((const uint8 *) src_i420_data, src_length,
                          (uint8 *) dst_i420_y_data, dst_width,
                          (uint8 *) dst_i420_u_data, dst_width >> 1,
                          (uint8 *) dst_i420_v_data, dst_width >> 1,
                          left, top,
                          width, height,
                          dst_width, dst_height,
                          libyuv::kRotate0, libyuv::FOURCC_I420);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_raysees_libyuv_YuvUtil_yuvCompress(JNIEnv *env, jclass type,
                                            jbyteArray nv21Src, jint width,
                                            jint height, jbyteArray rgbDst,
                                            jint dst_width, jint dst_height,
                                            jint mode, jint degree,
                                            jboolean isMirror) {

    jbyte *src_nv21_data = env->GetByteArrayElements(nv21Src, NULL);
    jbyte *dst_data = env->GetByteArrayElements(rgbDst, NULL);
    jbyte *tmp_dst_data = NULL;

    // nv21转化为i420
    jbyte *i420_data = (jbyte *) malloc(sizeof(jbyte) * width * height * 3 / 2);
    nv21ToI420(src_nv21_data, width, height, i420_data);
    tmp_dst_data = i420_data;

    // 镜像
    jbyte *i420_mirror_data = NULL;
    if (isMirror) {
        i420_mirror_data = (jbyte *) malloc(sizeof(jbyte) * width * height * 3 / 2);
        mirrorI420(tmp_dst_data, width, height, i420_mirror_data);
        tmp_dst_data = i420_mirror_data;
    }

    // 缩放
    jbyte *i420_scale_data = NULL;
    if (width != dst_width || height != dst_height) {
        i420_scale_data = (jbyte *) malloc(sizeof(jbyte) * width * height * 3 / 2);
        scaleI420(tmp_dst_data, width, height, i420_scale_data, dst_width, dst_height, mode);
        tmp_dst_data = i420_scale_data;
        width = dst_width;
        height = dst_height;
    }

    // 旋转
    jbyte *i420_rotate_data = NULL;
    if (degree == libyuv::kRotate90 || degree == libyuv::kRotate180 ||
        degree == libyuv::kRotate270) {
        i420_rotate_data = (jbyte *) malloc(sizeof(jbyte) * width * height * 3 / 2);
        rotateI420(tmp_dst_data, width, height, i420_rotate_data, degree);
        tmp_dst_data = i420_rotate_data;
    }

    LOGD("yuvCompress rotated width :  %d , height : %d", width, height);

//    // 转为argb
//    jbyte *argb_data = (jbyte *) malloc(sizeof(jbyte) * width * height * 3);
//    jint rotated_width = height;
//    jint rotated_height = width;
////    i420ToARGB(tmp_dst_data, rotated_width, rotated_height, argb_data);
//    convertI420ToARGB((const uint8_t*)(tmp_dst_data),
//                      (uint8_t*)(argb_data), rotated_width, rotated_height, rotated_width);
//    tmp_dst_data = argb_data;
//
//    LOGD("yuvCompress convert to argb end. rotated_width = %d, rotated_height = %d", rotated_width,
//         rotated_height);

    // 同步数据
    // memcpy(dst_data, tmp_dst_data, sizeof(jbyte) * width * height * 3 / 2);
    jint len = env->GetArrayLength(rgbDst);
    LOGD("yuvCompress rgbDst length = %d", len);
    memcpy(dst_data, tmp_dst_data, len);
    tmp_dst_data = NULL;
    env->ReleaseByteArrayElements(rgbDst, dst_data, 0);

    // 释放
    if (i420_data != NULL) free(i420_data);
    if (i420_mirror_data != NULL) free(i420_mirror_data);
    if (i420_scale_data != NULL) free(i420_scale_data);
    if (i420_rotate_data != NULL) free(i420_rotate_data);
//    if (argb_data != NULL) free(argb_data);
}

int convertI420ToARGB(const uint8_t *src_frame, uint8_t *dst_frame, int width, int height,
                      int dst_stride) {
    if (dst_stride == 0 || dst_stride == width)
        dst_stride = width * 4;
    const uint8_t *yplane = src_frame;
    const uint8_t *uplane = src_frame + width * height;
    const uint8_t *vplane = uplane + (width * height / 4);
    return libyuv::I420ToARGB(yplane, width, uplane, width / 2, vplane, width / 2, dst_frame,
                              dst_stride, width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_raysees_libyuv_YuvUtil_yuvI420ToARGB(JNIEnv *env, jclass clazz, jbyteArray i420_src,
                                              jint width, jint height, jbyteArray argb_dst) {
    jbyte *src_i420_data = env->GetByteArrayElements(i420_src, NULL);
    jbyte *dst_argb_data = env->GetByteArrayElements(argb_dst, NULL);
    i420ToARGB(src_i420_data, width, height, dst_argb_data);
    env->ReleaseByteArrayElements(argb_dst, dst_argb_data, 0);
}

//i420 --> argb
void i420ToARGB(jbyte *src_i420_data, jint width, jint height, jbyte *dst_argb_data) {
    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_y_size + src_u_size;

    libyuv::I420ToARGB(
            (uint8_t *) src_i420_y_data, width,
            (uint8_t *) src_i420_u_data, width >> 1,
            (uint8_t *) src_i420_v_data, width >> 1,
            (uint8_t *) dst_argb_data, width << 2,
            width, height
    );
}

//void convertToArgb(jbyteArray src_frame, int src_size,
//                   jbyteArray dst_argb, int dst_stride_argb,
//                   int crop_x, int crop_y,
//                   int src_width, int src_height,
//                   int crop_width, int crop_height) {
////    jbyte *yuvFrame = env->GetByteArrayElements(src_frame, NULL);
////    jbyte *rgbBuffer= env->GetByteArrayElements(dst_argb, NULL);
//    uint8_t *yuv = const_cast<uint8_t *>(src_frame);
//    uint8_t *rgb = const_cast<uint8_t *>(dst_argb);
//
//    libyuv::ConvertToARGB(yuv, src_size, rgb, dst_stride_argb, crop_x, crop_y, src_width,
//                          src_height, crop_width, crop_height, libyuv::kRotate0,
//                          libyuv::FOURCC_IYUV);
////    env->ReleaseByteArrayElements(src_frame, yuvFrame, 0);
////    env->ReleaseByteArrayElements(dst_argb, rgbBuffer, 0);
//}

//extern "C"
//JNIEXPORT void JNICALL
//Java_com_raysees_libyuv_YuvUtil_convertToArgb(JNIEnv *env, jobject thiz,
//                                              jbyteArray src_frame, int src_size,
//                                              jbyteArray dst_argb, int dst_stride_argb,
//                                              int crop_x, int crop_y,
//                                              int src_width, int src_height,
//                                              int crop_width, int crop_height,
//                                              int rotation,
//                                              int format) {
//
//    jbyte *yuvFrame = env->GetByteArrayElements(src_frame, NULL);
//    jbyte *rgbBuffer = env->GetByteArrayElements(dst_argb, NULL);
//    uint8_t *yuv = const_cast<uint8_t *>(yuvFrame);
//    uint8_t *rgb = const_cast<uint8_t *>(rgbBuffer);
//
//    libyuv::ConvertToARGB(yuv, src_size, rgb, dst_stride_argb, crop_x, crop_y, src_width,
//                          src_height, crop_width, crop_height, libyuv::kRotate0,
//                          libyuv::FOURCC_IYUV);
//    env->ReleaseByteArrayElements(src_frame, yuvFrame, 0);
//    env->ReleaseByteArrayElements(dst_argb, rgbBuffer, 0);
//}


extern "C"
JNIEXPORT void JNICALL
Java_com_raysees_libyuv_YuvUtil_yuvCropI420(JNIEnv *env, jclass type, jbyteArray src_, jint width,
                                            jint height, jbyteArray dst_, jint dst_width,
                                            jint dst_height,
                                            jint left, jint top) {
    //裁剪的区域大小不对
    if (left + dst_width > width || top + dst_height > height) {
        return;
    }
    //left和top必须为偶数，否则显示会有问题
    if (left % 2 != 0 || top % 2 != 0) {
        return;
    }
    // i420数据裁剪
    jint src_length = env->GetArrayLength(src_);
    jbyte *src_i420_data = env->GetByteArrayElements(src_, NULL);
    jbyte *dst_i420_data = env->GetByteArrayElements(dst_, NULL);
    cropI420(src_i420_data, src_length, width, height, dst_i420_data, dst_width, dst_height, left,
             top);
    env->ReleaseByteArrayElements(dst_, dst_i420_data, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_raysees_libyuv_YuvUtil_yuvMirrorI420(JNIEnv *env, jclass type, jbyteArray i420Src,
                                              jint width, jint height, jbyteArray i420Dst) {
    jbyte *src_i420_data = env->GetByteArrayElements(i420Src, NULL);
    jbyte *dst_i420_data = env->GetByteArrayElements(i420Dst, NULL);
    // i420数据镜像
    mirrorI420(src_i420_data, width, height, dst_i420_data);
    env->ReleaseByteArrayElements(i420Dst, dst_i420_data, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_raysees_libyuv_YuvUtil_yuvScaleI420(JNIEnv *env, jclass type, jbyteArray i420Src,
                                             jint width, jint height, jbyteArray i420Dst,
                                             jint dstWidth, jint dstHeight, jint mode) {
    jbyte *src_i420_data = env->GetByteArrayElements(i420Src, NULL);
    jbyte *dst_i420_data = env->GetByteArrayElements(i420Dst, NULL);
    // i420数据缩放
    scaleI420(src_i420_data, width, height, dst_i420_data, dstWidth, dstHeight, mode);
    env->ReleaseByteArrayElements(i420Dst, dst_i420_data, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_raysees_libyuv_YuvUtil_yuvRotateI420(JNIEnv *env, jclass type, jbyteArray i420Src,
                                              jint width, jint height, jbyteArray i420Dst,
                                              jint degree) {
    jbyte *src_i420_data = env->GetByteArrayElements(i420Src, NULL);
    jbyte *dst_i420_data = env->GetByteArrayElements(i420Dst, NULL);
    // i420数据旋转
    rotateI420(src_i420_data, width, height, dst_i420_data, degree);
    env->ReleaseByteArrayElements(i420Dst, dst_i420_data, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_raysees_libyuv_YuvUtil_yuvNV21ToI420(JNIEnv *env, jclass type, jbyteArray nv21Src,
                                              jint width, jint height, jbyteArray i420Dst) {
    jbyte *src_nv21_data = env->GetByteArrayElements(nv21Src, NULL);
    jbyte *dst_i420_data = env->GetByteArrayElements(i420Dst, NULL);
    // nv21转化为i420
    nv21ToI420(src_nv21_data, width, height, dst_i420_data);
    env->ReleaseByteArrayElements(i420Dst, dst_i420_data, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_raysees_libyuv_YuvUtil_yuvI420ToNV21(JNIEnv *env, jclass type, jbyteArray i420Src,
                                              jint width, jint height, jbyteArray nv21Dst) {

    jbyte *src_i420_data = env->GetByteArrayElements(i420Src, NULL);
    jbyte *dst_nv21_data = env->GetByteArrayElements(nv21Dst, NULL);
    // i420转化为nv21
    i420ToNv21(src_i420_data, width, height, dst_nv21_data);
    env->ReleaseByteArrayElements(nv21Dst, dst_nv21_data, 0);
}
