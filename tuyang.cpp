#include "tuyang.h"
#include "common/common.hpp"
#include "TYImageProc.h"


static char buffer[1024*1024];
struct CamInfo
{
    char                sn[32];
	TY_INTERFACE_INFO   iface;
    TY_INTERFACE_HANDLE hIface;
    TY_DEV_HANDLE       hDev;
    std::vector<char>   fb[2];
    TY_FRAME_DATA       frame;

    CamInfo() : hDev(0) {}
};

static uint32_t cam_size ;
static std::vector<CamInfo> cams(1);

static std::vector<TY_CAMERA_CALIB_INFO> depth_calib(1);//
static std::vector<TY_CAMERA_CALIB_INFO> color_calib(1);//

TUYANG_API void ty_camera_test(char* szString)
{
    LOGD("=======================================");
    LOGD("%s", szString);
    LOGD("=======================================");
    return;
}

TUYANG_API void ty_camera_test_int(int _interface)
{
	LOGD("Mode : %d", _interface);
}

TUYANG_API int ty_camera_init(bool trigger)
{
	int32_t deviceType = TY_INTERFACE_ALL;
    std::vector<char*> list;
    
    int32_t found;

	LOGD("=== Init lib");//sdk lib init
    ASSERT_OK( TYInitLib() );
    TY_VERSION_INFO ver;
    ASSERT_OK( TYLibVersion(&ver) );
    LOGD("     - lib version: %d.%d.%d", ver.major, ver.minor, ver.patch);

    std::vector<TY_DEVICE_BASE_INFO> selected;
    ASSERT_OK( selectDevice(deviceType, "", "", 10, selected) );
    
    if (list.size()) {
        cam_size = list.size();
    } else {
        cam_size = selected.size();
    }

	cams.resize(cam_size);
	depth_calib.resize(cam_size);
	color_calib.resize(cam_size);

    int32_t count = 0;
    for(uint32_t i = 0; i < selected.size(); i++){
        if (list.size()) {
            found = 0; 
            for(uint32_t j = 0; j < list.size(); j++){
               LOGD("=== check device: %s, %s", selected[i].id, list[j]);
               if (strcmp(selected[i].id, list[j]) == 0) {
                   LOGD("=== check device success");
                   found = 1; 
                   continue;
               }
            }
           
            if (!found) {
                continue;
            } 
        }

        LOGD("=== Open device: %s", selected[i].id);
        strncpy(cams[count].sn, selected[i].id, sizeof(cams[count].sn));

        ASSERT_OK( TYOpenInterface(selected[i].iface.id, &cams[count].hIface) );
        ASSERT_OK( TYOpenDevice(cams[count].hIface, selected[i].id, &cams[count].hDev) );

		cams[count].iface = selected[i].iface;

		 int32_t allComps;
        ASSERT_OK( TYGetComponentIDs(cams[count].hDev, &allComps) );
        
		if (allComps & TY_COMPONENT_DEPTH_CAM){
			LOGD("=== Has depth camera, open depth cam");//enable depth cam
			ASSERT_OK(TYEnableComponents(cams[count].hDev, TY_COMPONENT_DEPTH_CAM));
		}

		if (allComps & TY_COMPONENT_RGB_CAM){
			LOGD("=== Has RGB camera, open RGB cam");//enable rgb cam
			ASSERT_OK(TYEnableComponents(cams[count].hDev, TY_COMPONENT_RGB_CAM));
		}

		std::vector<TY_ENUM_ENTRY> image_mode_list;
        LOGD("=== Configure feature, set depth image resolution.");
		ASSERT_OK(get_feature_enum_list(cams[count].hDev, TY_COMPONENT_DEPTH_CAM, TY_ENUM_IMAGE_MODE, image_mode_list));
		for (uint32_t idx = 0; idx < image_mode_list.size(); idx++){
			TY_ENUM_ENTRY &entry = image_mode_list[idx];
			LOGD("Get RGB sensor image size: %d %d", TYImageWidth(entry.value), TYImageHeight(entry.value));
			if (TYImageWidth(entry.value) == DEPTH_IMG_WIDTH || TYImageHeight(entry.value) == DEPTH_IMG_WIDTH){
				LOGD("Select RGB Image Mode: %s", entry.description);
				int err = TYSetEnum(cams[count].hDev, TY_COMPONENT_DEPTH_CAM, TY_ENUM_IMAGE_MODE, entry.value);
				ASSERT(err == TY_STATUS_OK || err == TY_STATUS_NOT_PERMITTED);
				break;
			}
		}

		LOGD("=== Configure feature, set rgb image resolution.");
        ASSERT_OK(get_feature_enum_list(cams[count].hDev, TY_COMPONENT_RGB_CAM, TY_ENUM_IMAGE_MODE, image_mode_list));
        for (uint32_t idx = 0; idx < image_mode_list.size(); idx++){
            TY_ENUM_ENTRY &entry = image_mode_list[idx];
			LOGD("Get RGB sensor image size: %d %d", TYImageWidth(entry.value), TYImageHeight(entry.value));
            if (TYImageWidth(entry.value) == RGB_IMG_WIDTH || TYImageHeight(entry.value) == RGB_IMG_WIDTH){
                LOGD("Select RGB Image Mode: %s", entry.description);
                int err = TYSetEnum(cams[count].hDev, TY_COMPONENT_RGB_CAM, TY_ENUM_IMAGE_MODE, entry.value);
                ASSERT(err == TY_STATUS_OK || err == TY_STATUS_NOT_PERMITTED);
                break;
            }
        }

        LOGD("=== Prepare image buffer");
        uint32_t frameSize;
        ASSERT_OK( TYGetFrameBufferSize(cams[count].hDev, &frameSize) );
        LOGD("     - Get size of framebuffer, %d", frameSize);
	    ASSERT( frameSize >= DEPTH_IMG_WIDTH*DEPTH_IMG_HEIGHT*2 );

        LOGD("     - Allocate & enqueue buffers");
        cams[count].fb[0].resize(frameSize);
        cams[count].fb[1].resize(frameSize);
        LOGD("     - Enqueue buffer (%p, %d)", cams[count].fb[0].data(), frameSize);
        ASSERT_OK( TYEnqueueBuffer(cams[count].hDev, cams[count].fb[0].data(), frameSize) );
        LOGD("     - Enqueue buffer (%p, %d)", cams[count].fb[1].data(), frameSize);
        ASSERT_OK( TYEnqueueBuffer(cams[count].hDev, cams[count].fb[1].data(), frameSize) );

		LOGD("=== Read depth calib info");
		ASSERT_OK( TYGetStruct(cams[count].hDev, TY_COMPONENT_DEPTH_CAM, TY_STRUCT_CAM_CALIB_DATA
			, &depth_calib.at(count), sizeof(TY_CAMERA_CALIB_INFO)) );

		LOGD("=== Read color calib info");
		ASSERT_OK( TYGetStruct(cams[count].hDev, TY_COMPONENT_RGB_CAM, TY_STRUCT_CAM_CALIB_DATA
			, &color_calib.at(count), sizeof(TY_CAMERA_CALIB_INFO)) );

	//	TYSetBool(cams[count].hDev, TY_COMPONENT_LASER, TY_BOOL_LASER_AUTO_CTRL, false);
	//	TYSetInt(cams[count].hDev, TY_COMPONENT_LASER, TY_INT_LASER_POWER, 30);
       // LOGD("=== Register event callback");
        //ASSERT_OK(TYRegisterEventCallback(cams[count].hDev, eventCallback, NULL));

        TY_TRIGGER_PARAM para;
		if (trigger) {
			LOGD("=== Enable trigger mode");
			para.mode = TY_TRIGGER_MODE_SLAVE;
		}
		else {
			LOGD("=== Disable trigger mode");
			para.mode = TY_TRIGGER_MODE_OFF;
		}
		ASSERT_OK(TYSetStruct(cams[count].hDev, TY_COMPONENT_DEVICE, TY_STRUCT_TRIGGER_PARAM, &para, sizeof(para)));

		//for network only
		LOGD("======INTERFACE TYPE : %d", cams[count].iface.type);
		if (TYIsNetworkInterface(cams[count].iface.type)) {
			bool hasResend;
			ASSERT_OK(TYHasFeature(cams[count].hDev, TY_COMPONENT_DEVICE, TY_BOOL_GVSP_RESEND, &hasResend));
			if (hasResend) {
				LOGD("=== Open resend");
				ASSERT_OK(TYSetBool(cams[count].hDev, TY_COMPONENT_DEVICE, TY_BOOL_GVSP_RESEND, true));
			} else {
				LOGD("=== Not support feature TY_BOOL_GVSP_RESEND");
			}
		}
        LOGD("=== Start capture");

        ASSERT_OK( TYStartCapture(cams[count].hDev) );

        count++;
    }

	return 0;
}

