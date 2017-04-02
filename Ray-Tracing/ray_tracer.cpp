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

RayTraicer::RayTraicer(std::string file) {
    std::ifstream in(file);
    Vector cam, lh, rh, ld;
    std::map<std::string, Color> materials;
    scene_ = Scene();
    for(std::string line; std::getline(in, line);) {
        std::istringstream iss(line);
        std::string sub;
        iss >> sub;
        if(sub[0] == '#') { continue;}
        if(sub == "viewport") {
            long double x, y, z;
            while(sub != "endviewport") {
                std::getline(in, line);
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
                iss = std::istringstream(line);
                iss >> sub;
                if(sub[0] == '#') {
                    continue;
                }
                if(sub == "entry") {
                    std::string name;
                    long double r, g, b, alpha = 1, refl = 0, refr = 0;
                    while (sub != "endentry") {
                        std::getline(in, line);
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
            long double basePower, baseDist;
            while(sub != "endlights") {
                std::getline(in, line);
                iss = std::istringstream(line);
                iss >> sub;
                if(sub[0] == '#') {
                    continue;
                }
                if(sub == "reference") {
                    while(sub != "endreference") {
                        std::getline(in, line);
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
                    long double power;
                    Vector p;
                    while(sub != "endpoint") {
                        std::getline(in, line);
                        iss = std::istringstream(line);
                        iss >> sub;
                        if(sub[0] == '#') {
                            continue;
                        }
                        if(sub == "coords") {
                            long double x, y, z;
                            iss >> x >> y >> z;
                            p = Vector(x, y, z);
                        }
                        if(sub == "power") {
                            iss >> power;
                        }
                    }
                    scene_.addLight(LightSource(p, power * baseDist / basePower));
                }
            }
        }
        if(sub == "geometry") {
            while(sub != "endgeometry") {
                std::getline(in, line);
                iss = std::istringstream(line);
                iss >> sub;
                if(sub[0] == '#') continue;
                if(sub == "sphere") {
                    long double rad;
                    Vector p;
                    std::string mat;
                    while(sub != "endsphere") {
                        std::getline(in, line);
                        iss = std::istringstream(line);
                        iss >> sub;
                        if(sub[0] == '#') continue;
                        if(sub == "coords") {
                            long double x, y, z;
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
                        iss = std::istringstream(line);
                        iss >> sub;
                        if(sub[0] == '#') continue;
                        if(sub == "vertex") {
                            long double x, y, z;
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
                        iss = std::istringstream(line);
                        iss >> sub;
                        if(sub[0] == '#') continue;
                        if(sub == "vertex") {
                            long double x, y, z;
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

void RayTraicer::traceRays(int start, QImage &img) {
    if(start == -1) {
        for (int y = 0; y <= resY_; ++y) {
            for (int x = 0; x <= resX_; ++x) {
                Ray ray = camera_.getRay(resX_, resY_, x, y);
                SolidObject* obj = scene_.traceRay(ray).getObject();
                if(obj != nullptr) {
                    Color col = obj->getColor();
                    img.setPixelColor(x, y, QColor(col.getRed() * 255, col.getGreen() * 255, col.getBlue() * 255));
                } else {
                    img.setPixelColor(x, y, QColor(0, 0, 0));
                }
            }
        }
    } else {
        for (int y = start; y <= resY_; y+=8) {
            for (int x = 0; x <= resX_; ++x) {
                Ray ray = camera_.getRay(resX_, resY_, x, y);
                SolidObject* obj = scene_.traceRay(ray).getObject();
                if(obj != nullptr) {
                    Color col = obj->getColor();
                    img.setPixelColor(x, y, QColor(col.getRed() * 255, col.getGreen() * 255, col.getBlue() * 255));
                } else {
                    img.setPixelColor(x, y, QColor(0, 0, 0));
                }
            }
        }
    }
}

void RayTraicer::run(QImage &img) {
    std::thread tr1(&RayTraicer::traceRays, this, 0, std::ref(img)), tr2(&RayTraicer::traceRays, this, 1, std::ref(img));
    std::thread tr3(&RayTraicer::traceRays, this, 2, std::ref(img)), tr4(&RayTraicer::traceRays, this, 3, std::ref(img));
    std::thread tr5(&RayTraicer::traceRays, this, 4, std::ref(img)), tr6(&RayTraicer::traceRays, this, 5, std::ref(img));
    std::thread tr7(&RayTraicer::traceRays, this, 6, std::ref(img)), tr8(&RayTraicer::traceRays, this, 7, std::ref(img));
    tr1.join(); tr2.join(); tr3.join(); tr4.join();
    tr5.join(); tr6.join(); tr7.join(); tr8.join();
}
