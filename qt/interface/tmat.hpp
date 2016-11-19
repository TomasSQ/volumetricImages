#pragma once
#ifndef TMAT_HPP
#define TMAT_HPP

#include <ostream>

class TMat{
	public:
	using num=float;
	
	num xx,xy,xz,x1;
	num yx,yy,yz,y1;
	num zx,zy,zz,z1;
	
	void clear();
	void identity();
	
	num  det()const;
	TMat inv()const;
	
	TMat  operator* (const TMat& r)const;
	TMat& operator/=(num k);
	
	void translation(num tx=0,num ty=0,num tz=0);
	void scale(num sx=1,num sy=1,num sz=1);
	void rotation(int a,num deg);
	
	void appl(const num&x,const num&y,const num&z,
					num&xn,     num&yn,     num&zn)const;
};

std::ostream&operator<<(std::ostream&os,TMat const&m);

#endif /* TMAT_HPP_HPP */
