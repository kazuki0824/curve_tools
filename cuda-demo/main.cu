/**
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 */
#include <stdio.h>
#include <stdlib.h>


#include "helper_math.h"
#include "spline_gpu.hpp"
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

	float2 v1=f[0].speed(0.0); //v1 = length(v0) * normalize(v1); これはちゃんと動かない。最終的な速度はちゃんと渡そう

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

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
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

void motion(int x , int y) {
	int get_ww = glutGet( GLUT_WINDOW_WIDTH );
    int get_wh = glutGet( GLUT_WINDOW_HEIGHT );
    float x_= (x - get_ww / 2) / (float)get_ww;
    float y_ = (get_wh / 2 -y) / (float)get_wh;

    v0={x_,y_};
    v0 *= 2.0;

	printf("X = %f : Y = %f\n" , v0.x , v0.y);

    glutPostRedisplay();
}

int main(int argc, char **argv) {

	glutInitWindowPosition(100, 200);   // ウィンドウ作成時の座標指定
	glutInitWindowSize(900, 900);        // ウィンドウサイズ
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA); // Red Green Brue +α(透明度)= RGBA
	glutCreateWindow(argv[0]);         // ウィンドウを生成。 argv[0]だとファイル名がそのままウィンドウ名になる
	glutDisplayFunc(display);           // display関数内を表示
	init();
	glutMotionFunc(motion);

	glutMainLoop();

	return 0;
}