TUYANG_API int ty_cameraGetRGBImageWidth()
{
	return RGB_IMG_WIDTH;
}

TUYANG_API int ty_cameraGetRGBImageHeight()
{
	return RGB_IMG_HEIGHT;
}

TUYANG_API int ty_cameraGetDepthImageWidth()
{
	return DEPTH_IMG_WIDTH;
}

TUYANG_API int ty_cameraGetDepthImageHeight()
{
	return DEPTH_IMG_HEIGHT;
}

TUYANG_API int		ty_cameraSendSofTriggerSig(int idx)
{
	return TYSendSoftTrigger(cams[idx].hDev);
}

TUYANG_API void ty_cameraGetUndistortRGBImage(char* pRGB, int width, int height, int idx, char* pOut)
{
	TY_IMAGE_DATA src;
    src.width = width;
    src.height = height;
    src.size = width * height * 3;
    src.pixelFormat = TY_PIXEL_FORMAT_RGB;
    src.buffer = pRGB;

	TY_IMAGE_DATA dst;
    dst.width = width;
    dst.height = height;
    dst.size = width * height * 3;
    dst.buffer = pOut;
    dst.pixelFormat = TY_PIXEL_FORMAT_RGB;

	ASSERT_OK(TYUndistortImage(&color_calib.at(idx), &src, NULL, &dst));
}

