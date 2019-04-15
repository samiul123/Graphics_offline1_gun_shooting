#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<glm/glm/vec3.hpp>
#include <windows.h>
#include <GL/glut.h>
#include<iostream>
#include<vector>
#define pi (2*acos(0.0))

using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double cylinder_angle_a = 0.0;
double angle_up_a = 0.0, angle_up_b = 0.0, rotate_b = 0.0;
int c_a_a = 0;
int q = 0;
int m = 0;

struct point
{
	double x,y,z;
};
struct vector_3
{
    double v_x, v_y, v_z;
};
vector<struct vector_3> bullet_pos;
int slices = 24, stacks = 20;
struct point first = {0}, second = {0};
struct vector_3 u, r, l, gun;
struct point pos;
int n = 20;
int cylinderRadius = 10, cylinderHeight = 30;
int second_cylinder_radius = 6, second_cylinder_height = 40;
double x_dist = 0.0, y_dist = 0.0, z_dist = 0.0;
int x = 0, y = 0, z = 0;
double t_e = 0.0;
void drawSphere(double radius,int slices,int stacks, int m);
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 200,0,0);
			glVertex3f(-200,0,0);

			glVertex3f(0,-200,0);
			glVertex3f(0, 200,0);

			glVertex3f(0,0, 200);
			glVertex3f(0,0,-200);
		}glEnd();
	}
}




void drawSquare(double a, struct vector_3 v)
{
    //glColor3f(1.0,0.0,0.0);

	glBegin(GL_QUADS);{
		glVertex3f(v.v_x + a * sqrt(2) * cos(45 * .0175), v.v_y, v.v_z + a *sqrt(2)*sin(45 * .0175));
		glVertex3f(v.v_x - a * sqrt(2) * cos(45 * .0175), v.v_y, v.v_z + a *sqrt(2)*sin(45 * .0175));
		glVertex3f(v.v_x - a * sqrt(2) * cos(45 * .0175), v.v_y, v.v_z - a *sqrt(2)*sin(45 * .0175));
        glVertex3f(v.v_x + a * sqrt(2) * cos(45 * .0175), v.v_y, v.v_z - a *sqrt(2)*sin(45 * .0175));
	}glEnd();
}

struct point circle_points[100];
void drawCircle(double radius,int segments)
{
    int i;
    //struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        circle_points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        circle_points[i].z=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(circle_points[i].x,10,circle_points[i].z);
			glVertex3f(circle_points[i+1].x,10,circle_points[i+1].z);
        }
        glEnd();
    }
}


void draw_sphere_new(int radius, int m)
{
        struct point p;
        int i, j, condition_i, start_i;
        if(m == 1)
        {
            start_i = n/2;
            condition_i = n;
        }
        else{
            start_i = 0;
            condition_i = n /2;
        }

        float incO = 2 * pi / n;
        float incA = pi  / n;
        glBegin(GL_TRIANGLE_STRIP);
        {
            for(i = start_i; i <= condition_i; i++)
            {
                    for(j = 0; j <= n; j++)
                    {
                        if(j % 2 == 0)
                        {
                            glColor3f(0, 0, 0);
                        }
                        else{
                            glColor3f(1, 1, 1);
                        }
                        p.z = cos(pi - (incA*j))*radius;
                        p.y = sin(pi - (incA*j))*sin(incO*i)*radius;
                        p.x = sin(pi - (incA*j))*cos(incO*i)*radius;
                        glVertex3f(p.x, p.y, p.z);
                        p.z = cos(pi - (incA*j))*radius;
                        p.y = sin(pi - (incA*j))*sin(incO*(i+1))*radius;
                        p.x = sin(pi - (incA*j))*cos(incO*(i+1))*radius;
                        glVertex3f(p.x, p.y, p.z);
                    }
            }
        }
        glEnd();
}



