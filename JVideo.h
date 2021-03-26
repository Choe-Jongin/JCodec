#pragma once
#include "JEncoder.h"

class JVideo
{
    private:
    
    //original video info
    VideoCapture * video;
	int FPS;
	int CURRENT_FRAME;
	int TOTAL_FRAME;
	int WIDTH;
	int HEIGHT;
	int quartersize;

public:
    JVideo(){};
    JVideo(VideoCapture *video)
    {
       this->video       = video;
       FPS              = video->get(CAP_PROP_FPS);
       CURRENT_FRAME    = video->get(CAP_PROP_POS_FRAMES);
       TOTAL_FRAME      = video->get(CAP_PROP_FRAME_COUNT);
       WIDTH            = video->get(CAP_PROP_FRAME_WIDTH);
       HEIGHT           = video->get(CAP_PROP_FRAME_HEIGHT);
       quartersize      = WIDTH * HEIGHT / JEncoder::getInstance()->getScale(2);
    }
    ~JVideo(){};

    void update(){CURRENT_FRAME = video->get(CAP_PROP_POS_FRAMES);}
    
    int getFPS          (){return FPS;}
    int getCURRENT_FRAME(){return CURRENT_FRAME;}
    int getTOTAL_FRAME  (){return TOTAL_FRAME;}
    int getWIDTH        (){return WIDTH;}
    int getHEIGHT       (){return HEIGHT;}
    int getquartersize  (){return quartersize;}
    

    void printinfo(){
	    std::cout << "[Video Info]" << std::endl;
	    std::cout << "video FPS\t: " << FPS << std::endl;
	    std::cout << "video width\t: " << WIDTH << std::endl;
	    std::cout << "video height\t: " << HEIGHT << std::endl;
	    std::cout << "quartersize\t: " << quartersize << std::endl;
    }
    void printProgress(){
        std::cout
        << " ["
        << (int)((float)CURRENT_FRAME/(float)TOTAL_FRAME*100.0f)
        << "%]"
        << " frame : " 
        << CURRENT_FRAME 
        << " / "
        << TOTAL_FRAME 
        << std::endl;
    }
};