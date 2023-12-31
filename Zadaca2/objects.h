#pragma once
#include <cmath>
#include "geometry.h"
#include "material.h"

#include <iostream>
using namespace std;

struct Object
{
    Material material;
    
    virtual bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const = 0;    
};

struct Sphere : Object
{
    Vec3f c; // centar
    float r; // radius
    
    Sphere(const Vec3f &c, const float &r, const Material &mat) : c(c), r(r)
    {
        Object::material = mat;
    }
    
    bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const
    {
        float d2 = ray.direction * ray.direction;
        Vec3f e_minus_c = ray.origin - c;
        
        float disc = pow(ray.direction * e_minus_c, 2) - d2 * (e_minus_c * e_minus_c - r * r);
        
        bool ray_inside_sphere = e_minus_c * e_minus_c <= r * r;
        
        if (disc < 0)
        {
            return false;            
        }
        else
        {
            if (ray_inside_sphere)
            {
                t = (-ray.direction * e_minus_c + sqrt(disc))/d2;
            }
            else
            {
                t = (-ray.direction * e_minus_c - sqrt(disc))/d2;                
            }
            
            Vec3f normal_origin = ray.origin + ray.direction * t;
            normal = (normal_origin - c).normalize();
            
            return t > 0;
        }
    }
};



struct Cuboid : Object
{
    Vec3f min;
    Vec3f max;
    
    Cuboid(const Vec3f &min, const Vec3f &max, const Material &mat) : min(min), max(max)
    {
        Object::material = mat;
    }
    
    bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const{
        float tmin = (min[0] - ray.origin[0]) / ray.direction[0];
        float tmax = (max[0] - ray.origin[0]) / ray.direction[0];
        if(tmin > tmax) {
            swap(tmin,tmax);
        }

        float tymin = (min[1] - ray.origin[1]) / ray.direction[1];
        float tymax = (max[1] - ray.origin[1]) / ray.direction[1];

        if(tymin > tymax) swap(tymin,tymax);

        if((tmin >= tymax) || (tymin >= tmax)) 
            return false;

        if(tymin >= tmin){
            tmin = tymin;
        }

        if(tymax <= tmax){
             tmax = tymax;
         }

        float tzmin = (min[2] - ray.origin[2]) / ray.direction[2];
        float tzmax = (max[2] - ray.origin[2]) / ray.direction[2];

        if(tzmin > tzmax) swap(tzmin,tzmax);

        if((tmin >= tzmax) || (tzmin >= tmax))
            return false;

        if(tzmin >= tmin){
           tmin = tzmin;
        }

        if(tzmax <= tmax){
            tmax = tzmax;
        }
        t = tmin;
        
        if (abs((ray.origin + ray.direction * (t-0.001)).x - min.x) < 0.01)
            normal = Vec3f(-1, 0, 0);
        else if (abs((ray.origin + ray.direction * (t-0.001)).x - max.x) < 0.01)
            normal = Vec3f(1, 0, 0);
        else if (abs((ray.origin + ray.direction * (t-0.001)).y - min.y) < 0.01)
            normal = Vec3f(0, -1, 0);
        else if (abs((ray.origin + ray.direction * (t-0.001)).y - max.y) < 0.01)
            normal = Vec3f(0, 1, 0);
        else if (abs((ray.origin + ray.direction * (t-0.001)).z - min.z) < 0.01)
            normal = Vec3f(0, 0, -1);
        else if (abs((ray.origin + ray.direction * (t-0.001)).z - max.z) < 0.01)
            normal = Vec3f(0, 0, 1);

        return t > 0;
    }


};