void draw_cylinder(GLfloat radius, GLfloat height)
{
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    glPushMatrix();
    {
        glTranslatef(0, cylinderRadius, 0);

        drawSphere(cylinderRadius, slices, stacks, 1);
    }
    glPopMatrix();
    /** Draw the tube */
    glPushMatrix();
    {
        glTranslatef(0, cylinderRadius, 0);
        glRotatef(-90, 1, 0, 0);
        //glColor3f(1,0,0);
        glBegin(GL_QUAD_STRIP);
        angle = 0.0;
        int i = 0;
            while( angle < 2*pi ) {
                if(i % 2 == 0)
                {
                    glColor3f(0, 0, 0);
                }
                else{
                    glColor3f(1, 1, 1);
                }
                x = radius * cos(angle);
                y = radius * sin(angle);
                glVertex3f(x, y , height);
                glVertex3f(x, y , 0.0);
                angle = angle + angle_stepsize;
                i += 1;
            }
            glVertex3f(radius, 0.0, height);
            glVertex3f(radius, 0.0, 0.0);
        glEnd();

    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0, cylinderHeight + cylinderRadius, 0);
        //draw_sphere_new(cylinderRadius, 0);
        drawSphere(cylinderRadius, slices, stacks, 2);
    }
    glPopMatrix();

}
void draw_second_cylinder(GLfloat radius, GLfloat height)
{
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    glPushMatrix();
    {
        glTranslatef(0, 2 * cylinderRadius + cylinderHeight + second_cylinder_radius, 0);
        //glColor3f(0, 1, 0);
        draw_sphere_new(second_cylinder_radius, 1);
        //drawSphere(second_cylinder_radius, slices, stacks, 1);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(0, 2 * cylinderRadius + cylinderHeight + second_cylinder_radius, 0);
        glRotatef(-90, 1, 0, 0);
        //glColor3f(1,0,0);
        glBegin(GL_QUAD_STRIP);
        angle = 0.0;
        int i = 0;
            while( angle < 2*pi ) {
                if(i % 2 == 0)
                {
                    glColor3f(0, 0, 0);
                }
                else
                {
                    glColor3f(1, 1, 1);
                }
                x = radius * cos(angle);
                y = radius * sin(angle);
                glVertex3f(x, y , height);
                glVertex3f(x, y , 0.0);
                angle = angle + angle_stepsize;
                i += 1;
            }
            glVertex3f(radius, 0.0, height);
            glVertex3f(radius, 0.0, 0.0);
        glEnd();

    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(0, 2 * cylinderRadius + cylinderHeight + second_cylinder_radius + second_cylinder_height, 0);
        //glColor3f(0, 0, 1);
        glRotatef(90, 1, 0, 0);
        drawSphere(second_cylinder_radius, slices, stacks, 3);
    }
    glPopMatrix();

}
void drawSphere(double radius,int slices,int stacks, int m)
{
	int start_j, condition_j;
	if(m == 1)
    {
        start_j = slices/2;
        condition_j = slices;
    }
    else if(m == 2)
    {
        start_j = 0;
        condition_j = slices /2;
    }
    else{
        start_j = 0;
        condition_j = slices;
    }
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		if(m == 3)
        {
            r=2* radius - radius*cos(((double)i/(double)stacks)*(pi/2));
        }
        else
        {
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
        }
		//r=2* radius - radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=start_j;j<=condition_j;j++)
		{

			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);


		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{

                if(j % 2 == 0)
                {
                    glColor3f(0, 0, 0);
                }
                else
                {
                    glColor3f(1, 1, 1);
                }
                if(m != 3)
                {
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                }
                //lower hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSS()
{

        drawSquare(60, {0, 300, 0});


}
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':

			l.v_x = l.v_x * cos(cameraAngle) - r.v_x * sin(cameraAngle);
            l.v_y = l.v_y * cos(cameraAngle) - r.v_y * sin(cameraAngle);
            l.v_z = l.v_z * cos(cameraAngle) - r.v_z * sin(cameraAngle);
            r.v_x = r.v_x * cos(cameraAngle) + l.v_x * sin(cameraAngle);
            r.v_y = r.v_y * cos(cameraAngle) + l.v_y * sin(cameraAngle);
            r.v_z = r.v_z * cos(cameraAngle) + l.v_z * sin(cameraAngle);
			break;
        case '2':
            //cameraAngle += 0.1;
            l.v_x = l.v_x * cos(cameraAngle) + r.v_x * sin(cameraAngle);
            l.v_y = l.v_y * cos(cameraAngle) + r.v_y * sin(cameraAngle);
            l.v_z = l.v_z * cos(cameraAngle) + r.v_z * sin(cameraAngle);
            r.v_x = r.v_x * cos(cameraAngle) - l.v_x * sin(cameraAngle);
            r.v_y = r.v_y * cos(cameraAngle) - l.v_y * sin(cameraAngle);
            r.v_z = r.v_z * cos(cameraAngle) - l.v_z * sin(cameraAngle);
            break;
        case '3':
            l.v_x = l.v_x * cos(cameraAngle) + u.v_x * sin(cameraAngle);
            l.v_y = l.v_y * cos(cameraAngle) + u.v_y * sin(cameraAngle);
            l.v_z = l.v_z * cos(cameraAngle) + u.v_z * sin(cameraAngle);
            u.v_x = u.v_x * cos(cameraAngle) - l.v_x * sin(cameraAngle);
            u.v_y = u.v_y * cos(cameraAngle) - l.v_y * sin(cameraAngle);
            u.v_z = u.v_z * cos(cameraAngle) - l.v_z * sin(cameraAngle);

            break;
        case '4':
            l.v_x = l.v_x * cos(cameraAngle) - u.v_x * sin(cameraAngle);
            l.v_y = l.v_y * cos(cameraAngle) - u.v_y * sin(cameraAngle);
            l.v_z = l.v_z * cos(cameraAngle) - u.v_z * sin(cameraAngle);
            u.v_x = u.v_x * cos(cameraAngle) + l.v_x * sin(cameraAngle);
            u.v_y = u.v_y * cos(cameraAngle) + l.v_y * sin(cameraAngle);
            u.v_z = u.v_z * cos(cameraAngle) + l.v_z * sin(cameraAngle);

            break;
        case '5':
            u.v_x = u.v_x * cos(cameraAngle) + r.v_x * sin(cameraAngle);
            u.v_y = u.v_y * cos(cameraAngle) + r.v_y * sin(cameraAngle);
            u.v_z = u.v_z * cos(cameraAngle) + r.v_z * sin(cameraAngle);
            r.v_x = r.v_x * cos(cameraAngle) - u.v_x * sin(cameraAngle);
            r.v_y = r.v_y * cos(cameraAngle) - u.v_y * sin(cameraAngle);
            r.v_z = r.v_z * cos(cameraAngle) - u.v_z * sin(cameraAngle);
            break;
        case '6':
            u.v_x = u.v_x * cos(cameraAngle) - r.v_x * sin(cameraAngle);
            u.v_y = u.v_y * cos(cameraAngle) - r.v_y * sin(cameraAngle);
            u.v_z = u.v_z * cos(cameraAngle) - r.v_z * sin(cameraAngle);
            r.v_x = r.v_x * cos(cameraAngle) + u.v_x * sin(cameraAngle);
            r.v_y = r.v_y * cos(cameraAngle) + u.v_y * sin(cameraAngle);
            r.v_z = r.v_z * cos(cameraAngle) + u.v_z * sin(cameraAngle);
            break;
        case 'q':
            cylinder_angle_a += 0.5;
            if(cylinder_angle_a > 45)
            {
                cylinder_angle_a = 45;
            }
            first.y = (2 * cylinderRadius + cylinderHeight) * cos(cylinder_angle_a*.0175);
            first.x = (2 * cylinderRadius + cylinderHeight) * sin(cylinder_angle_a*.0175);
            second.y = (second_cylinder_height + second_cylinder_height) * cos((angle_up_a+angle_up_b)*.0175);
            second.z = (second_cylinder_height + second_cylinder_height) * sin((angle_up_a+angle_up_b)*.0175);
            x_dist = second.x - first.x;
            y_dist = second.y - first.y;
            z_dist = second.z - first.z;
            t_e = (260 - second.y) / y_dist;
            gun.v_x = second.x + t_e * x_dist;
            gun.v_y = second.y + t_e * y_dist;
            gun.v_z = second.z + t_e * z_dist;
            //bullet_pos.push_back(gun);
            cout << "gun.v_x: " << gun.v_x << " gun.v_y: " << gun.v_y << " gun.v_z: "<< gun.v_z<< endl;
            break;
        case 'w':
            cylinder_angle_a -= 0.5;
            if(cylinder_angle_a < -45)
            {
                cylinder_angle_a = -45;
            }
            first.y = (2 * cylinderRadius + cylinderHeight) * cos(cylinder_angle_a*.0175);
            first.x = (2 * cylinderRadius + cylinderHeight) * sin(cylinder_angle_a*.0175);
            second.y = (second_cylinder_height + second_cylinder_height) * cos((angle_up_a+angle_up_b)*.0175);
            second.z = (second_cylinder_height + second_cylinder_height) * sin((angle_up_a+angle_up_b)*.0175);
            x_dist = second.x - first.x;
            y_dist = second.y - first.y;
            z_dist = second.z - first.z;
            t_e = (260 - second.y) / y_dist;
            gun.v_x = second.x + t_e * x_dist;
            gun.v_y = second.y + t_e * y_dist;
            gun.v_z = second.z + t_e * z_dist;
            //bullet_pos.push_back(gun);
            cout << "gun.v_x: " << gun.v_x << " gun.v_y: " << gun.v_y << " gun.v_z: "<< gun.v_z<< endl;

            break;
        case 'e':
            angle_up_a += 0.5;
            if(angle_up_a > 45)
            {
                angle_up_a = 45;
            }
            cout << "angle: " << angle_up_a << endl;
            first.y = (2 * cylinderRadius + cylinderHeight) * cos(angle_up_a*.0175);
            first.z = (2 * cylinderRadius + cylinderHeight) * sin(angle_up_a*.0175);
            second.y = (second_cylinder_height + second_cylinder_height) * cos((angle_up_a + angle_up_b)*.0175);
            second.z = (second_cylinder_height + second_cylinder_height) * sin((angle_up_a + angle_up_b)*.0175);
            x_dist = second.x - first.x;
            y_dist = second.y - first.y;
            z_dist = second.z - first.z;
            t_e = (260 - second.y) / y_dist;
            gun.v_x = second.x + t_e * x_dist;
            gun.v_y = second.y + t_e * y_dist;
            gun.v_z = second.z + t_e * z_dist;
            //bullet_pos.push_back(gun);
            cout << "gun.v_x: " << gun.v_x << " gun.v_y: " << gun.v_y << " gun.v_z: "<< gun.v_z<< endl;
            break;
        case 'r':
            angle_up_a -= 0.5;
            if(angle_up_a < -45)
            {
                angle_up_a = -45;
            }
            cout << "angle: " << angle_up_a << endl;
            first.y = (2 * cylinderRadius + cylinderHeight) * cos(angle_up_a*.0175);
            first.z = (2 * cylinderRadius + cylinderHeight) * sin(angle_up_a*.0175);
            second.y = (second_cylinder_height + second_cylinder_height) * cos((angle_up_a + angle_up_b)*.0175);
            second.z = (second_cylinder_height + second_cylinder_height) * sin((angle_up_a + angle_up_b)*.0175);
            x_dist = second.x - first.x;
            y_dist = second.y - first.y;
            z_dist = second.z - first.z;
            t_e = (260 - second.y) / y_dist;
            gun.v_x = second.x + t_e * x_dist;
            gun.v_y = second.y + t_e * y_dist;
            gun.v_z = second.z + t_e * z_dist;
            cout << "gun.v_x: " << gun.v_x << " gun.v_y: " << gun.v_y << " gun.v_z: "<< gun.v_z<< endl;

            break;
        case 'a':
            angle_up_b += 0.5;
            if(angle_up_b > 45)
            {
                angle_up_b = 45;
            }
            first.y = (2 * cylinderRadius + cylinderHeight) * cos(angle_up_a*.0175);
            first.z = (2 * cylinderRadius + cylinderHeight) * sin(angle_up_a*.0175);
            second.y = (second_cylinder_height + second_cylinder_height) * cos((angle_up_a + angle_up_b)*.0175);
            second.z = (second_cylinder_height + second_cylinder_height) * sin((angle_up_a + angle_up_b)*.0175);
            x_dist = second.x - first.x;
            y_dist = second.y - first.y;
            z_dist = second.z - first.z;
            t_e = (260 - second.y) / y_dist;
            gun.v_x = second.x + t_e * x_dist;
            gun.v_y = second.y + t_e * y_dist;
            gun.v_z = second.z + t_e * z_dist;
            cout << "gun.v_x: " << gun.v_x << " gun.v_y: " << gun.v_y << " gun.v_z: "<< gun.v_z<< endl;

            break;
        case 's':
            angle_up_b -= 0.5;
            if(angle_up_b < -45)
            {
                angle_up_b = -45;
            }
            first.y = (2 * cylinderRadius + cylinderHeight) * cos(angle_up_a*.0175);
            first.z = (2 * cylinderRadius + cylinderHeight) * sin(angle_up_a*.0175);
            second.y = (second_cylinder_height + second_cylinder_height) * cos((angle_up_a + angle_up_b)*.0175);
            second.z = (second_cylinder_height + second_cylinder_height) * sin((angle_up_a + angle_up_b)*.0175);
            x_dist = second.x - first.x;
            y_dist = second.y - first.y;
            z_dist = second.z - first.z;
            t_e = (260 - second.y) / y_dist;
            gun.v_x = second.x + t_e * x_dist;
            gun.v_y = second.y + t_e * y_dist;
            gun.v_z = second.z + t_e * z_dist;
            cout << "gun.v_x: " << gun.v_x << " gun.v_y: " << gun.v_y << " gun.v_z: "<< gun.v_z<< endl;

            break;
        case 'd':
            rotate_b -= 0.5;
            if(rotate_b < -45)
            {
                rotate_b = -45;
            }
        case 'f':
            rotate_b += 0.5;
            if(rotate_b > 45)
            {
                rotate_b = 45;
            }
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	float fraction = 0.9;
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
            pos.y -= l.v_y * fraction;
			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;
			pos.y += l.v_y * fraction;
			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			pos.x += r.v_x * fraction;
			//pos.y += r.v_y * fraction;
			break;
		case GLUT_KEY_LEFT:
			//cameraAngle -= 0.03;
			pos.x -= r.v_x * fraction;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.z += u.v_z * fraction;
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos.z -= u.v_z * fraction;
			break;
		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){
				if(gun.v_x > -60 && gun.v_x < 60 && gun.v_z > -60 && gun.v_z < 60 && gun.v_y == 260)
                {
                        bullet_pos.push_back(gun);
                        gun = {0, 0, 0};
                        cout << gun.v_x << " " << gun.v_y << " " << gun.v_z;
                }

			}

			break;


		default:
			break;
	}
}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

    gluLookAt(pos.x, pos.y, pos.z, pos.x + l.v_x, pos.y + l.v_y, pos.z + l.v_z, u.v_x, u.v_y, u.v_z);

	glMatrixMode(GL_MODELVIEW);

	drawAxes();

    glPushMatrix();
    {

            glRotatef(cylinder_angle_a, 0, 0 ,1);
            glRotatef(angle_up_a, 1, 0, 0);
            draw_cylinder(cylinderRadius, cylinderHeight);
            glTranslatef(0, 2*cylinderRadius + cylinderHeight, 0);
            glRotatef(angle_up_b, 1, 0, 0);
            glTranslatef(0, -2*cylinderRadius - cylinderHeight, 0);
            glRotatef(rotate_b, 0, 1, 0);
            draw_second_cylinder(second_cylinder_radius, second_cylinder_height);
            //q = 0;
    }
    glPopMatrix();

    glPushMatrix();
    {
        for(vector<struct vector_3>::iterator it = bullet_pos.begin(); it != bullet_pos.end(); it++)
        {
                struct vector_3 pos_V = *it;
                //glTranslatef(pos_V.v_x,  pos_V.v_y, pos_V.v_z);
                //glTranslatef(0, 190, 0);

                glColor3f(1, 0, 0);
                drawSquare(4, pos_V);
        }

    }
    glPopMatrix();
    glColor3ub(105,105,105);
    drawSS();

	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=5.0;
	cameraAngle=0.05;
	angle=0;
	pos = {-90, -90, 20};
	gun = {0, 260, 0};
    u = {0, 0, 1};
    r = {1/ sqrt(2), -1/ sqrt(2), 0};
    //r = {.814, -.356, -.46};
    l = {1/ sqrt(2), 1/ sqrt(2), 0};
    //l = {1/3, 14/15, -2/15};
	//clear the screen
	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(80,	1,	1,	1000.0);
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Fully controllable camera and gun");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
