#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include<iostream>

std::vector<double>vx,vy,vz;

void read_vertex(const std::string& filename) {
    std::ifstream f(filename);
    if(!f){
        std::cout<<"file open failed!"<<std::endl;
        return ;
    }
    std::string line;
    while(std::getline(f,line)){
        if(line[0]=='v'&&line[1]==' '){
            std::istringstream in(line);
            std::string _;double x,y,z;
            in>>_>>x>>y>>z;
            vx.push_back(x);
            vy.push_back(y);
            vz.push_back(z);
        }
    }
    std::cout<<"have already gotten"<<vx.size()<<" vertixes"<<std::endl;
    return ;
}

void project_and_save(const std::string& infilename, const std::string& outfilename, int width, int height){
    std::ifstream in(infilename);
    std::ofstream out(outfilename);
    if(!in) {std::cout<<"cnmd infile"<<infilename<<" can't open!"<<std::endl; return;}
    if(!out) {std::cout<<"cnmd outfile"<<outfilename<<" can't open!"<<std::endl;return ;}
    std::string line;
    while(std::getline(in,line)){
        if(!(line[0]=='f'&&line[1]==' ')) continue ;
        std::string _,i1,i2,i3;
        std::istringstream iss(line);
        iss>>_>>i1>>i2>>i3;
        //透明
        int _i1 = std::stoi(i1)-1;
        int _i2 = std::stoi(i2)-1; 
        int _i3 = std::stoi(i3)-1;

        int sx0 = (vx[_i1] + 1.) * width  / 2.;
        int sy0 = (vy[_i1] + 1.) * height / 2.;
        int sx1 = (vx[_i2] + 1.) * width  / 2.;
        int sy1 = (vy[_i2] + 1.) * height / 2.;
        int sx2 = (vx[_i3] + 1.) * width  / 2.;
        int sy2 = (vy[_i3] + 1.) * height / 2.;

        out << sx0 << " " << sy0 << " " << sx1 << " " << sy1 << " 255 255 255\n";
        out << sx0 << " " << sy0 << " " << sx2 << " " << sy2 << " 255 255 255\n";
        out << sx2 << " " << sy2 << " " << sx1 << " " << sy1 << " 255 255 255\n";
        
        // out<<vx[_i1]<<" "<<vy[_i2]<<" "<<
    }
}