#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "libscn.cpp"
#include <stdio.h>


 Image3D img;
void banana(/*const*/ Image3D&img, QByteArray &qtBA){

    char buf[1000];
    /* write magic string */
    qtBA.append((char*)"P6\n");
    /* write nx,ny */
    int x = sprintf(buf, "%d %d\n",img.n[1], img.n[0]);
    qtBA.append((char*)buf, x);
    qtBA.append((char*)"255\n");
    /* write data */
    for(int ix=0;ix<img.n[0];++ix){
    for(int iy=0;iy<img.n[1];++iy){
        int iz=0;                 {
            int nc=img.data[ix][iy][iz].value.size();
            for(int ic=0;ic<nc;++ic){
                qtBA.append((char)(img.data[ix][iy][iz].value[ic%nc][0].to_ulong()));
            }

    }}}
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ImgFormat::detect::read(img,"/home/luciano/Downloads/libmc920/data/skull.scn");
    Image3D cut;
    img.ort_plane(cut, 1, -1, 10, 1, 1);

    ui->setupUi(this);

    QByteArray q;

    banana(cut, q);

    std::vector<std::vector<std::vector<Voxel> > > teste = cut.data;
    std::cout << cut.get_n(0)  << std::endl;
    std::cout << cut.get_n(1)  << std::endl;
    std::cout << cut.get_n(2)  << std::endl;
    int alocated = 15 + 3 *(cut.get_n(0) * cut.get_n(1));
    uchar imgV[alocated];
    imgV[0] = 'P';
    imgV[1] = '6';
    imgV[2] = '\n';
    imgV[3] = '1';
    imgV[4] = '9';
    imgV[5] = '3';
    imgV[6] = ' ';
    imgV[7] = '2';
    imgV[8] = '5';
    imgV[9] = '2';
    imgV[10] = '\n';
    imgV[11] = '2';
    imgV[12] = '5';
    imgV[13] = '5';
    imgV[14] = '\n';
    std::cout << "Alocated " << alocated << std::endl;

    int idx = 15;
    for(int i = 0 ; i < cut.get_n(0); i++ ){
        for(int j = 0 ; j < cut.get_n(1); j++ ){
            Voxel v = teste[i][j][0];
            int nc=v.value.size();
            for(int ic=0;ic<nc;++ic){
                uchar c = ((uchar)(v.value[ic%nc][0].to_ulong()));
                imgV[idx++] = c ;
            }

        }
    }


    std::cout << pix.loadFromData(q, "PPM") << std::endl;

    std::cout << q.count() << "  " << q[0];
    std::cout << "W:" << pix.rect().width() << " H:" << pix.rect().height() <<std::endl;
    scene = new QGraphicsScene(this);
    scene->addPixmap(pix);
    scene->setSceneRect(pix.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(pix.rect());
}




MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{

    ImgFormat::detect::read(img,"/home/luciano/Downloads/libmc920/data/skull.scn");
    Image3D cut;
    img.ort_plane(cut, 1, value, 1, 1, 1);
    ImgFormat::detect::write(cut,"banana.ppm" );
    std::cout << pix.load("banana.ppm");
     scene->addPixmap(pix);
    QWidget * viewport = ui->graphicsView->viewport();
    viewport->update();
}
