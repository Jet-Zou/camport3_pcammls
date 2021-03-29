using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace PercipioCamDemo
{
    class Program
    {
        //ty_camera_init(true:触发模式, false:视频模式)初始化camera
        [DllImport("percipio_cam.dll", EntryPoint = "ty_camera_init", CharSet = CharSet.Ansi, ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public extern static int ty_camera_init(bool trigger);

        //ty_cameraGetCnt(void):获取已经连接相机之数目
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetCnt", CallingConvention = CallingConvention.Cdecl)]
        public extern static int ty_cameraGetCnt();//

        //ty_cameraGetSN(idx:设备序号):获取设备sn号
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetSN", CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr ty_cameraGetSN(int idx);

        //ty_cameraGetDepthCalibIntrinsicWidth(idx:设备序号):获取深度图标定尺寸(宽)
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetDepthCalibIntrinsicWidth", CallingConvention = CallingConvention.Cdecl)]
        public extern static int ty_cameraGetDepthCalibIntrinsicWidth(int idx);

        //ty_cameraGetDepthCalibIntrinsicHeight(idx:设备序号):获取深度图标定尺寸(高)
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetDepthCalibIntrinsicHeight", CallingConvention = CallingConvention.Cdecl)]
        public extern static int ty_cameraGetDepthCalibIntrinsicHeight(int idx);

        //ty_cameraGetColorCalibIntrinsicWidth(idx:设备序号):获取RGB图标定尺寸(宽)
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetColorCalibIntrinsicWidth", CallingConvention = CallingConvention.Cdecl)]
        public extern static int ty_cameraGetColorCalibIntrinsicWidth(int idx);

        //ty_cameraGetColorCalibIntrinsicHeight(idx:设备序号):获取RGB图标定尺寸(高)
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetColorCalibIntrinsicHeight", CallingConvention = CallingConvention.Cdecl)]
        public extern static int ty_cameraGetColorCalibIntrinsicHeight(int idx);

        //ty_cameraGetRGBImageWidth(void):获取RGB图像输出尺寸(宽)
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetRGBImageWidth", CallingConvention = CallingConvention.Cdecl)]
        public extern static int ty_cameraGetRGBImageWidth();

        ////ty_cameraGetRGBImageHeight(void):获取RGB图像输出尺寸(高)
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetRGBImageHeight", CallingConvention = CallingConvention.Cdecl)]
        public extern static int ty_cameraGetRGBImageHeight();

        //ty_cameraGetDepthImageWidth(void):获取深度图像输出尺寸(宽)
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetDepthImageWidth", CallingConvention = CallingConvention.Cdecl)]
        public extern static int ty_cameraGetDepthImageWidth();

        //ty_cameraGetDepthImageHeight(void):获取深度图像输出尺寸(高)
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetDepthImageHeight", CallingConvention = CallingConvention.Cdecl)]
        public extern static int ty_cameraGetDepthImageHeight();

        //ty_cameraGetDepthCalibIntrinsic():获取深度图标定内参(3x3 浮点矩阵)
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetDepthCalibIntrinsic", CallingConvention = CallingConvention.Cdecl)]
        public extern static void ty_cameraGetDepthCalibIntrinsic(int idx, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] float[] fIntrinsic);

        //忽略
        //[DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetDepthCalibExtrinsic", CallingConvention = CallingConvention.Cdecl)]
        //public extern static void ty_cameraGetDepthCalibExtrinsic(int idx, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] float[] fExtrinsic);

        //忽略
        //[DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetDepthCalibDistortion", CallingConvention = CallingConvention.Cdecl)]
        //public extern static void ty_cameraGetDepthCalibDistortion(int idx, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] float[] fDistortion);

        //ty_cameraGetColorCalibIntrinsic():获取RGB标定内参(3x3 浮点矩阵)
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetColorCalibIntrinsic", CallingConvention = CallingConvention.Cdecl)]
        public extern static void ty_cameraGetColorCalibIntrinsic(int idx, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] float[] fIntrinsic);

        //ty_cameraGetColorCalibExtrinsic():获取RGB标定外参(4x4 浮点矩阵)
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetColorCalibExtrinsic", CallingConvention = CallingConvention.Cdecl)]
        public extern static void ty_cameraGetColorCalibExtrinsic(int idx, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] float[] fExtrinsic);

        //ty_cameraGetColorCalibDistortion():获取RGB标定畸变(1x12:k1,k2,p1,p2,k3,k4,k5,k6,s1,s2,s3,s4)
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetColorCalibDistortion", CallingConvention = CallingConvention.Cdecl)]
        public extern static void ty_cameraGetColorCalibDistortion(int idx, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] float[] fDistortion);

        //ty_camera_fetch_all_image:获取彩色图像与深度图图像
        [DllImport("percipio_cam.dll", EntryPoint = "ty_camera_fetch_all_image", CallingConvention = CallingConvention.Cdecl)]
        public extern static void ty_camera_fetch_all_image([MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)]  byte[] rgb, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)]  ushort[] depth, int idx);

        //ty_cameraMapDepthImage2RGBCoordinate:将深度图对齐至RGB坐标系
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraMapDepthImage2RGBCoordinate", CallingConvention = CallingConvention.Cdecl)]
        public extern static void ty_cameraMapDepthImage2RGBCoordinate([MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)]  ushort[] depth_in, int src_width, int src_height, int dst_width, int dst_height, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)]  ushort[] depth_out, int idx);

        //ty_cameraGetUndistortRGBImage:RGB图像畸变校正
        [DllImport("percipio_cam.dll", EntryPoint = "ty_cameraGetUndistortRGBImage", CallingConvention = CallingConvention.Cdecl)]
        public extern static void ty_cameraGetUndistortRGBImage([MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)]  byte[] rgb_in, int width, int height, int idx, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)]  byte[] rgb_out);

        //ty_camera_close:关闭设备，释放资源
        [DllImport("percipio_cam.dll", EntryPoint = "ty_camera_close", CallingConvention = CallingConvention.Cdecl)]
        public extern static bool ty_camera_close();
        static void Main(string[] args)
        {
            Console.WriteLine("test start\n");
            ty_camera_init(false);//初始化camera

            int cnt = ty_cameraGetCnt();
            Console.WriteLine(string.Format("percipio device cnt :{0}", cnt));

            int cam_idx = 0;//default

            IntPtr sn_ptr = ty_cameraGetSN(cam_idx);
            string sn_string = Marshal.PtrToStringAnsi(sn_ptr);
            Console.WriteLine(string.Format("percipio device sn :{0}", sn_string));

            int color_width = ty_cameraGetRGBImageWidth();
            int color_height = ty_cameraGetRGBImageHeight();
            Console.WriteLine(string.Format("RGB image size :{0} {1}", color_width, color_height));

            int depth_width = ty_cameraGetDepthImageWidth();
            int depth_height = ty_cameraGetDepthImageHeight();
            Console.WriteLine(string.Format("Depth image size :{0} {1}",depth_width, depth_height));

            //get camera calib parameter
            int m_calib_depth_intr_width = ty_cameraGetDepthCalibIntrinsicWidth(cam_idx);
            int m_calib_depth_intr_height = ty_cameraGetDepthCalibIntrinsicHeight(cam_idx);

            float[] f_depth_intrinsic = new float[3 * 3];
            ty_cameraGetDepthCalibIntrinsic(cam_idx, f_depth_intrinsic);
            Console.WriteLine(string.Format("Depth calib parameter:"));
            Console.WriteLine(string.Format("    size:"));
            Console.WriteLine(string.Format("        {0}, {1}", m_calib_depth_intr_width, m_calib_depth_intr_height));
            Console.WriteLine(string.Format("    intrinsic:"));
            Console.WriteLine(string.Format("        {0},      {1},      {2}", f_depth_intrinsic[0], f_depth_intrinsic[1], f_depth_intrinsic[2]));
            Console.WriteLine(string.Format("        {0},      {1},      {2}", f_depth_intrinsic[3], f_depth_intrinsic[4], f_depth_intrinsic[5]));
            Console.WriteLine(string.Format("        {0},      {1},      {2}", f_depth_intrinsic[6], f_depth_intrinsic[7], f_depth_intrinsic[8]));

            int m_calib_rgb_intr_width = ty_cameraGetColorCalibIntrinsicWidth(cam_idx);
            int m_calib_rgb_intr_height = ty_cameraGetColorCalibIntrinsicHeight(cam_idx);

            float[] f_rgb_intrinsic = new float[3 * 3];
            float[] f_rgb_extrinsic = new float[4 * 4];
            float[] f_rgb_distortion = new float[1 * 12];
            ty_cameraGetColorCalibIntrinsic(cam_idx, f_rgb_intrinsic);
            ty_cameraGetColorCalibExtrinsic(cam_idx, f_rgb_extrinsic);
            ty_cameraGetColorCalibDistortion(cam_idx, f_rgb_distortion);
            Console.WriteLine(string.Format("RGB calib parameter:"));
            Console.WriteLine(string.Format("    size:"));
            Console.WriteLine(string.Format("        {0}, {1}", m_calib_rgb_intr_width, m_calib_rgb_intr_height));
            Console.WriteLine(string.Format("    intrinsic:"));
            Console.WriteLine(string.Format("        {0},      {1},      {2}", f_rgb_intrinsic[0], f_rgb_intrinsic[1], f_rgb_intrinsic[2]));
            Console.WriteLine(string.Format("        {0},      {1},      {2}", f_rgb_intrinsic[3], f_rgb_intrinsic[4], f_rgb_intrinsic[5]));
            Console.WriteLine(string.Format("        {0},      {1},      {2}", f_rgb_intrinsic[6], f_rgb_intrinsic[7], f_rgb_intrinsic[8]));
            Console.WriteLine(string.Format("    extrinsic:"));
            Console.WriteLine(string.Format("        {0},      {1},      {2},      {3}", f_rgb_extrinsic[0],  f_rgb_extrinsic[1],  f_rgb_extrinsic[2],  f_rgb_extrinsic[3]));
            Console.WriteLine(string.Format("        {0},      {1},      {2},      {3}", f_rgb_extrinsic[4],  f_rgb_extrinsic[5],  f_rgb_extrinsic[6],  f_rgb_extrinsic[7]));
            Console.WriteLine(string.Format("        {0},      {1},      {2},      {3}", f_rgb_extrinsic[8],  f_rgb_extrinsic[9],  f_rgb_extrinsic[10], f_rgb_extrinsic[11]));
            Console.WriteLine(string.Format("        {0},      {1},      {2},      {3}", f_rgb_extrinsic[12], f_rgb_extrinsic[13], f_rgb_extrinsic[14], f_rgb_extrinsic[15]));
            Console.WriteLine(string.Format("    distortion:"));
            Console.WriteLine(string.Format("        {0},      {1},      {2},      {3}", f_rgb_distortion[0], f_rgb_distortion[1], f_rgb_distortion[2],  f_rgb_distortion[3]));
            Console.WriteLine(string.Format("        {0},      {1},      {2},      {3}", f_rgb_distortion[4], f_rgb_distortion[5], f_rgb_distortion[6],  f_rgb_distortion[7]));
            Console.WriteLine(string.Format("        {0},      {1},      {2},      {3}", f_rgb_distortion[8], f_rgb_distortion[9], f_rgb_distortion[10], f_rgb_distortion[11]));

            byte[] data_rgb = new byte[color_width * color_height * 3];
            byte[] data_rgb2 = new byte[color_width * color_height * 3];
            ushort[] data_depth = new ushort[depth_width * depth_height];
            ushort[] data_depth2 = new ushort[color_width * color_height];

            while (true) {
                ty_camera_fetch_all_image(data_rgb, data_depth, cam_idx);

                int m_depth_center_offset = depth_width * depth_height / 2 + depth_width / 2;
                Console.WriteLine(string.Format("Depth image center value :{0}", data_depth[m_depth_center_offset]));

                ty_cameraMapDepthImage2RGBCoordinate(data_depth, depth_width, depth_height, color_width, color_height, data_depth2, cam_idx);

                ty_cameraGetUndistortRGBImage(data_rgb, color_width, color_height, cam_idx, data_rgb2);

                int rgb_offset_center = color_width * color_height / 2 + color_width / 2;
                Console.WriteLine(string.Format("RGBD image center info:{0} {1} {2} {3}", data_rgb2[3 * rgb_offset_center + 2], data_rgb2[3 * rgb_offset_center + 1], data_rgb2[3 * rgb_offset_center + 0], data_depth2[rgb_offset_center]));
            }

            ty_camera_close();
        }
    }
}