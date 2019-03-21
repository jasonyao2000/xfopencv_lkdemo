#include <stdio.h>
#include <tchar.h>
#include <io.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include <stdio.h>


#include <Windows.h>
  
#include <iterator>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;


//获取所有的文件名
void GetAllFiles(string path, vector<string>& files)
{

	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

}

void getImages(string path, vector<string>& imagesList)
{
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;
	string p;

	hFile = _findfirst(p.assign(path).append("\\*.jpg").c_str(), &fileinfo);

	if (hFile != -1) {
		do {
			imagesList.push_back(path+fileinfo.name);//保存类名
		} while (_findnext(hFile, &fileinfo) == 0);
	}
}

//获取特定格式的文件名
void GetAllFormatFiles(string path, vector<string>& files, string format)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
					GetAllFormatFiles(p.assign(path).append("\\").append(fileinfo.name), files, format);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}

// 该函数有两个参数，第一个为路径字符串(string类型，最好为绝对路径)；
// 第二个参数为文件夹与文件名称存储变量(vector类型,引用传递)。
// 在主函数中调用格式(并将结果保存在文件"AllFiles.txt"中，第一行为总数)：

//vector<string> get_all_files_names_within_folder(string folder)
//{
//	vector<string> names;
//	char search_path[200];
//	sprintf(search_path, "%s*.*", folder.c_str());
//	WIN32_FIND_DATA fd;
//	HANDLE hFind = ::FindFirstFile(search_path, &fd);
//	if (hFind != INVALID_HANDLE_VALUE)
//	{
//		do 
//		{
//			// read all (real) files in current folder
//			// , delete '!' read other 2 default folder . and ..
//			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
//			{
//				names.push_back(fd.cFileName);
//			}
//		}while (::FindNextFile(hFind, &fd));
//		::FindClose(hFind);
//	}
//	return names;
//}

int string_replase(string &s1, const string &s2, const string &s3)
{
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while ((pos = s1.find(s2, pos)) != string::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}
	return 0;
}


int main()
{
	string filePath = "c:\\vastai_projects\\dataset\\test1\\";
	vector<string> files;
	char * distAll = "AllFiles.txt";

	//读取所有的文件，包括子文件的文件
	//GetAllFiles(filePath, files);

	//读取所有格式为jpg的文件
	string format = ".jpg";
	//GetAllFiles(filePath, files);
//	GetAllFormatFiles(filePath, files, format);
	getImages(filePath, files);
	//ofstream ofn(distAll);
	//int size = files.size();
	//ofn << size << endl;
	//for (int i = 0; i < size; i++)
	//{
	//	ofn << files[i] << endl;
	//	cout << files[i] << endl;
	//}
	//ofn.close();
	cv::Mat src,dst;
	string dstfn;

	string s2 = "###ip##";
	string s1 = "http://123###ip##678.com";
	string s3 = "192";
	string_replase(s1, s2, s3);

	for (int i = 0; i < files.size(); i++)
	{
		src = cv::imread(files.at(i));
		cv::resize(src, dst, cv::Size(src.cols * 4, src.rows * 4), 0, 0, cv::INTER_CUBIC);
		dstfn = files.at(i);
		string_replase(dstfn, "HR", "HR_BICUBIC");
		cv::imwrite(dstfn,dst);

		cv::imshow("src", src);
		cv::waitKey(1000);
	}
	return 0;
}