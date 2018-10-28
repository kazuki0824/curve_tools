#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>

#include "helper_math.h"
#include "spline_gpu.hcu"
#include <vector>
#include <GL/freeglut.h>
#include "ferguson_coons_gpu.hcu"

cublasHandle_t cublashandle;
cusparseHandle_t cusparsehandle;
float2 v0={0.0,0.0};
void spl() {

	float2 point[6] = { { 0.1, 0.5 }, { 0.5, 0 }, { 0.5, 0.5 },{0.25,0.25} ,{-0.4,0.1}, {0.0,-0.5}};

	pSpline_polynomial f[32];
	pSpline(point, f, 6, &cusparsehandle);

	glBegin(GL_POINTS);
	//glBegin(GL_LINE_STRIP);

	for (int i = 0; i < 6; ++i) {
		pSpline_polynomial c = f[i];
		for (float t = 0.0; t < 1.0; t += 0.05) {
			float curve = fabsf(1.0/c.curvature_radius(t)) *0.3;
			glColor3f(curve, 0.7, 0.0);
			float2 p = c.x_and_y(t);
			glVertex2f(p.x+0.3, p.y+0.3);
			//printf("t=%f, (%f,%f)\n",t,p.x,p.y);
		}
	}
	glColor3f(1.0,0.0,0.0);
	glPointSize(0.5);

	float2 v1=f[0].speed(0.0);

	float2 x0={0.0,0.0};
	float2 x1={0.3,0.3};

	pSpline_polynomial c = Ferguson(&cublashandle, v0, v1,x0, x1);

	for (float t = 0.0; t < 1.0; t += 0.05) {
		float2 p = c.x_and_y(t);
		glVertex2f(p.x, p.y);
	}

	glEnd();
	glColor3f(0.0,0.1,0.9);
	glBegin(GL_LINES);
	glVertex2f(0.0,0.0);
	glVertex2f(v0.x,v0.y);
	glEnd();


}
static unsigned char image[1000][1000][3] = {0}; //取り込む画像
void display() {
	//ros::spinOnce();

	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2f(-1 , 1); //ずらして
	glDrawPixels(1000, 1000, GL_RGB, GL_UNSIGNED_BYTE, &image[0][0][0]);

	glBegin(GL_LINES);
	glColor3d(1.0, 1.0, 1.0);   // 基準線の描画
	glVertex2d(-1.0, 0.0);
	glVertex2d(1.0, 0.0);
	glVertex2d(0.0, 1.0);
	glVertex2d(0.0, -1.0);
	glEnd();

	spl();

	glFlush();

}

void poseCallback(const geometry_msgs::Pose::ConstPtr pose)
{
	glutPostRedisplay();
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);   // 背景色　黒
	cusparseCreate(&cusparsehandle);
	cublasCreate_v2(&cublashandle);

}
void deinit()
{
	cublasDestroy_v2(cublashandle);
	cusparseDestroy(cusparsehandle);
}
void mySigintHandler(int sig)
{
	deinit();
	ros::shutdown();
}

int main(int argc, char **argv) {

	/*
	ros::init(argc,argv,"Trajectory");
	ros::NodeHandle nh("~");
*/
	puts("Init OpenGL");
	glutInitWindowPosition(100, 200);   // ウィンドウ作成時の座標指定
	glutInitWindowSize(1000, 1000);        // ウィンドウサイズ
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA); // Red Green Blue +α(透明度)= RGBA
	glutCreateWindow(argv[0]);         // ウィンドウを生成。 argv[0]だとファイル名がそのままウィンドウ名になる
	glutDisplayFunc(display);           // display関数内を表示
	puts("Complete");

	puts("Initializing CUDA libraries...");
	init();

	puts("Initializing ROS-related functions...");
	signal(SIGINT, mySigintHandler);
	//TODO:
	//ros::Subscriber s = nh.subscribe("ros_pose_v",100, poseCallback);

	glutMainLoop();
	return 0;
}
