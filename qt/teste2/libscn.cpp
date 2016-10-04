#ifndef _LIBSCN
#define _LIBSCN

#pragma once
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <sstream>

#include <iomanip>

#include <vector>
#include <tuple>

#include <bitset>

#include "prettyprint.hpp"

#define DEBUG_BUILD 1
#ifdef DEBUG_BUILD
#define DEBUG(x)	do{if(1){std::cerr<<(x)<<std::endl;}}while(0)
#define DBG(x)		do{std::cerr<<#x<<":\t["<<(x)<<"]"<<std::endl;}while(0)
#else
#define DBG(x)		;
#endif

/* Non-image related utils */
namespace Util{
	std::string padded_num(int n,int pad){
		std::ostringstream ss;
		ss<<std::setw(pad)<<std::setfill('0')<<n;
		std::string ret=ss.str();
		if(ret.length()>pad){ret.erase(0,ret.length()-pad);}
		return ret;
	}
	void sys(const std::string&cmd){
		if(std::system(cmd.c_str()))
			std::cerr<<"Error: "<<cmd<<std::endl;
	}
}

/* Conversion between color spaces and palletes */
namespace ColorSpace{
	typedef int			num;
	typedef std::tuple<num,num,num> t3n;
	typedef double		rat;
	typedef std::tuple<rat,rat,rat> t3r;

	/*         [0,360), [0,1] ,  [0,1]  */
	t3r HSL2RGB(double H,double S,double L){
		/*__ R-y-G-c-B-m-R */
		/*Hl 0-1-2-3-4-5-6 */
		/* X 0-1-0-1-0-1-0 */
		/* X  / \ / \ / \  */
		double C = ( 1.0f - fabs(2.0f*L - 1.0f) ) * (S) ;
		double Hl = H/(60.0f) ;

		double X = C * (1.0f - fabs( fmod(Hl,2) - 1.0f ) ) ;

		t3r r(0,0,0);
		     if(Hl < 0.0f){}
		else if(Hl < 1.0f){r=t3r{C,X,0};}
		else if(Hl < 2.0f){r=t3r{X,C,0};}
		else if(Hl < 3.0f){r=t3r{0,C,X};}
		else if(Hl < 4.0f){r=t3r{0,X,C};}
		else if(Hl < 5.0f){r=t3r{X,0,C};}
		else if(Hl <= 6.0f){r=t3r{C,0,X};}

		double m = (L) - ((C)/2.0f);

		double R = (std::get<0>(r)+m);
		double G = (std::get<1>(r)+m);
		double B = (std::get<2>(r)+m);

		return t3r{R,G,B};
	}

	namespace WTP{
		//int [((1)<<(1*8))*(8-1)+3][3];
		const int MAX=1792;
		int T[1800][3];bool v(0);
		void init(){if(v)return;
			int t=0,i=0;int r=0,g=0,b=0;
			// change to do-while
			for(i=0;i<256;++i,++r,    ++b){T[t][0]=r;T[t][1]=g;T[t][2]=b;++t;}--r;    --b;
			for(i=0;i<256;++i,--r        ){T[t][0]=r;T[t][1]=g;T[t][2]=b;++t;}++r;
			for(i=0;i<256;++i,    ++g    ){T[t][0]=r;T[t][1]=g;T[t][2]=b;++t;}    --g;
			for(i=0;i<256;++i,        --b){T[t][0]=r;T[t][1]=g;T[t][2]=b;++t;}        ++b;
			for(i=0;i<256;++i,++r        ){T[t][0]=r;T[t][1]=g;T[t][2]=b;++t;}--r;
			for(i=0;i<256;++i,    --g    ){T[t][0]=r;T[t][1]=g;T[t][2]=b;++t;}    ++g;
			for(i=0;i<256;++i,    ++g,++b){T[t][0]=r;T[t][1]=g;T[t][2]=b;++t;}    --g;--b;
			T[t][0]=255;T[t][1]=255;T[t][2]=255;
			v=true;
		}
		/*        [0.0 ,1.0]*/
		t3n GS2RGB(double gs){
			if(!v){init();}
			if(gs>1)gs=1;
			int t=gs*(MAX-1);
			return t3n{T[t][0],T[t][1],T[t][2]};
		}
	}


}