TUYANG_API void ty_cameraMapDepthImage2RGBCoordinate(unsigned short* depthIn, int src_width, int src_height, int dst_width, int dst_height, unsigned short* depthOut, int idx)
{
	TY_CAMERA_CALIB_INFO _depth_calib = depth_calib.at(idx);
	TY_CAMERA_CALIB_INFO _color_calib = color_calib.at(idx);
	ASSERT_OK(
		TYMapDepthImageToColorCoordinate(
		&_color_calib,
		src_width, src_height, depthIn,
		&_color_calib,
		dst_width, dst_height, depthOut
		)
		);
}

TUYANG_API int ty_cameraGetDepthCalibIntrinsicWidth(int idx)
{
	return depth_calib.at(idx).intrinsicWidth;
}

TUYANG_API int ty_cameraGetDepthCalibIntrinsicHeight(int idx)
{
	return depth_calib.at(idx).intrinsicHeight;
}

TUYANG_API void ty_cameraGetDepthCalibIntrinsic(int idx, float* fIntrinsic)
{
	memcpy((void*)fIntrinsic, (void*)&depth_calib.at(idx).intrinsic, sizeof(TY_CAMERA_INTRINSIC));
}

TUYANG_API void ty_cameraGetDepthCalibExtrinsic(int idx, float* fExtrinsic)
{
	memcpy((void*)fExtrinsic, (void*)&depth_calib.at(idx).extrinsic, sizeof(TY_CAMERA_EXTRINSIC));
}

TUYANG_API void ty_cameraGetDepthCalibDistortion(int idx, float* fDistortion)
{
	memcpy((void*)fDistortion, (void*)&depth_calib.at(idx).distortion, sizeof(TY_CAMERA_DISTORTION));
}

TUYANG_API int  ty_cameraGetColorCalibIntrinsicWidth(int idx)
{
	return color_calib.at(idx).intrinsicWidth;
}

TUYANG_API int ty_cameraGetColorCalibIntrinsicHeight(int idx)
{
	return color_calib.at(idx).intrinsicHeight;
}

TUYANG_API void ty_cameraGetColorCalibIntrinsic(int idx, float* fIntrinsic)
{
	memcpy((void*)fIntrinsic, (void*)&color_calib.at(idx).intrinsic, sizeof(TY_CAMERA_INTRINSIC));
}

TUYANG_API void ty_cameraGetColorCalibExtrinsic(int idx, float* fExtrinsic)
{
	memcpy((void*)fExtrinsic, (void*)&color_calib.at(idx).extrinsic, sizeof(TY_CAMERA_EXTRINSIC));
}

