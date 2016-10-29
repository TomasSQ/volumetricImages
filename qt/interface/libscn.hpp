#pragma once
#ifndef LIBSCN_HPP
#define LIBSCN_HPP

#include <vector>
#include <bitset>
#include <functional>

#define DEBUG_BUILD 1
#ifdef DEBUG_BUILD
#define DEBUG(x)	do{if(1){std::cerr<<(x)<<std::endl;}}while(0)
#define DBG(x)		do{std::cerr<<#x<<":\t["<<(x)<<"]"<<std::endl;}while(0)
#else
#define DBG(x)		;
#endif

/* Non-image related utils */
namespace Util{
	std::string padded_num(int n,int pad);
	void sys(const std::string&cmd);
}

/* Conversion between color spaces and palettes */
namespace ColorSpace{
	typedef int			num;
	typedef std::tuple<num,num,num> t3n;
	typedef double		rat;
	typedef std::tuple<rat,rat,rat> t3r;

	/*         [0,360), [0,1] ,  [0,1]  */
	t3r HSL2RGB(double H,double S,double L);

	namespace BM2RW{
		const int MAX=1792;
		extern int T[1800][3];
		extern bool v;
		void init();
		t3n GS2RGB(double gs);
	}
}

/* Class to store Voxel values */
class Voxel{public:
	std::vector<std::vector<std::bitset<8> > >value;
	auto num_channels()const;
	auto num_bytes()const;
	unsigned long long to_ullong(int ic=0)const;
	unsigned long to_ulong(int ic=0)const;
	bool operator==(const Voxel& rhs)const;
	bool operator!=(const Voxel& rhs)const;
	bool operator <(const Voxel& rhs)const;
	bool operator >(const Voxel& rhs)const;
	bool operator<=(const Voxel& rhs)const;
	bool operator>=(const Voxel& rhs)const;

	void set(const std::vector<std::vector<std::bitset<8> > >&new_value);
};

/* Class that stores image metadata */
class MetaData{public:
	float d[3];//std::vector<float>d;
	std::string unid;
	int b;
	std::string mgc;
	int mv;
};

/* Namespace for handling Voxel value conversion */
namespace VoxelMap{
	using t3i=std::tuple<int,int,int>;
	using t3d=std::tuple<double,double,double>;
	int transform(const Voxel&v);
	Voxel transform(int i);
	Voxel transform(const t3i&rgb);
	Voxel transform(const t3d&rgb);
}

/* Class that represents 3D or 2D images */
class Image3D{
	using t3i=std::tuple<int,int,int>;
	public:
	std::vector<std::vector<std::vector<Voxel> > >data;
	int n[3];int n3;
	MetaData md;

	int get_n(int d)const;

	Voxel& ref(int x,int y,int z);
	Voxel& ref(const t3i&t);
	const Voxel& cref(int x,int y,int z)const;
	const Voxel& cref(const t3i&t)const;

	Voxel& operator[](const t3i&t);
	Voxel& operator[](std::size_t idx);
	const Voxel& operator[](const t3i&t)const;
	const Voxel& operator[](std::size_t idx)const;

	Voxel get(int x,int y,int z)const;
	Voxel get(const t3i&t)const;
	void set(int x,int y,int z,const Voxel&v);
	void set(const t3i&t,const Voxel&v);

	t3i pos(std::size_t idx)const;
	int pos(int p0,int p1,int p2)const;
	int pos(const t3i& t)const;

	t3i pos(int a0,int p0,int a1,int p1,int a2,int p2)const;

	bool bounded_n(int x,int y,int z)const;
	bool bounded_0(int x,int y,int z)const;
	bool in_range(int x,int y,int z)const;

	/* Extract orthogonal plane determined by {a_,p_.aX,sX,sY} to ret[_][_][0] */
	/*TRF-1*/
	void ort_plane(Image3D&ret,int a_=0,int p_=-1,int aX=0,int sX=1,int sY=1)const;

	void map(std::function<void(Voxel&)>f);
	template<class Compare>
	Voxel top(Compare comp)const;

	/*TRF-2*/
	void linear_transform(int I1,int I2,int k1,int k2);

	/*TRF-3*/
	void colorize(const Image3D&label);
	void apply_pallete();
    void apply_pallete_over_HSL2RGB_space();

	/* ret must already be the same size of *this */
	void erode_by_1(Image3D&ret)const;
	void erode_by_2N(Image3D&ret,int n)const;

	/* mask must be the same size of *this */
	void subtract(const Image3D&mask);

	void mirror(Image3D&ret,int a=0)const;

};
/* Functions for Images */
namespace ImageFunc{
	void combine(Image3D&ret,const Image3D&I1,const Image3D&I2);
}
/* Handles IO for image files */
namespace ImgFormat{
	namespace PPM{
		enum class Color{cR,cG,cB};
		void read(Image3D&img,const std::string& path);
		void write(/*const*/ Image3D&img,const std::string& path);
	}
	namespace SCN{
		void read(Image3D&img,const std::string& path);
		void write(/*const*/ Image3D&img,const std::string& path);
	}
	namespace detect{
		void read(Image3D&img,const std::string& path);
		void write(/*const*/ Image3D&img,const std::string& path);
	}
	namespace TMP{
		void read(const Image3D&img,const std::string& path);
		void write(const Image3D&img,const std::string& path);
	}
}
/* Conversion using imagemagick */
namespace ImageMagick{
	/* convert "path" to "path.format" */
	void convert(const std::string& path,const std::string& format="png");
	/* animate files in "in_dir" to "out_path.gif" */
	void animate(const std::string& in_dir,const std::string& out_path);
}
/* Quick demo procedures */
namespace ImgMacro{
	extern std::string pfx;
	void init();
	void ort_animate(const Image3D&img,const std::string& out_path,int a_=0,int aX=0,int sX=1,int sY=1);
}



#endif /* LIBSCN_HPP */


