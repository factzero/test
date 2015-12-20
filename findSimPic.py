# -*- coding:UTF8 -*-
import os
import sys
import cv2

if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding( "utf-8" )
    InPath  = r'E:\test\照片_bak'
    uInPath = unicode(InPath, 'utf8')
    ShowW = 512;
    ShowH = 512;
    cv2.namedWindow('Img', cv2.WINDOW_AUTOSIZE)
    for filename in os.listdir(uInPath):
        Fullfile = os.path.join(uInPath, filename);
        print Fullfile
        Fullfile = Fullfile.decode('utf8').encode('gbk')
        cvImgOri  = cv2.imread(Fullfile)
        #ShowH = ShowW*cvImgOri.shape[0]/cvImgOri.shape[1]
        cvImgShow = cv2.resize(cvImgOri, (ShowW, ShowH))
        cv2.imshow('Img', cvImgShow)
        cv2.waitKey(10)

    cv2.destroyAllWindows()