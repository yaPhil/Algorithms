//
// Created by philipp on 31.03.17.
//

#include "ray_tracer.h"
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include "light_source.h"

bool strIsUninformative(std::string &str)
{
    for (uint i = 0; i < str.size(); ++i) {
        if (str[i] == '#')
            return true;
        if (str[i] != ' ' || str[i] != '\t' || str[i] != '\n' || str[i] != '\r')
            return false;
    }
    return true;
}

RayTraicer::RayTraicer(std::string file) {
    std::ifstream in(file);
    Vector cam, lh, rh, ld;
    std::map<std::string, Color> materials;
    scene_ = Scene();
    for(std::string line; std::getline(in, line);) {
        if(strIsUninformative(line)) { continue; }
        std::istringstream iss(line);
        std::string sub;
        iss >> sub;
        if(sub[0] == '#') { continue;}
        if(sub == "viewport") {
            double x, y, z;
            while(sub != "endviewport") {
                std::getline(in, line);
                if(strIsUninformative(line)) { continue; }
                iss = std::istringstream(line);
                iss >> sub;
                if(sub[0] == '#')
                    continue;
                if(sub == "origin") {
                    iss >> x >> y >> z;
                    cam = Vector(x, y, z);
                    continue;
                }
                if(sub == "topleft") {
                    iss >> x >> y >> z;
                    lh = Vector(x, y, z);
                    continue;
                }
                if(sub == "topright") {
                    iss >> x >> y >> z;
                    rh = Vector(x, y, z);
                    continue;
                }
                if(sub == "bottomleft") {
                    iss >> x >> y >> z;
                    ld = Vector(x, y, z);
                    continue;
                }
                if(sub == "resolution") {
                    iss >> resX_ >> resY_;
                }
            }
            camera_ = Camera(cam, lh, rh, ld);
        }
        if(sub == "materials") {
            while(sub != "endmaterials") {
                std::getline(in, line);
                if(strIsUninformative(line)) { continue; }
                iss = std::istringstream(line);
                iss >> sub;
                if(sub[0] == '#') {
                    continue;
                }
                if(sub == "entry") {
                    std::string name;
                    double r, g, b, alpha = 1, refl = 0, refr = 0;
                    while (sub != "endentry") {
                        std::getline(in, line);
                        if(strIsUninformative(line)) { continue; }
                        iss = std::istringstream(line);
                        iss >> sub;
                        if(sub[0] == '#') continue;
                        if(sub == "name") {
                            iss >> name;
                        }
                        if(sub == "color") {
                            iss >> r >> g >> b;
                            r /= 255.0; g /= 255.0; b /= 255.0;
                        }
                        if(sub == "alpha") {
                            iss >> alpha;
                        }
                        if(sub == "reflect") {
                            iss >> refl;
                        }
                        if(sub == "refract") {
                            iss >> refr;
                        }
                    }
                    materials.insert(std::make_pair(name, Color(r, g, b, alpha, refl, refr)));
                }
            }
        }
        if(sub == "lights") {
            double basePower, baseDist;
            while(sub != "endlights") {
                std::getline(in, line);
                if(strIsUninformative(line)) { continue; }
                iss = std::istringstream(line);
                iss >> sub;
                if(sub[0] == '#') {
                    continue;
                }
                if(sub == "reference") {
                    while(sub != "endreference") {
                        std::getline(in, line);
                        if(strIsUninformative(line)) { continue; }
                        iss = std::istringstream(line);
                        iss >> sub;
                        if(sub[0] == '#') {
                            continue;
                        }
                        if(sub == "power") {
                            iss >> basePower;
                        }
                        if(sub == "distance") {
                            iss >> baseDist;
                        }
                    }
                }
                if(sub == "point") {
                    double power;
                    Vector p;
                    while(sub != "endpoint") {
                        std::getline(in, line);
                        if(strIsUninformative(line)) { continue; }
                        iss = std::istringstream(line);
                        iss >> sub;
                        if(sub[0] == '#') {
                            continue;
                        }
                        if(sub == "coords") {
                            double x, y, z;
                            iss >> x >> y >> z;
                            p = Vector(x, y, z);
                        }
                        if(sub == "power") {
                            iss >> power;
                        }
                    }
                    scene_.addLight(LightSource(p, power * power * baseDist * baseDist / (basePower * basePower)));
                }
            }
        }
        if(sub == "geometry") {
            while(sub != "endgeometry") {
                std::getline(in, line);
                if(strIsUninformative(line)) { continue; }
                iss = std::istringstream(line);
                iss >> sub;
                if(sub[0] == '#') continue;
                if(sub == "sphere") {
                    double rad;
                    Vector p;
                    std::string mat;
                    while(sub != "endsphere") {
                        std::getline(in, line);
                        if(strIsUninformative(line)) { continue; }
                        iss = std::istringstream(line);
                        iss >> sub;
                        if(sub[0] == '#') continue;
                        if(sub == "coords") {
                            double x, y, z;
                            iss >> x >> y >> z;
                            p = Vector(x, y, z);
                        }
                        if(sub == "radius") {
                            iss >> rad;
                        }
                        if(sub == "material") {
                            iss >> mat;
                        }
                    }
                    scene_.addObject(new Sphere(materials[mat], p, rad));
                }
                if(sub == "triangle") {
                    Vector a, b, c;
                    bool isAset = false, isBset = false, isCset = false;
                    std::string mat;
                    while(sub != "endtriangle") {
                        std::getline(in, line);
                        if(strIsUninformative(line)) { continue; }
                        iss = std::istringstream(line);

                        iss >> sub;
                        if(sub[0] == '#') continue;
                        if(sub == "vertex") {
                            double x, y, z;
                            iss >> x >> y >> z;
                            if(!isAset && !isBset && !isCset) {
                                a = Vector(x, y, z);
                                isAset = true;
                                continue;
                            }
                            if(isAset && !isBset && !isCset) {
                                b = Vector(x, y, z);
                                isBset = true;
                                continue;
                            }
                            if(isAset && isBset && !isCset) {
                                c = Vector(x, y, z);
                                isCset = true;
                                continue;
                            }
                        }
                        if(sub == "material") {
                            iss >> mat;
                        }
                    }
                    scene_.addObject(new Triangle(materials[mat], a, b, c));
                }
                if(sub == "quadrangle") {
                    Vector a, b, c, d;
                    bool isAset = false, isBset = false, isCset = false;
                    std::string mat;
                    while(sub != "endquadrangle") {
                        std::getline(in, line);
                        if(strIsUninformative(line)) { continue; }
                        iss = std::istringstream(line);
                        iss >> sub;
                        if(sub[0] == '#') continue;
                        if(sub == "vertex") {
                            double x, y, z;
                            iss >> x >> y >> z;
                            if(!isAset && !isBset && !isCset) {
                                a = Vector(x, y, z);
                                isAset = true;
                                continue;
                            }
                            if(isAset && !isBset && !isCset) {
                                b = Vector(x, y, z);
                                isBset = true;
                                continue;
                            }
                            if(isAset && isBset && !isCset) {
                                c = Vector(x, y, z);
                                isCset = true;
                                continue;
                            }
                            if(isAset && isBset && isCset) {
                                d = Vector(x, y, z);
                                continue;
                            }

                        }
                        if(sub == "material") {
                            iss >> mat;
                        }
                    }
                    //napisat chetirehugolnik
                    //
                    //
                }
            }
        }
    }
}

