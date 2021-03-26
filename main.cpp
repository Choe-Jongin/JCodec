
#include "JEncoder.h"
#include "JVideo.h"
#include <iostream>
#include <stdio.h>
#include <thread>
using namespace std;

#define FOR(x,y,z) for( int x = y ; x < z ; x++ )

void JKeyflush()
{
	while(getchar() != '\n');
}

int main(int argc, char** argv)
{	
	//setting option
	bool showOriginal 	= true;
	bool showQuarter 	= false;

	string originfilename("../resource/original.mov");

	for( int i = 1 ; i < argc ; i++)
	{

		String str(argv[i]);
		string prop;
		string value;

		int index = str.find(":");
		if( index == string::npos || index == 0)
		{
			cout << "can't undersanted the argument : " << argv[i] << endl;
			continue;
		}

		prop.assign(str.substr(0, index));
		value.assign(str.substr(index+1));

		if( prop.compare("filename") == 0)
			originfilename.assign(value.c_str());

		if( prop.compare("scale") == 0 || prop.compare("quarterscale") == 0 )
		{
			int scale = stoi(value.c_str());
			if( scale != 2 && scale != 4)
				cout <<" WARNING SCALE(" << scale << ") MAY NOT SURPPORT" << endl;
			JEncoder::getInstance()->setScale(scale);
		}
		


	}

	char input = 0;
	// while(input != 'N' && input != 'n' || input != 'Y' && input != 'y'){
	// 	cout << "show original video? [y/n]" << endl;
	// 	input = getchar();
	// 	switch (input)
	// 	{
	// 	case 'Y':
	// 	case 'y':
	// 		showOriginal = true;
	// 		break;
	// 	case 'N':
	// 	case 'n':
	// 		break;
	// 	case 27: 
	// 		return 0;
	// 		break;
	// 	}
	// }
	
	input = 0;
	while(input != 'N' && input != 'n' && input != 'Y' && input != 'y'){
		std::cout << "show quartermovie? [y/n]" << endl;
		input = getchar();
		switch (input)
		{
		case 'Y':
		case 'y':
			showQuarter = true;
			break;
		case 'N':
		case 'n':
			break;
		case 27:
			return 0;
			break;
		}
	}
	input = 0;
	
	JKeyflush();

	//load original video
	VideoCapture video(originfilename.c_str());
	JVideo originalVideo(&video);

	//video not exist
	if (!video.isOpened())
	{
		cout << "JEncoder : Can't open the video" << endl;
		return -1;
	}
	if(originalVideo.getWIDTH() % JEncoder::getInstance()->getScale() != 0 || originalVideo.getHEIGHT() % JEncoder::getInstance()->getScale() != 0 )
	{
		cout <<" WARNING SCALE(" << JEncoder::getInstance()->getScale() << ") MAY NOT SURPPORT" << endl;
	}

	//make original movie window
	if( showOriginal == true )
	{
		namedWindow("original", WINDOW_NORMAL);
		resizeWindow("original", 1280, 720);
	}
	//show original video info
	originalVideo.printinfo();

	//original video each frame
	Mat img;

	//compression format & quartersize
	int fourcc = VideoWriter::fourcc('D', 'I', 'V', 'X');
	Size quarterSize(originalVideo.getWIDTH() / JEncoder::getInstance()->getScale(), originalVideo.getHEIGHT() / JEncoder::getInstance()->getScale());

	// video writer for each quartermoive
	vector<VideoWriter*> writer_quartermovies;
	for( int i = 0 ; i < JEncoder::getInstance()->getScale(2) ; ++i )
	{
		string quarfilename("../outfile/quartermovie");
		quarfilename.append(to_string(i));
		quarfilename.append(".avi");
		printf("filename : %s\n",quarfilename.c_str());
		writer_quartermovies.push_back(new VideoWriter(quarfilename.c_str(), fourcc, originalVideo.getFPS(), quarterSize));
	}

	//buffer for every quartermovies
	vector<vector<uchar>*> vec_quartermovies;	
	for( int i = 0 ; i < JEncoder::getInstance()->getScale(2) ; ++i )
	{
		vec_quartermovies.push_back(new vector<uchar>());
		for(int j = 0 ; j < originalVideo.getquartersize() * 3 ; ++j )
			vec_quartermovies[i]->push_back(0);
	}

	//start extracting
	while (1)
	{
		//Update frame
		originalVideo.update();

		//get a single frame
		video >> img;

		//make quartermovie frame data
		vector<thread> threads;
		for (int i = 0; i < JEncoder::getInstance()->getScale(2); ++i)
		{
			//0,0 -> 0,1 -> 1,0 -> 1,1
			int startcol = i / JEncoder::getInstance()->getScale();
			int startrow = i % JEncoder::getInstance()->getScale();
			threads.push_back(thread(JEncoder::peakquartervec, &img, vec_quartermovies[i], startcol, startrow));
		}
		
		//wait
		for (int i = 0; i < JEncoder::getInstance()->getScale(2); ++i) threads[i].join();
		//delete threads
		threads.clear();

		//data(vector) to frame
		vector<Mat> img_quartermovies;
		for (int i = 0; i < JEncoder::getInstance()->getScale(2); ++i)
			img_quartermovies.push_back(Mat(quarterSize, CV_8UC3, &(*vec_quartermovies[i])[0]));
		
		//extract movie		
		for (int i = 0; i < JEncoder::getInstance()->getScale(2); ++i)
			(*(writer_quartermovies[i])) << img_quartermovies[i];
		
		//show progress
		originalVideo.printProgress();

		//if complete or empty image
		if (img.empty())
		{
			cout << "complete!!"<<endl;
			break;
		}

		//show window		
		if ( showOriginal == true )
			imshow("original", img);
		if ( showQuarter == true )
		{
			for (int i = 0; i < JEncoder::getInstance()->getScale(2); i++)
			{
				string windowname("quartermovie");
				windowname.append(to_string(i));
				imshow(windowname.c_str(), img_quartermovies[i]);
			}
		}
		//delete
		img_quartermovies.clear();

		//if (waitKey( FPS == 0 ? 1 : 1000/FPS) > 0 ) break;

		input = waitKey(1);
		if (input == 27)
		{
			break;
		}else if(input == 's' || input == 'S'){
			showQuarter = !showQuarter;
			if (showQuarter == false)
			{
				FOR(i, 0, JEncoder::getInstance()->getScale(2))
				{
					string windowname("quartermovie");
					windowname.append(to_string(i));
					destroyWindow(windowname.c_str());
				}
			}
		}

	}

	//delete 
	writer_quartermovies.clear();			//delete videowriter;
	for(int i = 0 ; i < JEncoder::getInstance()->getScale(2) ; ++i )
			vec_quartermovies[i]->clear();	//delete each pixel of quartermovie
	vec_quartermovies.clear();				//delete all quartermovie data

	cout <<"END"<<endl;
	originalVideo.printProgress();
	return 0;
}