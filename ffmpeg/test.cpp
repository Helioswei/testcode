#include "./common.h"

using namespace std;
int main(){
    string inVideo("/root/test1.swf");
    AVFormatContext *inFmtCont = NULL;
    inFmtCont = avformat_alloc_context();
    unsigned char *iobuffer = (unsigned char*)av_malloc(32768);
    AVIOContext *avio = avio_alloc_context(iobuffer, 32768, 0, NULL, fill_iobuffer, NULL, NULL);
    inFmtCont -> pb = avio;

    //打开输入的视频文件
    int ret;
    ret = avformat_open_input(&inFmtCont, "nothing", NULL, NULL);
    if (ret < 0){
        cout << "打开文件失败" << endl;
        return -1;
    }
    //查找流
    ret = avformat_find_stream_info(inFmtCont, NULL);
    if (ret < 0){
        cout << "查找流失败" << endl;
        return -1;
    }
    for (int i = 0; i < inFmtCont -> nb_streams; i++){
        cout << i << endl;
    }
    AVStream *outStrem = inFmtCont -> streams[0] ;
    int width = outStrem -> codecpar -> width;
    int height = outStrem -> codecpar -> height;
    cout << "width: " << width << "height: " << height << endl;

    avformat_close_input(&inFmtCont);
    


    return 0;
}