void RayTraicer::addObject(SolidObject *obj) {
    scene_.addObject(obj);
}

void RayTraicer::addLight(LightSource light) {
    scene_.addLight(light);
}

int RayTraicer::getResX() {
    return resX_;
}

int RayTraicer::getResY() {
    return resY_;
}

Vector RayTraicer::getIllumination(Ray ray, Vector point, SolidObject* obj) {
    Color baseColor = obj->getColor();
    Vector baseCol = Vector(baseColor.getRed(), baseColor.getGreen(), baseColor.getBlue());
    Vector color = baseCol * LightSource::baseShining;
    for(int i = 0; i < scene_.getLightsNumber(); ++i) {
        LightSource lighter = scene_.getLight(i);
        Ray light = Ray(point, lighter.getPosition());
        Vector toLight = lighter.getPosition() - point;
        if(dotProduct(obj->getNorm(point), -toLight) * dotProduct(obj->getNorm(point), (point - ray.getBegin())) < 0) {
            continue;
        }
        Intersect lightInter = scene_.traceRay(light, obj);
        if(!lightInter.getResult() || (toLight.length() - (lightInter.getPoint() - point).length()  <= EPS)) {
            color = color + (baseCol * lighter.getIntense() * std::abs(dotProduct(obj->getNorm(point), toLight))
                             / ((toLight).sqrLength() * toLight.length()));
        }
    }
    return Vector(std::min(color.getX(), (double)1), std::min(color.getY(), (double)1), std::min(color.getZ(), (double)1));
}

