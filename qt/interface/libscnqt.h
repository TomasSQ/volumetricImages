#ifndef LIBSCNQT_H
#define LIBSCNQT_H

#include <QByteArray>
#include "libscn.hpp"

class libscnqt
{

public:
    libscnqt();
    void generateInMemoryImg(Image3D&, QByteArray*);
    void generateInMemoryImgParallel(Image3D&, QByteArray*);
};

#endif // LIBSCNQT_H