/* Class to store Voxel values */
class Voxel{public:
	std::vector<std::vector<std::bitset<8> > >value;
	auto num_channels()const{return value.size();}
	auto num_bytes()   const{return value.empty()?0:value[0].size();}
	auto to_ullong(int ic=0)const{unsigned long long ret(0);int ib(0);
		for(auto const &b:value[ic])ret+=(b.to_ullong()<<((ib++)*8));
		return ret;
	}
	auto to_ulong(int ic=0)const{unsigned long ret(0);int ib(0);
		for(auto const &b:value[ic])ret+=(b.to_ulong()<<((ib++)*8));
        return ret;
	}
	bool operator==(const Voxel& rhs)const{return value==rhs.value;}
	bool operator!=(const Voxel& rhs)const{return !(value==rhs.value);}

	bool operator <(const Voxel& rhs)const{return to_ulong() <rhs.to_ulong();}
	bool operator >(const Voxel& rhs)const{return to_ulong() >rhs.to_ulong();}
	bool operator<=(const Voxel& rhs)const{return to_ulong()<=rhs.to_ulong();}
	bool operator>=(const Voxel& rhs)const{return to_ulong()>=rhs.to_ulong();}

	void set(const std::vector<std::vector<std::bitset<8> > >&new_value){value=new_value;}
};
std::ostream&operator<<(std::ostream&os,Voxel const&v){return os<<v.value;}

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
	int transform(const Voxel&v){
		/* average all channels */
		int ret=0;
		for(int ic=0;ic<v.num_channels();++ic)
			ret+=v.to_ulong(ic);
		ret/=v.num_channels();
		return ret;
	}
	Voxel transform(int i){
		Voxel ret;
		ret.value.resize(1);//only 1 channel
		do{
			ret.value[0].push_back(std::bitset<8>(i));
			i>>=8;
		}while(i>0);
		return ret;
	}

	Voxel transform(const t3i&rgb){
		Voxel ret;
		ret.value.resize(3);
		ret.value[0].push_back(std::bitset<8>(std::get<0>(rgb)));
		ret.value[1].push_back(std::bitset<8>(std::get<1>(rgb)));
		ret.value[2].push_back(std::bitset<8>(std::get<2>(rgb)));
		return ret;
	}
	Voxel transform(const t3d&rgb){
		int r = 255*(std::get<0>(rgb));
		int g = 255*(std::get<1>(rgb));
		int b = 255*(std::get<2>(rgb));
		return transform(t3i{r,g,b});
	}
}

/* Class that represents 3D or 2D images */
class Image3D{
	using t3i=std::tuple<int,int,int>;
	public:
	std::vector<std::vector<std::vector<Voxel> > >data;
	int n[3];int n3;
	MetaData md;

	int get_n(int d)const{return n[d];}

	Voxel& ref(int x,int y,int z){return data[x][y][z];}
	Voxel& ref(const t3i&t){return ref(std::get<0>(t),std::get<1>(t),std::get<2>(t));}

	const Voxel& cref(int x,int y,int z)const{return data[x][y][z];}
	const Voxel& cref(const t3i&t)const{return cref(std::get<0>(t),std::get<1>(t),std::get<2>(t));}

	Voxel& operator[](const t3i&t){return ref(t);}
	Voxel& operator[](std::size_t idx){return ref(pos(idx));}

	const Voxel& operator[](const t3i&t)const{return cref(t);}
	const Voxel& operator[](std::size_t idx)const{return cref(pos(idx));}

	Voxel get(int x,int y,int z)const{return data[x][y][z];}
	Voxel get(const t3i&t)const{return  get(std::get<0>(t),std::get<1>(t),std::get<2>(t));}

	void set(int x,int y,int z,const Voxel&v){data[x][y][z]=v;}
	void set(const t3i&t,const Voxel&v){set(std::get<0>(t),std::get<1>(t),std::get<2>(t),v);}

	t3i pos(std::size_t idx)const{
		int x,y,z;
		z = idx/(n[0]*n[1]);
		idx -= (z*n[0]*n[1]);
		y = idx / n[0];
		x = idx % n[0];
		return t3i(x,y,z);
	}
	int pos(int p0,int p1,int p2)const{return (p2*n[0]*n[1])+(p1*n[0])+p0;}
	int pos(const t3i& t)const{return pos(std::get<0>(t),std::get<1>(t),std::get<2>(t));}

	t3i pos(int a0,int p0,int a1,int p1,int a2,int p2)const{
		t3i ret;
		std::get<0>(ret) = a0!=0?a1!=0?p2:p1:p0;
		std::get<1>(ret) = a0!=1?a1!=1?p2:p1:p0;
		std::get<2>(ret) = a0!=2?a1!=2?p2:p1:p0;
		return ret;
	}

