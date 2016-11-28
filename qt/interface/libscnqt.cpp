#include "libscnqt.h"
#include <QDataStream>
#include <QIODevice>
#include <iostream>


void libscnqt::generateInMemoryImg(Image3D& img, QByteArray* qtBA){
    QDataStream qDS(qtBA, QIODevice::ReadWrite);
    char buf[1000];
    /* write magic string */
    qDS.writeRawData((char*)"P6\n", 3);
    /* write nx,ny */
    int x = sprintf(buf, "%d %d\n",img.n[1], img.n[0]);
    qDS.writeRawData((char*) buf, x);
    qDS.writeRawData((char*)"255\n", 4);
    /* write data */
    for(int ix=0;ix<img.n[0];++ix){
    for(int iy=0;iy<img.n[1];++iy){
        int iz=0;                 {
            int nc=img.data[ix][iy][iz].value.size();
            for(int ic=0;ic<3;++ic){
                char c[1];
                c[0]=img.data[ix][iy][iz].value[ic%nc][0].to_ulong();
                qDS.writeRawData((char*) c, 1);
            }

    }}}
}

void libscnqt::generateInMemoryImgParallel(Image3D& img, QByteArray* qtBA){
    char buf[1000];
    int x = sprintf(buf, "P6\n%d %d\n255\n",img.n[1], img.n[0]);

    /* write data */
    int size = x + img.n[0] * img.n[1] * 3+1;
    qtBA->resize(size);
    int i ;
    for(i = 0 ; i < x; i++){
        (*qtBA)[i] = buf[i];
    }

#pragma omp parallel for
    for(int ix=0;ix<img.n[0];++ix){
    for(int iy=0;iy<img.n[1];++iy){
        int iz=0;                 {
            int nc=img.data[ix][iy][iz].value.size();
            for(int ic=0;ic<3;++ic){
                int a = i + ix * img.n[1] *3 + iy * 3 + ic + 1;
                (*qtBA)[a] = img.data[ix][iy][iz].value[ic%nc][0].to_ulong();
            }

    }}}
}



libscnqt::libscnqt()
{

}
