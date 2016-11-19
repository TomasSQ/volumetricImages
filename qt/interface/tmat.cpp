#include "tmat.hpp"

#include <ostream>
#include <cmath>

#define PI 3.14159265

/*class TMat{*/

	using num=float;

	void TMat::clear(){
		xx=xy=xz=x1=0;
		yx=yy=yz=y1=0;
		zx=zy=zz=z1=0;
	}
	void TMat::identity(){
		clear();
		xx=yy=zz=1;
	}
	
	num TMat::det()const{
		num sp = xx*yy*zz + xy*yz*zx + xz*yx*zy;
		num sm = zx*yy*xz + zy*yz*xx + zz*yx*xy;
		return sp-sm;
	}
	
	TMat TMat::inv()const{
		TMat r;
		//r.11=22*33-23*32;r.12=13*32-12*33;r.13=12*23-13*22;
		//r.21=23*31-21*33;r.22=11*33-13*31;r.23=13*21-11*23;
		//r.31=21*32-22*31;r.32=12*31-11*32;r.33=11*22-12*21;
		r.xx=yy*zz-yz*zy;r.xy=xz*zy-xy*zz;r.xz=xy*yz-xz*yy;
		r.yx=yz*zx-yx*zz;r.yy=xx*zz-xz*zx;r.yz=xz*yx-xx*yz;
		r.zx=yx*zy-yy*zx;r.zy=xy*zx-xx*zy;r.zz=xx*yy-xy*yx;
		//r.14= 12*24*33 + 13*22*34 + 14*23*32 - 12*23*34 - 13*24*32 - 14*22*33;
		//r.24= 11*23*34 + 13*24*31 + 14*21*33 - 11*24*33 - 13*21*34 - 14*23*31;
		//r.34= 11*24*32 + 12*21*34 + 14*22*31 - 11*22*34 - 12*24*31 - 14*21*32;
		r.x1= xy*y1*zz + xz*yy*z1 + x1*yz*zy - xy*yz*z1 - xz*y1*zy - x1*yy*zz;
		r.y1= xx*yz*z1 + xz*y1*zx + x1*yx*zz - xx*y1*zz - xz*yx*z1 - x1*yz*zx;
		r.z1= xx*y1*zy + xy*yx*z1 + x1*yy*zx - xx*yy*z1 - xy*y1*zx - x1*yx*zy;
		//r.x1=r.y1=r.z1=0;
		r/=det();
		return r;
	}
	
	TMat TMat::operator*(const TMat& r)const{
		TMat ret;
		ret.xx= xx*r.xx + xy*r.yx + xz*r.zx;
		ret.xy= xx*r.xy + xy*r.yy + xz*r.zy;
		ret.xz= xx*r.xz + xy*r.yz + xz*r.zz;
		ret.x1= xx*r.x1 + xy*r.y1 + xz*r.z1 + x1*1;
		
		ret.yx= yx*r.xx + yy*r.yx + yz*r.zx;
		ret.yy= yx*r.xy + yy*r.yy + yz*r.zy;
		ret.yz= yx*r.xz + yy*r.yz + yz*r.zz;
		ret.y1= yx*r.x1 + yy*r.y1 + yz*r.z1 + y1*1;
		
		ret.zx= zx*r.xx + zy*r.yx + zz*r.zx;
		ret.zy= zx*r.xy + zy*r.yy + zz*r.zy;
		ret.zz= zx*r.xz + zy*r.yz + zz*r.zz;
		ret.z1= zx*r.x1 + zy*r.y1 + zz*r.z1 + z1*1;
		return ret;
	}
	
	TMat& TMat::operator/=(num k){
		this->xx/=k;this->xy/=k;this->xz/=k;this->x1/=k;
		this->yx/=k;this->yy/=k;this->yz/=k;this->y1/=k;
		this->zx/=k;this->zy/=k;this->zz/=k;this->z1/=k;
		return *this;
	}
	
	void TMat::translation(num tx,num ty,num tz){
		TMat::identity();   x1=tx; y1=ty; z1=tz;
	}
	void TMat::scale(num sx,num sy,num sz){
		TMat::clear();xx=sx; yy=sy; zz=sz;
	}
	void TMat::rotation(int a,num deg){
		num rad=deg*PI/180.0;
		num s(sin(rad)),c(cos(rad));
		TMat::identity();
		if(a==0){    yy=zz=c; yz=-s; zy= s; }
		if(a==1){ xx=   zz=c; zx=-s; xz= s; }
		if(a==2){ xx=yy   =c; xy=-s; yx= s; }
	}
	
	void TMat::appl(const num&x,const num&y,const num&z,
						  num&xn,     num&yn,     num&zn)const{
		xn = xx*x + xy*y + xz*z + x1;
		yn = yx*x + yy*y + yz*z + y1;
		zn = zx*x + zy*y + zz*z + z1;
	}
	
	
/*};*/

std::ostream&operator<<(std::ostream&os,TMat const&m){
	return os<<"["	<<m.xx<<","<<m.xy<<","<<m.xz<<","<<m.x1<<";"
					<<m.yx<<","<<m.yy<<","<<m.yz<<","<<m.y1<<";"
					<<m.zx<<","<<m.zy<<","<<m.zz<<","<<m.z1<<"]";
}