	/* Extract orthogonal plane determined by {a_,p_.aX,sX,sY} to ret[_][_][0] */
	/*TRF-1*/
	void ort_plane(Image3D&ret,int a_=0,int p_=-1,int aX=0,int sX=1,int sY=1)const{
		/* Parameter remapping */
		int axis	= a_;			// [0,1,2]	->	[0,1,2]
		int plane	= p_<0? n[a_]/2 : p_;
		int fstAxis	= (a_+1+aX)%3;	// [0,1]	->	[0,1,2]
		int sigFst	= sX*2-1;		// [0,1]	->	[-1,+1]
		int sigSnd	= sY*2-1;		// [0,1]	->	[-1,+1]
		/* Finding cut's axes */
		int cut_axis[2];
		cut_axis[0] = fstAxis;
		cut_axis[1] = 3-(axis+fstAxis);
		/* Finding iterating values */
		int x0,xd,xn;
		     if(sigFst== +1){x0=0;					xd=+1;	xn=n[cut_axis[0]];}
		else if(sigFst== -1){x0=n[cut_axis[0]]-1;	xd=-1;	xn=-1;}
		else{std::cerr<<"Error: invalid parameters."<<std::endl;return;}

		int y0,yd,yn;
		     if(sigSnd== +1){y0=0;					yd=+1;	yn=n[cut_axis[1]];}
		else if(sigSnd== -1){y0=n[cut_axis[1]]-1;	yd=-1;	yn=-1;}
		else{std::cerr<<"Error: invalid parameters."<<std::endl;return;}

		/* Copying data */
		ret.n[0] = n[cut_axis[0]];
		ret.n[1] = n[cut_axis[1]];
		ret.n[2] = 1;
		ret.n3=ret.n[0]*ret.n[1]*ret.n[2];
		ret.md.mv = 0;
		/* Copying cut */
		ret.data      .resize(ret.n[0]);for(int xi=x0,x=0;x<ret.n[0];xi+=xd,++x){
		ret.data[x]   .resize(ret.n[1]);for(int yi=y0,y=0;y<ret.n[1];yi+=yd,++y){
		ret.data[x][y].resize(ret.n[2]);                                        {
					ret.set(x,y,0,get(pos(axis,plane,cut_axis[0],xi,cut_axis[1],yi)));
				}
			}
		}
		return;
	}

	void map(std::function<void(Voxel&)>f){
		for(int iv=0;iv<n3;++iv)f(ref(pos(iv)));
	}

	template<class Compare>
	Voxel top(Compare comp)const{
		const Voxel*ret=&cref(pos(0));
		//for(int iv=0;iv<n3;++iv){if(comp(*ret,(cref(pos(iv)))))ret=&cref(pos(iv));}
		/*
		for(int ix=0;ix<n[0];++ix)for(int iy=0;iy<n[1];++iy)for(int iz=0;iz<n[2];++iz){
			if(comp(*ret,(cref(ix,iy,iz))))ret=&cref(ix,iy,iz);
		}//*/
		//*
		for(auto const&Vx:data)
		for(auto const&Vxy:Vx)
		for(auto const&Vxyz:Vxy)
			if(comp(*ret,Vxyz))ret=&Vxyz;
		//*/
		return *ret;
	}

	/*TRF-2*/
	void linear_transform(int I1,int I2,int k1,int k2){
		int k;int I;
		int kd = (k2-k1);
		int Id = (I2-I1);if(Id==0)Id=1;
		for(int iv=0;iv<n3;++iv){
			I=VoxelMap::transform(get(pos(iv)));
			     if(I< I1) k = k1;
			else if(I>=I2) k = k2;
			else           k = k1 + ((I-I1)*kd)/Id;
			set(pos(iv),VoxelMap::transform(k));
		}
	}

	/*TRF-3*/
	void colorize(const Image3D&label){
		Voxel l_max = label.top(std::less<Voxel>());
		Voxel v_min = top(std::greater<Voxel>());
		Voxel v_max = top(std::less<Voxel>());
		int l_n =	l_max.to_ulong();
		int x_min = v_min.to_ulong();
		int x_max = v_max.to_ulong();

		double x_d = (x_max-x_min);
		double y_d = (1.0-0.0);

		bool color_0=false;
		if(color_0){l_n++;}
		double H_lut[l_n+1];
		for(int i=0;i<=l_n;++i)
			H_lut[i]=(((double)i)*360.0)/((double)l_n);

		for(int iv=0;iv<n3;++iv){
			double x_i = ref(pos(iv)).to_ulong();
			double y_i = (((x_i-x_min)*(y_d))/x_d)+0.0;

			int l_i = label[iv].to_ulong();

			double S =  color_0 || l_i!=0 ? 1.0 : 0.0;
			double L = y_i;
			double H = H_lut[l_i];

			set(pos(iv),VoxelMap::transform(ColorSpace::HSL2RGB(H,S,L)));
		}
	}