Vector RayTraicer::getColor(Ray ray, int depth) {
    Intersect inter = scene_.traceRay(ray, nullptr);
    if(!inter.getResult()) {
        return Vector();
    }
    SolidObject* obj = inter.getObject();
    Color baseMaterial = obj->getColor();
    Vector point = inter.getPoint();
    Vector baseColor = getIllumination(ray, point, obj);
    if(depth == MAX_RECURSIVE_DEPTH)
        return baseColor;
    Vector rayDirection = ray.getBegin() - point;
    Vector norm = obj->getNorm(point);
    Vector reflectedDirection = (norm * dotProduct(rayDirection, norm) * 2.0) - rayDirection;
    Ray reflectedRay = Ray(point + reflectedDirection.normed() * EPS_MARGIN, point + reflectedDirection);
    Vector reflectedColor;
    if(baseMaterial.getReflection() != 0) {
        reflectedColor = getColor(reflectedRay, depth + 1);
    } else {
        reflectedColor = Vector();
    }
    //napisat prelomlenie lucha
    //
    //
    rayDirection = point - ray.getBegin();
    double cos = -dotProduct(rayDirection, norm) / rayDirection.length() / norm.length();
    double coef;
    Vector refractedColor;
    Ray refractedRay;
    if (baseMaterial.getRefraction() != 0) {
        if (cos < -EPS) {
            cos *= -1;
            norm = -norm;
            coef = std::max(baseMaterial.getRefraction(), 1.0 / baseMaterial.getRefraction());
        }
        else
        {
            coef = std::min(baseMaterial.getRefraction(), 1.0 / baseMaterial.getRefraction());
        }

        double k = 1.0 - coef * coef * (1.0 - cos * cos);
        if (k >= 0){
            Vector refractedDirection = (coef * rayDirection + (coef * cos - std::sqrt(k)) * norm).normed();
            refractedRay = Ray(point + refractedDirection.normed() * EPS_MARGIN, point + refractedDirection);
            refractedColor = getColor(refractedRay, depth + 1);
        }
    }
    return reflectedColor * baseMaterial.getReflection() + (1 - baseMaterial.getReflection()) *
            (baseMaterial.getAlpha() * baseColor + (1 - baseMaterial.getAlpha()) * refractedColor);
}

void RayTraicer::setColor(int x, int y, QImage &img, int aa) {
    Vector meanColor;
    for (int i = 0; i < aa; ++i) {
        for (int j = 0; j < aa; ++j) {
            Ray ray = camera_.getRay(resX_ * aa + aa - 1 , resY_ * aa + aa - 1, x * aa + i, y * aa + j);
            Vector color = getColor(ray, 0);
            meanColor = meanColor + color;
        }
    }
    meanColor = meanColor / (aa * aa);

    img.setPixelColor(x, y, QColor(std::min(meanColor.getX(), (double)1) * 255, std::min(meanColor.getY(), (double)1) * 255,
                                       std::min(meanColor.getZ(), (double)1) * 255));
}

void RayTraicer::traceRays(int start, QImage &img, int aa) {
    if(start == -1) {
        for (int y = 0; y <= resY_; ++y) {
            for (int x = 0; x <= resX_; ++x) {
                if (x == 250 && y == 330) {
                    std::cout << "stop";
                }
                setColor(x, y, img, aa);
            }
        }
    } else {
        for (int y = start; y <= resY_; y+=8) {
            for (int x = 0; x <= resX_; x++) {
                setColor(x, y, img, aa);
            }
        }
    }
}

void RayTraicer::run(QImage &img, int aa) {
    std::thread tr1(&RayTraicer::traceRays, this, 0, std::ref(img), aa), tr2(&RayTraicer::traceRays, this, 1, std::ref(img), aa);
    std::thread tr3(&RayTraicer::traceRays, this, 2, std::ref(img), aa), tr4(&RayTraicer::traceRays, this, 3, std::ref(img), aa);
    std::thread tr5(&RayTraicer::traceRays, this, 4, std::ref(img), aa), tr6(&RayTraicer::traceRays, this, 5, std::ref(img), aa);
    std::thread tr7(&RayTraicer::traceRays, this, 6, std::ref(img), aa), tr8(&RayTraicer::traceRays, this, 7, std::ref(img), aa);
    tr1.join(); tr2.join(); tr3.join(); tr4.join();
    tr5.join(); tr6.join(); tr7.join(); tr8.join();
}
