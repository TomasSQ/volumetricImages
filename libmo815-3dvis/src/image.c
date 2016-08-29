#include "image.h"


char ValidVoxel(Image *img, Voxel u)
{
  if ((u.x >= 0)&&(u.x < img->xsize)&&
      (u.y >= 0)&&(u.y < img->ysize)&&
      (u.z >= 0)&&(u.z < img->zsize) )
    return(1);
  else
    return(0);
}

Voxel GetVoxelCoord(Image *img, int p)
{
  Voxel u;

  u.x = GetXCoord(img,p);
  u.y = GetYCoord(img,p);
  u.z = GetZCoord(img,p);

  return(u);
}

Image *CreateImage(int xsize, int ysize, int zsize)
{
  Image *img=NULL;
  int    i;

  img = (Image *) calloc(1,sizeof(Image));
  img->n     = xsize*ysize*zsize;
  img->xsize = xsize; 
  img->ysize = ysize; 
  img->zsize = zsize; 
  img->dx    = img->dy = img->dz = 1.0;
  img->val   = AllocIntArray(img->n);
  img->tby   = AllocIntArray(img->ysize);
  img->tbz   = AllocIntArray(img->zsize);

  img->tby[0] = 0;
  for (i=1; i < img->ysize; i++)
    img->tby[i]=img->tby[i-1]+img->xsize;

  img->tbz[0] = 0;
  for (i=1; i < img->zsize; i++)
    img->tbz[i]=img->tbz[i-1]+img->xsize*img->ysize;

 
 return(img);
}

void DestroyImage(Image *img)
{

  if(img != NULL){
    free(img->val); 
    free(img->tby);
    free(img->tbz);
    free(img);
  }

}

Image *ReadImage(char *filename)
{
  FILE           *fp=NULL;
  unsigned short *data16=NULL;
  unsigned char  *data8=NULL;
  char            type[10];
  int             p,xsize,ysize,zsize,depth;
  Image          *img=NULL;

  fp = fopen(filename,"rb");
  if (fp == NULL){
    Error(MSG2,"ReadImage");
  }

  fscanf(fp,"%s\n",type);
  if((strcmp(type,"SCN")==0)){
    fscanf(fp,"%d %d %d\n",&xsize,&ysize,&zsize);
    img = CreateImage(xsize,ysize,zsize);
    fscanf(fp,"%f %f %f\n",&img->dx,&img->dy,&img->dz);
    fscanf(fp,"%d\n",&depth);    


    switch(depth) {
    case 8:
      data8 = AllocUCharArray(img->n); 
      fread(data8, sizeof(unsigned char), img->n, fp);
      for (p=0; p < img->n; p++) 
	img->val[p] = (int) data8[p];
      free(data8);
      break;
    case 16:
      data16 = AllocUShortArray(img->n); 
      fread(data16, sizeof(unsigned short), img->n, fp);
      for (p=0; p < img->n; p++) 
	img->val[p] = (int) data16[p];
      free(data16);
      break;
    default:
      Error("Invalid image depth","ReadImage");
    }
  }else{
    Error("Invalid image format","ReadImage");
  }


  fclose(fp);

  return(img);
}

void WriteImage(Image *img,char *filename)
{
  FILE           *fp;
  int             p, Imax, Imin;
  unsigned char  *data8; 
  unsigned short *data16; 

  fp = fopen(filename,"wb");
  if (fp == NULL){
    Error(MSG2,"WriteImage");
  }

  /* shift intensities if necessary */
  
  Imin = MinimumValue(img); 

  if (Imin < 0) {
    char msg[200];
    sprintf(msg,"Min. intensity is %d. Shifting intensities...\n",Imin);
    Warning(msg,"WriteImage");
    for (p=0; p < img->n; p++) 
      img->val[p] = img->val[p] - Imin; 
  }
    
  /* computing depth */ 

  Imax = MaximumValue(img); 
  
  if (Imax < 65536) { /* 16-bit data */

    fprintf(fp,"SCN\n");
    fprintf(fp,"%d %d %d\n",img->xsize,img->ysize,img->zsize);
    fprintf(fp,"%f %f %f\n",img->dx,img->dy,img->dz);
    fprintf(fp,"16\n");
    
    data16 = AllocUShortArray(img->n); 
    for (p=0; p < img->n; p++) 
      data16[p] = (unsigned short) img->val[p]; 

    fwrite(data16, sizeof(unsigned short), img->n, fp); 
    free(data16); 

  }else{

    if (Imax < 256) { /* 8-bit data */
      fprintf(fp,"SCN\n");
      fprintf(fp,"%d %d %d\n",img->xsize,img->ysize,img->zsize);
      fprintf(fp,"%f %f %f\n",img->dx,img->dy,img->dz);
      fprintf(fp,"8\n");
      
      data8 = AllocUCharArray(img->n); 
      for (p=0; p < img->n; p++) 
	data8[p] = (unsigned char) img->val[p]; 
      
      fwrite(data8, sizeof(unsigned char), img->n, fp); 
      free(data8); 
    } else {
      Error("Image depth is different from 8 and 16","WriteImage");
    }
  }
  fclose(fp);
}

/* ----------------- Voxel-based algorithms ---------------------*/

int MinimumValue(Image *img)
{
  int p, Imin = INFINITY_INT;

  for (p=0; p < img->n; p++) 
    if (img->val[p] < Imin) 
      Imin = img->val[p];

  return(Imin);
}

int MaximumValue(Image *img)
{
  int p, Imax = -INFINITY_INT;

  for (p=0; p < img->n; p++) 
    if (img->val[p] > Imax) 
      Imax = img->val[p];

  return(Imax);
}

Image  *WindowAndLevel(Image *img, int window, int level, int H)
{
  int    p, Imin, Imax; 
  Image *img_corr = CreateImage(img->xsize,img->ysize,img->zsize);  
  float  K; 

  if (window < 0) 
    Error("window must be >= 0","WindowAndLevel"); 

  if (H <= 0) 
    Error("H must be > 0","WindowAndLevel"); 

  Imin = level - window/2;
  Imax = level + window/2;

  if (window > 0)  { 
    K    = (float)H /(float)(Imax - Imin); 
  
    for (p=0; p < img->n; p++) {
      if (img->val[p] <= Imin) {
	img_corr->val[p] = 0; 
      } else {
	if (img->val[p] >= Imax) {
	  img_corr->val[p] = H; 
	} else { /* compute linear stretching */
	  img_corr->val[p] = K * (img->val[p] - Imin);
	}
      }  
    }

  } else {
    for (p=0; p < img->n; p++) {
      if (img->val[p] >= level) 
	  img_corr->val[p] = H; 
    }
  }

  return(img_corr);
}

/* ----------------- Adjacency-based algorithms ---------------------*/


Image *LinearFilter(Image *img, Kernel *K)
{
  Image *img_filt=CreateImage(img->xsize,img->ysize,img->zsize);
  int    p, q, i;
  Voxel  u, v;
  float  filt;

  for (p=0; p < img->n; p++) {
    u = GetVoxelCoord(img,p);
    filt = 0.0;
    for (i=0; i < K->A->n; i++) {
      v = GetAdjacentVoxel(K->A,u,i);
      if (ValidVoxel(img,v)){
	q = GetVoxelIndex(img,v);
	filt = filt + img->val[q] * K->w[i];
      }
    }
    img_filt->val[p] = ROUND(filt);
  }

  return(img_filt); 
}

/* Connectivity-based methods */

Image        *LabelBinaryImage(Image *img, AdjRel *A)
{

  return(NULL);
}




/* ----------------- Connectivity-based algorithms ---------------------*/