	void apply_pallete(){
		int x_min = top(std::greater<Voxel>()).to_ulong();
		int x_max = top(std::less   <Voxel>()).to_ulong();
		double x_d = (x_max-x_min);
		double y_d = (1.0-0.0);
		for(int iv=0;iv<n3;++iv){
			double x_i = ref(pos(iv)).to_ulong();
			double y_i = (((x_i-x_min)*(y_d))/x_d)+0.0;
			//double H = y_i*300.0;double S = 1.0;double L = 0.5;
			//set(pos(iv),VoxelMap::transform(ColorSpace::HSL2RGB(H,S,L)));
			set(pos(iv),VoxelMap::transform(ColorSpace::WTP::GS2RGB(y_i)));
		}
	}

};

/* Handles IO for image files */
namespace ImgFormat{

	namespace PPM{
		enum class Color{cR,cG,cB};
		void read(Image3D&img,const std::string& path){
			std::ifstream ifs;ifs.open(path.c_str(),std::ifstream::in);
			/* read magic string */
			ifs>>img.md.mgc;
			/* read nx,ny */
			ifs>>img.n[1]>>img.n[0];img.n[2]=1;
			img.n3=img.n[0]*img.n[1]*img.n[2];
			/* read max value */
			ifs>>img.md.mv;
			ifs.get();
			/* read data */
            img.data	                 .resize(img.n[0]);
            for(int ix=0;ix<img.n[0];++ix){
                img.data[ix].resize(img.n[1]);
                for(int iy=0;iy<img.n[1];++iy){
                    img.data[ix][iy].resize(img.n[2]);{
                        img.data[ix][iy][0].value.resize(3);
                        for(int ic=0;ic<3;++ic){
                            img.data[ix][iy][0].value[ic].resize(1);
                            {
                                img.data[ix][iy][0].value[ic][0]=ifs.get();
                            }
                        }
                    }
                }
			}
			ifs.close();
		}
		void write(/*const*/ Image3D&img,const std::string& path){
			std::ofstream ofs;ofs.open(path.c_str(),std::ofstream::out);
			/* write magic string */
			if(img.md.mgc.empty())img.md.mgc="P6";
			ofs<<img.md.mgc<<"\n";
			/* write nx,ny */
			ofs<<img.n[1]<<" "<<img.n[0]<<"\n";
			/* write max value */
			if(img.md.mv==0)img.md.mv=255;
			ofs<<img.md.mv<<"\n";
			/* write data */
			for(int ix=0;ix<img.n[0];++ix){
			for(int iy=0;iy<img.n[1];++iy){
			    int iz=0;                 {
					int nc=img.data[ix][iy][iz].value.size();
					for(int ic=0;ic<3;++ic){
						ofs<<((char)(img.data[ix][iy][iz].value[ic%nc][0].to_ulong()));
						//ofs<<((char)(img.get(ix,iy,0).value[ic%nc][0].to_ulong()));
					}

			}}}
			ofs.close();
		}
	}

