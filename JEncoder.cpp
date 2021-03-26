#include "JEncoder.h"
JEncoder * JEncoder::instance = nullptr;
JEncoder::JEncoder( int quarterScale )
{
    this->quarterScale = quarterScale;
}
JEncoder::~JEncoder()
{
}
JEncoder *JEncoder::getInstance()
{
    if (JEncoder::instance == NULL)
        instance = new JEncoder();
    return instance;
}

void JEncoder::peakquartervec(Mat* img, std::vector<uchar>* imgvec, int startcol, int startrow) {
    
	if (img == NULL)
		return;

	int index = 0;
    int scale = JEncoder::getInstance()->getScale();
	for (int row = startrow; row < img->rows; row += scale)
		for (int col = startcol; col < img->cols; col += scale)
			for (int i = 0; i < 3; ++i)
					(*imgvec)[index++] = (img->data[row * img->cols * 3 + col * 3 + i]);
}