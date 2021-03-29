#include "common/common.hpp"
#ifdef TUYANG_EXPORTS
#ifdef _WIN32
#define TUYANG_API __declspec(dllexport)
#else
#define TUYANG_API __attribute__((visibility("default")))
#endif
#else
#ifdef _WIN32
#define TUYANG_API __declspec(dllimport)
#else
#define TUYANG_API __attribute__((visibility("default")))
#endif
#endif

#ifndef EXTERN_C
#define EXTERN_C    extern "C"
#endif

#define DEPTH_IMG_WIDTH		640
#define DEPTH_IMG_HEIGHT	480

#define RGB_IMG_WIDTH		1280
#define RGB_IMG_HEIGHT		960

EXTERN_C TUYANG_API void ty_camera_test(char* szString);
EXTERN_C TUYANG_API void ty_camera_test_int(int _interface);

/// @brief  device init.
EXTERN_C TUYANG_API int ty_camera_init(bool trigger);

/// @brief  get 3d-camera device count.
/// @retval percipio device count.
EXTERN_C TUYANG_API int ty_cameraGetCnt(void);

/// @brief  get 3d-camera device serial number.
/// @param  [in]		idx     device index.
/// @retval percipio device sn.
EXTERN_C TUYANG_API char* ty_cameraGetSN(int  idx);

/// @brief  get 3d-camera depth calib-para.
/// @param  [in]		idx				device index.
/// @retval percipio device depth calib image width.
EXTERN_C TUYANG_API int		ty_cameraGetDepthCalibIntrinsicWidth(int idx);

/// @brief  get 3d-camera depth calib-para.
/// @param  [in]		idx				device index.
/// @retval percipio device depth calib image height.
EXTERN_C TUYANG_API int		ty_cameraGetDepthCalibIntrinsicHeight(int idx);

/// @brief  get 3d-camera depth calib-para.
/// @param  [in]		idx				device index.
/// @retval percipio device depth intrinsic calib para.
EXTERN_C TUYANG_API void	ty_cameraGetDepthCalibIntrinsic(int idx, float* fIntrinsic);

/// @brief  get 3d-camera depth calib-para.
/// @param  [in]		idx				device index.
/// @retval percipio device depth extrinsic calib para.
EXTERN_C TUYANG_API void	ty_cameraGetDepthCalibExtrinsic(int idx, float* fExtrinsic);

/// @brief  get 3d-camera depth calib-para.
/// @param  [in]		idx				device index.
/// @retval percipio device depth distortion calib para: 
EXTERN_C TUYANG_API void	ty_cameraGetDepthCalibDistortion(int idx, float* fDistortion);

/// @brief  get 3d-camera color calib-para.
/// @param  [in]		idx				device index.
/// @retval percipio device color calib image width.
EXTERN_C TUYANG_API int		ty_cameraGetColorCalibIntrinsicWidth(int idx);

/// @brief  get 3d-camera color calib-para.
/// @param  [in]		idx				device index.
/// @retval percipio device color calib image height.
EXTERN_C TUYANG_API int		ty_cameraGetColorCalibIntrinsicHeight(int idx);

/// @brief  get 3d-camera color calib-para.
/// @param  [in]		idx				device index.
/// @retval percipio device color intrinsic calib-para.
EXTERN_C TUYANG_API void	ty_cameraGetColorCalibIntrinsic(int idx, float* fIntrinsic);

/// @brief  get 3d-camera color calib-para.
/// @param  [in]		idx				device index.
/// @retval percipio device color extrinsic calib-para.
EXTERN_C TUYANG_API void	ty_cameraGetColorCalibExtrinsic(int idx, float* fExtrinsic);

/// @brief  get 3d-camera color calib-para.
/// @param  [in]		idx				device index.
/// @retval percipio device color distortion calib-para.
EXTERN_C TUYANG_API void	ty_cameraGetColorCalibDistortion(int idx, float* fDistortion);

EXTERN_C TUYANG_API int		ty_cameraGetRGBImageWidth();
EXTERN_C TUYANG_API int		ty_cameraGetRGBImageHeight();
EXTERN_C TUYANG_API int		ty_cameraGetDepthImageWidth();
EXTERN_C TUYANG_API int		ty_cameraGetDepthImageHeight();

EXTERN_C TUYANG_API int		ty_cameraSendSofTriggerSig(int idx);

EXTERN_C TUYANG_API bool	ty_camera_fetch_rgb_image(char* color, int idx);
EXTERN_C TUYANG_API bool	ty_camera_fetch_depth_image(unsigned short* depth, int idx);
EXTERN_C TUYANG_API bool	ty_camera_fetch_all_image(char* color, unsigned short* depth, int idx);

/// @brief Do rgb image undistortion
/// @param  [in]	pRGB          Source rgb image data.
/// @param  [in]    width         Source image width.
/// @param  [in]    height        Source image height.
/// @param  [in]    idx			  Device index.
/// @param  [out]   pOut		  Dest rgb image data.
EXTERN_C TUYANG_API void ty_cameraGetUndistortRGBImage(char* pRGB, int width, int height, int idx, char* pOut);

EXTERN_C TUYANG_API void ty_cameraMapDepthImage2RGBCoordinate(unsigned short* depthIn, int src_width, int src_height, int dst_width, int dst_height, unsigned short* depthOut, int idx);

EXTERN_C TUYANG_API bool ty_camera_close();