	namespace SCN{
		void read(Image3D&img,const std::string& path){
			std::ifstream ifs;ifs.open(path.c_str(),std::ifstream::in);
			/* read magic string */
			ifs>>img.md.mgc;
			/* read nx,ny,nz */
			ifs>>img.n[2]>>img.n[1]>>img.n[0];
			img.n3=img.n[0]*img.n[1]*img.n[2];
			/* read dx,dy,dz */
			ifs>>img.md.d[2]>>img.md.d[1]>>img.md.d[0];
			/* read b */
			ifs>>img.md.b;
			int nb=(img.md.b/8);
			ifs.get();
			/* read data */
			img.data                     .resize(img.n[0]);for(int ix=0;ix<img.n[0];++ix){
			img.data[ix]                 .resize(img.n[1]);for(int iy=0;iy<img.n[1];++iy){
			img.data[ix][iy]             .resize(img.n[2]);for(int iz=0;iz<img.n[2];++iz){
			img.data[ix][iy][iz].value   .resize(1);                                     {
			img.data[ix][iy][iz].value[0].resize(nb);      for(int ib=0;ib<nb      ;++ib){
								img.data[ix][iy][iz].value[0][ib]=ifs.get();
							}
						}
					}
				}
			}
			ifs.close();
		}
		void write(/*const*/ Image3D&img,const std::string& path){
			std::ofstream ofs;ofs.open(path.c_str(),std::ofstream::out);
			/* write magic string */
			if(img.md.mgc.empty())img.md.mgc="SCN";
			ofs<<img.md.mgc<<"\n";
			/* write nx,ny,nz */
			ofs<<img.n[2]<<" "<<img.n[1]<<" "<<img.n[0]<<"\n";
			/* write dx,dy,dz */
			ofs<<std::fixed<<std::setprecision(6);
			ofs<<img.md.d[2]<<" "<<img.md.d[1]<<" "<<img.md.d[0]<<"\n";
			/* write b */
			ofs<<img.md.b<<std::endl;
			//int nb=(img.md.b/8);
			/* write data */
			for(int ix=0;ix<img.n[0];++ix){
			for(int iy=0;iy<img.n[1];++iy){
			for(int iz=0;iz<img.n[2];++iz){
			    int ic=0;                 {
			for(int ib=0;ib<img.data[ix][iy][iz].value[ic].size();++ib){
					ofs<<(char)img.data[ix][iy][iz].value[ic][ib].to_ulong();
			}}}}}
			ofs.close();
		}
	}

	namespace detect{
		void read(Image3D&img,const std::string& path){
			std::ifstream ifs;ifs.open(path.c_str(),std::ifstream::in);
			/* read magic string */
			std::string mgc;ifs>>mgc;ifs.close();
			     if(mgc.front()=='P'){PPM::read(img,path);}
			else if(mgc      =="SCN"){SCN::read(img,path);}
			else{std::cerr<<"Error: not able to detect file."<<std::endl;return;}

		}
		void write(/*const*/ Image3D&img,const std::string& path){
			std::string ext(path.substr(path.size()-4));for(auto &e:ext){e=toupper(e);}
			     if(ext==".PPM"){PPM::write(img,path);}
			else if(ext==".SCN"){SCN::write(img,path);}
			else{std::cerr<<"Error: not able to detect format."<<std::endl;return;}
		}
	}

	namespace TMP{
		void read(const Image3D&img,const std::string& path){}
		void write(const Image3D&img,const std::string& path){}
	}

}

/* Conversion using imagemagick */
namespace ImageMagick{
	/* convert "path" to "path.format" */
	void convert(const std::string& path,const std::string& format="png"){
		std::string cmd;
		cmd="convert "+path+" "+path+"."+format+" ;";
		Util::sys(cmd);
		cmd="rm "+path+" ;";
		Util::sys(cmd);
	}
	/* animate files in "in_dir" to "out_path.gif" */
	void animate(const std::string& in_dir,const std::string& out_path){
		std::string cmd;
		int delay=4;/*s/100*/
		cmd="convert -delay "+std::to_string(delay)+" -loop 0 "+in_dir+"/* "+out_path+".gif ;";
		Util::sys(cmd);
		cmd="rm -rf "+in_dir+" ;";
		Util::sys(cmd);
	}
}

/* Quick demo procedures */
namespace ImgMacro{
	std::string pfx;

	void init(){
		pfx="/tmp/mob/";
		std::string cmd;
		cmd="mkdir -p "+pfx+"/png/ ;";Util::sys(cmd);
		cmd="mkdir -p "+pfx+"/ppm/ ;";Util::sys(cmd);
		cmd="mkdir -p "+pfx+"/gif/ ;";Util::sys(cmd);
	}

	void ort_animate(const Image3D&img,const std::string& out_path,int a_=0,int aX=0,int sX=1,int sY=1){
		std::string cmd;
		cmd="mkdir -p "+pfx+"/tmp/ ;";
		Util::sys(cmd);
		Image3D cut;
		for(int fi=0;fi<img.n[a_];++fi){
			img.ort_plane(cut,a_,fi,aX,sX,sY);
			std::string fnm=pfx+"tmp/frm"+Util::padded_num(fi,3)+".ppm";
			ImgFormat::detect::write(cut,fnm);
		}
		ImageMagick::animate(pfx+"tmp/",out_path);
	}

}

#endif
