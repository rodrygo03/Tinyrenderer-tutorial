#include "vendor/tgaimage/tgaimage.h"
#include "vendor/model/model.h"
#include "vendor/model/geometry.h"

#include <vector>
#include <iostream>


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
	bool transpose = false;
	if (std::abs(x0-x1) < std::abs(y0-y1))	// transpose x and y
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		transpose = true;
	}
	if (x0 > x1)	// draw left to right
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	for (int x=x0; x<=x1; x++)
	{
		float t = (x-x0) / (float)(x1-x0);
		int y = y0*(1-t) + y1*t;
		if (transpose)
			image.set(y, x, color);	
		else	
			image.set(x, y, color);
	}
}


void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color)
{
	if (t0.y > t1.y)
	 	std::swap(t0, t1);
	if (t0.y > t2.y)
		std::swap(t0, t2);
	if (t1.y > t2.y)
		std::swap(t1, t2);
	
	int t_height = t2.y - t0.y;
	for (int y=t0.y; y<=t1.y; y++)
	{
		int segment_height = t1.y - t0.y + 1;
		float alpha = (float) (y-t0.y) / t_height;
		float beta = (float) (y-t0.y) / segment_height; // handle division by zero
		Vec2i A = t0 + (t2-t0)*alpha;
		Vec2i B = t0 + (t1-t0)*beta;
		if (A.x > B.x)
			std::swap(A, B);
		for (int j=A.x; j<=B.x; j++)
		{
			image.set(j, y, color);
		}
	}
	for (int y=t1.y; y<=t2.y; y++)
	{
		int segment_height = t2.y - t1.y + 1;
		float alpha = (float) (y-t0.y) / t_height;
		float beta = (float) (y-t1.y) / segment_height; // handle division by zero
		Vec2i A = t0 + (t2-t0)*alpha;
		Vec2i B = t1 + (t2-t1)*beta;
		if (A.x > B.x)
			std::swap(A, B);
		for (int j=A.x; j<=B.x; j++)
		{
			image.set(j, y, color);
		}
	}

}

int main(int argc, char** argv) 
{
	const int width = 800;
	const int height = 800;
	
	// Traingle rasterization demo:
	Model* model = new Model("../res/obj/african_head.obj");
	
    TGAImage image(width, height, TGAImage::RGB);
    Vec3f light_dir(0,0,-1);
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j=0; j<3; j++) {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = Vec2i((v.x+1.)*width/2., (v.y+1.)*height/2.);
            world_coords[j]  = v;
        }
        Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        n.normalize();
        float intensity = n*light_dir;
        if (intensity>0) {
            triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
        }
    }

	image.flip_vertically(); // origin at the left bottom corner of the image
	image.write_tga_file("triangle_rasterization.tga");
	delete model;
	return 0;
}
