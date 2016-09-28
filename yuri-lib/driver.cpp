//#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <map>

#include "libscn.cpp"

#define	arg_has(argm_k)	(!argm[argm_k].empty())
std::map<std::string,std::string> argm;
void arg_parse(int argc, char* argv[]){
	std::vector<std::string> args(argv, argv+argc);
	for(int i=1;i<args.size();++i){
		std::size_t found=args[i].find('=');
		if(found!=std::string::npos){
			std::string k = args[i].substr(0,found);
			std::string v = args[i].substr(found+1,std::string::npos);
			//boost::to_upper(k);
			for(auto &ik:k){ik=toupper(ik);}
			argm[k] = v;
		}
		else{
			std::string k = args[i];
			for(auto &ik:k){ik=toupper(ik);}
			argm[k] = "1";
		}
	}
}

int main(int argc, char* argv[]){
	std::ios::sync_with_stdio(0);
	//srand(time(NULL));
	arg_parse(argc,argv);
	if(arg_has("-H")||arg_has("-HELP")){
		std::cout<<"Parameters:"<<std::endl;
		std::cout<<argv[0]<<" [-v] -inp_img=<PATH>"<<std::endl;
		std::cout<<"\t[-cut=<PATH>] [-gif=<PATH>]"<<std::endl;
		std::cout<<"\t\t[-p_a_=<NUM>] [-p_p_=<NUM>] [-p_ax=<NUM>] [-p_sx=<NUM>] [-p_sy=<NUM>]"<<std::endl;
		std::cout<<"\t\t[-esp=<R|N>] [-p_slc=<A|S|C>]"<<std::endl;
		std::cout<<"\t[-t_cut=<PATH>]"<<std::endl;
		std::cout<<"\t\t[-t_uni=<PATH>] [-t_per=<PATH>]"<<std::endl;
		std::cout<<"\t\t[-t_pal=<PATH>]"<<std::endl;
		std::cout<<"\t[-label=<PATH>]"<<std::endl;
		std::cout<<"\t[-t_lpr=<PATH>]"<<std::endl;
		exit(0);
	}

	/* Create tmp directory */
	ImgMacro::init();

	/* Load an image */
	Image3D img;
	ImgFormat::detect::read(img,argm["-INP_IMG"]);
	if(arg_has("-V"))std::cout<<"Image Loaded"<<std::endl;

	/* TAREFA 1: Extrair corte */
	/* Parameters */
	int a_=0;int p_=-1;int aX=0;int sX=1;int sY=1;
	a_=2;aX=0;sX=0;sY=1;//
	char slc=toupper(argm["-P_SLC"].front());
	char esp=toupper(argm["-ESP"].front());
	if(arg_has("-P_SLC")){
		     if(slc=='A')a_=0;
		else if(slc=='S')a_=2;
		else if(slc=='C')a_=1;
	}
	if(arg_has("-ESP")){
		if(a_<0||a_>2){std::cerr<<"Wrong parameter"<<std::endl;}
		else if(a_==0){
			     if(esp=='R'){a_=0;aX=0;sX=1;sY=1;}
			else if(esp=='N'){a_=0;aX=0;sX=1;sY=0;}}
		else if(a_==2){
			     if(esp=='R'){a_=2;aX=0;sX=0;sY=0;}
			else if(esp=='N'){a_=2;aX=0;sX=0;sY=1;}}
		else if(a_==1){
							 {a_=1;aX=1;sX=0;sY=1;}}
	}
	if(arg_has("-CUT")||arg_has("-GIF")){
		if(arg_has("-P_A_"))a_=std::stoi(argm["-P_A_"]);
		if(arg_has("-P_P_"))p_=std::stoi(argm["-P_P_"]);
		if(arg_has("-P_AX"))aX=std::stoi(argm["-P_AX"]);
		if(arg_has("-P_SX"))sX=std::stoi(argm["-P_SX"]);
		if(arg_has("-P_SY"))sY=std::stoi(argm["-P_SY"]);
	}
	Image3D cut;
	if(arg_has("-CUT")){
		img.ort_plane(cut,a_,p_,aX,sX,sY);
		ImgFormat::detect::write(cut,argm["-CUT"]);
		ImageMagick::convert(argm["-CUT"]);
		if(arg_has("-V"))std::cout<<"Slice extracted"<<std::endl;
	}
	/* Cutting animation */
	if(arg_has("-GIF")){
		ImgMacro::ort_animate(img,argm["-GIF"],a_,aX,sX,sY);
		if(arg_has("-V"))std::cout<<"Animated"<<std::endl;
	}

	/* TAREFA 2: Transformação Linear */
	int I1,I2,k1,k2;
	Voxel v_max=img.top(std::less<Voxel>());
	Voxel v_min=img.top(std::greater<Voxel>());
	int Imin=v_min.to_ulong();
	int Imax=v_max.to_ulong();

	/* Threshold */	//I1= 127;I2= 128;k1= 000;k2= 255;
	/* Normalize */	  I1=Imin;I2=Imax;k1= 000;k2= 255;
	/* Negative */	//I1=Imin;I2=Imax;k1=Imax;k2=Imin;

	//DBG(I2);return 0;
	//I2 = (I2*6)/(12); //good for brain

	Image3D uni;
	if(arg_has("-T_UNI")){
		uni=img;
		uni.linear_transform(I1,I2,k1,k2);
		ImgMacro::ort_animate(uni,argm["-T_UNI"],a_,aX,sX,sY);
		if(arg_has("-V"))std::cout<<"Unified transformation applied"<<std::endl;
	}
	if(arg_has("-T_CUT")){
		Voxel v_max=cut.top(std::less<Voxel>());
		int Imax=v_max.to_ulong();
		cut.linear_transform(0,Imax,0,255);
		ImgFormat::detect::write(cut,argm["-T_CUT"]);
		ImageMagick::convert(argm["-T_CUT"]);
	}
	if(arg_has("-T_PER")){
		using ImgMacro::pfx ;
		std::string out_path=argm["-T_PER"];
		std::string cmd;cmd="mkdir -p "+pfx+"/tmp/ ;";Util::sys(cmd);
		Image3D frm;
		for(int fi=0;fi<img.n[a_];++fi){
			img.ort_plane(frm,a_,fi,aX,sX,sY);
			/*Pick one*/
				/*1*/	frm.linear_transform(I1,frm.top(std::less<Voxel>()).to_ulong(),k1,k2);
				/*2*///	frm.apply_pallete();
			std::string fnm=pfx+"tmp/frm"+Util::padded_num(fi,3)+".ppm";ImgFormat::detect::write(frm,fnm);}
		ImageMagick::animate(pfx+"tmp/",out_path);
		if(arg_has("-V"))std::cout<<"Transformation per frame applied"<<std::endl;
	}
	if(arg_has("-T_PAL")){
		Image3D cpy=img;
		cpy.apply_pallete();
		ImgMacro::ort_animate(cpy,argm["-T_PAL"],a_,aX,sX,sY);
		if(arg_has("-V"))std::cout<<"Custom pallete applied"<<std::endl;
	}
	/* TAREFA 3: Rotulos */
	Image3D imgL;
	if(arg_has("-LABEL")){
		Image3D cpy=img;
		ImgFormat::detect::read(imgL,argm["-LABEL"]);
		cpy.colorize(imgL);
		ImgMacro::ort_animate(cpy,argm["-GIF"]+".label",a_,aX,sX,sY);

		//cpy.ort_plane(cut,a_,p_,aX,sX,sY);
		//ImgFormat::detect::write(cut,argm["-CUT"]+".label.ppm");
		//ImageMagick::convert(argm["-CUT"]+".label.ppm");
		if(arg_has("-V"))std::cout<<"Colorized with label"<<std::endl;
	}
	if(arg_has("-T_LPR")){
		using ImgMacro::pfx ;
		std::string out_path=argm["-T_LPR"];
		std::string cmd;cmd="mkdir -p "+pfx+"/tmp/ ;";Util::sys(cmd);
		Image3D frm;
		Image3D laf;
		for(int fi=0;fi<img.n[a_];++fi){
			img .ort_plane(frm,a_,fi,aX,sX,sY);
			imgL.ort_plane(laf,a_,fi,aX,sX,sY);
			/*Pick one*/
				/*1*/	frm.linear_transform(I1,frm.top(std::less<Voxel>()).to_ulong(),k1,k2);
				/*2*///	frm.apply_pallete();
			frm.colorize(laf);
			std::string fnm=pfx+"tmp/frm"+Util::padded_num(fi,3)+".ppm";ImgFormat::detect::write(frm,fnm);}
		ImageMagick::animate(pfx+"tmp/",out_path);
		if(arg_has("-V"))std::cout<<"Transformation per frame applied"<<std::endl;
	}

	return 0;
}