TUYANG_API void ty_cameraGetColorCalibDistortion(int idx, float* fDistortion)
{
	memcpy((void*)fDistortion, (void*)&color_calib.at(idx).distortion, sizeof(TY_CAMERA_DISTORTION));
}

TUYANG_API int ty_cameraGetCnt(void)
{
	return cam_size;
}

TUYANG_API char* ty_cameraGetSN(int  idx)
{
	return cams[idx].sn;
}

TUYANG_API bool ty_camera_fetch_rgb_image(char* color, int idx)
{
	cv::Mat tempRGB;
	int err = TYFetchFrame(cams[idx].hDev, &cams[idx].frame, -1);
	if (err == TY_STATUS_OK) {
		CamInfo* pData = (CamInfo*)&cams[idx];
		parseFrame(cams[idx].frame, 0, 0, 0, &tempRGB, NULL);

        cv::Mat tempMat;
		if (!tempRGB.empty()) {
			printf("====color image info : %d %d\n", tempRGB.cols, tempRGB.rows);
			memcpy(color, tempRGB.data, 3 * tempRGB.cols * tempRGB.rows);
		}
		else {
			printf("====color image is empty!\n");
		}

		ASSERT_OK(TYEnqueueBuffer(cams[idx].hDev, cams[idx].frame.userBuffer, cams[idx].frame.bufferSize));

		return true;
	}
	else {
		printf("TYFetchFrame err :%d\n", err);
		return false;
	}
}

TUYANG_API bool ty_camera_fetch_depth_image(unsigned short* depth, int idx)
{
	cv::Mat tempDepth;
	int err = TYFetchFrame(cams[idx].hDev, &cams[idx].frame, -1);
	if (err == TY_STATUS_OK) {

		CamInfo* pData = (CamInfo*)&cams[idx];
		parseFrame(cams[idx].frame, &tempDepth, 0, 0, 0);

		if (!tempDepth.empty()){
			printf("====depth image info : %d %d\n", tempDepth.cols, tempDepth.rows);
			memcpy(depth, tempDepth.data, 2 * tempDepth.cols * tempDepth.rows);
		}
		else {
			printf("====depth image is empty!\n");
		}

		ASSERT_OK(TYEnqueueBuffer(cams[idx].hDev, cams[idx].frame.userBuffer, cams[idx].frame.bufferSize));
		return true;
	}
	else {
		printf("TYFetchFrame err :%d\n", err);
		return false;
	}
}

TUYANG_API bool ty_camera_fetch_all_image(char* color, unsigned short* depth, int idx)
{
	cv::Mat tempRGB, tempDepth;
	int err = TYFetchFrame(cams[idx].hDev, &cams[idx].frame, -1);
	if (err == TY_STATUS_OK) {
		CamInfo* pData = (CamInfo*)&cams[idx];
		parseFrame(cams[idx].frame, &tempDepth, 0, 0, &tempRGB);

        cv::Mat tempMat;
		if (!tempRGB.empty()) {
			printf("====color image info : %d %d\n", tempRGB.cols, tempRGB.rows);
			memcpy(color, tempRGB.data, 3 * tempRGB.cols * tempRGB.rows);
		}
		else {
			printf("====color image is empty!\n");
		}

		if (!tempDepth.empty()){
			printf("====depth image info : %d %d\n", tempDepth.cols, tempDepth.rows);
			memcpy(depth, tempDepth.data, 2 * tempDepth.cols*tempDepth.rows);
		}
		else {
			printf("====depth image is empty!\n");
		}

		ASSERT_OK(TYEnqueueBuffer(cams[idx].hDev, cams[idx].frame.userBuffer, cams[idx].frame.bufferSize));

		return true;
	}
	else
		return false;
	return true;
}

TUYANG_API bool ty_camera_close()//
{
	for(uint32_t i = 0; i < cam_size; i++){
        ASSERT_OK( TYStopCapture(cams[i].hDev) );
        ASSERT_OK( TYCloseDevice(cams[i].hDev) );
        ASSERT_OK( TYCloseInterface(cams[i].hIface) );
    }
    ASSERT_OK( TYDeinitLib() );
	return true;
}
