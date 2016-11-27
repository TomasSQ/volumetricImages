#include "libscn.hpp"

#include "prettyprint.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

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

/* Conversion between color spaces and palettes */
namespace ColorSpace{
	typedef int			num;
	typedef std::tuple<num,num,num> t3n;
	typedef double		rat;
	typedef std::tuple<rat,rat,rat> t3r;

	/*         [0,360), [0,1] ,  [0,1]  */
	t3r HSL2RGB(double H,double S,double L){
		/* Visualization: */
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

	namespace BM2RW{
		int T[1800][3];
		bool v(0);
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
/*class Voxel {*/
	auto Voxel::num_channels()const{return value.size();}
	auto Voxel::num_bytes()   const{return value.empty()?0:value[0].size();}
	unsigned long long Voxel::to_ullong(int ic)const{unsigned long long ret(0);int ib(0);
		for(auto const &b:value[ic])ret+=(b.to_ullong()<<((ib++)*8));
		return ret;
	}
	unsigned long Voxel::to_ulong(int ic)const{unsigned long ret(0);int ib(0);
		for(auto const &b:value[ic])ret+=(b.to_ulong()<<((ib++)*8));
		return ret;
	}
	bool Voxel::operator==(const Voxel& rhs)const{return value==rhs.value;}
	bool Voxel::operator!=(const Voxel& rhs)const{return !(value==rhs.value);}

	bool Voxel::operator <(const Voxel& rhs)const{return to_ulong() <rhs.to_ulong();}
	bool Voxel::operator >(const Voxel& rhs)const{return to_ulong() >rhs.to_ulong();}
	bool Voxel::operator<=(const Voxel& rhs)const{return to_ulong()<=rhs.to_ulong();}
	bool Voxel::operator>=(const Voxel& rhs)const{return to_ulong()>=rhs.to_ulong();}

	void Voxel::set(const std::vector<std::vector<std::bitset<8> > >&new_value){value=new_value;}
	template<class T>
	void Voxel::set(T new_value){value=std::vector<std::vector<std::bitset<8> > >(1,std::vector<std::bitset<8> >(1,new_value));}
/*};*/
std::ostream&operator<<(std::ostream&os,Voxel const&v){return os<<v.value;}

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
/*class Image3D{*/
	using t3i=std::tuple<int,int,int>;

	int Image3D::get_n(int d)const{return n[d];}

	Voxel& Image3D::ref(int x,int y,int z){return data[x][y][z];}
	Voxel& Image3D::ref(const t3i&t){return ref(std::get<0>(t),std::get<1>(t),std::get<2>(t));}

	const Voxel& Image3D::cref(int x,int y,int z)const{return data[x][y][z];}
	const Voxel& Image3D::cref(const t3i&t)const{return cref(std::get<0>(t),std::get<1>(t),std::get<2>(t));}
	
	Voxel& Image3D::operator[](const t3i&t){return ref(t);}
	Voxel& Image3D::operator[](std::size_t idx){return ref(pos(idx));}

	const Voxel& Image3D::operator[](const t3i&t)const{return cref(t);}
	const Voxel& Image3D::operator[](std::size_t idx)const{return cref(pos(idx));}

	Voxel Image3D::get(int x,int y,int z)const{return data[x][y][z];}
	Voxel Image3D::get(const t3i&t)const{return get(std::get<0>(t),std::get<1>(t),std::get<2>(t));}

	void Image3D::set(int x,int y,int z,const Voxel&v){data[x][y][z]=v;}
	void Image3D::set(const t3i&t,const Voxel&v){set(std::get<0>(t),std::get<1>(t),std::get<2>(t),v);}

	t3i Image3D::pos(std::size_t idx)const{
		int x,y,z;
		z = idx/(n[0]*n[1]);
		idx -= (z*n[0]*n[1]);
		y = idx / n[0];
		x = idx % n[0];
		return t3i(x,y,z);
	}
	int Image3D::pos(int p0,int p1,int p2)const{return (p2*n[0]*n[1])+(p1*n[0])+p0;}
	int Image3D::pos(const t3i& t)const{return pos(std::get<0>(t),std::get<1>(t),std::get<2>(t));}

	t3i Image3D::pos(int a0,int p0,int a1,int p1,int a2,int p2)const{
		t3i ret;
		std::get<0>(ret) = a0!=0?a1!=0?p2:p1:p0;
		std::get<1>(ret) = a0!=1?a1!=1?p2:p1:p0;
		std::get<2>(ret) = a0!=2?a1!=2?p2:p1:p0;
		return ret;
	}

	bool Image3D::bounded_n(int x,int y,int z)const{return x<n[0]&&y<n[1]&&z<n[2];}
	bool Image3D::bounded_0(int x,int y,int z)const{return 0<=x && 0<=y && 0<=z  ;}
	bool Image3D::in_range(int x,int y,int z)const{return bounded_n(x,y,z)&&bounded_0(x,y,z);}

	void Image3D::resize(int n0,int n1,int n2){
        if(n[0]>=n0&&n[1]>=n1&&n[2]>=n2)return;
		n[0]=n0;n[1]=n1;n[2]=n2;
								 data.resize(n[0]);
			for(auto &Vx__:data){Vx__.resize(n[1]);
			for(auto &Vxy_:Vx__){Vxy_.resize(n[2]);
			}}
		
	}

	/* Interpolation */
	const Voxel& Image3D::get_bounded(int x,int y,int z)const{
		if(in_range(x,y,z))return cref(x,y,z);
		else return cref(0,0,0);
	}
	
	const Voxel& Image3D::get_NN(float x,float y,float z)const{
		int xi(x),yi(y),zi(z);
		return get_bounded(xi,yi,zi);
	}
	
	Voxel Image3D::get_trili(float x,float y,float z)const{
		Voxel ret;
		int x0(floor(x)),y0(floor(y)),z0(floor(z));
		int x1( ceil(x)),y1( ceil(y)),z1( ceil(z));
		float V000 = static_cast<float>(get_bounded(x0,y0,z0).to_ulong());
		float V001 = static_cast<float>(get_bounded(x0,y0,z1).to_ulong());
		float V010 = static_cast<float>(get_bounded(x0,y1,z0).to_ulong());
		float V011 = static_cast<float>(get_bounded(x0,y1,z1).to_ulong());
		float V100 = static_cast<float>(get_bounded(x1,y0,z0).to_ulong());
		float V101 = static_cast<float>(get_bounded(x1,y0,z1).to_ulong());
		float V110 = static_cast<float>(get_bounded(x1,y1,z0).to_ulong());
		float V111 = static_cast<float>(get_bounded(x1,y1,z1).to_ulong());
		x-=x0;
		y-=y0;
		z-=z0;
		//DBG(x);DBG(y);DBG(z);
		float Vxyz;
		Vxyz = 	V000*(1-x)*(1-y)*(1-z)+
				V100*  x  *(1-y)*(1-z)+
				V010*(1-x)*  y  *(1-z)+
				V001*(1-x)*(1-y)*  z  +
				V101*  x  *(1-y)*  z  +
				V011*(1-x)*  y  *  z  +
				V110*  x  *  y  *(1-z)+
				V111*  x  *  y  *  z  ;
		
		return VoxelMap::transform(static_cast<int>(Vxyz));
	}


    void Image3D::apply_pallete_over_HSL2RGB_space(){
            int x_min = top(std::greater<Voxel>()).to_ulong();
            int x_max = top(std::less   <Voxel>()).to_ulong();
            double x_d = (x_max-x_min);
            for(auto &Vx:data){
            for(auto &Vxy:Vx){
            for(auto &Vxyz:Vxy){
                double x_i = Vxyz.to_ulong();
                double y_i = (((x_i-x_min)/* *(y_d)*/)/x_d)+0.0;
                /* Vanilla rainbow pallete: */
                double H = y_i*300.0;double S = 1.0;double L = 0.5;
                Vxyz=VoxelMap::transform(ColorSpace::HSL2RGB(H,S,L));

            }}}
        }

	/* Extract orthogonal plane determined by {a_,p_.aX,sX,sY} to ret[_][_][0] */
	/*TRF-1*/
	void Image3D::ort_plane(Image3D&ret,int a_,int p_,int aX,int sX,int sY)const{
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

	void Image3D::map(std::function<void(Voxel&)>f){
		//for(int iv=0;iv<n3;++iv)f(ref(pos(iv)));
		for(auto &Vx:data)
		for(auto &Vxy:Vx)
		for(auto &Vxyz:Vxy)
			f(Vxyz);
	}

	template<class Compare>
	Voxel Image3D::top(Compare comp)const{
		const Voxel*ret=&cref(pos(0));
		/* Different ways of iterating through all voxels */
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
	void Image3D::linear_transform(int I1,int I2,int k1,int k2){
		int k;int I;
		int   kd = (k2-k1);
		float Id = (I2-I1);if(Id==0)Id=1;

		/* Used for generic transformations: */
			//float exp=0.5;
			//Id=pow(			Id	,exp);
			//Id=pow(std::log(1+Id)	,exp);
		/* Optmization */
			float mf=(kd)/(Id);
		
		for(auto &Vx:data){
		for(auto &Vxy:Vx){
		for(auto &Vxyz:Vxy){
			I=VoxelMap::transform(Vxyz);
			     if(I< I1) k = k1;
			else if(I>=I2) k = k2;
			else{
				  k = k1 + (				(	I-I1		)	*mf);
				//k = k1 + (pow				(	I-I1	,exp)	*mf);
				//k = k1 + (pow	(std::log	(1+	I-I1)	,exp)	*mf);
			}
			Vxyz=VoxelMap::transform(k);
		}}}
	}

	/*TRF-3*/
	void Image3D::colorize(const Image3D&label){
		Voxel l_max = label.top(std::less<Voxel>());
		Voxel v_min = top(std::greater<Voxel>());
		Voxel v_max = top(std::less<Voxel>());
		int l_n =	l_max.to_ulong();
		int x_min = v_min.to_ulong();
		int x_max = v_max.to_ulong();

		double x_d = (x_max-x_min);
		double y_d = (1.0-0.0);

		/* label 0 will be colored? */
			bool color_0=false;
			if(color_0){l_n++;}
		/* hue phase adjust */
			double H_phase=120.0+180.0;
		/* saturation level */
			double S_max=1.0;
		
		/* LUT: label -> hue */
		double H_lut[l_n+1];
		for(int i=0;i<=l_n;++i)
			H_lut[i]=fmod(((((double)i)*360.0)/((double)l_n))+H_phase,360.0);

		for(int ix=1;ix<n[0]-1;++ix){
		for(int iy=1;iy<n[1]-1;++iy){
		for(int iz=1;iz<n[2]-1;++iz){
			double x_i = cref(ix,iy,iz).to_ulong();
			double y_i = (((x_i-x_min)/* *(y_d)*/)/x_d)+0.0;

			int l_i = label.cref(ix,iy,iz).to_ulong();

			double S =  color_0 || l_i!=0 ? S_max : 0.0;
			double L = y_i;
			double H = H_lut[l_i];

			set(ix,iy,iz,VoxelMap::transform(ColorSpace::HSL2RGB(H,S,L)));
		}}}
	}

	void Image3D::apply_pallete(){
		int x_min = top(std::greater<Voxel>()).to_ulong();
		int x_max = top(std::less   <Voxel>()).to_ulong();
		double x_d = (x_max-x_min);
		double y_d = (1.0-0.0);
		for(auto &Vx:data){
		for(auto &Vxy:Vx){
		for(auto &Vxyz:Vxy){
			double x_i = Vxyz.to_ulong();
			double y_i = (((x_i-x_min)/* *(y_d)*/)/x_d)+0.0;
			
			/* Vanilla rainbow pallete: */
				//double H = y_i*300.0;double S = 1.0;double L = 0.5;
				//Vxyz=VoxelMap::transform(ColorSpace::HSL2RGB(H,S,L));
			
			Vxyz=VoxelMap::transform(ColorSpace::BM2RW::GS2RGB(y_i));
		}}}
	}

	/* ret must already be the same size of *this */
	void Image3D::erode_by_1(Image3D&ret)const{
		/* ret ~is already~ doesnt have to be a copy of *this */
			//ret=(*this);
		Voxel k0=VoxelMap::transform(0);
		/*for(int i=0;i<n[0];++i){
			ret.set(i,0   ,0   ,k0);
			ret.set(i,n[1],0   ,k0);
			ret.set(i,0   ,n[2],k0);
			ret.set(i,n[1],n[2],k0);
		}*/
		for(int ix=1;ix<n[0]-1;++ix){
		for(int iy=1;iy<n[1]-1;++iy){
		for(int iz=1;iz<n[2]-1;++iz){
			if(
				(cref(ix,iy,iz)!=cref(ix+1,iy,iz))||
				(cref(ix,iy,iz)!=cref(ix-1,iy,iz))||
				(cref(ix,iy,iz)!=cref(ix,iy+1,iz))||
				(cref(ix,iy,iz)!=cref(ix,iy-1,iz))||
				(cref(ix,iy,iz)!=cref(ix,iy,iz+1))||
				(cref(ix,iy,iz)!=cref(ix,iy,iz-1))||
				/*
				(cref(ix,iy,iz)!=cref(ix  ,iy-1,iz-1))||
				(cref(ix,iy,iz)!=cref(ix  ,iy+1,iz-1))||
				(cref(ix,iy,iz)!=cref(ix-1,iy  ,iz-1))||
				(cref(ix,iy,iz)!=cref(ix+1,iy  ,iz-1))||
				(cref(ix,iy,iz)!=cref(ix  ,iy-1,iz+1))||
				(cref(ix,iy,iz)!=cref(ix  ,iy+1,iz+1))||
				(cref(ix,iy,iz)!=cref(ix-1,iy  ,iz+1))||
				(cref(ix,iy,iz)!=cref(ix+1,iy  ,iz+1))||
				(cref(ix,iy,iz)!=cref(ix-1,iy-1,iz  ))||
				(cref(ix,iy,iz)!=cref(ix+1,iy-1,iz  ))||
				(cref(ix,iy,iz)!=cref(ix-1,iy+1,iz  ))||
				(cref(ix,iy,iz)!=cref(ix+1,iy+1,iz  ))||
				//*/
				/*
				(cref(ix,iy,iz)!=cref(ix-1,iy-1,iz-1))||
				(cref(ix,iy,iz)!=cref(ix+1,iy-1,iz-1))||
				(cref(ix,iy,iz)!=cref(ix-1,iy+1,iz-1))||
				(cref(ix,iy,iz)!=cref(ix+1,iy+1,iz-1))||
				(cref(ix,iy,iz)!=cref(ix-1,iy-1,iz+1))||
				(cref(ix,iy,iz)!=cref(ix+1,iy-1,iz+1))||
				(cref(ix,iy,iz)!=cref(ix-1,iy+1,iz+1))||
				(cref(ix,iy,iz)!=cref(ix+1,iy+1,iz+1))||
				//*/
			0){
				ret.set(ix,iy,iz,k0);
			}
			else{ret.set(ix,iy,iz,cref(ix,iy,iz));}
		}}}
		return;
	}

	void Image3D::erode_by_2N(Image3D&ret,int n)const{
		/* Avoid need of scratch img */
		Image3D aux;
		ret=*this;aux=ret;
		for(int i=0;i<n;++i){
			ret.erode_by_1(aux);
			aux.erode_by_1(ret);
		}
	}

	/* mask must be the same size of *this */
	void Image3D::subtract(const Image3D&mask){
		Voxel k0=VoxelMap::transform(0);
		Voxel w0=VoxelMap::transform(255);
		for(int ix=1;ix<n[0]-1;++ix){
		for(int iy=1;iy<n[1]-1;++iy){
		for(int iz=1;iz<n[2]-1;++iz){
			if(cref(ix,iy,iz)==mask.cref(ix,iy,iz)){
				set(ix,iy,iz,k0);
			}//else{set(ix,iy,iz,w0);}
		}}}
		return;
	}

	void Image3D::mirror(Image3D&ret,int a)const{
		/* Setting data */
			ret=(*this);
		int in[3],ia[3];
		in[0]=ret.n[0];
		in[1]=ret.n[1];
		in[2]=ret.n[2];
		in[a]/=2;
		for(int ix=0;ix<in[0];++ix){
		for(int iy=0;iy<in[1];++iy){
		for(int iz=0;iz<in[2];++iz){
			Voxel v=get(ix,iy,iz);
			ia[0]=ix;ia[1]=iy;ia[2]=iz;
			ia[a]=(ret.n[a]-1)-(ia[a]);
			Voxel vi=get(ia[0],ia[1],ia[2]);
			ret.set(ix   ,iy   ,iz  ,vi);
			ret.set(ia[0],ia[1],ia[2],v );
			//ret.set(x,y,0,);
		}}}
		return;
	}

	/* Compute Transformed image boundaries */
	void Image3D::transformation_bounds(const TMat&T,int m[3],int M[3])const{
		int p[3][2];float c[3];
		/* original square cuboid points */
		for(int i=0;i<3;++i){p[i][0]=0;p[i][1]=get_n(i)-1;}
		/* initial value */
		T.appl(p[0][0],p[1][0],p[2][0], c[0],c[1],c[2]);
		for(int i=0;i<3;++i){M[i]=m[i]=c[i];}
		/* min & max */
		for(int ix=0;ix<2;++ix){
		for(int iy=0;iy<2;++iy){
		for(int iz=0;iz<2;++iz){
			T.appl( p[0][ix],p[1][iy],p[2][iz], c[0],c[1],c[2]);
			for(int i=0;i<3;++i){
				if(c[i]<m[i])m[i]=c[i];
				if(c[i]>M[i])M[i]=c[i];
			}
		}}}
		return;
	}

	/* ret already has correct size, dont compute bounds */
	void Image3D::simple_project(Image3D&ret,const TMat&T,int p_)const{
		int iz=p_;
#pragma omp parallel for
		for(int ix=0;ix<ret.get_n(0);++ix){
		for(int iy=0;iy<ret.get_n(1);++iy){
            float x,y,z;
            T.appl(ix,iy,iz,x,y,z);
			ret.set(ix,iy,0, Image3D::get_NN(x,y,z) );
			//ret.set(ix,iy,0, Image3D::get_trili(x,y,z) );
		}}
	}

    /* ret already has correct size, dont compute bounds */
    void Image3D::project_with_maximum(Image3D& ret, const TMat&T,int p_) const {

        int iz=p_;
#pragma omp parallel for
        for(int ix=0;ix<ret.get_n(0);++ix){
        for(int iy=0;iy<ret.get_n(1);++iy){
            float x,y,z;
            T.appl(ix,iy,iz,x,y,z);
            if(ret.cref(ix,iy, 0) < Image3D::get_NN(x,y,z)){
                ret.set(ix, iy, 0, Image3D::get_NN(x,y,z));
            }

        }}
    }


	/* Project image without additional info */
	void Image3D::project(Image3D&ret,const TMat&T,int p_)const{
		/* Get image boundaries */
		TMat Ti(T.inv());
		int m[3],M[3]; float d[3];
		transformation_bounds(Ti,m,M);
		for(int i=0;i<3;++i){d[i]=M[i]-m[i];}
		/* Automagically re-center and avoid cropping */
		Ti.translation(m[0],m[1],m[2]);
		TMat Ta(T*Ti);
		ret.resize(d[0],d[1],1);
		ret.md.mv=0;
		/* Create projection */
		simple_project(ret,Ta,p_);
	}

	/* img must be the same size of *this */
	void Image3D::maximum(const Image3D&img){
		for(int ix=0;ix<n[0];++ix){
		for(int iy=0;iy<n[1];++iy){
		for(int iz=0;iz<n[2];++iz){
			if(cref(ix,iy,iz)<img.cref(ix,iy,iz))
				set(ix,iy,iz,img.cref(ix,iy,iz));
		}}}
	}
	/* img must be the same size of *this */
	void Image3D::disjunct(const Image3D&img){
		for(int ix=0;ix<n[0];++ix){
		for(int iy=0;iy<n[1];++iy){
		for(int iz=0;iz<n[2];++iz){
			if(cref(ix,iy,iz).to_ulong()==0)
				set(ix,iy,iz,img.cref(ix,iy,iz));
		}}}
	}
	

    void Image3D::MIP(Image3D&ret,const TMat&T)const{int a_=2;
        /* Get image boundaries */
        TMat Ti(T.inv());int m[3],M[3];float d[3];
        transformation_bounds(Ti,m,M);
        for(int i=0;i<3;++i){d[i]=M[i]-m[i];}
        /* Automagically re-center and avoid cropping */
        Ti.translation(m[0],m[1],m[2]);TMat Ta(T*Ti);
        d[0]=get_n(0);
        d[1]=get_n(1);
        ret.resize(d[0],d[1],1);
        ret.md.mv=0;
        /* initial value */
        simple_project(ret,Ta,0);
        /* maximum project */
        std::cout << d[a_] << std::endl;
        for(int mi=1;mi<d[a_]/2;++mi){
            project_with_maximum(ret, Ta, mi);
//            ret.maximum(frm);
        }
    }


	/* aggregator with memory */
	void Image3D::operate_memo(const Image3D&img,std::vector<std::vector<unsigned int> >&memo){
        int c1,cB,cD,cT;
		c1=2;cB=32;cD=1,cT=16;//skull
		//c1=2;cB=16;cD=1,cT=32;//brain
#pragma omp parallel for
		for(int ix=0;ix<n[0];++ix){
		for(int iy=0;iy<n[1];++iy){
		for(int iz=0;iz<n[2];++iz){
			auto img_xyz(img.cref(ix,iy,iz).to_ulong());
			if(img_xyz==0){ continue; }
			
			auto ths_xyz(cref(ix,iy,iz).to_ulong());
			//DBG(memo[ix][iy]);
			ths_xyz += (img_xyz*c1 ) / ( (memo[ix][iy]+1)+(c1-1) );
			if(ths_xyz>255){ths_xyz=255;}
			
			if(img_xyz>cT)memo[ix][iy]+=(img_xyz)/cB;
			memo[ix][iy]+=cD;
			
			set(ix,iy,iz,VoxelMap::transform(ths_xyz));
		}}}
	}
	
	/* aggregate all projections on axis a_ */
	void Image3D::aggregate_projections(Image3D&ret,const TMat&T)const{int a_=2;
		/* Get image boundaries */
		TMat Ti(T.inv());int m[3],M[3];float d[3];
		transformation_bounds(Ti,m,M);
		for(int i=0;i<3;++i){d[i]=M[i]-m[i];}
		/* Automagically re-center and avoid cropping */
		Ti.translation(m[0],m[1],m[2]);TMat Ta(T*Ti);
		Image3D frm;frm.resize(d[0],d[1],1);frm.md.mv=0;
		/* initial value */
		simple_project(frm,Ta,0);
		ret=frm;
		/* memory started as projection */
		//Image3D memo(frm);
		std::vector<std::vector<unsigned int> >memo(ret.get_n(0),std::vector<unsigned int>(ret.get_n(1),0));
		/* project and aggregate */
		for(int mi=1;mi<d[a_];++mi){//DBG(mi);
			simple_project(frm,Ta,mi);
            ret.operate_memo(frm,memo);
		}
	}

/*};*/

/* Functions for Images */
namespace ImageFunc{
	void combine(Image3D&ret,const Image3D&I1,const Image3D&I2){
		/*R=I1,G=I2,B=avg(I1,I2)*/
		ret = I1;
		int dx,dy,dz;
		int ax,ay,az;
		/* Values for skull+brain combo: */
			dx=-40;//axi
			dy=00;//cor
			dz=-18;//sag
		auto C0 = VoxelMap::transform(0).value[0];
		for(int ix=0;ix<ret.n[0];++ix){
		for(int iy=0;iy<ret.n[1];++iy){
		for(int iz=0;iz<ret.n[2];++iz){
			ax=ix+dx;
			ay=iy+dy;
			az=iz+dz;
			auto Cs=ret.ref(ix,iy,iz).value[0];
			if(I2.in_range(ax,ay,az)){
				auto Cb=I2.cref(ax,ay,az).value[0];

				ret.ref(ix,iy,iz).value[0]=(Cb);
				ret.ref(ix,iy,iz).value.push_back(Cs);
				ret.ref(ix,iy,iz).value.push_back(Cs);
				//ret.ref(ix,iy,iz).value.push_back(I2.cref(ax,ay,az).value[0]);
			}
			else{
				ret.ref(ix,iy,iz).value[0]=C0;
				ret.ref(ix,iy,iz).value.push_back(Cs);
				ret.ref(ix,iy,iz).value.push_back(Cs);
			}
			//unsigned long avg = (I1.cref(ix,iy,iz).to_ulong() + I2.cref(ix,iy,iz).to_ulong())/(2);
			//ret.ref(ix,iy,iz).value.push_back(VoxelMap::transform(avg).value[0]);
		}}}
		return;
	}
}

/* Handles IO for image files */
namespace ImgFormat{

	namespace PPM{
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
			img.data	                 .resize(img.n[0]);for(int ix=0;ix<img.n[0];++ix){
			img.data[ix]                 .resize(img.n[1]);for(int iy=0;iy<img.n[1];++iy){
			img.data[ix][iy]             .resize(img.n[2]);                              {
			img.data[ix][iy][0].value    .resize(3);       for(int ic=0;ic<3       ;++ic){
			img.data[ix][iy][0].value[ic].resize(1);                                     {
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
					//if(nc!=1)DBG(nc);
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
	void convert(const std::string& path,const std::string& format){
		std::string cmd;
		cmd="convert "+path+" "+path+"."+format+" ;";
		Util::sys(cmd);
		cmd="rm "+path+" ;";
		//Util::sys(cmd);
	}
	/* animate files in "in_dir" to "out_path.gif" */
	void animate(const std::string& in_dir,const std::string& out_path){
		std::string cmd;
		int delay=4;/*s/100*/
		cmd="convert -delay "+std::to_string(delay)+" -loop 0 "+in_dir+"/* "+out_path+".gif ;";
		Util::sys(cmd);
		cmd="rm -rf "+in_dir+" ;";
		//Util::sys(cmd);
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

	void ort_animate(const Image3D&img,const std::string& out_path,int a_,int aX,int sX,int sY){
		std::string cmd;
		cmd="mkdir -p "+pfx+"/tmp/ ;";
		Util::sys(cmd);
		Image3D cut;
		for(int fi=0;fi<img.n[a_];++fi){
			img.ort_plane(cut,a_,fi,aX,sX,sY);
			std::string fnm=pfx+"tmp/frm"+Util::padded_num(fi,3)+".ppm";
			ImgFormat::PPM::write(cut,fnm);
		}
		ImageMagick::animate(pfx+"tmp/",out_path);
	}

}




