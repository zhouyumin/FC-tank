#ifndef CONFIG_H
#define CONFIG_H

#include <string>
extern char map[26][27];
extern const std::string rootdir;
//extern QPixmap resizePic(QPixmap&p,int size);
enum direct{up,right,down,left};
#define resizePic(p,w,h) p.scaled(w,h,Qt::AspectRatioMode::IgnoreAspectRatio,Qt::TransformationMode::SmoothTransformation)
#define WIDTH 640
#define HEIGHT 600
#define BASESIZE 20
#define SIZE 40

#endif // CONFIG_H
